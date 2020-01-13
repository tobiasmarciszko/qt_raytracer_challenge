#include "engine.h"
#include "computations.h"
#include "intersection.h"
#include "world.h"
#include "appsettings.h"

// Hiding this functionality within a macro to be able to disable it while running tests
// since AppSettings will differ and the setting won't be properly checked

//#define FAST_RENDER_ENABLED AppSettings::get().fastRenderEnabled()
#define FAST_RENDER_ENABLED false

namespace Raytracer::Engine {

Computations prepare_computations(const Intersection& i, const Ray& r, const Intersections& xs)
{
    Computations comps;

    comps.t = i.t;
    comps.object = i.object;
    comps.point = r.position(comps.t);
    comps.eyev = -r.direction();
    comps.normalv = comps.object->normal_at(comps.point);

    if (comps.normalv.dot(comps.eyev) < 0) {
        comps.inside = true;
        comps.normalv = -comps.normalv;
    } else {
        comps.inside = false;
    }

    comps.over_point = comps.point + comps.normalv * EPSILON;
    comps.under_point = comps.point - comps.normalv * EPSILON;

    if (FAST_RENDER_ENABLED) { return comps; }

    comps.reflectv = r.direction().reflect(comps.normalv);

    // Refraction
    std::vector<const Shape*> containers{};

    for(const auto& is: xs) {
        if (i == is) {
            if (containers.empty()) {
                comps.n1 = 1.0;
            } else {
                comps.n1 = containers.back()->material.refractive_index;
            }
        }

        bool found = false;
        for (auto it = containers.begin(); it != containers.end(); it++) {
            if (is.object == *it) {
                containers.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            containers.push_back(is.object);
        }

        if (i == is) {
            if (containers.empty()) {
                comps.n2 = 1.0;
            } else {
                comps.n2 = containers.back()->material.refractive_index;
            }

            break;
        }
    }

    return comps;
}

Intersections intersect_world(const World& w, const Ray& r)
{
    Intersections is;

    for (const auto& object: w.shapes) {
        const Intersections xs = object->intersect(r);

        for (const auto& intersection: xs) {
            is.emplace_back(intersection);
        }
    }

    std::sort(is.begin(),
              is.end(),
              [](const Intersection &i1,
                 const Intersection &i2) -> bool
              {
                  return i1.t < i2.t;
              });

    return is;
}

bool is_shadowed(const World& world, const Point& point, const Light& light)
{
    if (FAST_RENDER_ENABLED) return false;

    const Vector v = light.position() - point;
    const auto distance = v.magnitude();
    const auto direction = v.normalize();

    const Ray r = Ray(point, direction);

    const auto intersections = intersect_world(world, r);
    const auto h = hit(intersections);

    return (h.has_value() && (h->t < distance));
}

bool is_shadowed(const World& world, const Point& point)
{
    return is_shadowed(world, point, world.lights.front());
}

Color color_at(const World& w, const Ray& r, const LightingModel& lightingModel, int remaining)
{

    const Color black{0, 0, 0};

    const auto is = intersect_world(w, r);
    const auto h = hit(is);
    if (!h.has_value()) {
        return Color(0, 0, 0);
    }

    const auto comps = prepare_computations(h.value(), r, is);
    const auto color = shade_hit(w, comps, lightingModel, remaining);

    return color;
}

Color reflected_color(const World& w, const Computations& comps, const LightingModel& lightingModel, int remaining)
{

    // Prevent infinite recursion
    if (remaining <= 0) {
        return black;
    }

    if (equal(comps.object->material.reflective, 0)) {
        return black;
    }

    const Ray reflect_ray(comps.over_point, comps.reflectv);
    const Color color = color_at(w, reflect_ray, lightingModel, remaining - 1);

    return color * comps.object->material.reflective;
}

Color reflected_color(const World& w, const Computations& comps, int remaining)
{
    return reflected_color(w, comps, LightingModel::Phong, remaining);
}

Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel, int remaining)
{

    Color c{0, 0, 0};
    for (const auto& light: w.lights) {
        const auto in_shadow = is_shadowed(w, comps.over_point, light);

        const auto surface = lighting(
            comps.object->material,
            comps.object,
            light,
            comps.over_point,
            comps.eyev,
            comps.normalv,
            in_shadow,
            lightingModel);

        if (FAST_RENDER_ENABLED) return c + surface;

        const auto reflected = reflected_color(w, comps, lightingModel, remaining);
        const auto refracted = refracted_color(w, comps, lightingModel, remaining);

        // If the object is both reflective and refractive, the schlick approximation is used:

        const auto material = comps.object->material;
        if (material.reflective > 0 && material.transparency >0) {
            const auto reflectance = schlick(comps);
            c = c + (surface + reflected * reflectance + refracted * (1 - reflectance));
        } else
        {
            c = c + (surface + reflected + refracted);
        }
    }

    return c;
}

Matrix<4,4> view_transform(const Point& from, const Point& to, const Vector& up)
{

    const auto forward = Vector(to - from).normalize();
    const auto upn = up.normalize();
    const auto left = forward.cross(upn);
    const auto true_up = left.cross(forward);

    const auto orientation = Matrix<4,4>{
        left.x,      left.y,     left.z,    0,
        true_up.x,   true_up.y,  true_up.z, 0,
        -forward.x, -forward.y, -forward.z, 0,
        0,             0,            0,           1
    };

    return orientation * translation(-from.x, -from.y, -from.z);
}

Color refracted_color(const World& world, const Computations& comps, const LightingModel& lightingModel, int remaining)
{

    // Prevent infinite recursion
    if (remaining <= 0) {
        return black;
    }

    if (equal(comps.object->material.transparency, 0)) {
        return black;
    }

    // Check for total internal reflection:
    // # Find the ratio of first index of refraction to the second.
    // # (Yup, this is inverted from the definition of Snell's Law.)
    const auto n_ratio = comps.n1 / comps.n2;

    // # cos(theta_i) is the same as the dot product of the two vectors
    const auto cos_i = comps.eyev.dot(comps.normalv);

    // # Find sin(theta_t)^2 via trigonometric identity
    const auto sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));

    // Total internal reflection if the value is greater than one!
    if (sin2_t > 1) return black;

    // Refracted Color with a refracted ray

    // # Find cos(theta_t) via trigonometric identity
    const auto cos_t = std::sqrt(1.0F - sin2_t);

    // # Compute the direction of the refracted ray
    const auto direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio;

    // # Create the refracted ray
    const Ray refract_ray{comps.under_point, direction};

    // # Find the color of the refracted ray, making sure to multiply
    // # by the transparency value to account for any opacity
    return color_at(world, refract_ray, lightingModel, remaining - 1) * comps.object->material.transparency;
}

float schlick(const Computations& comps)
{
    // # find the cosine of the angle between the eye and normal vectors
    auto cos = comps.eyev.dot(comps.normalv);

    // # total internal reflection can only occur if n1 > n2
    if (comps.n1 > comps.n2) {
        const auto n = comps.n1 / comps.n2;
        const auto sin2_t = (n * n) * (1 - (cos * cos));

        if (sin2_t > 1) return 1;

        // # compute cosine of theta_t using trig identity
        const auto cos_t = std::sqrt(1 - sin2_t);

        // when n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }

    const auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
    return r0 + (1 - r0) * std::pow((1 - cos), 5);
}

Color lighting(
    const Material& material,
    const Shape* object,
    const Light& light,
    const Point& point,
    const Vector& eyev,
    const Vector& normalv,
    const bool in_shadow,
    const LightingModel& lightingModel
    )
{

    const auto black = Color(0, 0, 0);
    Color ambient;
    Color diffuse;
    Color specular;

    Color materialColor = material.color;

    // Check if the material has a pattern first
    if (material.pattern_ptr) {
        materialColor = pattern_at_shape(material.pattern_ptr, object, point);
    }

    // combine the surface color with the light's color/intensity
    const Color effective_color = materialColor * light.intensity();

    // find the direction to the light source
    const Vector lightv = Vector(light.position() - point).normalize();

    // compute the ambient contribution
    ambient = effective_color * material.ambient;

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    const auto light_dot_normal = lightv.dot(normalv);

    if (light_dot_normal < 0 || in_shadow) {
        diffuse = black;
        specular = black;
    } else {
        // compute the diffuse contribution
        diffuse = effective_color * material.diffuse * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the
        // reflection vector and the eye vector. A negative number means the
        // light reflects away from the eye
        const Vector reflectv = Vector(-lightv).reflect(normalv);
        const auto reflect_dot_eye = reflectv.dot(eyev);

        if (reflect_dot_eye <= 0) {
            specular = black;
        } else {
            // compute the specular contribution

            float factor;

            if (lightingModel == LightingModel::Phong) {
                factor = std::pow(reflect_dot_eye, material.shininess);
            }

            if (lightingModel == LightingModel::BlinnPhong) {
                const auto halfway = Vector(lightv + eyev).normalize();
                const auto halfway_dot = normalv.dot(halfway);

                factor = std::pow(halfway_dot, material.shininess);
            }

            specular = light.intensity() * material.specular * factor;
        }
    }

    // Add the three contributions together to get the final shading
    return ambient + diffuse + specular;
}


std::optional<Intersection> hit(Intersections intersections)
{
    // sort the list of intersections and return the first intersection
    // with a non negative t value
    //
    // if no such value exists, return an empty Intersection

    std::sort(intersections.begin(), intersections.end(), [](
                                                              const Intersection& i1,
                                                              const Intersection& i2) -> bool {
        return i1.t < i2.t;
    });

    for (const Intersection& intersect: intersections) {
        if (intersect.t >= 0) {
            return {intersect};
        }
    }

    return {};
}

}
// end namespace Raytracer::Engine

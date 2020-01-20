#include "vector.h"

float Vector::magnitude() const {
    // aka length
    // Pythagoras to the rescue!
    return std::sqrt((x*x) + (y*y) + (z*z));
}

Vector Vector::normalize() const {
    const float magnitude = this->magnitude();
    return {x / magnitude,
            y / magnitude,
            z / magnitude};
}

float Vector::dot(const Vector& b) const {
    return x * b.x +
           y * b.y +
           z * b.z;
}

Vector Vector::cross(const Vector& b) const {
    return {y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x};
}

Vector Vector::reflect(const Vector& normal) const {
    return {*this - (normal * 2 * dot(normal))};
}

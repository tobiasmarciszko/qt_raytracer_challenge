#ifndef OBJECT_H
#define OBJECT_H

#include <cstdlib>
#include "material.h"

class Object
{
public:
    Object() {
        m_id = std::rand();
    }

    inline int id() const {
        return m_id;
    }

    inline Material material() const {
        return m_material;
    }

    inline void set_material(const Material& m) {
        m_material = m;
    }

    bool operator==(const Object& object) const {
        return object.id() == m_id;
    }

private:
    int m_id;
    Material m_material = Material();
};

#endif // OBJECT_H

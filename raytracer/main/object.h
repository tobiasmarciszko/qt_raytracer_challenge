#ifndef OBJECT_H
#define OBJECT_H

#include <cstdlib>

class Object
{
public:
    Object() {
        m_id = std::rand();
    }

    int id() const {
        return m_id;
    }

    bool operator==(const Object& object) const {
        return object.id() == m_id;
    }

private:
    int m_id;
};

#endif // OBJECT_H

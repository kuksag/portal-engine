#ifndef PORTAL_ENGINE_PRIMITIVES_H
#define PORTAL_ENGINE_PRIMITIVES_H

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "model.h"
#include "shader.h"

class Primitive : public Model {//TODO: don't let user create this
private:
    glm::vec3 color;

public:
    Primitive(const glm::vec3& position, const glm::vec3& color,
              const std::string& path);

    void set_color(glm::vec3 color);
    [[nodiscard]] glm::vec3 get_color() const;
};

struct Sphere : public Primitive {
    explicit Sphere(const glm::vec3& position = {0, 0, 0},
                    const glm::vec3& color = {1, 1, 1});
};

struct Cube : public Primitive {
    Cube(const glm::vec3& position = {0, 0, 0},
         const glm::vec3& color = {1, 1, 1});
};

struct Plane : public Primitive {
    Plane(const glm::vec3& position = {0, 0, 0},
          const glm::vec3& color = {1, 1, 1});
};

struct Cylinder : public Primitive {
    Cylinder(const glm::vec3& position = {0, 0, 0},
             const glm::vec3& color = {1, 1, 1});
};

struct Torus : public Primitive {
    Torus(const glm::vec3& position = {0, 0, 0},
          const glm::vec3& color = {1, 1, 1});
};

struct Cone : public Primitive {
    Cone(const glm::vec3& position = {0, 0, 0},
         const glm::vec3& color = {1, 1, 1});
};

#endif    // PORTAL_ENGINE_PRIMITIVES_H

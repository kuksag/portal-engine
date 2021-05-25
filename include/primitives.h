#ifndef PORTAL_ENGINE_PRIMITIVES_H
#define PORTAL_ENGINE_PRIMITIVES_H

#define PORTAL_ENGINE_PRIMITIVES_COUNT 6

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "model.h"
#include "shader.h"
#include <optional>

class Primitive : public Model {//TODO: don't let user create this
private:
    glm::vec3 color;

public:
    Primitive(const glm::vec3& position, const glm::vec3& color,
              const std::string& path, bool need_load, std::shared_ptr<ShaderProgram> shader);

    void set_unvisible();
    bool is_visible() const;

    void set_color(glm::vec3 color);
    [[nodiscard]] glm::vec3 get_color() const;
};

struct Sphere : Primitive {
    explicit Sphere(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
                    const glm::vec3& color = {1, 1, 1});
};

struct Plane : Primitive {
    Plane(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
          const glm::vec3& color = {1, 1, 1});
    [[nodiscard]] bool crossed(glm::vec3 first_point, glm::vec3 last_point) const;
};

struct Cylinder : Primitive {
    Cylinder(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
             const glm::vec3& color = {1, 1, 1});
};

struct Torus : Primitive {
    Torus(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
          const glm::vec3& color = {1, 1, 1});
};

struct Cone : Primitive {
    Cone(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
         const glm::vec3& color = {1, 1, 1});
};

struct Cube : Primitive {
private:

  std::vector<std::shared_ptr<Plane>> planes;

public:

    Cube(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position = {0, 0, 0},
         const glm::vec3& color = {1, 1, 1});

    const std::vector<std::shared_ptr<Plane>>& get_planes() const;    
};

#endif    // PORTAL_ENGINE_PRIMITIVES_H

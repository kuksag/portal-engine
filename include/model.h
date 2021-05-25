#ifndef PORTAL_ENGINE_MODEL_H
#define PORTAL_ENGINE_MODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "camera.h"
#include "drawable.h"
#include "light_source.h"
#include "mesh.h"
#include "shader.h"

class Model : public Drawable {
private:
    std::vector<Mesh> meshes;

    std::string directory;

    void deep_load_meshes(aiNode *node, const aiScene *scene);
    Mesh load_mesh(aiMesh *ai_mesh, const aiScene *scene);
    std::vector<Texture> load_textures(aiMaterial *ai_material,
                                       aiTextureType type);

    // TODO: move this to resource manager
    Texture load_texture(const char *path);

    void set_matrices(const Camera &camera, const std::vector< std::shared_ptr<LightSource> >& ls) const;

public:
    Model(const std::string &path, std::shared_ptr<ShaderProgram> shader, bool need_load);
    void draw(const Camera &camera, const std::vector< std::shared_ptr<LightSource> >& light_sources) const override;
    void depth_test_draw(const Camera &camera, std::shared_ptr<ShaderProgram> depth_shader) const override;

    void move_to(std::shared_ptr<Model> to);

    ~Model() override = default;
};

#endif    // PORTAL_ENGINE_MODEL_H

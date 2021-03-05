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

class Model : public Drawable {//TODO: don't let user create this
private:
    std::vector<Mesh> meshes;

    std::string directory;

    void deep_load_meshes(aiNode *node, const aiScene *scene);
    Mesh load_mesh(aiMesh *ai_mesh, const aiScene *scene);
    std::vector<Texture> load_textures(aiMaterial *ai_material,
                                       aiTextureType type);

    // TODO: move this to resource manager
    Texture load_texture(const char *path);

public:
    Model(const std::string &path, std::shared_ptr<ShaderProgram> shader);
    void draw(const Camera &camera, const std::vector<LightSource>& light_sources) const override;

    ~Model() override = default;
};

#endif    // PORTAL_ENGINE_MODEL_H

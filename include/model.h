#ifndef PORTAL_ENGINE_MODEL_H
#define PORTAL_ENGINE_MODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <memory>
#include <string>
#include "drawable.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "light_source.h"

class Model : public Drawable
{
private:

    std::vector<Mesh> meshes;

    std::string directory;

    void deep_load_meshes(aiNode *node, const aiScene *scene);
    Mesh load_mesh(aiMesh *ai_mesh, const aiScene *scene);
    std::vector<Texture> load_textures(aiMaterial *ai_material, aiTextureType type);

    Texture load_texture(const char *path); //TODO: move this to resource manager

public:

    Model(const std::string& path, std::shared_ptr<ShaderProgram> shader);
    void draw(const Camera& camera, const std::vector <LightSource> &light_sources) const override;

    virtual ~Model() = default;
};

#endif    // PORTAL_ENGINE_MODEL_H
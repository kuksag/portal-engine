#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "light_source.h"

Model::Model(const std::string &path, std::shared_ptr<ShaderProgram> shader,
             bool need_load)
    : Drawable(std::move(shader)),
      directory(path.substr(0, path.find_last_of('/'))) {
    if (!need_load) return;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString()
                  << std::endl;
        return;
    }

    deep_load_meshes(scene->mRootNode, scene);
}

void Model::depth_test_draw(const Camera &camera,
                            std::shared_ptr<ShaderProgram> depth_shader) const {
    depth_shader->use();
    auto MVP = camera.get_projection_matrix() * camera.get_view_matrix() *
               get_model_matrix();
    glUniformMatrix4fv(shader->get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);
    auto model_matrix = get_model_matrix();
    glUniformMatrix4fv(shader->get_uniform_id("model_matrix"), 1, GL_FALSE,
                       &model_matrix[0][0]);
    for (const auto &i : meshes) i.depth_test_draw(depth_shader);
}

void Model::set_matrices(
    const Camera &camera,
    const std::vector<std::shared_ptr<LightSource> > &light_sources) const {
    shader->use();
    auto model_matrix =
        /*glm::inverse(link ? link->get_model_matrix() : glm::mat4(1)) * */
        get_model_matrix();
    auto MVP = camera.get_projection_matrix() * camera.get_view_matrix() *
               model_matrix;    // TODO: one calculation
    // model_matrix = glm::inverse(link ? link->get_model_matrix() :
    // glm::mat4(1)) * model_matrix;
    auto normal_transformation = glm::mat3(transpose(inverse(model_matrix)));

    glUniformMatrix4fv(shader->get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(shader->get_uniform_id("model_matrix"), 1, GL_FALSE,
                       &model_matrix[0][0]);
    glUniformMatrix3fv(shader->get_uniform_id("normal_transformation"), 1,
                       GL_FALSE, &normal_transformation[0][0]);
    for (std::size_t i = 0; i < light_sources.size(); ++i) {
        if (light_sources[i]->shadowed()) {
            auto light_matrix =
                light_sources[i]->get_camera().get_projection_matrix() *
                light_sources[i]->get_camera().get_view_matrix() * model_matrix;
            glUniformMatrix4fv(shader->get_uniform_id("light_matrix[" +
                                                      std::to_string(i) + "]"),
                               1, GL_FALSE, &light_matrix[0][0]);
        }
    }
}

void Model::draw(
    const Camera &camera,
    const std::vector<std::shared_ptr<LightSource> > &light_sources) const {
    shader->use();
    set_matrices(camera, light_sources);

    auto set_vec3 = [](GLuint id,
                       glm::vec3 data) {    // TODO: make ShaderProgram method
        glUniform3f(id, data.x, data.y, data.z);
    };
    set_vec3(shader->get_uniform_id("camera_pos"), camera.get_position());
    glUniform1i(shader->get_uniform_id("count_of_light_sources"),
                light_sources.size());

    for (std::size_t i = 0; i < light_sources.size(); ++i) {
        std::stringstream position_uniform_name;
        position_uniform_name << "light_sources[" << i << "].position";
        set_vec3(shader->get_uniform_id(position_uniform_name.str()),
                 light_sources[i]->get_position());

        std::stringstream color_uniform_name;
        color_uniform_name << "light_sources[" << i << "].color";
        set_vec3(shader->get_uniform_id(color_uniform_name.str()),
                 light_sources[i]->get_color());

        std::stringstream intensity_uniform_name;
        intensity_uniform_name << "light_sources[" << i << "].intensity";
        glUniform1f(shader->get_uniform_id(intensity_uniform_name.str()),
                    light_sources[i]->intensity());

        if (light_sources[i]->shadowed()) {
            std::stringstream depth_map_uniform_name;
            depth_map_uniform_name << "depth_map" << i << "";
            glUniform1i(shader->get_uniform_id(depth_map_uniform_name.str()),
                        i + 1);
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_2D, light_sources[i]->get_depth_map());
        }

        std::stringstream is_shadowed_uniform_fs_name;
        is_shadowed_uniform_fs_name << "is_shadowed_fs[" << i << "]";
        glUniform1i(shader->get_uniform_id(is_shadowed_uniform_fs_name.str()),
                    light_sources[i]->shadowed() ? 1 : 0);
    }

    for (const auto &i : meshes) i.draw();
}

void Model::deep_load_meshes(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(load_mesh(ai_mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        deep_load_meshes(node->mChildren[i], scene);
}

Mesh Model::load_mesh(aiMesh *ai_mesh, const aiScene *scene) {
    std::vector<Vertex> vertexes;
    std::vector<GLuint> indexes;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < ai_mesh->mNumVertices; i++) {
        Vertex vertex{};
        vertex.Position.x = ai_mesh->mVertices[i].x;
        vertex.Position.y = ai_mesh->mVertices[i].y;
        vertex.Position.z = ai_mesh->mVertices[i].z;

        if (ai_mesh->HasNormals()) {
            vertex.Normal.x = ai_mesh->mNormals[i].x;
            vertex.Normal.y = ai_mesh->mNormals[i].y;
            vertex.Normal.z = ai_mesh->mNormals[i].z;
        }

        if (ai_mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = ai_mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = ai_mesh->mTextureCoords[0][i].y;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertexes.push_back(vertex);
    }

    for (GLuint i = 0; i < ai_mesh->mNumFaces; i++)
        for (GLuint j = 0; j < 3; j++)
            indexes.push_back(ai_mesh->mFaces[i].mIndices[j]);

    aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps =
        load_textures(ai_material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return Mesh(vertexes, indexes, textures, shader);
}

std::vector<Texture> Model::load_textures(aiMaterial *ai_material,
                                          aiTextureType type) {
    std::vector<Texture> textures;    // TODO: resourse manager
    for (unsigned int i = 0; i < ai_material->GetTextureCount(type); i++) {
        aiString ai_texture_name;
        ai_material->GetTexture(type, i, &ai_texture_name);
        std::string path = ai_texture_name.C_Str();
        path = this->directory + '/' + path;
        textures.push_back(load_texture(path.c_str()));
    }

    return textures;
};

Texture Model::load_texture(const char *path) {
    Texture texture;
    glGenTextures(1, &texture.id);

    int width, height;
    unsigned char *data =
        SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    if (data) {
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else
        std::cout << "Texture failed to load at path: " << path << std::endl;

    SOIL_free_image_data(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void Model::move_to(std::shared_ptr<Model> to) {
    set_model_matrix(to->get_model_matrix());
}
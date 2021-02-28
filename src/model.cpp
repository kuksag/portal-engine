#include "model.h"

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "light_source.h"
#include "sstream"

Model::Model(const std::string& path, std::shared_ptr<ShaderProgram> shader) : Drawable(shader), directory(path.substr(0, path.find_last_of('/'))) {

	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

	deep_load_meshes(scene->mRootNode, scene);
}

void Model::draw(const Camera& camera, const std::vector <LightSource> &light_sources) const {
    auto MVP = camera.get_projection_matrix() * camera.get_view_matrix() * get_model_matrix(); //TODO: one calculation
    auto normal_transformation = glm::mat3(transpose(inverse(get_model_matrix())));
    auto model_matrix = glm::mat3(get_model_matrix());
    glUniformMatrix4fv(shader->get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);


    try {
        glUniformMatrix3fv(shader->get_uniform_id("model_matrix"), 1, GL_FALSE,
                           &model_matrix[0][0]);
    } catch (...) {
    }


    try {
        glUniformMatrix3fv(shader->get_uniform_id("normal_transformation"), 1,
                           GL_FALSE, &normal_transformation[0][0]);
    } catch (...) {
    }

    try {
        glUniform3f(shader->get_uniform_id("camera_pos"),
                    camera.get_position().x, camera.get_position().y,
                    camera.get_position().z);
    } catch(...) {
    }

    //Установление всех источников света
    for (std::size_t i = 0; i < light_sources.size(); ++i) {
        std::stringstream pos_uniform_name;
        pos_uniform_name << "light_pos" << i;
        glm::vec3 pos = light_sources[i].get_pos();
        glUniform3f(shader->get_uniform_id(pos_uniform_name.str()), pos.x, pos.y, pos.z);

        std::stringstream color_uniform_name;
        color_uniform_name<< "light_color" << i;
        glm::vec3 color = light_sources[i].get_color();
        glUniform3f(shader->get_uniform_id(color_uniform_name.str()), color.x, color.y, color.z);
    }

    for (const auto& i : meshes)
		i.draw();
}

void Model::deep_load_meshes(aiNode *node, const aiScene *scene) {
	
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(load_mesh(ai_mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        deep_load_meshes(node->mChildren[i], scene);
}

Mesh Model::load_mesh(aiMesh *ai_mesh, const aiScene *scene)
{
    std::vector<Vertex> vertexes;
    std::vector<GLuint> indexes;
    std::vector<Texture> textures;

    for(GLuint i = 0; i < ai_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = ai_mesh->mVertices[i].x;
        vertex.Position.y = ai_mesh->mVertices[i].y;
        vertex.Position.z = ai_mesh->mVertices[i].z;

        if (ai_mesh->HasNormals())
        {
            vertex.Normal.x = ai_mesh->mNormals[i].x;
            vertex.Normal.y = ai_mesh->mNormals[i].y;
            vertex.Normal.z = ai_mesh->mNormals[i].z;
        }

        if(ai_mesh->mTextureCoords[0])
        {
            vertex.TexCoords.x = ai_mesh->mTextureCoords[0][i].x; 
            vertex.TexCoords.y = ai_mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertexes.push_back(vertex);
    }

    for(GLuint i = 0; i < ai_mesh->mNumFaces; i++)
        for(GLuint j = 0; j < 3; j++)
            indexes.push_back(ai_mesh->mFaces[i].mIndices[j]);        

    aiMaterial* ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];    

    std::vector<Texture> diffuseMaps = load_textures(ai_material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return Mesh(vertexes, indexes, textures, shader);
}

std::vector<Texture> Model::load_textures(aiMaterial *ai_material, aiTextureType type) {
    std::vector<Texture> textures; //TODO: resourse manager
    for(unsigned int i = 0; i < ai_material->GetTextureCount(type); i++)
    {
        aiString ai_texture_name;
        ai_material->GetTexture(type, i, &ai_texture_name);
        std::string path = ai_texture_name.C_Str();
        path = this->directory + '/' + path;
        textures.push_back(load_texture(path.c_str()));
    }

    return textures;
};


Texture Model::load_texture(const char *path)
{
    Texture texture;
    glGenTextures(1, &texture.id);

    int width, height;
    unsigned char *data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    if (data)
    {
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at path: " << path << std::endl;

    SOIL_free_image_data(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
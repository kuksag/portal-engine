#include "model.h"

#include <iostream>
#include <vector>

Model::Model(const std::string& path, std::shared_ptr<ShaderProgram> shader) : shader(shader), directory(path.substr(0, path.find_last_of('/'))) {

	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

	deep_load_meshes(scene->mRootNode, scene);
}

void Model::draw() const {
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

    // aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];    

    // vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return Mesh(vertexes, indexes, textures, shader);
}

//     vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
//     {
//         vector<Texture> textures;
//         for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
//         {
//             aiString str;
//             mat->GetTexture(type, i, &str);
//             // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//             bool skip = false;
//             for(GLuint j = 0; j < textures_loaded.size(); j++)
//             {
//                 if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//                 {
//                     textures.push_back(textures_loaded[j]);
//                     skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//                     break;
//                 }
//             }
//             if(!skip)
//             {   // if texture hasn't been loaded already, load it
//                 Texture texture;
//                 texture.id = TextureFromFile(str.C_Str(), this->directory);
//                 texture.type = typeName;
//                 texture.path = str.C_Str();
//                 textures.push_back(texture);
//                 textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//             }
//         }
//         return textures;
//     }
// };


// GLuint TextureFromFile(const char *path, const string &directory, bool gamma)
// {
//     string filename = string(path);
//     filename = directory + '/' + filename;

//     GLuint textureID;
//     glGenTextures(1, &textureID);

//     int width, height;
//     filename = "res/textures/container.jpeg";
//     unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
//     if (data)
//     {
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     }
//     else
//     {
//         std::cout << "Texture failed to load at path: " << filename << std::endl;
//     }

//     SOIL_free_image_data(data);
//     glBindTexture(GL_TEXTURE_2D, 0);

//     return textureID;
// }
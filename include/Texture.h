#ifndef PORTAL_ENGINE_TEXTURE_H
#define PORTAL_ENGINE_TEXTURE_H

#include <GL/glew.h>

#include <string>

struct Texture {
private:
    GLuint texture_id;
public:
    explicit Texture(const std::string &path_to_texture);
    void bind() const;
    void unbind() const;
};

#endif    // PORTAL_ENGINE_TEXTURE_H

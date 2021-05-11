#include "light_source.h"

#include "drawable.h"

glm::vec3 LightSource::get_color() const { return color; }

glm::vec3 LightSource::get_position() const {
    return glm::vec3(get_model_matrix() * glm::vec4(glm::vec3(), 1.0f));
}

LightSource::LightSource(glm::vec3 position_, glm::vec3 color_)
    : color(color_) {
    translate(position_);
    camera.set_view_matrix(glm::lookAt(position_,
                                       glm::vec3(0.0f, 0.0f, 0.0f) - position_,
                                       glm::vec3(0.0f, 1.0f, 0.0f)));
    init_depth_map();
}

const Camera &LightSource::get_camera() const { return camera; }

GLuint LightSource::get_depth_map() const { return depth_map; }
void LightSource::init_depth_map() {
    glGenFramebuffers(1, &depth_map_fbo);
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = {0.3f, 0.3f, 0.3f, 0.3f};
    glTexParameterfv(GL_TEXTURE_2D, GL_CONVOLUTION_BORDER_COLOR, border_color);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void LightSource::gen_depth_map(const std::vector<Drawable *> &drawables) {
    static const std::shared_ptr<ShaderProgram> depth_shader{
        new ShaderProgram("shaders/depth.vertex", "shaders/depth.fragment")};
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (auto &drawable : drawables) {
        drawable->depth_test_draw(camera, depth_shader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

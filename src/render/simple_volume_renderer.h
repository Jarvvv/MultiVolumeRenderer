//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_SIMPLE_VOLUME_RENDERER_H
#define VOLUMERENDER_SIMPLE_VOLUME_RENDERER_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<sv/Render/VolumeRenderer.h>
#include<data/simple_volume_manager.h>
#include<sv/Render/Shader.h>
#include<sv/Control/Controller.h>
#include<string>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#define TF_TEXTURE_BINDING 0
#define PREINT_TF_TEXTURE_BINDING 1
#define ENTRYPOS_TEXTURE_BINDING 2
#define EXITPOS_TEXTURE_BINDING 3
#define VOLUMEDATA_TEXTURE_BINDING 4
#define ENTRYPOS_IMAGE_BINDING 0
#define EXITPOS_IMAGE_BINDING 1


#define RAYCAST_POS_FRAG "C:/Users/vr/projects/VolumeRenderer/src/render/shader/raycast_pos_f.glsl"
#define RAYCAST_POS_VERT "C:/Users/vr/projects/VolumeRenderer/src/render/shader/raycast_pos_v.glsl"
#define RAYCASTING_FRAG "C:/Users/vr/projects/VolumeRenderer/src/render/shader/raycasting_f.glsl"
#define RAYCASTING_VERT "C:/Users/vr/projects/VolumeRenderer/src/render/shader/raycasting_v.glsl"

/**
 * raw volume data renderer, no accelerate
 */
class SimpleVolumeRenderer: public IVolumeRenderer{
public:
    SimpleVolumeRenderer()
    :window_width(1200),window_height(900)
    {
        initGL();
        volume_manager=std::make_unique<SimpleVolumeManager>();
        raycastpos_shader=std::make_unique<sv::Shader>(RAYCAST_POS_VERT,RAYCAST_POS_FRAG);
        raycasting_shader=std::make_unique<sv::Shader>(RAYCASTING_VERT,RAYCASTING_FRAG);
    }
    void setupVolume(const char* file_path) override ;
    void setupTransferFunc(std::map<uint8_t,std::array<double,4>> color_setting) override;
    void init() override;
    void render() override;
    void setupController() override;
public:
    void initGL();
    void setupProxyCube(GLfloat x,GLfloat y,GLfloat z);
    void setupScreenQuad();
    void setupRaycastPosFramebuffer();
    void setupShaderUniform();
public:
    void deleteGLResource();
    ~SimpleVolumeRenderer();
private:
    /**
     * GL texture
     */
    GLuint transfer_func_tex;
    GLuint preInt_tf_tex;
    GLuint entrypos_tex;
    GLuint exitpos_tex;
    GLuint volume_tex;
    /**
     * proxy cube
     */
    GLuint proxy_cube_vao;
    GLuint proxy_cube_vbo;
    GLuint proxy_cube_ebo;
    std::array<std::array<GLfloat,3>,8> proxy_cube_vertices;
    std::array<GLuint,36> proxy_cube_vertex_indices;
    GLfloat proxy_cube_x_len,proxy_cube_y_len,proxy_cube_z_len;
    /**
     * screen quad
     */
    GLuint screen_quad_vao;
    GLuint screen_quad_vbo;
//      GLuint screen_quad_ebo;//no need to use
    std::array<GLfloat,24> screen_quad_vertices;//6 vertices for x y and u v
    /**
     * framebuffer for record raycast entry and exit pos
     */
    GLuint raycastpos_fbo;
    GLuint raycastpos_rbo;
    /**
     * glsl shader
     */
    std::unique_ptr<sv::Shader> raycastpos_shader;
    std::unique_ptr<sv::Shader> raycasting_shader;
    /**
     * glfw window resource
     */
    GLFWwindow *window;
    uint32_t window_width,window_height;
};

#endif //VOLUMERENDER_SIMPLE_VOLUME_RENDERER_H

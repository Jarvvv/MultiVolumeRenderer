//
// Created by wyz on 20-12-4.
//
#include"block_volume_renderer.h"
#include<utils/help_gl.h>
#include<data/block_volume_manager.h>
#include<cudaGL.h>
BlockVolumeRenderer::BlockVolumeRenderer()
        :window_width(1200),window_height(900)
{
    volume_manager=std::make_unique<BlockVolumeManager>();
    camera=std::make_unique<sv::RayCastOrthoCamera>(glm::vec3(1.f,1.f,1.f),window_width/2,window_height/2);
}
void BlockVolumeRenderer::setupVolume(const char *file_path)
{
    volume_manager->setupVolumeData(file_path);

}

void BlockVolumeRenderer::setupTransferFunc(std::map<uint8_t, std::array<double, 4>> color_setting)
{

}

void BlockVolumeRenderer::init()
{
    initGL();
    initCUDA();
    setupController();

    createVirtualBoxes();

}

std::function<void(GLFWwindow*,float)> process_input;

void BlockVolumeRenderer::render()
{
    GL_CHECK
    float last_frame_t=0.f;
    while(!glfwWindowShouldClose(window)){

        float current_frame_t=glfwGetTime();
        float delta_t=current_frame_t-last_frame_t;
        last_frame_t=current_frame_t;

        //1.event process
        glfwPollEvents();
        process_input(window,delta_t);
        auto obb=camera->getOBB();
        updateCurrentBlocks(obb);

        //2.calculate new pass render needing blocks
        //sort these blocks by distance to viewport
        volume_manager->setupBlockReqInfo();
        //3.update blocks: uncompress and loading
        //multi-thread uncompress and copy from device to cuda array
        //if a block is waiting for loading but turn to no need now, stop its task
        //while copying from device to texture, device can't be accessed by other thread or task
        while(volume_manager->getBlock()) {


            copyDeviceToTexture();
            //4.update current blocks' status: cached and empty; update block map table
            //update every 16ms(fixed time interval)

        }

        //5.render a frame
        //ray stop if sample at an empty block
    }

}



void BlockVolumeRenderer::setupVolumeTexture()
{

    assert(view_depth_level<8);
    assert(volume_texes.size()==view_depth_level);
    for(size_t i=0;i<view_depth_level;i++){
        glGenTextures(1,&volume_texes[i]);
        glBindTexture(GL_TEXTURE_3D,volume_texes[i]);
        glBindTextureUnit(i+2,volume_texes[i]);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTextureStorage3D(volume_texes[i],1,GL_R8,window_width,window_height,block_length);

        CUDA_DRIVER_API_CALL(cuGraphicsGLRegisterImage(&cu_resources[i], volume_texes[i],GL_TEXTURE_3D, CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD));

    }
}

void BlockVolumeRenderer::deleteGLTexture()
{
    for(size_t i=0;i<volume_texes.size();i++){
        CUDA_DRIVER_API_CALL(cuGraphicsUnregisterResource(cu_resources[i]));
    }
    glDeleteTextures(volume_texes.size(),volume_texes.data());
}

void BlockVolumeRenderer::initCUDA()
{
    CUDA_DRIVER_API_CALL(cuInit(0));
    CUdevice cuDevice=0;
    CUDA_DRIVER_API_CALL(cuDeviceGet(&cuDevice, 0));
    CUDA_DRIVER_API_CALL(cuCtxCreate(&cu_context,0,cuDevice));
}
/**
 * index, CUdeviceptr
 */
void BlockVolumeRenderer::copyDeviceToTexture() {

    std::array<uint32_t,3> tex_pos_index;

    CUarray cu_array;
    CUDA_MEMCPY3D m = { 0 };
    m.srcMemoryType=CU_MEMORYTYPE_DEVICE;

    m.dstMemoryType=CU_MEMORYTYPE_ARRAY;
    m.dstArray=cu_array;


}

void BlockVolumeRenderer::initGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(window_width, window_height, "Volume Render", NULL, NULL);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
}

std::function<void(GLFWwindow *window, int width, int height)> framebuffer_resize_callback;
void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height){
    framebuffer_resize_callback(window,width,height);
}
std::function<void(GLFWwindow* window, int button, int action, int mods)> mouse_button_callback;
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    mouse_button_callback(window,button,action,mods);
}
std::function<void(GLFWwindow *window, double xpos, double ypos)> mouse_move_callback;
void glfw_mouse_move_callback(GLFWwindow *window, double xpos, double ypos){
    mouse_move_callback(window,xpos,ypos);
}
std::function<void(GLFWwindow *window, double xoffset, double yoffset)> scroll_callback;
void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    scroll_callback(window,xoffset,yoffset);
}
std::function<void(GLFWwindow *window, int key, int scancode, int action, int mods)> keyboard_callback;
void glfw_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    keyboard_callback(window,key,scancode,action,mods);
}

void BlockVolumeRenderer::setupController()
{
    static bool clicked;
    framebuffer_resize_callback=[&](GLFWwindow *window, int width, int height){
        std::cout<<__FUNCTION__ <<std::endl;
    };
    mouse_button_callback=[&](GLFWwindow* window, int button, int action, int mods){
        if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS){
            print("mouse button left pressed");
        }
        else if(button==GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS){
            print("mouse button right pressed");
        }
    };
    mouse_move_callback=[&](GLFWwindow *window, double xpos, double ypos){
        if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){

        }
        else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
            camera->processMouseMove(xpos,ypos);
        }
    };
    scroll_callback=[&](GLFWwindow *window, double xoffset, double yoffset){
        camera->processMouseScroll(yoffset);
    };
    keyboard_callback=[&](GLFWwindow *window, int key, int scancode, int action, int mods){
        if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }


    };
    glfwSetFramebufferSizeCallback(window,glfw_framebuffer_resize_callback);
    glfwSetMouseButtonCallback(window,glfw_mouse_button_callback);
    glfwSetCursorPosCallback(window,glfw_mouse_move_callback);
    glfwSetScrollCallback(window,glfw_scroll_callback);
    glfwSetKeyCallback(window,glfw_keyboard_callback);

    process_input=[&](GLFWwindow *window,float delta_time){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::FORWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::BACKWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::RIGHT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::UP, delta_time);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera->processMovementByKey(sv::CameraMoveDirection::DOWN, delta_time);
    };
}

void BlockVolumeRenderer::updateCurrentBlocks(const sv::OBB &view_box)
{
    auto aabb=view_box.getAABB();
    std::unordered_set<sv::AABB,Myhash> current_intersect_blocks;
    for(auto& it:virtual_blocks){
        if(aabb.intersect(it)){
            current_intersect_blocks.insert(it);
        }
    }
    assert(new_need_blocks.size()==0);
    assert(no_need_blocks.size()==0);
    for(auto& it:current_intersect_blocks){
        if(current_blocks.find(it)==current_blocks.cend()){
            new_need_blocks.insert(it);
        }
    }
    for(auto& it:current_blocks){
        if(current_intersect_blocks.find(it)==current_intersect_blocks.cend()){
            no_need_blocks.insert(it);
        }
    }
    current_blocks=std::move(current_intersect_blocks);

    for(auto& it:volume_tex_manager){
        auto t=sv::AABB(it.block_index);
        //not find
        if(current_blocks.find(t)==current_blocks.cend()){
            it.valid=false;
        }
        else{
            assert(it.valid==true);
        }
    }


}

void BlockVolumeRenderer::createVirtualBoxes()
{
    for(uint32_t z=0;z<dim[2];z++){
        for(uint32_t y=0;y<dim[1];y++){
            for(uint32_t x=0;x<dim[0];x++){
                virtual_blocks.emplace_back(glm::vec3(x*block_length,y*block_length,z*block_length),
                                            glm::vec3((x+1)*block_length,(y+1)*block_length,(z+1)*block_length),
                                            std::array<uint32_t,3>{x,y,z});
            }
        }
    }

}

/**
 * call after getting volume textures size
 */
void BlockVolumeRenderer::createVolumeTexManager() {
    assert(view_depth_level==volume_texes.size());
    for(uint32_t i=0;i<vol_tex_block_nx;i++){
        for(uint32_t j=0;j<vol_tex_block_ny;j++){
            for(uint32_t k=0;k<view_depth_level;k++){
                BlockTableItem item;
                item.pos_index={i,j,k};
                item.block_index={INVALID,INVALID,INVALID};
                item.valid=false;
                item.cached=false;
                volume_tex_manager.push_back(std::move(item));
            }
        }
    }
}

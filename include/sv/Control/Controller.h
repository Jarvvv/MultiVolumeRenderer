//
// Created by wyz on 20-12-8.
//

#ifndef VOLUMERENDERER_CONTROLLER_H
#define VOLUMERENDERER_CONTROLLER_H
#include<sv/Control/Camera.h>
#include<GLFW/glfw3.h>
#include<functional>
namespace sv{
    using FramebufferSize_Callback=std::function<void(void*,int,int)>;
    using CursorPos_Callback=std::function<void(void*,double,double)> ;
    using Scroll_Callback= std::function<void(void*,double,double)> ;
    using Keyboard_Callback= std::function<void(void*,int,int,int,int)> ;
    using MouseButton_Callback= std::function<void(void*,int,int,int)> ;
    using Process_Input=std::function<void(void*)> ;
    class Controller{
    public:
        Controller()=delete;
        static void FramebufferSizeCallback(GLFWwindow*,int,int);
        static void CursorPosCallback(GLFWwindow*,double,double);
        static void ScrollCallback(GLFWwindow*,double,double);
        static void KeyCallback(GLFWwindow*,int,int,int,int);
        static void MouseButtonCallback(GLFWwindow*,int,int,int);
        static void processInput(GLFWwindow*);
        static sv::Camera& getCamera(){ return camera; }

        static FramebufferSize_Callback framebufferresize_event;
        static CursorPos_Callback cursorpos_event;
        static Scroll_Callback scroll_event;
        static Keyboard_Callback keyboard_event;
        static MouseButton_Callback mousebutton_event;
        static Process_Input processinput_event;

    public:
        static float last_frame;
        static float delta_time;
    private:
        static sv::Camera camera;
        static bool first_mouse;
        static double last_x;
        static double last_y;

    };

}


#endif //VOLUMERENDERER_CONTROLLER_H

//
// Created by wyz on 20-12-8.
//

#ifndef VOLUMERENDERER_CAMERA_H
#define VOLUMERENDERER_CAMERA_H
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<math.h>
#include<iostream>
#include<sv/Utils/boundingbox.h>
namespace sv{
    enum class CameraDefinedKey{
        MOVE_FASTER,MOVE_SLOWER,
        ROTATE_FASTER,ROTATE_SLOWER,
        FAR,NEAR
    };
    enum class CameraMoveDirection{
        FORWARD,BACKWARD,
        LEFT,RIGHT,
        UP,DOWN
    };
    class Camera{

    public:
        Camera(glm::vec3 camera_pos):
        pos(camera_pos),up(glm::vec3(0.0f,1.0f,0.0f)),
        front(glm::vec3(0.0f,0.0f,-1.0f)),
//        right(glm::vec3(1.0f,0.0f,0.0f)),
        world_up(glm::vec3(0.0f,1.0f,0.0f)),
        yaw(-90.0f),pitch(0.0f),
        move_speed(0.03f),
        mouse_sensitivity(0.1f),
        zoom(20.0f)
        {
            updateCameraVectors();
        }

        glm::mat4 getViewMatrix();
        void processMovementByKeyboard(CameraMoveDirection direction,float delta_t);
        void processMouseMovement(float xoffset,float yoffset);
        void processMouseScroll(float yoffset);
        void processKeyboardForArgs(CameraDefinedKey arg);
        void updateCameraVectors();

    private:
        glm::vec3 pos;//point
        glm::vec3 front;//vector
        glm::vec3 up;//vector
        glm::vec3 right;//vector

        glm::vec3 world_up;//vector

        float yaw;
        float pitch;

        float move_speed;
        float mouse_sensitivity;
        float zoom;
    public:
        float getZoom(){return zoom;}
    };
    inline glm::mat4 Camera::getViewMatrix()
    {
//        std::cout<<pos.x<<" "<<pos.y<<" "<<pos.z<<std::endl;
//        std::cout<<front.x<<" "<<front.y<<" "<<front.z<<std::endl;
//        std::cout<<up.x<<" "<<up.y<<" "<<up.z<<std::endl;
        return glm::lookAt(pos,pos+front,up);
    }
    inline void Camera::processMovementByKeyboard(CameraMoveDirection direction,float delta_t)
    {

        float ds=move_speed*delta_t;
        switch (direction) {
            case CameraMoveDirection::FORWARD: pos+=front*ds;break;
            case CameraMoveDirection::BACKWARD: pos-=front*ds;break;
            case CameraMoveDirection::LEFT: pos-=right*ds;break;
            case CameraMoveDirection::RIGHT: pos+=right*ds;break;
            case CameraMoveDirection::UP: pos+=up*ds;break;
            case CameraMoveDirection::DOWN: pos-=up*ds;break;
        }
    }
    inline void Camera::processMouseMovement(float xoffset,float yoffset)
    {
        yaw+=xoffset*mouse_sensitivity;
        pitch+=yoffset*mouse_sensitivity;
        if(pitch>60.0f)
            pitch=60.0f;
        if(pitch<-60.0f)
            pitch=-60.0f;
        updateCameraVectors();
    }
    inline void Camera::processMouseScroll(float yoffset)
    {
        zoom-=yoffset;
        if(zoom<0.1f)
            zoom=0.1f;
        if(zoom>45.0f)
            zoom=45.0f;
    }
    inline void Camera::processKeyboardForArgs(CameraDefinedKey arg)
    {
        if(arg==CameraDefinedKey::MOVE_FASTER)
            move_speed*=2;
        else if(arg==CameraDefinedKey::MOVE_SLOWER)
            move_speed/=2;
        else if(arg==CameraDefinedKey::ROTATE_FASTER)
            mouse_sensitivity*=2;
        else if(arg==CameraDefinedKey::ROTATE_SLOWER)
            mouse_sensitivity/=2;
    }
    inline void Camera::updateCameraVectors()
    {
        glm::vec3 f;
        f.x=std::cos(glm::radians(yaw))*std::cos(glm::radians(pitch));
        f.y=std::sin(glm::radians(pitch));
        f.z=std::sin(glm::radians(yaw))*std::cos(glm::radians(pitch));
        front=glm::normalize(f);
        right=glm::normalize(glm::cross(front,world_up));
        up=glm::normalize(glm::cross(right,front));
    }

    class RayCastCamera{
    public:
        RayCastCamera()=default;
        virtual void updateCameraVectors()=0;
    protected:
        glm::vec3 view_pos;
        glm::vec3 view_direction;//keep unit
        glm::vec3 up,right;//keep unit
        glm::vec3 world_up;//keep unit
        float n,f;

        float yaw,pitch;

        float move_speed;
        float move_sense;


    };
    class RayCastOrthoCamera: public RayCastCamera{
    public:
        RayCastOrthoCamera(glm::vec3 view_pos,uint32_t half_w,uint32_t half_h):
        RayCastCamera(),half_x_n(half_w),half_y_n(half_h)
        {
            this->view_pos=view_pos;
            this->world_up=glm::vec3(0.f,1.f,0.f);
            this->yaw=-90.f;
            this->pitch=0.f;
            this->move_speed=0.03f;
            this->move_sense=0.05f;
            this->space_x=this->space_y=1.f;

            this->n=0.f;//assert!!!
            this->f=256.f;

            updateCameraVectors();
        }
        OBB getOBB(){
            assert(this->n==0.f);
            auto center_pos=view_pos+view_direction*(f+n)/2.f;
            return OBB(center_pos,right,up,view_direction,half_x_n*space_x,half_y_n*space_y,(f+n)/2.f);
        }
        void setupOBB(OBB& obb){
            assert(this->n==0.f);
            obb.center_pos=view_pos+view_direction*(f+n)/2.f;;
            obb.unit_x=right;
            obb.unit_y=up;
            obb.unit_z=view_direction;
            obb.half_wx=half_x_n*space_x;
            obb.half_wy=half_y_n*space_y;
            obb.half_wz=(n+f)/2.f;
            obb.updateHalfLenVec();
        }

        void processMovementByKey(CameraMoveDirection direction,float delta_t);
        void processMouseMove(float xoffset,float yoffset);
        void processMouseScroll(float yoffset);
        void processKeyForArg(CameraDefinedKey arg);
        void updateCameraVectors() override;
    private:
        float space_x,space_y;//x-direction and y-direction gap distance between two rays
        uint32_t half_x_n, half_y_n;


    };

    inline void RayCastOrthoCamera::processMovementByKey(CameraMoveDirection direction, float delta_t) {
        float ds=move_speed*delta_t;
        switch (direction) {
            case CameraMoveDirection::FORWARD: view_pos+=view_direction*ds;break;
            case CameraMoveDirection::BACKWARD: view_pos-=view_direction*ds;break;
            case CameraMoveDirection::LEFT: view_pos-=right*ds;break;
            case CameraMoveDirection::RIGHT: view_pos+=right*ds;break;
            case CameraMoveDirection::UP: view_pos+=up*ds;break;
            case CameraMoveDirection::DOWN: view_pos-=up*ds;break;
        }
    }

    inline void RayCastOrthoCamera::processMouseMove(float xoffset, float yoffset) {
        yaw+=xoffset*move_sense;
        pitch+=yoffset*move_sense;
        if(pitch>60.0f)
            pitch=60.0f;
        if(pitch<-60.0f)
            pitch=-60.0f;
        updateCameraVectors();
    }

    inline void RayCastOrthoCamera::processMouseScroll(float yoffset) {
        if(yoffset>0){
            space_x*=1.05;
            space_y*=1.05;
        }
        else{
            space_x*=0.95;
            space_y*=0.95;
        }
    }

    inline void RayCastOrthoCamera::processKeyForArg(CameraDefinedKey arg) {
        if(arg==CameraDefinedKey::MOVE_FASTER)
            move_speed*=2;
        else if(arg==CameraDefinedKey::MOVE_SLOWER)
            move_speed/=2;
        else if(arg==CameraDefinedKey::ROTATE_FASTER)
            move_sense*=2;
        else if(arg==CameraDefinedKey::ROTATE_SLOWER)
            move_sense/=2;
        else if(arg==CameraDefinedKey::FAR)
            f*=1.1f;
        else if(arg==CameraDefinedKey::NEAR)
            f*=0.9f;
    }

    inline void RayCastOrthoCamera::updateCameraVectors() {
        glm::vec3 f;
        f.x=std::cos(glm::radians(yaw))*std::cos(glm::radians(pitch));
        f.y=std::sin(glm::radians(pitch));
        f.z=std::sin(glm::radians(yaw))*std::cos(glm::radians(pitch));
        view_direction=glm::normalize(f);
        right=glm::normalize(glm::cross(view_direction,world_up));
        up=glm::normalize(glm::cross(right,view_direction));
    }










    class RayCastPerspectCamera: public RayCastCamera{

    };
}

#endif //VOLUMERENDERER_CAMERA_H

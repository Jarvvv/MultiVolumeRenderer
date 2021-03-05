//
// Created by wyz on 20-12-10.
//
#include<glad/glad.h>
#include<sv/Control/Controller.h>

#include <mpi.h>
#include <memory>

namespace sv {
    sv::Camera sv::Controller::camera = sv::Camera(glm::vec3(0.5f, 0.5f, 1.5f));
    bool sv::Controller::first_mouse = true;
    double sv::Controller::last_x;
    double sv::Controller::last_y;
    float sv::Controller::last_frame = 0.0f;
    float sv::Controller::delta_time = 0.0f;

    FramebufferSize_Callback sv::Controller::framebufferresize_event=FramebufferSize_Callback();
    CursorPos_Callback sv::Controller::cursorpos_event=CursorPos_Callback();
    Scroll_Callback sv::Controller::scroll_event=Scroll_Callback();
    Keyboard_Callback sv::Controller::keyboard_event=Keyboard_Callback();
    MouseButton_Callback sv::Controller::mousebutton_event=MouseButton_Callback();
    Process_Input sv::Controller::processinput_event=Process_Input();


    void sv::Controller::FramebufferSizeCallback(GLFWwindow *window, int w, int h) {
        glViewport(0, 0, w, h);
    }

    void sv::Controller::CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        if (first_mouse) {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }
        double dx = xpos - last_x;
        double dy = last_y - ypos;

        last_x = xpos;
        last_y = ypos;

        // camera.processMouseMovement(dx, dy);
    }

    void sv::Controller::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        // camera.processMouseScroll(yoffset);
    }

    void sv::Controller::KeyCallback(GLFWwindow *, int, int, int, int) {

    }

    void sv::Controller::MouseButtonCallback(GLFWwindow *, int, int, int) {

    }

    void sv::Controller::processInput(GLFWwindow *window, int world_rank, int world_size) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        InputType operation = InputType::NONE;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            operation = InputType::MOVE_FORWARD;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            operation = InputType::MOVE_BACKWARD;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            operation = InputType::MOVE_LEFT;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            operation = InputType::MOVE_RIGHT;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            operation = InputType::MOVE_UP;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            operation = InputType::MOVE_DOWN;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            operation = InputType::ZOOM_UP;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            operation = InputType::ZOOM_DOWN;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            operation = InputType::TURN_UP;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            operation = InputType::TURN_DOWN;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            operation = InputType::TURN_LEFT;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            operation = InputType::TURN_RIGHT;

        static std::unique_ptr<InputType> inputs;
        if (world_rank == 0 && !inputs) {
            inputs = std::unique_ptr<InputType>(new InputType[world_size]);
        }

        MPI_Gather(&operation, 1, MPI_INT, inputs.get(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(world_rank == 0) {
            for(int i = 0; i < world_size; i++) {
                camera.processInput(*(inputs.get() + i), delta_time);
            }
        }

        MPI_Bcast(&camera.pos[0], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&camera.front[0], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&camera.zoom, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

}
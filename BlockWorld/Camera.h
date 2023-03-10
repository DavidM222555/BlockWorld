#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "World.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

enum Mouse_Movement {
    LEFT_MOUSE,
    RIGHT_MOUSE,
};


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

const float JUMP_CONSTANT = 0.15f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, World& world)
    {
        float velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD) {
            auto possibleNewPosition = Position + glm::vec3(Front.x * velocity, 0, Front.z * velocity);

            if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
                    && !world.collidesOnY(possibleNewPosition, Position)) {
                Position += glm::vec3(Front.x * velocity, 0, Front.z * velocity);
            }

        }

        if (direction == BACKWARD) {
            auto possibleNewPosition = Position - glm::vec3(Front.x * velocity, 0, Front.z * velocity);

            if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
                    && !world.collidesOnY(possibleNewPosition, Position)) {
                Position -= glm::vec3(Front.x * velocity, 0, Front.z * velocity);

            }

        }

        if (direction == LEFT) {
            auto possibleNewPosition = Position - glm::vec3(Right.x * velocity, 0, Right.z * velocity);

            if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
                    && !world.collidesOnY(possibleNewPosition, Position)) {
                Position -= glm::vec3(Right.x * velocity, 0, Right.z * velocity);

            }

        }

        if (direction == RIGHT) {
            auto possibleNewPosition = Position + glm::vec3(Right.x * velocity, 0, Right.z * velocity);

            if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
                    && !world.collidesOnY(possibleNewPosition, Position)) {
                Position += glm::vec3(Right.x * velocity, 0, Right.z * velocity);
            }

        }

        if (direction == UP) {
            auto possibleNewPosition = Position + glm::vec3(0, Up.y * velocity + JUMP_CONSTANT, 0);

            if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
                    && !world.collidesOnY(possibleNewPosition, Position)) {
                Position += glm::vec3(0, Up.y * velocity + JUMP_CONSTANT, 0);
            }
        }

        if (direction == DOWN) {
            auto possibleNewPosition = Position - glm::vec3(0, Up.y * velocity, 0);

            //if (!world.collidesOnX(possibleNewPosition, Position) && !world.collidesOnZ(possibleNewPosition, Position)
            //        && !world.collidesOnY(possibleNewPosition, Position)) {
            //    Position -= glm::vec3(0, Up.y * velocity, 0);
            //}

            Position -= glm::vec3(0, Up.y * velocity, 0);
        }
    }

    void processMouse(Mouse_Movement mm)
    {
        if (mm == LEFT) 
        {
            std::cout << "Handling left mouse click \n";
            // handleBlockDestruction(Position, Front);
        }

    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    glm::vec3 getPosition() 
    {
        return Position;
    }

    void changePositionX(float dx)
    {
        Position.x += dx;
    }

    void changePositionY(float dy) 
    {
        Position.y += dy;
    }

    void changePositionZ(float dz)
    {
        Position.z += dz;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));

        // std::cout << "Front.x: " << front.x << " Front.y: " << front.y << " Front.z: " << front.z << "\n";
    }
};

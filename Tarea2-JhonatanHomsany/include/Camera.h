#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement { 
    FORWARD, 
    BACKWARD, 
    LEFT, 
    RIGHT, 
    UP, 
    DOWN 
};

class Camera{
    private: 
        glm::vec3 position;
        float yaw;
        float pitch;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 front;
        glm::vec3 worldUp;
        float fov;
        float nearPlane;
        float farPlane;
        float movementSpeed;
        float mouseSensitivity;
        
        void updateVectors();

    public: 
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = -20.0f);
        ~Camera();

        //Getters
        glm::vec3 getPosition() const {return position;}
        float getYaw() const {return yaw;}
        float getPitch() const {return pitch;}
        glm::vec3 getUpVector() const {return up;}
        glm::vec3 getRightVector() const {return right;}
        glm::vec3 getFrontVector() const {return front;}
        glm::vec3 getWorldUp() const {return worldUp;}
        float getFov() const {return fov;}
        float getNearPlane() const {return nearPlane;}
        float getFarPlane() const {return farPlane;}
        //Setters
        void setPosition(glm::vec3 position) {this->position = position;}
        void setWorldUp(glm::vec3 worldUp) {this->worldUp = worldUp;}
        void setFov(float fov) {this->fov = fov;}
        void setNearPlane(float nearPlane) {this->nearPlane = nearPlane;}
        void setFarPlane(float farPlane) {this->farPlane = farPlane;}
        void setMovementSpeed(float movementSpeed) {this->movementSpeed = movementSpeed;}
        void setMouseSensitivity(float mouseSensitivity) {this->mouseSensitivity = mouseSensitivity;}
        //Funciones de camara
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspect) const;
        void movement(CameraMovement direction, float deltaTime);
        void rotate(float xOffset, float yOffset);
        void zoom(float yOffset);
};
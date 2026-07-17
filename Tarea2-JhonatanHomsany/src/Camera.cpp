#include "../include/Camera.h"
#include <cmath>
#include <iostream>

using namespace std;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : position(position), worldUp(worldUp), yaw(yaw), pitch(pitch),
      fov(45.0f), nearPlane(0.1f), farPlane(1000.0f),
      movementSpeed(2.5f), mouseSensitivity(0.1f)
{
    updateVectors();
}

Camera::~Camera(){

}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::movement(CameraMovement direction, float deltaTime){
    float velocity = movementSpeed * deltaTime;
    if(direction == CameraMovement::FORWARD){
        position += front * velocity;
    }
    if(direction == CameraMovement::BACKWARD){
        position -= front * velocity;
    }
    if(direction == CameraMovement::LEFT){
        position -= right * velocity;
    }
    if(direction == CameraMovement::RIGHT){
        position += right * velocity;
    }
    if(direction == CameraMovement::UP){
        position += worldUp * velocity;
    }
    if(direction == CameraMovement::DOWN){
        position -= worldUp * velocity;
    }
}

void Camera::rotate(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::zoom(float yOffset){
    fov -= yOffset;
    if (fov < 1.0f)  fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
}

void Camera::updateVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
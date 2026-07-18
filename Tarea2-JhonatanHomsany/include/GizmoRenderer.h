#pragma once
#include <glm/glm.hpp>

class Shader;

class GizmoRenderer{
    public:
        Shader* shader;
        bool showAxis;
        bool showBoundingBox;
        unsigned int VAO, VBO;

        GizmoRenderer();
        ~GizmoRenderer();

        void init();
        void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);
        void cleanup();
};
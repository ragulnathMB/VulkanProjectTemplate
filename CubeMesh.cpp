#include "CubeMesh.hpp"

CubeMesh::CubeMesh(VkDevice device, VkPhysicalDevice physicalDevice)
    : Mesh(device, physicalDevice,
        // 🟦 Vertices
        {
            // Front face
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}}, // 0
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // 1
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 2
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}}, // 3

            // Back face
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}}, // 4
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}}, // 5
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}, // 6
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}}, // 7
        },
        // 🟨 Indices
        {
            // front
            0, 1, 2, 2, 3, 0,
            // right
            1, 5, 6, 6, 2, 1,
            // back
            5, 4, 7, 7, 6, 5,
            // left
            4, 0, 3, 3, 7, 4,
            // bottom
            4, 5, 1, 1, 0, 4,
            // top
            3, 2, 6, 6, 7, 3
        }
    ) {}

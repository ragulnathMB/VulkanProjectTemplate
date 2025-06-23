#pragma once

#include "Mesh.hpp"

class CubeMesh : public Mesh {
public:
    CubeMesh(VkDevice device, VkPhysicalDevice physicalDevice);
};

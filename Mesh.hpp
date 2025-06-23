#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include "VulkanBuffer.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    static VkVertexInputBindingDescription getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

class Mesh {
public:
    Mesh(VkDevice device,
        VkPhysicalDevice physicalDevice,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices);

    ~Mesh();

    void bind(VkCommandBuffer commandBuffer) const;
    void draw(VkCommandBuffer commandBuffer) const;

private:
    VkDevice device;
    VulkanBuffer* vertexBuffer;
    VulkanBuffer* indexBuffer;

    uint32_t indexCount;
};

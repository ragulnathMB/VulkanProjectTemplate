#include "Mesh.hpp"
#include <cstring>

// Vertex input binding for Vulkan pipeline
VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription binding{};
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return binding;
}

// Describe layout of Vertex attributes (e.g., position)
std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attrs(2); // ← change to 2

    attrs[0].binding = 0;
    attrs[0].location = 0;
    attrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[0].offset = offsetof(Vertex, position);

    attrs[1].binding = 0;
    attrs[1].location = 1;
    attrs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[1].offset = offsetof(Vertex, color); // ← make sure Vertex has `color`

    return attrs;
}


// Constructor: Upload vertex and index data to GPU buffers
Mesh::Mesh(VkDevice device,
    VkPhysicalDevice physicalDevice,
    const std::vector<Vertex>& vertices,
    const std::vector<uint32_t>& indices)
    : device(device), indexCount(static_cast<uint32_t>(indices.size())) {

    VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();
    vertexBuffer = new VulkanBuffer(
        device, physicalDevice,
        vertexSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    vertexBuffer->copyData(vertices.data(), vertexSize);

    VkDeviceSize indexSize = sizeof(indices[0]) * indices.size();
    indexBuffer = new VulkanBuffer(
        device, physicalDevice,
        indexSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    indexBuffer->copyData(indices.data(), indexSize);
}

// Destructor
Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}

// Bind vertex and index buffers for drawing
void Mesh::bind(VkCommandBuffer commandBuffer) const {
    VkBuffer vertexBuf = vertexBuffer->getBuffer();
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuf, &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

// Issue draw command for the mesh
void Mesh::draw(VkCommandBuffer commandBuffer) const {
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
}

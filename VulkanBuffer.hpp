#pragma once

#include <vulkan/vulkan.h>

class VulkanBuffer {
public:
    VulkanBuffer(VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties);

    ~VulkanBuffer();

    VkBuffer getBuffer() const { return buffer; }
    VkDeviceMemory getMemory() const { return bufferMemory; }

    // For updating buffer contents (e.g., uniform data)
    void copyData(const void* srcData, VkDeviceSize size);

private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

    void createBuffer(VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

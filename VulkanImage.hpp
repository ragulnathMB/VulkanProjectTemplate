#pragma once

#include <vulkan/vulkan.h>

class VulkanImage {
public:
    VulkanImage(VkDevice device,
        VkPhysicalDevice physicalDevice,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties);

    ~VulkanImage();

    VkImage getImage() const { return image; }
    VkDeviceMemory getMemory() const { return imageMemory; }

    VkImageView createImageView(VkFormat format, VkImageAspectFlags aspectFlags);

private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;

    void createImage(uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

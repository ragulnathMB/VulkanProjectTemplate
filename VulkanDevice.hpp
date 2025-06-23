#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

class VulkanDevice {
public:
    // Constructor: initializes device and queues
    VulkanDevice(VkInstance instance, VkSurfaceKHR surface);

    // Destructor: cleans up logical device
    ~VulkanDevice();

    // Accessors
    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
    VkQueue getGraphicsQueue() const { return graphicsQueue; }
    VkQueue getPresentQueue() const { return presentQueue; }
	// get index of graphics queue family
    uint32_t getGraphicsQueueFamilyIndex() const {
        return queueIndices.graphicsFamily.value();
    }



    // Utility to find suitable memory type (used when creating buffers/images)
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

private:
    // Vulkan handles
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    VkSurfaceKHR surface;
    VkInstance instance;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices queueIndices;  // ✅ Cached result after device selection

    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device); // still private
    void createLogicalDevice();

};

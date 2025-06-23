#pragma once

#include <vulkan/vulkan.h>

class VulkanRenderPass {
public:
    VulkanRenderPass(VkDevice device, VkFormat swapchainImageFormat);
    ~VulkanRenderPass();

    VkRenderPass get() const { return renderPass; }

private:
    VkDevice device;
    VkRenderPass renderPass = VK_NULL_HANDLE;

    void createRenderPass(VkFormat swapchainImageFormat);
};

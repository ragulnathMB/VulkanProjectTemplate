#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanFramebuffer {
public:
    VulkanFramebuffer(VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& swapchainImageViews,
        VkExtent2D extent);

    ~VulkanFramebuffer();

    const std::vector<VkFramebuffer>& getFramebuffers() const { return framebuffers; }

private:
    VkDevice device;
    std::vector<VkFramebuffer> framebuffers;

    void createFramebuffers(VkRenderPass renderPass,
        const std::vector<VkImageView>& swapchainImageViews,
        VkExtent2D extent);
};

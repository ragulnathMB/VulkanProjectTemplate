#include "VulkanFramebuffer.hpp"
#include <stdexcept>

// Constructor: create framebuffers for each swapchain image view
VulkanFramebuffer::VulkanFramebuffer(VkDevice device,
    VkRenderPass renderPass,
    const std::vector<VkImageView>& swapchainImageViews,
    VkExtent2D extent)
    : device(device) {
    createFramebuffers(renderPass, swapchainImageViews, extent);
}

// Destructor: destroy all framebuffers
VulkanFramebuffer::~VulkanFramebuffer() {
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

// Create framebuffers — one per swapchain image view
void VulkanFramebuffer::createFramebuffers(VkRenderPass renderPass,
    const std::vector<VkImageView>& swapchainImageViews,
    VkExtent2D extent) {
    framebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
        VkImageView attachments[] = {
            swapchainImageViews[i] // One color attachment (no depth yet)
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}

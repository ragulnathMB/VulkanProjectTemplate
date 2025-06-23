#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Mesh.hpp"
#include "Camera.hpp"

class VulkanCommand {
public:
    VulkanCommand(VkDevice device,
        uint32_t queueFamilyIndex,
        VkRenderPass renderPass,
        VkExtent2D extent,
        const std::vector<VkFramebuffer>& framebuffers,
        VkPipeline pipeline);

    ~VulkanCommand();

    const std::vector<VkCommandBuffer>& getCommandBuffers() const { return commandBuffers; }
    VkCommandBuffer getCommandBuffer(uint32_t index) const;

    // ✅ Add per-frame recording
    void recordCommandBuffer(uint32_t imageIndex, Mesh* mesh, Camera* camera,
        VkPipelineLayout layout, VkDescriptorSet descriptorSet);



private:
    VkDevice device;
    VkRenderPass renderPass;
    VkExtent2D extent;
    std::vector<VkFramebuffer> framebuffers;
    VkPipeline pipeline;
    std::vector<VulkanBuffer*> uniformBuffers;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;


    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    void createCommandPool(uint32_t queueFamilyIndex);
    void allocateCommandBuffers(size_t count);
};

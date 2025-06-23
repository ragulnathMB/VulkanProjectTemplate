#include "VulkanCommand.hpp"
#include <stdexcept>

VulkanCommand::VulkanCommand(VkDevice device,
    uint32_t queueFamilyIndex,
    VkRenderPass renderPass,
    VkExtent2D extent,
    const std::vector<VkFramebuffer>& framebuffers,
    VkPipeline pipeline)
    : device(device),
    renderPass(renderPass),
    extent(extent),
    framebuffers(framebuffers),
    pipeline(pipeline) {

    createCommandPool(queueFamilyIndex);
    allocateCommandBuffers(framebuffers.size());
}

VulkanCommand::~VulkanCommand() {
    vkDestroyCommandPool(device, commandPool, nullptr);
}

void VulkanCommand::createCommandPool(uint32_t queueFamilyIndex) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

void VulkanCommand::allocateCommandBuffers(size_t count) {
    commandBuffers.resize(count);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

VkCommandBuffer VulkanCommand::getCommandBuffer(uint32_t index) const {
    return commandBuffers.at(index);
}

void VulkanCommand::recordCommandBuffer(
    uint32_t imageIndex,
    Mesh* mesh,
    Camera* camera,
    VkPipelineLayout pipelineLayout,
    VkDescriptorSet descriptorSet)
{
    VkCommandBuffer cmd = commandBuffers[imageIndex];

    // --- Begin command buffer recording ---
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    // --- Begin render pass ---
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearColor = { {{ 0.1f, 0.1f, 0.1f, 1.0f }} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // --- Bind the graphics pipeline ---
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    // --- Bind the descriptor set (for uniforms like MVP matrices) ---
    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0,                  // firstSet
        1,                  // descriptorSetCount
        &descriptorSet,
        0, nullptr          // dynamic offset count & values
    );

    // --- Bind mesh buffers (vertex + index) and issue draw command ---
    mesh->bind(cmd);
    mesh->draw(cmd);

    // --- End render pass and command buffer ---
    vkCmdEndRenderPass(cmd);

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

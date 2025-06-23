#include "VulkanSync.hpp"
#include <stdexcept>

// Constructor: create semaphores and fences
VulkanSync::VulkanSync(VkDevice device, size_t maxFramesInFlight)
    : device(device), maxFrames(maxFramesInFlight) {
    createSyncObjects();
}

// Destructor: cleanup sync objects
VulkanSync::~VulkanSync() {
    for (size_t i = 0; i < maxFrames; ++i) {
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }
}

// Create Vulkan semaphores and fences
void VulkanSync::createSyncObjects() {
    imageAvailableSemaphores.resize(maxFrames);
    renderFinishedSemaphores.resize(maxFrames);
    inFlightFences.resize(maxFrames);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Start signaled so first frame doesn't wait

    for (size_t i = 0; i < maxFrames; ++i) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create synchronization objects!");
        }
    }
}

// Wait for fence of a given frame
void VulkanSync::waitForFrame(size_t frameIndex) const {
    vkWaitForFences(device, 1, &inFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
}

// Reset fence after usage
void VulkanSync::resetFence(size_t frameIndex) const {
    vkResetFences(device, 1, &inFlightFences[frameIndex]);
}

// Accessors
VkSemaphore VulkanSync::getImageAvailableSemaphore(size_t frameIndex) const {
    return imageAvailableSemaphores[frameIndex];
}

VkSemaphore VulkanSync::getRenderFinishedSemaphore(size_t frameIndex) const {
    return renderFinishedSemaphores[frameIndex];
}

VkFence VulkanSync::getInFlightFence(size_t frameIndex) const {
    return inFlightFences[frameIndex];
}

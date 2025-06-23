#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanSync {
public:
    VulkanSync(VkDevice device, size_t maxFramesInFlight);
    ~VulkanSync();

    VkSemaphore getImageAvailableSemaphore(size_t frameIndex) const;
    VkSemaphore getRenderFinishedSemaphore(size_t frameIndex) const;
    VkFence getInFlightFence(size_t frameIndex) const;

    void waitForFrame(size_t frameIndex) const;
    void resetFence(size_t frameIndex) const;

private:
    VkDevice device;
    size_t maxFrames;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    void createSyncObjects();
};

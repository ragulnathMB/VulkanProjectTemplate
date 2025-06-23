#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanWindow.hpp"
#include "VulkanInstance.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanCommand.hpp"
#include "VulkanSync.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "Mesh.hpp"
#include "VulkanBuffer.hpp"

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;


class VulkanContext {
public:
    VulkanContext(uint32_t width, uint32_t height, const char* title);
    ~VulkanContext();
    void run();

private:
    void init();
    void drawFrame();
    void cleanup();

    uint32_t width, height;
    const char* title;
    uint32_t currentFrame = 0;

    GLFWwindow* window;

    VulkanWindow* vulkanWindow;
    VulkanInstance* instance;
    VulkanDevice* device;
    VulkanSwapchain* swapchain;
    VulkanRenderPass* renderPass;
    VulkanFramebuffer* framebuffer;
    VulkanPipeline* pipeline;
    VulkanCommand* command;
    VulkanSync* sync;
    Camera* camera;
    Timer* timer;
    Mesh* mesh;

    // 🔥 Added for Uniform Buffers and Descriptor Sets
    std::vector<VulkanBuffer*> uniformBuffers;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

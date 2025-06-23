#include "VulkanContext.hpp"
#include "Input.hpp"
#include "Utils.hpp"
#include "CubeMesh.hpp"
#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

struct MVP {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

VulkanContext::VulkanContext(uint32_t w, uint32_t h, const char* t)
    : width(w), height(h), title(t) {
    init();
}

VulkanContext::~VulkanContext() {
    cleanup();
}

void VulkanContext::init() {
    LOG_INFO("Initializing Vulkan Context...");

    vulkanWindow = new VulkanWindow(width, height, title);
    window = vulkanWindow->getGLFWWindow();
    Input::init(window);

    instance = new VulkanInstance(true);
    vulkanWindow->createAndGetSurface(instance->getInstance());

    device = new VulkanDevice(instance->getInstance(), vulkanWindow->getSurface());
    swapchain = new VulkanSwapchain(device->getPhysicalDevice(), device->getDevice(), vulkanWindow->getSurface(), width, height);
    renderPass = new VulkanRenderPass(device->getDevice(), swapchain->getImageFormat());
    framebuffer = new VulkanFramebuffer(device->getDevice(), renderPass->get(), swapchain->getImageViews(), swapchain->getExtent());

    pipeline = new VulkanPipeline(
        device->getDevice(),
        swapchain->getExtent(),
        renderPass->get(),
        "vert.spv",
        "frag.spv"
    );

    // Create Uniform Buffers
    VkDeviceSize bufferSize = sizeof(MVP);
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        uniformBuffers[i] = new VulkanBuffer(
            device->getDevice(),
            device->getPhysicalDevice(),
            bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
    }

    // Create Descriptor Pool
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK(vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool));

    // Create Descriptor Sets
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, pipeline->getDescriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    VK_CHECK(vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()));

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i]->getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
    }

    command = new VulkanCommand(
        device->getDevice(),
        device->getGraphicsQueueFamilyIndex(),
        renderPass->get(),
        swapchain->getExtent(),
        framebuffer->getFramebuffers(),
        pipeline->get()
    );

    sync = new VulkanSync(device->getDevice(), MAX_FRAMES_IN_FLIGHT);
    camera = new Camera(45.0f, width / (float)height, 0.1f, 100.0f);
    timer = new Timer();
    mesh = new CubeMesh(device->getDevice(), device->getPhysicalDevice());

    LOG_INFO("Vulkan Context Initialized.");
}

void VulkanContext::run() {
    while (!vulkanWindow->shouldClose()) {
        vulkanWindow->pollEvents();
        timer->update();
        drawFrame();
    }

    vkDeviceWaitIdle(device->getDevice());
}

void VulkanContext::drawFrame() {
    size_t frameIndex = currentFrame;

    sync->waitForFrame(frameIndex);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        device->getDevice(),
        swapchain->getSwapchain(),
        UINT64_MAX,
        sync->getImageAvailableSemaphore(frameIndex),
        VK_NULL_HANDLE,
        &imageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        LOG_WARN("Swapchain out of date — skipping frame.");
        return;
    }

    sync->resetFence(frameIndex);
    float elapsedTime = timer->getTimeSinceStart();
    float angle = glm::radians(elapsedTime * 45.0f); // 45 degrees/sec

    // Update uniform buffer (MVP)
    MVP mvp{};
    mvp.model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp.view = camera->getViewMatrix();
    mvp.proj = camera->getProjectionMatrix();
    mvp.proj[1][1] *= -1.0f; // Vulkan NDC

    uniformBuffers[frameIndex]->copyData(&mvp, sizeof(MVP));

    command->recordCommandBuffer(
        imageIndex,
        mesh,
        camera,
        pipeline->getLayout(),
        descriptorSets[frameIndex]
    );

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { sync->getImageAvailableSemaphore(frameIndex) };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    VkCommandBuffer cmd = command->getCommandBuffer(imageIndex);
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    VkSemaphore signalSemaphores[] = { sync->getRenderFinishedSemaphore(frameIndex) };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VK_CHECK(vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, sync->getInFlightFence(frameIndex)));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapchains[] = { swapchain->getSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        LOG_WARN("Swapchain suboptimal or outdated — recreate needed");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanContext::cleanup() {
    LOG_INFO("Cleaning up...");

    vkDeviceWaitIdle(device->getDevice());

    for (auto buf : uniformBuffers) {
        delete buf;
    }

    vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);

    delete mesh;
    delete camera;
    delete timer;
    delete sync;
    delete command;
    delete pipeline;
    delete framebuffer;
    delete renderPass;
    delete swapchain;
    delete device;
    

    // 🔽 Manually destroy surface if not handled in VulkanWindow
    vkDestroySurfaceKHR(instance->getInstance(), vulkanWindow->getSurface(), nullptr);

    delete vulkanWindow;
    delete instance;

    glfwTerminate();
}

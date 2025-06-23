#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class VulkanPipeline {
public:
    VulkanPipeline(VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        const std::string& vertShaderPath,
        const std::string& fragShaderPath);

    ~VulkanPipeline();

    VkPipeline get() const { return pipeline; }
    VkPipelineLayout getLayout() const { return pipelineLayout; }
    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VkDevice device;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

    std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createGraphicsPipeline(VkExtent2D extent,
        VkRenderPass renderPass,
        const std::string& vertShaderPath,
        const std::string& fragShaderPath);
};

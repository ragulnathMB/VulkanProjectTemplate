#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class VulkanInstance {
public:
    VulkanInstance(bool enableValidation);
    ~VulkanInstance();

    VkInstance getInstance() const { return instance; }

private:
    VkInstance instance = VK_NULL_HANDLE;
    bool validationEnabled;

    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    void createInstance();

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
};



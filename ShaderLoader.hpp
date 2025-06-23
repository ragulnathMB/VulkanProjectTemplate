#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class ShaderLoader {
public:
    // Reads a SPIR-V shader file and returns binary code
    static std::vector<char> readSPIRV(const std::string& filename);

    // Creates a VkShaderModule from SPIR-V code
    static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};


#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

// ===== Logging Macros =====
#define LOG_INFO(msg)    std::cout << "[INFO]  " << msg << std::endl
#define LOG_WARN(msg)    std::cout << "[WARN]  " << msg << std::endl
#define LOG_ERROR(msg)   std::cerr << "[ERROR] " << msg << std::endl

// ===== Vulkan Error Checker =====
// Use this to wrap Vulkan calls and check results
#define VK_CHECK(call) \
    do { \
        VkResult result = (call); \
        if (result != VK_SUCCESS) { \
            std::cerr << "[VK_ERROR] " << #call << " failed with code " << result << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (0)

// ===== File Reading Utility =====
// Reads a file fully into a string
inline std::string readTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + filename);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Reads binary file into vector<char> (useful for shaders)
inline std::vector<char> readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + filename);

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

// ===== Debug Macro =====
// Print current line and file
#define DEBUG_POINT() \
    std::cout << "[DEBUG] " << __FILE__ << ":" << __LINE__ << std::endl

// ===== Safe Release =====
// Destroys Vulkan object if it's not null
#define SAFE_DESTROY(handle, destroyFn, device) \
    if (handle != VK_NULL_HANDLE) { \
        destroyFn(device, handle, nullptr); \
        handle = VK_NULL_HANDLE; \
    }

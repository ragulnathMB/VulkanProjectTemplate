#include "VulkanWindow.hpp"
#include <stdexcept>
#include <iostream>

// Constructor: create GLFW window and initialize members
VulkanWindow::VulkanWindow(uint32_t width, uint32_t height, const std::string& title)
    : width(width), height(height), title(title) {

    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    // Tell GLFW not to use OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create the actual window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }

    std::cout << "GLFW window created successfully.\n";
}

// Destructor: clean up
VulkanWindow::~VulkanWindow() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

// Create a Vulkan surface from GLFW window
VkSurfaceKHR VulkanWindow::createAndGetSurface(VkInstance instance) {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan window surface!");
    }
	VulkanWindow::setSurface(surface);
}


// Check if the window should close (user pressed close)
bool VulkanWindow::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// Poll window/input events (must be called every frame)
void VulkanWindow::pollEvents() const {
    glfwPollEvents();
}

// Get window width
uint32_t VulkanWindow::getWidth() const {
    return width;
}

// Get window height
uint32_t VulkanWindow::getHeight() const {
    return height;
}

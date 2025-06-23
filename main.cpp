#include "VulkanContext.hpp"
#include <iostream>

int main() {
    try {
        VulkanContext app(800, 600, "Silhouette Carving 3D Viewer");
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

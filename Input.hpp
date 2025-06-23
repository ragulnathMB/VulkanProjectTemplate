#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class Input {
public:
    static void init(GLFWwindow* window);

    static void pollEvents(); // Must be called every frame

    static bool isKeyPressed(int key);
    static double getMouseDeltaX();
    static double getMouseDeltaY();

private:
    static inline GLFWwindow* window = nullptr;
    static inline std::unordered_map<int, bool> keyStates;

    static inline double lastMouseX = 0.0;
    static inline double lastMouseY = 0.0;
    static inline double deltaX = 0.0;
    static inline double deltaY = 0.0;

    static void updateMouse();
};

#include "Input.hpp"

// Initialize input system
void Input::init(GLFWwindow* win) {
    window = win;
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
}

// Call this once per frame
void Input::pollEvents() {
    glfwPollEvents();

    // Update key states
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        keyStates[key] = (glfwGetKey(window, key) == GLFW_PRESS);
    }

    // Update mouse position delta
    updateMouse();
}

// Track relative mouse movement
void Input::updateMouse() {
    double currentX, currentY;
    glfwGetCursorPos(window, &currentX, &currentY);

    deltaX = currentX - lastMouseX;
    deltaY = currentY - lastMouseY;

    lastMouseX = currentX;
    lastMouseY = currentY;
}

// Query if key is currently pressed
bool Input::isKeyPressed(int key) {
    return keyStates[key];
}

// Get mouse movement since last frame
double Input::getMouseDeltaX() {
    return deltaX;
}

double Input::getMouseDeltaY() {
    return deltaY;
}

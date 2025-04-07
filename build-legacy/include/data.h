#ifndef DATA_H
#define DATA_H

#include "utils.h"

// Window dimensions - changed to variables instead of constants
int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

// Delay for frame rate control
int delay = 1000/240; // ~60 FPS

// Default texture path
std::string texturePath = "textures/test.png";

// Current shader paths
std::string currentVertexPath = "shaders/shader1/vertex.glsl";
std::string currentFragmentPath = "shaders/shader1/fragment.glsl";

// Set up vertex data (a quad made of two triangles)
float vertices[] = {
    // positions        // texture coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  // top left
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f   // bottom left
};


// indices are used to draw the two triangles that make up the quad
unsigned int indices[] = {
    0, 1, 2,  // first triangle
    0, 2, 3   // second triangle
};

// Set the colors (for example, pink and blue)
float colorValues[] = {
    1.0f, 0.0f, 1.0f,  // Pink   (colors[0])
    0.0f, 0.0f, 1.0f,  // Blue   (colors[1])
    0.0f, 1.0f, 1.0f,  // Cyan   (colors[2])
    1.0f, 1.0f, 0.0f,  // *   (colors[3])
    1.0f, 0.0f, 1.0f,  // *   (colors[4])
    0.0f, 1.0f, 1.0f   // *   (colors[5])
};

// Automatic reload settings
bool autoReloadEnabled = false;
Uint32 lastReloadTime = 0;
const Uint32 RELOAD_INTERVAL = 1000; // Reload every 1 second when auto-reload is enabled



#endif // DATA_H
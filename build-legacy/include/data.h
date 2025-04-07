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
    0.0f, 0.0f, 1.0f,  // blue
    0.0f, 1.0f, 0.0f,  // green
    0.0f, 1.0f, 1.0f,  // cyan
    1.0f, 0.0f, 0.0f,  // red
    1.0f, 0.0f, 1.0f,  // magenta
    1.0f, 1.0f, 0.0f,  // yellow
    1.0f, 1.0f, 1.0f,  // white
    0.0f, 0.0f, 0.0f   // black
};

// Number of circles to draw
const int numCircles = 100;

// Circle structure
struct CircleCoord {
    float x = std::rand() % WINDOW_WIDTH;      // X position
    float y = std::rand() % WINDOW_HEIGHT;     // Y position
    float r = std::rand() % 50 + 10;           // R Radius
};

// Mouse click tracking
bool mouseClicked = false;
float clickX = 0.5f;
float clickY = 0.5f;
float clickTime = 0.0f;

// Automatic reload settings
bool autoReloadEnabled = false;
Uint32 lastReloadTime = 0;
const Uint32 RELOAD_INTERVAL = 1000; // Reload every 1 second when auto-reload is enabled

#endif // DATA_H
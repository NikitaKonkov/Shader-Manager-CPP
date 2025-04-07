#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "data.h"

// Function to toggle auto-reload
void toggleAutoReload() {
    autoReloadEnabled = !autoReloadEnabled;
    std::cout << "Auto-reload " << (autoReloadEnabled ? "enabled" : "disabled") << std::endl;
}

// Function to handle window resize
void handleResize(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    
    // Update the OpenGL viewport to match the new window size
    glViewport(0, 0, width, height);
    
    std::cout << "Window resized to: " << width << "x" << height << std::endl;
}

// Function to load and compile shaders
GLuint loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    GLuint shaderProgram = createShaderProgram(vertexPath, fragmentPath);
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program!" << std::endl;
    }
    return shaderProgram;
}

// Function to reload the current shader
GLuint reloadCurrentShader(GLuint currentProgram) {
    // Delete the current program
    if (currentProgram != 0) {
        glDeleteProgram(currentProgram);
    }
    
    // Load and compile the updated shader
    std::cout << "Reloading shaders from: " << currentVertexPath << " and " << currentFragmentPath << std::endl;
    GLuint newProgram = loadShaders(currentVertexPath, currentFragmentPath);
    
    if (newProgram == 0) {
        std::cerr << "Shader reload failed! Keeping previous shader." << std::endl;
        // If reload fails, try to recreate the original shader
        newProgram = loadShaders(currentVertexPath, currentFragmentPath);
    } else {
        std::cout << "Shader reloaded successfully!" << std::endl;
    }
    
    return newProgram;
}

// Function to load a texture from file
GLuint loadTexture(const std::string& path) {
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return 0;
    }
    
    // Load image
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return 0;
    }
    
    // Create texture from surface pixels
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Determine format
    GLenum format;
    if (surface->format->BytesPerPixel == 4) {
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    } else if (surface->format->BytesPerPixel == 3) {
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    } else {
        std::cerr << "Warning: Image is not truecolor!" << std::endl;
        format = GL_RGB;
    }
    
    // Upload pixels to texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    
    // Free surface
    SDL_FreeSurface(surface);
    
    return textureID;
}


#endif // SHADER_MANAGER_H
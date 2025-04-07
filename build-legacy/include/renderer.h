#ifndef RENDERER_H
#define RENDERER_H

#include "shader_manager.h"


// Forward declarations of helper functions
void updateAttributeLocations(GLuint shaderProgram, GLuint VAO, GLint& posAttrib, GLint& texAttrib);
void updateUniformLocations(GLuint shaderProgram, GLint& millisLoc, GLint& backgroundLoc, 
                           GLint& colorsLocation, GLint& circlesLocation);

int renderer() {
    //----------------------------------------------------------------------
    // Initialization
    //----------------------------------------------------------------------
    
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Set OpenGL attributes - Request compatibility profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Create window with resizable flag
    SDL_Window* window = SDL_CreateWindow(
        "GLSL Shader Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "GLEW could not be initialized! Error: " << glewGetErrorString(glewError) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    //----------------------------------------------------------------------
    // Resource Loading
    //----------------------------------------------------------------------
    
    // Load background texture
    GLuint backgroundTexture = loadTexture(texturePath);
    if (backgroundTexture == 0) {
        std::cerr << "Failed to load background texture: " << texturePath << std::endl;
        // Continue without texture
    }
    
    // Load initial shader program
    GLuint shaderProgram = loadShaders(currentVertexPath, currentFragmentPath);
    if (shaderProgram == 0) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    //----------------------------------------------------------------------
    // Circle Data Initialization
    //----------------------------------------------------------------------
    
    // Structure to hold circle data (x, y, radius)
    struct CircleData {
        float x;
        float y;
        float radius;
    };
    
    // Create and initialize circles with random positions and sizes
    std::vector<CircleData> circles;
    circles.reserve(numCircles);
    
    for (int i = 0; i < numCircles; i++) {
        CircleData circle;
        circle.x = static_cast<float>(rand() % WINDOW_WIDTH);
        circle.y = static_cast<float>(rand() % WINDOW_HEIGHT);
        circle.radius = static_cast<float>(rand() % 5 + 1);
        circles.push_back(circle);
    }
    
    //----------------------------------------------------------------------
    // OpenGL Setup
    //----------------------------------------------------------------------
    
    // Create VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind VAO
    glBindVertexArray(VAO);
    
    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Get attribute locations
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    GLint texAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
    
    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    
    // Texture coordinate attribute
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);
    
    // Unbind VAO
    glBindVertexArray(0);
    
    // Get uniform locations
    GLint millisLoc = glGetUniformLocation(shaderProgram, "millis");
    GLint backgroundLoc = glGetUniformLocation(shaderProgram, "background");
    GLint colorsLocation = glGetUniformLocation(shaderProgram, "colors");
    GLint circlesLocation = glGetUniformLocation(shaderProgram, "circles");
    
    // Initialize viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //----------------------------------------------------------------------
    // Main Loop
    //----------------------------------------------------------------------
    
    bool quit = false;
    SDL_Event e;
    
    // For timing
    Uint32 startTime = SDL_GetTicks();
    
    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_1:
                        // Switch to shader 1
                        currentVertexPath = "shaders/shader1/vertex.glsl";
                        currentFragmentPath = "shaders/shader1/fragment.glsl";
                        shaderProgram = reloadCurrentShader(shaderProgram);
                        updateUniformLocations(shaderProgram, millisLoc, backgroundLoc, colorsLocation, circlesLocation);
                        break;
                    case SDLK_2:
                        // Switch to shader 2
                        currentVertexPath = "shaders/shader2/vertex.glsl";
                        currentFragmentPath = "shaders/shader2/fragment.glsl";
                        shaderProgram = reloadCurrentShader(shaderProgram);
                        updateUniformLocations(shaderProgram, millisLoc, backgroundLoc, colorsLocation, circlesLocation);
                        break;
                    case SDLK_r:
                        // Reload current shader
                        shaderProgram = reloadCurrentShader(shaderProgram);
                        updateAttributeLocations(shaderProgram, VAO, posAttrib, texAttrib);
                        updateUniformLocations(shaderProgram, millisLoc, backgroundLoc, colorsLocation, circlesLocation);
                        break;
                    case SDLK_a:
                        // Toggle auto-reload
                        toggleAutoReload();
                        break;
                }
            }
            // Handle window resize event
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                handleResize(e.window.data1, e.window.data2);
            }
        }
        
        // Auto-reload shader if enabled
        if (autoReloadEnabled) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastReloadTime > RELOAD_INTERVAL) {
                shaderProgram = reloadCurrentShader(shaderProgram);
                updateAttributeLocations(shaderProgram, VAO, posAttrib, texAttrib);
                updateUniformLocations(shaderProgram, millisLoc, backgroundLoc, colorsLocation, circlesLocation);
                lastReloadTime = currentTime;
                system("cls");
            }
        }
        
        // Generate random value for shader
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        
        // Calculate elapsed time in milliseconds
        float currentTime = SDL_GetTicks() - startTime;
        
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use the shader program
        glUseProgram(shaderProgram);
        
        // Activate texture unit 0 and bind the background texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        
        // Update uniforms
        if (colorsLocation != -1) {
            glUniform3fv(colorsLocation, sizeof(colorValues) / sizeof(float) / 3, colorValues);
        }
        
        if (backgroundLoc != -1) {
            glUniform1i(backgroundLoc, 0);
        }
        
        if (millisLoc != -1) {
            glUniform1f(millisLoc, currentTime);
        }
        
        GLint randomLoc = glGetUniformLocation(shaderProgram, "random");
        if (randomLoc != -1) {
            glUniform1f(randomLoc, randomValue);
        }
        
        // Update the circles uniform
        if (circlesLocation != -1) {
            // Create an array to hold circle data in the format expected by the shader
            std::vector<float> circleData(numCircles * 3); // x, y, radius for each circle
            
            // Normalize coordinates to shader space (-1 to 1)
            for (int i = 0; i < numCircles; i++) {
                // Convert screen coordinates to normalized device coordinates (-1 to 1)
                float normalizedX = (circles[i].x / static_cast<float>(WINDOW_WIDTH)) * 2.0f - 1.0f;
                float normalizedY = (circles[i].y / static_cast<float>(WINDOW_HEIGHT)) * 2.0f - 1.0f;
                // Flip Y coordinate since screen coordinates have Y pointing down
                normalizedY = -normalizedY;
                
                // Normalize radius (as a fraction of screen width)
                float normalizedRadius = circles[i].radius / static_cast<float>(WINDOW_WIDTH) * 2.0f;
                
                // Store the data in the format expected by the shader
                circleData[i * 3] = normalizedX;     // x position
                circleData[i * 3 + 1] = normalizedY; // y position
                circleData[i * 3 + 2] = normalizedRadius; // radius
            }
            
            // Pass the circle data to the shader
            glUniform3fv(circlesLocation, numCircles, circleData.data());
        }
        
        // Draw the quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap buffers
        SDL_GL_SwapWindow(window);
        
        // Add a small delay to reduce CPU usage
        SDL_Delay(delay);
    }
    
    //----------------------------------------------------------------------
    // Cleanup
    //----------------------------------------------------------------------
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

// Helper function to update attribute locations after shader reload
void updateAttributeLocations(GLuint shaderProgram, GLuint VAO, GLint& posAttrib, GLint& texAttrib) {
    posAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    texAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
    
    glBindVertexArray(VAO);
    
    if (posAttrib != -1) {
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(posAttrib);
    }
    
    if (texAttrib != -1) {
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(texAttrib);
    }
    
    glBindVertexArray(0);
}

// Helper function to update uniform locations after shader reload
void updateUniformLocations(GLuint shaderProgram, GLint& millisLoc, GLint& backgroundLoc, 
                           GLint& colorsLocation, GLint& circlesLocation) {
    millisLoc = glGetUniformLocation(shaderProgram, "millis");
    backgroundLoc = glGetUniformLocation(shaderProgram, "background");
    colorsLocation = glGetUniformLocation(shaderProgram, "colors");
    circlesLocation = glGetUniformLocation(shaderProgram, "circles");
}

#endif // RENDERER_H
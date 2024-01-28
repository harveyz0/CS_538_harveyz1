#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "box.hpp"
using namespace std;


class A01RenderEngine {
    private:
        // Data for display buffer
        unsigned int windowTextureID = 0;
        unsigned char *frontBuffer = 0;
        unsigned char *screenBuffer = 0;
        int windowWidth = 0;
        int windowHeight = 0;
        int nrComponents = 3;
        int totalBufferSize = 0;

        // BONUS
        int currentCol = 100;
        Box currentBox = Box(1, 1, 1, 1);

        // Internal drawing functions
        void clearBuffer(   unsigned char *buffer,
                            unsigned char r,
                            unsigned char g,
                            unsigned char b);
        void drawOneFrame();
        void drawAABox( unsigned char *buffer,
                        const Box& box,
                        unsigned char r,
                        unsigned char g,
                        unsigned char b);

    
        Box getPoints(Box);

    public:
        A01RenderEngine(int windowWidth, int windowHeight);
        ~A01RenderEngine();
        // Copies display buffer to window texture
        void renderToWindowTexture();
};



#endif

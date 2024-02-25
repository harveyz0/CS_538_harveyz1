#pragma once

#include "Buffer.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "Settings.hpp"
#include "Timing.hpp"
#include "Vector.hpp"
#include "XML.hpp"
#include "tinyxml2.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;
using namespace potato;
using namespace tinyxml2;

class ExerciseRenderEngine
{
private:
  // Data for display buffer
  unsigned int windowTextureID = 0;
  Image<Vec3f>* frontBuffer = 0;
  Image<Vec3f>* backBuffer = 0;
  Image<Vec3f>* screenBuffer = 0;

  int windowWidth = 0;
  int windowHeight = 0;

  // Threading stuff
  thread* drawThread = 0;
  bool drawThreadRunning = false;
  mutex frontBufferMutex;
  bool USE_VSYNC = true;

  // Lines
  vector<Line<int, float>> allLines;

  // Timing stuff
  Timekeeper timekeeper;

  // BONUS
  int currentCol = 0;

  // Internal drawing functions
  void drawingLoop();
  void drawAABox(Image<Vec3f>* buffer,
                 int sx,
                 int sy,
                 int ex,
                 int ey,
                 Vec3f color);
  void swapBuffers();

public:
  ExerciseRenderEngine(int windowWidth, int windowHeight);
  ~ExerciseRenderEngine();
  // Copies display buffer to window texture
  void renderToWindowTexture();
};

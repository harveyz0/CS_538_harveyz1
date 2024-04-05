#pragma once

// Comment to turn off testing
#define ENABLE_TESTING

// Set target FPS
#define TARGET_FPS     60
#define USE_TARGET_FPS false

// Print out stats every X number of seconds
#define AVE_STATS_TIME_WINDOW 1.0
#define PRINT_AVE_STATS       true

// Test geometry fan options
#define GEO_FAN_BLADE_CNT 8

// Drawing options
#define DRAW_LINES_AS_WIREFRAME false
#define LINE_DDA                0
#define LINE_MID                1
#define NO_LINE                 2
#define LINE_ALGORITHM          LINE_DDA
// #define LINE_ALGORITHM              LINE_MID
#define SKIP_FILL true

// Renderer options
#define BASE_RENDERER    0
#define EXAMPLE_RENDERER 1
#define FORWARD_RENDERER 2

#define RENDERER_CHOICE  FORWARD_RENDERER

#define CLIP_LEFT        -1.0f
#define CLIP_RIGHT       1.0f
#define CLIP_BOTTOM      -1.0f
#define CLIP_TOP         1.0f
#define CLIP_NEAR        -1.0f
#define CLIP_FAR         1.0f

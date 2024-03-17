#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "Bary.hpp"
#include "Settings.hpp"
#include "glm/glm.hpp"
#include "tinyxml2.h"
#include "MeshData.hpp"
#include "MeshGLData.hpp"
#include "GLSetup.hpp"
#include "Shader.hpp"
#include "PotatoRenderEngine.hpp"
#include "PotatoForwardEngine.hpp"
#include "PotatoExampleEngine.hpp"
#include "Vector.hpp"
using namespace std;
using namespace tinyxml2;

// Last known mouse position (in pixel coordinates)
glm::vec2 lastMouse;

// Set window width and height
int windowWidth = 800;
int windowHeight = 600;

// Create very simple mesh: a quad (4 vertices, 6 indices, 2 triangles)
void createSimpleQuad(Mesh &m) {
	// Clear out vertices and elements
	m.vertices.clear();
	m.indices.clear();

	// Create four corners
	Vertex upperLeft, upperRight;
	Vertex lowerLeft, lowerRight;

	// Set positions of vertices
	// Note: glm::vec3(x, y, z)
	upperLeft.position = glm::vec3(-1.0, 1.0, 0.0);
	upperRight.position = glm::vec3(1.0, 1.0, 0.0);
	lowerLeft.position = glm::vec3(-1.0, -1.0, 0.0);
	lowerRight.position = glm::vec3(1.0, -1.0, 0.0);

	// Set texture coordinates
	upperLeft.texcoords = glm::vec2(0,1);
	upperRight.texcoords = glm::vec2(1,1);
	lowerLeft.texcoords = glm::vec2(0,0);
	lowerRight.texcoords = glm::vec2(1,0);

	// Add to mesh's list of vertices
	m.vertices.push_back(upperLeft);
	m.vertices.push_back(upperRight);	
	m.vertices.push_back(lowerLeft);
	m.vertices.push_back(lowerRight);
	
	// Add indices for two triangles
	m.indices.push_back(0);
	m.indices.push_back(3);
	m.indices.push_back(1);

	m.indices.push_back(0);
	m.indices.push_back(2);
	m.indices.push_back(3);
}

// Mouse movement callback
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	// Get relative mouse motion (from last known position)
	glm::vec2 curMouse = glm::vec2(xpos, ypos);
	glm::vec2 relMouse = curMouse - lastMouse;

	// Get CURRENT buffer size (could be different if window resized)
	int sw, sh;
	glfwGetFramebufferSize(window, &sw, &sh);

	// If not zero...
	if(sw > 0 && sh > 0) {
		// Normalize mouse coordinates
		curMouse.x /= (float)sw;
		curMouse.y /= (float)sh;

		relMouse.x /= (float)sw;
		relMouse.y /= (float)sh;

		// Rescale to original window width/height
		curMouse.x *= windowWidth;
		curMouse.y *= windowHeight;

		relMouse.x *= windowWidth;
		relMouse.y *= windowHeight;

		// Need to flip Y coordinates (so that up is really up)
		relMouse.y = -relMouse.y;
		curMouse.y = windowHeight - 1 - curMouse.y;

		// DO MOUSE MOVEMENT STUFF
		// Uncomment to print mouse coordinates
		//cout << "Mouse pos: (" << curMouse.x << "," << curMouse.y << ")" << endl;
		//cout << "Relative mouse: (" << relMouse.x << "," << relMouse.y << ")" << endl;
	}

	// Set last mouse pos
	lastMouse = glm::vec2(xpos, ypos);
}

// Mouse button callback
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
	switch(button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			if(action == GLFW_PRESS) {
				cout << "LEFT DOWN!" << endl;
			}
			else if(action == GLFW_RELEASE) {
				cout << "LEFT UP!" << endl;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if(action == GLFW_PRESS) {
				cout << "RIGHT DOWN!" << endl;
			}
			else if(action == GLFW_RELEASE) {
				cout << "RIGHT UP!" << endl;
			}
			break;
		default:
			break;
	}
}

// Keyboard callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Check for escape
	if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
	}
		
	// Check for other keys
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch(key) {
			case GLFW_KEY_W:
				cout << "W pressed!" << endl;
				break;
			default:
				break;
		}
	}
}

// Main 
int main(int argc, char **argv) {

	// Do a quick check on command line arguments
	//
	//
//	Vec3i A(0, 0, 0);
//	Vec3i B(5, 0, 0);
//	Vec3i C(3, 1, 0);
//	Vec3i D(5, 8, 0);
//
//	Vec3i E1 = B - A;
//	Vec3i E2 = C - B;
//	Vec3i E3 = D - C;
//	Vec3i E4 = A - D;
//
//	cout << "Cross E1 X E2 " << E1.cross(E2) << endl;
//	cout << "Cross E2 X E3 " << E2.cross(E3) << endl;
//	cout << "Cross E3 X E4 " << E3.cross(E4) << endl;
//	cout << "Cross E4 X E1 " << E4.cross(E1) << endl;
//
	//
	//
//	Vec3f W(1,1,1);
//	Vec3f T(0,1,0);
//	Vec3f TXW = T.cross(W);


//	Vec3f U = TXW / TXW.length();
//	Vec3f V = W.cross(U);
//
//	cout << "Cross T X W " << TXW << endl;
//	cout << "Length TXW " << TXW.length() << endl;
//	cout << "TXW / TXW.lenght " << U << endl;
//	cout << "U " << U << " V " << V << " W " << W.normalize();
//
//
//	
//
//	Vec3f A;
//	Vec3f B;
//	Vec3f C;
//
//
//
//	A = Vec3f(2, 2, 0);
//	B = Vec3f(5, 3, 0);
//	C = Vec3f(2, 5, 0);
//
//	BaryData bd(A, B, C);
//	cout << "bd bary " << barycentric(bd, 1, 1) << endl;
//
//	return 0;
	cout << "Number of command line arguments: " << (argc-1) << endl;
    for(int i = 1; i < argc; i++) {
        cout << "* Arg " << i << ": " << argv[i] << endl;
    }
	
	// Are we in debugging mode?
	bool DEBUG_MODE = true;

	stringstream windowTitle;
	windowTitle << "PotatoRenderApp ; fan blade count ";
	windowTitle << GEO_FAN_BLADE_CNT;
	windowTitle << " ; Line Algo ";
	if(LINE_ALGORITHM == LINE_DDA){
		windowTitle << "DDA ";
	} else if(LINE_ALGORITHM == LINE_MID){
		windowTitle << "MID ";
	} else {
		windowTitle << "UNKNOWN ";
	}

	if(SKIP_FILL){
		windowTitle << "NO FILL ";
	}

	if(DRAW_LINES_AS_WIREFRAME){
		windowTitle << "; Wireframe";
	}

	// GLFW setup
	// Switch to 4.1 if necessary for macOS
	GLFWwindow* window = setupGLFW(windowTitle.str(), 4, 3, windowWidth, windowHeight, DEBUG_MODE);

	// GLEW setup
	setupGLEW(window);

	// Get initial mouse position
	double mx, my;
	glfwGetCursorPos(window, &mx, &my);
	lastMouse = glm::vec2(mx, my);

	// Set mouse motion callback
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// Set mouse button callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	// Uncomment this to make mouse invisible
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set keyboard callback
	glfwSetKeyCallback(window, key_callback);

	// Check OpenGL version
	checkOpenGLVersion();

	// Set up debugging (if requested)
	if(DEBUG_MODE) checkAndSetupOpenGLDebugging();

	// Set the background color to white (so we can see if the quad is covering the window)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	

	// Create and load shaders
	GLuint programID = 0;
	try {		
		// Load vertex shader code and fragment shader code
		string vertexCode = readFileToString("./shaders/PotatoRenderApp/Quad.vs");
		string fragCode = readFileToString("./shaders/PotatoRenderApp/Quad.fs");

		// Print out shader code, just to check
		if(DEBUG_MODE) printShaderCode(vertexCode, fragCode);

		// Create shader program from code
		programID = initShaderProgramFromSource(vertexCode, fragCode);
	}
	catch (exception e) {		
		// Close program
		cleanupGLFW(window);
		exit(EXIT_FAILURE);
	}

	// Create simple quad to cover the window
	Mesh m;
	createSimpleQuad(m);

	// Create OpenGL mesh (VAO) from data
	MeshGL mgl;
	createMeshGL(m, mgl);

    // Get texture uniform location
    GLint uniformTextureID = glGetUniformLocation(programID, "quadTexture");
	
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

    // Create Potato Render Engine
	PotatoRenderEngine *engine; 
	if(RENDERER_CHOICE == BASE_RENDERER) { 
		engine = new PotatoRenderEngine(windowWidth, windowHeight); 
	} 
	else if(RENDERER_CHOICE == EXAMPLE_RENDERER) { 
		engine = new PotatoExampleEngine(windowWidth, windowHeight); 
	} 
	else if(RENDERER_CHOICE == FORWARD_RENDERER) { 
    	engine = new PotatoForwardEngine(windowWidth, windowHeight); 
	} 
	else { 
		throw std::invalid_argument("Bad renderer choice!"); 
	} 
	// end render check 
 
	engine->initialize(); 
 
	while (!glfwWindowShouldClose(window)) { 
		// Set viewport size 
		int fwidth, fheight; 
		glfwGetFramebufferSize(window, &fwidth, &fheight); 
		glViewport(0, 0, fwidth, fheight); 
 
		// Clear the framebuffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
		// Use shader program 
		glUseProgram(programID); 
 
        // Set texture uniform variable 
        glUniform1i(uniformTextureID, 0); 
 
        // Render to window texture 
        engine->renderToWindowTexture(); 
 
		// Draw object 
		drawMesh(mgl); 
 
		// Swap buffers and poll for window events 
		glfwSwapBuffers(window); 
		glfwPollEvents(); 
 
		// Sleep for 15 ms 
		this_thread::sleep_for(chrono::milliseconds(15)); 
	} 
 
    // Clean up engine 
	engine->shutdown(); 
    delete engine; 
    engine = NULL; 
 
	// Clean up mesh 
	cleanupMesh(mgl); 
 
	// Clean up shader programs 
	glUseProgram(0); 
	glDeleteProgram(programID); 
 
	// Destroy window and stop GLFW 
	cleanupGLFW(window); 
 
	return 0; 
} 

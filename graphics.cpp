#include "graphics.h"

using namespace glm;

Graphics::Graphics() {
   if(initWindow() == 0)
      valid = true;
   else
      std::cerr << "Error setting up window" << std::endl;
}

int Graphics::initWindow() {
   // Initialize GLFW
   if(!glfwInit()) {
      std::cerr << "Error initializing GLFW" << std::endl;
      return -1;
   }
   
   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   // Set up window and create OpenGL context
   window = glfwCreateWindow(w_width * 2, w_height * 2, w_title, NULL, NULL);
   if(window == NULL) {
      std::cerr << "Error initializing Window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   
   // Initialize GLEW
   glewExperimental = true; // Needed for core profile
   if(glewInit() != GLEW_OK) {
      std::cerr << "Error initializing GLEW" << std::endl;
      return -1;
   }
   
   if(!GLEW_EXT_geometry_shader4) {
      std::cerr << "No geometry shader support" << std::endl;
      return 1;
   }
   
   // Ensure we capture the escape key
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   
   // Black background
   glClearColor(0.1f, 0.8f, 0.8f, 0.0f);
   
   // Enable depth test
   glEnable(GL_DEPTH_TEST);
   // Accept only closer fragments
   glDepthFunc(GL_LESS);
   // Cull triangles in which the normal doesn't face the camera
   glEnable(GL_CULL_FACE);
   
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
   
   camera = new Camera();
   
	// Initialize our little text library with the Holstein font
	initText2D( "fonts/monaco.png" );
   
   return 0;
}

void Graphics::beginRender() {
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::endRender() {
   // Swap Buffers
   glfwSwapBuffers(window);
   glfwPollEvents();
}

void Graphics::terminate() {
	// Cleanup VBO
   glDeleteVertexArrays(1, &vertexArrayID);
   
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
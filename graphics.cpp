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
   
   const char *flatVertShader = "shaders/FlatVertexShader.glsl";
   const char *flatGeomShader = "shaders/FlatGeometryShader.glsl";
   const char *flatFragShader = "shaders/FlatFragmentShader.glsl";
   flatProgramID = LoadShaders(flatVertShader, flatGeomShader, flatFragShader);
   
   // Get a handle for the MVP uniform
   mvpID = glGetUniformLocation(flatProgramID, "MVP");
   
   flatColorID = glGetUniformLocation(flatProgramID, "color");
   flatWidthID = glGetAttribLocation(flatProgramID, "width");
   
   // Create buffers for flat squares
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &colorBuffer);
   glGenBuffers(1, &spriteVertexBufferID);
   glGenBuffers(1, &spriteUVBufferID);
   
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

/*
void Graphics::renderFlatSprite(GLuint texture, vec2 uvTopLeft, float uvWidth, float uvHeight, vec2 bottomLeft, float width, float height) {
   renderFlatSprite(texture, uvTopLeft, uvWidth, uvHeight, bottomLeft, width, height, -1);
}
 
void Graphics::renderFlatSprite(GLuint texture, vec2 uvTopLeft, float uvWidth, float uvHeight, vec2 bottomLeft, float width, float height, float tint) {
	// Bind shader
	glUseProgram(spriteProgramID);
   
	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
		
   glm::vec2 bottomRight = bottomLeft + vec2(width, 0);
   glm::vec2 topLeft     = bottomLeft + vec2(0, height);
   glm::vec2 topRight    = topLeft + vec2(width, 0);
   
   vertices.push_back(topLeft);
	vertices.push_back(bottomLeft);
	vertices.push_back(topRight);
   
	vertices.push_back(bottomRight);
	vertices.push_back(topRight);
	vertices.push_back(bottomLeft);
   
	vec2 uvTopRight    = uvTopLeft + vec2(uvWidth, 0);
   vec2 uvBottomLeft  = uvTopLeft + vec2(0, uvHeight);
	vec2 uvBottomRight = uvTopRight + vec2(0, uvHeight);
	
	UVs.push_back(uvTopLeft   );
	UVs.push_back(uvBottomLeft );
	UVs.push_back(uvTopRight  );
      
	UVs.push_back(uvBottomRight);
	UVs.push_back(uvTopRight);
	UVs.push_back(uvBottomLeft);
   
	glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, spriteUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
   
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(spriteUniformID, 0);
   glUniform1f(spriteHueID, tint);
   
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(flatVertexPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, spriteVertexBufferID);
	glVertexAttribPointer(flatVertexPositionID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
   
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(flatVertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, spriteUVBufferID);
	glVertexAttribPointer(flatVertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
   
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()) );
   
	glDisable(GL_BLEND);
   
	glDisableVertexAttribArray(flatVertexPositionID);
	glDisableVertexAttribArray(flatVertexUVID);
}
 */

void Graphics::terminate() {
   
	// Cleanup VBO
	glDeleteBuffers(1, &vertexBuffer);
   glDeleteVertexArrays(1, &vertexArrayID);
   
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
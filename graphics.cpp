#include "shader.h"
#include "graphics.h"
#include "text2D.h"

using namespace glm;

Graphics::Graphics() {
   if(initWindow() == 0)
      valid = true;
   else
      std::cout << "Error setting up window" << std::endl;
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
   window = glfwCreateWindow(w_width, w_height, w_title, NULL, NULL);
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
      std::cerr << "No support for geometry shaders found" << std::endl;
      return 1;
   }
   
   // Ensure we capture the escape key
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   
   // Connect input handler
   Input::setWindow(window);
   glfwSetMouseButtonCallback(window, Input::click);
   
   // Black background
   glClearColor(0.1f, 0.8f, 0.8f, 0.0f);
   
   // Enable depth test
   glEnable(GL_DEPTH_TEST);
   // Accept only closer fragments
   glDepthFunc(GL_LESS);
   // Cull triangles in which the normal doesn't face the camera
   //glEnable(GL_CULL_FACE);
   
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
   
   const char *vertShader = "shaders/VertexShader.glsl";
   const char *geomShader = "shaders/GeometryShader.glsl";
   const char *fragShader = "shaders/FragmentShader.glsl";
   programID = LoadShaders(vertShader, geomShader, fragShader);
   
   const char *flatVertShader = "shaders/FlatVertexShader.glsl";
   const char *flatGeomShader = "shaders/FlatGeometryShader.glsl";
   const char *flatFragShader = "shaders/FlatFragmentShader.glsl";
   flatProgramID = LoadShaders(flatVertShader, flatGeomShader, flatFragShader);
   
   const char *spriteVertShader = "shaders/SpriteVertexShader.glsl";
   const char *spriteFragShader = "shaders/SpriteFragmentShader.glsl";
   spriteProgramID = LoadShaders(spriteVertShader, spriteFragShader);
   
   // Get a handle for the MVP uniform
   mvpID = glGetUniformLocation(programID, "MVP");
   
   // Get a handle for our buffers
   vertexPositionID = glGetAttribLocation(programID, "vertex");
   vertexColorID = glGetAttribLocation(programID, "color");
   
   flatColorID = glGetUniformLocation(flatProgramID, "color");
   flatWidthID = glGetAttribLocation(flatProgramID, "width");
   
   flatVertexPositionID = glGetAttribLocation(spriteProgramID, "vertexPosition");
   flatVertexUVID = glGetAttribLocation(spriteProgramID, "vertexUV");
   spriteUniformID = glGetUniformLocation(spriteProgramID, "myTextureSampler");
   spriteHueID = glGetUniformLocation(spriteProgramID, "hue");
   
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
   
   projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
   view = camera->getViewMatrix();
}

void Graphics::endRender() {
   // Swap Buffers
   glfwSwapBuffers(window);
   glfwPollEvents();
}

void Graphics::renderText(char *string, int x, int y, int size) {
   printText2D(string, x, y, size);
}

void Graphics::renderSprite(int vertexBuffer, int colorBuffer, int numVertices, vec3 position) {
   // Use shader
   glUseProgram(programID);
   
   // Compute model matrices
   mat4 model = translate(mat4(1.0f), position);
   
   mat4 MVP = projection * view * model;
   
   // Send transformation to the currently bound shader's MVP
   glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexPositionID);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glVertexAttribPointer(vertexPositionID,   // The attribute to configure
                         4,                  // size
                         GL_UNSIGNED_BYTE,   // type
                         GL_FALSE,           // normalized?
                         0,                  // stride
                         (void*)0            // array buffer offset
                         );
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexColorID);
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
   glVertexAttribPointer(vertexColorID,      // The attribute to configure
                         3,                  // size
                         GL_UNSIGNED_BYTE,   // type
                         GL_FALSE,           // normalized?
                         0,                  // stride
                         (void*)0            // array buffer offset
                         );
   
   // Draw the triangle !
   glDrawArrays(GL_LINES, 0, numVertices);
   
   glDisableVertexAttribArray(vertexPositionID);
   glDisableVertexAttribArray(vertexColorID);
}

void Graphics::renderSquareFlat(byte3 color, vec3 bottomLeft, float width, float height) {
   
   // Use shader
   glUseProgram(flatProgramID);
   
   GLfloat vertex[8] = { bottomLeft.x, bottomLeft.y, bottomLeft.z, 1,
                         bottomLeft.x + width, bottomLeft.y + height, bottomLeft.z, 255 };
   byte3 rgb[2] = { color, color };
   
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertex),
                vertex, GL_STATIC_DRAW);
   
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(rgb),
                rgb, GL_STATIC_DRAW);
   
   mat4 MVP = mat4(1.0f);
   
   // Send transformation to the currently bound shader's MVP
   glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexPositionID);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glVertexAttribPointer(vertexPositionID,   // The attribute to configure
                         4,                  // size
                         GL_FLOAT,           // type
                         GL_FALSE,           // normalized?
                         0,                  // stride
                         (void*)0            // array buffer offset
                         );
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexColorID);
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
   glVertexAttribPointer(vertexColorID,      // The attribute to configure
                         3,                  // size
                         GL_UNSIGNED_BYTE,   // type
                         GL_FALSE,           // normalized?
                         0,                  // stride
                         (void*)0            // array buffer offset
                         );
   
   // Draw the triangle !
   glDrawArrays(GL_LINES, 0, 2);
   
   glDisableVertexAttribArray(vertexPositionID);
   glDisableVertexAttribArray(vertexColorID);
}

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
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
   
	glDisable(GL_BLEND);
   
	glDisableVertexAttribArray(flatVertexPositionID);
	glDisableVertexAttribArray(flatVertexUVID);
}

void Graphics::terminate() {
   
	// Cleanup VBO
	glDeleteBuffers(1, &vertexBuffer);
   glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
   
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
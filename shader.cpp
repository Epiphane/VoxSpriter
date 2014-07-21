#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>

#include "shader.h"

using namespace std;

GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);
GLuint LoadShaders(const char *vertFilePath, const char *fragFilePath);

GLuint compileShader(const char *filePath, GLenum shaderType);

GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath) {
	GLint result = GL_FALSE;
	int infoLogLength;
   
   // Create shaders
   GLuint vertexShader = compileShader(vertFilePath, GL_VERTEX_SHADER);
   GLuint geomShader = compileShader(geomFilePath, GL_GEOMETRY_SHADER);
   GLuint fragShader = compileShader(fragFilePath, GL_FRAGMENT_SHADER);
   
	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragShader);
	glAttachShader(programID, geomShader);
	glLinkProgram(programID);
   
	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0){
		vector<char> errorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
		printf("%s\n", &errorMessage[0]);
	}
   
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	glDeleteShader(geomShader);
   
	return programID;
}

GLuint LoadShaders(const char *vertFilePath, const char *fragFilePath) {
	GLint result = GL_FALSE;
	int infoLogLength;
   
   // Create shaders
   GLuint vertexShader = compileShader(vertFilePath, GL_VERTEX_SHADER);
   GLuint fragShader = compileShader(fragFilePath, GL_FRAGMENT_SHADER);
   
	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragShader);
	glLinkProgram(programID);
   
	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0){
		vector<char> errorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
		printf("%s\n", &errorMessage[0]);
	}
   
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
   
	return programID;
}

GLuint compileShader(const char *filePath, GLenum shaderType) {
	GLuint shaderID = glCreateShader(shaderType);
   
	// Read the Shader code from the file
	string shaderCode;
	ifstream shaderStream(filePath, ios::in);
	if(shaderStream.is_open()){
		string line = "";
		while(getline(shaderStream, line))
			shaderCode += "\n" + line;
		shaderStream.close();
	}
   
	GLint result = GL_FALSE;
	int infoLogLength;
   
   // Compile Shader
   printf("Compiling shader : %s\n", filePath);
   const char *sourcePointer = shaderCode.c_str();
   glShaderSource(shaderID, 1, &sourcePointer , NULL);
   glCompileShader(shaderID);

   // Check Shader
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
   glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
   if(infoLogLength > 0){
      vector<char> errorMessage(infoLogLength+1);
      glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
      printf("%s\n", &errorMessage[0]);
   }
   
   return shaderID;
}

/*
 * ------------ SHADER CODE --------------
 */

using namespace glm;

bool ThreeDimension::initialized = false;
GLuint ThreeDimension::programID = 0;
GLuint ThreeDimension::mvp = 0;
GLuint ThreeDimension::vertexPosition = 0;
GLuint ThreeDimension::vertexColor = 0;

bool ImageProgram::initialized = false;
GLuint ImageProgram::programID = 0;
GLuint ImageProgram::tintHue = 0;
GLuint ImageProgram::textureSample = 0;
GLuint ImageProgram::vertexPosition = 0;
GLuint ImageProgram::vertexUV = 0;

void Program::bufferData(int bufferType, int size, void *data) {
   std::cout << "Buffer data not supported for ";
   switch(bufferType) {
      case VERTEX_BUFFER:
         std::cout << "vertex buffer";
         break;
      case COLOR_BUFFER:
         std::cout << "color buffer";
         break;
      case UV_BUFFER:
         std::cout << "UV buffer";
         break;
   }
   std::cout << " type" << std::endl;
}

void Program::buffer(int bufferId, int size, void *data) {
   glBindBuffer(GL_ARRAY_BUFFER, bufferId);
   glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ThreeDimension::ThreeDimension() {
   if(!initialized) {
      programID = LoadShaders("shaders/3DVertexShader.glsl", "shaders/3DGeometryShader.glsl", "shaders/3DFragmentShader.glsl");
   
      // Get a handle for the MVP uniform
      mvp = glGetUniformLocation(programID, "MVP");
   
      // Get a handle for our buffers
      vertexPosition = glGetAttribLocation(programID, "vertex");
      vertexColor = glGetAttribLocation(programID, "color");
      
      initialized = true;
   }
   
   camera = 0;
   
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &colorBuffer);
}

void ThreeDimension::bufferData(int bType, int size, void *data) {
   if(bType == VERTEX_BUFFER) {
      Program::buffer(vertexBuffer, size, data);
   }
   else if(bType == COLOR_BUFFER) {
      Program::buffer(colorBuffer, size, data);
   }
   else {
      Program::bufferData(bType, size, data);
   }
}

void ThreeDimension::render() {
   // Use shader
   glUseProgram(programID);
   
   // Compute model matrices
   mat4 model = translate(mat4(1.0f), position);
   
   if(camera) {
      mat4 MVP = camera->getVPMatrix() * model;
      glUniformMatrix4fv(mvp, 1, GL_FALSE, &MVP[0][0]);
   }
   else {
      glUniformMatrix4fv(mvp, 1, GL_FALSE, &model[0][0]);
   }
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexPosition);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glVertexAttribPointer(vertexPosition, 4, vertexFormat,
                         GL_FALSE, 0, 0);
   
   // First attribute buffer: vertices
   glEnableVertexAttribArray(vertexColor);
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
   glVertexAttribPointer(vertexColor, 3, GL_UNSIGNED_BYTE,
                         GL_FALSE, 0, 0);
   
   // Draw the triangle !
   glDrawArrays(GL_LINES, 0, numVertices);
   
   glDisableVertexAttribArray(vertexPosition);
   glDisableVertexAttribArray(vertexColor);
}

ImageProgram::ImageProgram() {
   if(!initialized) {
      programID = LoadShaders("shaders/SpriteVertexShader.glsl", "shaders/SpriteGeometryShader.glsl", "shaders/SpriteFragmentShader.glsl");
      
      tintHue = glGetUniformLocation(programID, "tintHue");
      textureSample = glGetUniformLocation(programID, "textureSample");
      vertexPosition = glGetAttribLocation(programID, "vertex");
      vertexUV = glGetAttribLocation(programID, "vertexUV");
      
      
      initialized = true;
   }
   
   tint = -1;
   
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &uvBuffer);
}

void ImageProgram::setTexture(const char *imageName) {
   texture = loadImage(imageName);
}

void ImageProgram::bufferData(int bufferType, int size, void *data) {
   if(bufferType == VERTEX_BUFFER) {
      Program::buffer(vertexBuffer, size, data);
   }
   else if(bufferType == UV_BUFFER) {
      Program::buffer(uvBuffer, size, data);
   }
   else {
      Program::bufferData(bufferType, size, data);
   }
}

void ImageProgram::render() {
   glUseProgram(programID);
   
   glUniform1f(tintHue, tint);
   
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureSample, 0);
   
   glEnableVertexAttribArray(vertexPosition);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glVertexAttribPointer(vertexPosition, 3, GL_FLOAT,
                         GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(vertexUV);
   glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
   glVertexAttribPointer(vertexUV, 2, GL_FLOAT,
                         GL_FALSE, 0, 0);
   
   // Blending function?
   
   glDrawArrays(GL_LINES, 0, numVertices);
   
   glDisableVertexAttribArray(vertexPosition);
   glDisableVertexAttribArray(vertexUV);
}
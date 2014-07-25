#ifndef ____shader__
#define ____shader__

#include "globals.h"
#include "camera.h"
#include "texture.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);
GLuint LoadShaders(const char *vertFilePath, const char *fragFilePath);

GLuint compileShader(const char *filePath, GLenum shaderType);

class Program {
public:
   static const int VERTEX_BUFFER = 0;
   static const int COLOR_BUFFER = 1;
   static const int UV_BUFFER = 2;
   
   void bufferData(int bufferType, int size, void *data);
   
   Program() {};
   void setNumVertices(int _num) { numVertices = _num; }
   
protected:
   GLuint numVertices;
   
   void buffer(int bufferId, int size, void *data);
};

class ThreeDimension : public Program {
public:
   ThreeDimension();
   void setCamera(Camera *_camera) { camera = _camera; }
   void setVertexFormat(GLuint _format) { vertexFormat = _format; }
   ThreeDimension *clone();
   
   void bufferData(int bufferType, int size, void *data);
   void setPosition(glm::vec3 _position) { position = _position; }
   
   void render();
   
private:
   static GLuint programID;
   static bool initialized;
   static GLuint mvp, vertexPosition, vertexColor;
   GLuint vertexBuffer, colorBuffer, vertexFormat;
   
   Camera *camera;
   glm::vec3 position;
};

class ImageProgram : public Program {
public:
   ImageProgram();
   void setTexture(const char *imageName);
   void setTexture(GLuint _texture) { texture = _texture; }
   void setTint(float _tint) { tint = _tint; }
   
   void bufferData(int bufferType, int size, void *data);
   void render();
   
private:
   static GLuint programID;
   static bool initialized;
   static GLuint tintHue, textureSample, vertexPosition, vertexUV;
   GLuint vertexBuffer, uvBuffer, texture;
   
   float tint;
};

#endif

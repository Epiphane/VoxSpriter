#ifndef ____graphics__
#define ____graphics__

// Include GLM
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "camera.h"
#include "globals.h"
#include "shader.h"
#include "text2D.h"

class Graphics {
public:
   Graphics();
   bool isValid() { return valid; }
   Camera *getCamera() { return camera; }
   GLFWwindow *getWindow() { return window; }
   
   int initWindow();
   void beginRender();
   void endRender();
   void terminate();
   
private:
   GLFWwindow *window;
   
   Camera *camera;
   
   GLuint flatProgramID, vertexArrayID;
   GLuint mvpID;
   GLuint vertexBuffer, colorBuffer;
   GLuint spriteVertexBufferID, spriteUVBufferID;
   GLuint flatColorID, flatWidthID;
   
   bool valid = false;
};

#endif /* defined(____window__) */

#ifndef ____graphics__
#define ____graphics__

#include <iostream>
#include <vector>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "camera.h"
#include "globals.h"

class Graphics {
private:
   GLFWwindow *window;
   
   Camera *camera;
   
   GLuint programID, flatProgramID, vertexArrayID;
   GLuint mvpID;
   GLuint vertexBuffer, colorBuffer;
   GLuint vertexColorID, vertexPositionID;
   GLuint flatColorID, flatWidthID;
   
   glm::mat4 projection, view;
   
   bool valid = false;
public:
   Graphics();
   bool isValid() { return valid; }
   Camera *getCamera() { return camera; }
   
   void render();
   void renderSprite(int vertexBuffer, int colorBuffer, int numVertices, glm::vec3 position);
   void renderSquareFlat(byte3 color, glm::vec3 topLeft, float width, float height);
   
   int getBuffer();
   
   int initWindow();
   void beginRender();
   void endRender();
   void terminate();
};

#endif /* defined(____window__) */

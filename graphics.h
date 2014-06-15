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

class Graphics {
private:
   GLFWwindow *window;
   
   Camera *camera;
   
   GLuint programID, vertexArrayID;
   GLuint mvpID;
   GLuint vertexBuffer, colorBuffer;
   GLuint vertexColorID, vertexPositionID;
   
   glm::mat4 projection, view;
   
   bool valid = false;
public:
   Graphics();
   bool isValid() { return valid; }
   Camera *getCamera() { return camera; }
   
   void render();
   void renderSprite(int vertexBuffer, int colorBuffer, int numVertices,
                     glm::vec3 position);
   
   int getBuffer();
   
   int initWindow();
   void beginRender();
   void endRender();
   void terminate();
};

#endif /* defined(____window__) */

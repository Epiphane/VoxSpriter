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
   
   GLuint programID, flatProgramID, spriteProgramID, vertexArrayID;
   GLuint mvpID;
   GLuint vertexBuffer, colorBuffer;
   GLuint vertexColorID, vertexPositionID;
   GLuint flatVertexPositionID, flatVertexUVID;
   GLuint spriteVertexBufferID, spriteUVBufferID, spriteUniformID, spriteHueID;
   GLuint flatColorID, flatWidthID;
   
   glm::mat4 projection, view;
   
   bool valid = false;
public:
   Graphics();
   bool isValid() { return valid; }
   Camera *getCamera() { return camera; }
   
   void render();
   void renderSprite(int vertexBuffer, int colorBuffer, int numVertices, glm::vec3 position);
   void renderSquareFlat(byte3 color, glm::vec3 bottomLeft, float width, float height);
   void renderFlatSprite(GLuint texture, glm::vec2 uvTopLeft, float uvWidth, float uvHeight, glm::vec2 bottomLeft, float width, float height);
   void renderFlatSprite(GLuint texture, glm::vec2 uvTopLeft, float uvWidth, float uvHeight, glm::vec2 bottomLeft, float width, float height, float tint);
   void renderText(char *string, int x, int y, int size);
   void renderText(char *string, double x, double y, int size);
   
   int getBuffer();
   
   int initWindow();
   void beginRender();
   void endRender();
   void terminate();
};

#endif /* defined(____window__) */

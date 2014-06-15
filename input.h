#ifndef ____input__
#define ____input__

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum Input_State {
   InGame = 0
};

class Input {
public:
   static bool shouldExit();
   
   static void setState(Input_State state);
   static void setWindow(GLFWwindow *_window);
   static void setHandler(void *_game);
   static void tick();
   static void getMousePos(double *x, double *y);
   static bool keyState(int key, int state);
};

#endif
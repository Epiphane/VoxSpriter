#include "input.h"
#include "application.h"
#include "globals.h"

static Application *application;
static GLFWwindow *window;
static bool inputShouldExit;
static Input_State state;

static bool ready = false;

void Input::setState(Input_State _state) {
   state = _state;
   ready = true;
   
   if(state == InGame) {
      //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPos(window, w_width / 2, w_height / 2);
   }
}

bool Input::keyState(int key, int state) {
   return (glfwGetKey(window, key) == state);
}

void Input::tick() {
   if(window == NULL)
      return;
   
   inputShouldExit = glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ||
          glfwWindowShouldClose(window) != 0;
}

void Input::getMousePos(double *x, double *y) {
   if(!ready)
      return;
   
   glfwGetCursorPos(window, x, y);
}

void Input::setHandler(void *_game) {
   application = static_cast<Application *>(_game);
}

void Input::setWindow(GLFWwindow *_window) {
   window = _window;
}

bool Input::shouldExit() {
   return inputShouldExit;
}
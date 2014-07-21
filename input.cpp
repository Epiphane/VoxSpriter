#include "input.h"

using namespace std;

// Input variables
static bool ready;
static GLFWwindow *window;
static bool inputShouldExit;

// Exit callback
class ExitCommand : public Command {
public:
   virtual void execute(bool press) {
      if(press)
         inputShouldExit = true;
   }
};

// Key callbacks
static bool keyStates[GLFW_KEY_LAST];
static Command *keyCallbacks[GLFW_KEY_LAST];
static vector<Command *> mouseCallbacks;

void Input::clear() {
   for(int i = 0; i < GLFW_KEY_LAST; i ++) {
      keyStates[i] = false;
      keyCallbacks[i] = 0;
   }
   
   mouseCallbacks.clear();
}

void Input::init(GLFWwindow *_window) {
   window = _window;
   
   // Connect input handler
   glfwSetMouseButtonCallback(window, Input::click);
   
   Input::setCallback(GLFW_KEY_ESCAPE, new ExitCommand());
   
   inputShouldExit = false;
   ready = true;
}

void Input::setCallback(int key, Command *callBack) {
   keyCallbacks[key] = callBack;
}

void Input::setMouseCallback(Command *callBack) {
   mouseCallbacks.push_back(callBack);
}

void Input::tick() {
   for(int i = 0; i < GLFW_KEY_LAST; i ++) {
      if(keyCallbacks[i] != 0) {
         if(keyStates[i] ^ (glfwGetKey(window, i) == GLFW_PRESS)) {
            keyStates[i] = !keyStates[i];
            keyCallbacks[i]->execute(keyStates[i]);
         }
      }
   }
   
   if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      x = x / w_width - 1;
      y = 1 - y / w_height;
      
      vector<Command *>::iterator it = mouseCallbacks.begin();
      for(; it < mouseCallbacks.end(); it ++) {
         (*it)->execute(x, y);
      }
   }
}

bool Input::keyDown(int key) {
   return keyStates[key];
}

bool Input::mouseState(int button, int state) {
   return (glfwGetMouseButton(window, button) == state);
}

bool Input::shouldExit() {
   return inputShouldExit || glfwWindowShouldClose(window);
}

void Input::getMousePos(double *x, double *y) {
   glfwGetCursorPos(window, x, y);
}

void Input::click(GLFWwindow *window, int button, int action, int mods) {
}
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

static vector<int> mods;

// Key callbacks
struct KeyCallback {
   int mod;
   Command *command;
   KeyCallback *next;
};
static bool keyStates[GLFW_KEY_LAST];
static KeyCallback *keyCallbacks[GLFW_KEY_LAST];

static vector<Command *> mouseCallbacks;

void Input::clear() {
   for(int i = 0; i < GLFW_KEY_LAST; i ++) {
      keyStates[i] = false;
      if(keyCallbacks[i]) {
         delete keyCallbacks[i];
         keyCallbacks[i] = 0;
      }
   }
   
   mods.clear();
   mouseCallbacks.clear();
}

void Input::init(GLFWwindow *_window) {
   window = _window;
   
   // Connect input handler
   glfwSetMouseButtonCallback(window, Input::click);
   
   Input::setCallback(GLFW_KEY_ESCAPE, new ExitCommand());
   
   Input::addMod(GLFW_KEY_LEFT_CONTROL);
   Input::addMod(GLFW_KEY_LEFT_SHIFT);
   Input::addMod(GLFW_KEY_LEFT_SUPER);
   
   inputShouldExit = false;
   ready = true;
}

void Input::addMod(int key) {
   mods.push_back(key);
}

void Input::setModCallback(int tMod, int key, Command *callBack) {
   int mod = 0;
   for(int ndx = 0; ndx < mods.size(); ndx ++) {
      if(mods[ndx] == tMod) {
         mod = (1 << ndx);
      }
   }
   
   KeyCallback *keyCallback = new KeyCallback;
   keyCallback->mod = mod;
   keyCallback->command = callBack;
   keyCallback->next = keyCallbacks[key];
   
   keyCallbacks[key] = keyCallback;
}

void Input::setCallback(int key, Command *callBack) {
   setModCallback(0, key, callBack);
}

void Input::setMouseCallback(Command *callBack) {
   mouseCallbacks.push_back(callBack);
}

bool Input::mouseDown = false;
void Input::tick() {
   int currentMod = 0;
   for(int mod = 0; mod < mods.size(); mod ++) {
      if(glfwGetKey(window, mods[mod]) == GLFW_PRESS) {
         currentMod += (1 << mod);
      }
   }
   
   for(int i = 0; i < GLFW_KEY_LAST; i ++) {
      if(keyCallbacks[i] != 0) {
         if(keyStates[i] ^ (glfwGetKey(window, i) == GLFW_PRESS)) {
            KeyCallback *cursor = keyCallbacks[i];
            keyStates[i] = !keyStates[i];
            
            // Has this key been modded?
            if(cursor->next) {
               while(cursor) {
                  if(currentMod == cursor->mod) {
                     cursor->command->execute(keyStates[i]);
                  }
               
                  cursor = cursor->next;
               }
            }
            else {
               keyCallbacks[i]->command->execute(keyStates[i]);
            }
         }
      }
   }
   
   double x, y;
   glfwGetCursorPos(window, &x, &y);
   x = x / w_width - 1;
   y = 1 - y / w_height;
   if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) { 
      vector<Command *>::iterator it = mouseCallbacks.begin();
      for(; it < mouseCallbacks.end(); it ++) {
         (*it)->execute(x, y, true);
      }
      mouseDown = true;
   }
   else if(mouseDown) {
      vector<Command *>::iterator it = mouseCallbacks.begin();
      for(; it < mouseCallbacks.end(); it ++) {
         (*it)->execute(x, y, false);
      }
      mouseDown = false;
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
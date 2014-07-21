#ifndef ____input__
#define ____input__

#include "globals.h"

/*
 * Command superclass
 * Simple events extend this class, so this
 * Isn't directly used.
 */
class Command {
public:
   virtual ~Command() {}
   virtual void execute(bool press) {
      std::cout << "Unhandled event!" << std::endl;
   }
   
   virtual void execute(double x, double y) {
      std::cout << "Unhandled event!" << std::endl;
   }
};

class Input {
public:
   static void clear();
   static void init(GLFWwindow *_window);
   static bool shouldExit();
   
   static void tick();
   static void setCallback(int key, Command *callBack);
   static void setMouseCallback(Command *callBack);
   
   static void getMousePos(double *x, double *y);
   static bool keyDown(int key);
   static bool mouseState(int button, int state);
   
   static void click(GLFWwindow *window, int button, int action, int mods);
};

#endif
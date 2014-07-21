#include "application.h"

int main(int argc, char *argv[])
{
   Application application;
   
   // Initialize application
   if(application.init())
      return 1;
   
   // application loop
   double lastTime, elapsedTime, currentTime;
   elapsedTime = 0;
   lastTime = glfwGetTime();
   while(application.isRunning()) {
      currentTime = glfwGetTime();
      elapsedTime += currentTime - lastTime;
      
      while(elapsedTime > SEC_PER_FRAME) {
         // Update application
         application.update();
         elapsedTime -= SEC_PER_FRAME;
      }
      
      // Render the screen
      application.render();
      lastTime = currentTime;
   }
   
   // Clean up assets
   application.quit();
   
	return 0;
}

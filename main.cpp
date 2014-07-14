#include <ctime>

#include "application.h"

int main(int argc, char *argv[])
{
   Application application;
   
   // Initialize application
   if(application.init())
      return 1;
   
   // application loop
   long currentTime, lastTime;
   currentTime = clock();
   while(application.isRunning()) {
      lastTime = currentTime;
      currentTime = clock();
      std::cout << static_cast<float>(currentTime - lastTime) / CLOCKS_PER_SEC << std::endl;
      
      // Update application
      application.update();
      
      // Render the screen
      application.render();
   }
   
   // Clean up assets
   application.quit();
   
	return 0;
}

#include <ctime>

#include "application.h"

int main( void )
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
      
      // Update application
      application.update();
      
      // Render the screen
      application.render();
   }
   
   // Clean up assets
   application.quit();
   
	return 0;
}

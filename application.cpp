#include "application.h"

using namespace glm;

int Application::init() {
   Input::clear();
   
   graphics = new Graphics();
   Input::init(graphics->getWindow());
   
   if(graphics->isValid()) {
      palette = new Palette();
      camera = graphics->getCamera();
      sprite = new Sprite(camera, palette);
      
      ui = new UI();
      ui->setPalette(palette);
      
      running = true;
      
      return 0;
   }
   return 1;
}

bool Application::load(const char *data, int length) {
   return sprite->loadSprite(data, length);
   //return true;
}

void Application::render() {
   graphics->beginRender();
   
   sprite->render();
   ui->render();
   
   graphics->endRender();
}

void Application::update() {
   Input::tick();
   
   camera->update();
   
   vec3 ray, direction;
   camera->computeRayAndDir(&ray, &direction);
   sprite->update(ray, direction);
   
   if(Input::shouldExit())
      running = false;
}

void Application::quit() {
   graphics->terminate();
   sprite->~Sprite();
}

int openApplication(const char *data, unsigned long length) {
   Application application;
   
   // Initialize application
   if(application.init())
      return 0;
   
   application.load(data, static_cast<int>(length));
   
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
   
   return 1;
}
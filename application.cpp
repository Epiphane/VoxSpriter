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
//
//  application.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/12/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "application.h"

int Application::init() {
   graphics = new Graphics();
   if(graphics->isValid()) {
      camera = graphics->getCamera();
      Input::setHandler(this);
      Input::setState(InGame);
      
      sprite = new Sprite();
      
      running = true;
      
      return 0;
   }
   return 1;
}

void Application::render() {
   graphics->beginRender();
   
   sprite->render(graphics);
   
   graphics->endRender();
}

void Application::update() {
   Input::tick();
   
   double speed = 0.75, angle_speed = PI / 32;
   double dxz = 0, dy = 0, dz = 0;
   if(Input::keyState(GLFW_KEY_D, GLFW_PRESS))
      dxz -= angle_speed;
   if(Input::keyState(GLFW_KEY_A, GLFW_PRESS))
      dxz += angle_speed;
   if(Input::keyState(GLFW_KEY_W, GLFW_PRESS))
      dy += angle_speed;
   if(Input::keyState(GLFW_KEY_S, GLFW_PRESS))
      dy -= angle_speed;;
   if(Input::keyState(GLFW_KEY_Q, GLFW_PRESS))
      dz += speed;
   if(Input::keyState(GLFW_KEY_E, GLFW_PRESS))
      dz -= speed;
   
   if(dxz != 0 || dy != 0)
      camera->rotateAroundOrigin(dxz, dy);
   
   if(dz != 0)
      camera->zoom(dz);
   
   if(Input::shouldExit())
   running = false;
}

void Application::quit() {
   graphics->terminate();
   sprite->~Sprite();
}
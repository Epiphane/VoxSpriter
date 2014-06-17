//
//  application.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/12/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "application.h"

using namespace glm;

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
   
   vec3 ray;
   ray = camera->getPosition();
   //direction = camera->getDirection();
   
   mat4 projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f) * camera->getViewMatrix();
   mat4 invProjection = glm::inverse(projection);
   double x, y;
   Input::getMousePos(&x, &y);
   y = w_height - y;
   glm::vec4 lRayStart_NDC(
      (x/static_cast<double>(w_width) - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
      (y/static_cast<double>(w_height) - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
      -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
      1.0f
      );
   glm::vec4 lRayEnd_NDC(
      (x/static_cast<double>(w_width)  - 0.5f) * 2.0f,
      (y/static_cast<double>(w_height) - 0.5f) * 2.0f,
      0.0,
      1.0f
      );
   
   lRayStart_NDC = invProjection * lRayStart_NDC;
   lRayEnd_NDC = invProjection * lRayEnd_NDC;
   lRayStart_NDC /= lRayStart_NDC.w;
   lRayEnd_NDC /= lRayEnd_NDC.w;
   vec4 d = glm::normalize(lRayEnd_NDC - lRayStart_NDC);
   vec3 direction = vec3(d.x, d.y, d.z);
   
   //std::cout << "Backtrace: " << ray << " -> " << direction << std::endl;
   //std::cout << "Camera: " << ray << " -> " << camera->getDirection() << std::endl;
   
   sprite->castSelect(ray, direction);
   
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
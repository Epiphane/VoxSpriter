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

bool savePress = false;
void Application::update() {
   Input::tick();
   
   // Use the mouse to update the sprite (the cursor)
   vec3 ray, direction;
   computeRayAndDir(&ray, &direction);
   sprite->update(ray, direction);
   
   // Move camera according to WASD
   const double speed = 0.75, angle_speed = PI / 32;
   double dxz = 0, dy = 0, dz = 0;
   if(Input::keyState(GLFW_KEY_D, GLFW_PRESS))
      dxz -= angle_speed;
   if(Input::keyState(GLFW_KEY_A, GLFW_PRESS))
      dxz += angle_speed;
   if(Input::keyState(GLFW_KEY_W, GLFW_PRESS))
      dy += angle_speed;
   if(Input::keyState(GLFW_KEY_S, GLFW_PRESS)) {
      if(savePress && Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE)) {
         sprite->saveSprite("sprite.vxp");
      }
      savePress = Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS);
      dy -= angle_speed;
   }
   if(Input::keyState(GLFW_KEY_Q, GLFW_PRESS))
      dz += speed;
   if(Input::keyState(GLFW_KEY_E, GLFW_PRESS))
      dz -= speed;
   
   if(dxz != 0 || dy != 0)
      camera->rotateAroundOrigin(dxz, dy);
   
   if(dz != 0)
      camera->zoom(dz);
   
   if(Input::mouseState(GLFW_MOUSE_BUTTON_1, GLFW_PRESS)) {
      double x, y;
      
      Input::getMousePos(&x, &y);
      sprite->mouseDown(x, y);
   }
   
   if(Input::shouldExit())
      running = false;
}

void Application::click(int button, int action, int mods) {
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      vec3 ray, direction;
      computeRayAndDir(&ray, &direction);
      sprite->click(ray, direction, action, mods);
   }
}

void Application::computeRayAndDir(glm::vec3 *ray, glm::vec3 *dir) {
   double x, y;
   
   mat4 projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
   mat4 view = camera->getViewMatrix();
   mat4 invProjection = glm::inverse(projection * view);
   
   Input::getMousePos(&x, &y);
   x = (x / static_cast<double>(w_width) - 0.5f) * 2.0f;
   y = ((w_height - y)/static_cast<double>(w_height) - 0.5f) * 2.0f;
   
   glm::vec4 lRayStart_NDC(x, y, -1.0, 1.0f);
   glm::vec4 lRayEnd_NDC(x, y, 0.0, 1.0f);
   lRayStart_NDC = invProjection * lRayStart_NDC;
   lRayEnd_NDC = invProjection * lRayEnd_NDC;
   lRayStart_NDC /= lRayStart_NDC.w;
   lRayEnd_NDC /= lRayEnd_NDC.w;
   
   vec4 d = glm::normalize(lRayEnd_NDC - lRayStart_NDC);
   *dir = vec3(d.x, d.y, d.z);
   *dir /= 10;

   *ray = camera->getPosition();
}

void Application::quit() {
   graphics->terminate();
   sprite->~Sprite();
}
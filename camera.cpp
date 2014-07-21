#include "camera.h"

using namespace glm;

const float speed = 0.75;
const float angle_speed = PI / 32;

void Camera::MoveCamera::execute(bool press) {
   if(ndx == FORWARD || ndx == BACKWARD)
      parent->movement[ndx] = press ? speed : 0;
   else
      parent->movement[ndx] = press ? angle_speed : 0;
}

Camera::Camera() {
   up = vec3(0, 1, 0);
   
   dist = 48;
   setAngles(vec2(PI / 4, PI / 6));
   
   for(int i = 0; i < 6; i ++)
      movement[i] = 0;
   
   // Set motion callbacks
   Input::setCallback(GLFW_KEY_W, new MoveCamera(this, UP));
   Input::setCallback(GLFW_KEY_S, new MoveCamera(this, DOWN));
   Input::setCallback(GLFW_KEY_A, new MoveCamera(this, LEFT));
   Input::setCallback(GLFW_KEY_D, new MoveCamera(this, RIGHT));
   Input::setCallback(GLFW_KEY_E, new MoveCamera(this, FORWARD));
   Input::setCallback(GLFW_KEY_Q, new MoveCamera(this, BACKWARD));
   
   vpMatrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f) * lookAt(position, vec3(0, 0, 0), up);
}

void Camera::update() {
   rotateAroundOrigin(movement[RIGHT] - movement[LEFT], movement[UP] - movement[DOWN]);
   
   zoom(movement[FORWARD] - movement[BACKWARD]);
   
   vpMatrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f) * lookAt(position, vec3(0, 0, 0), up);
}

void Camera::rotateAroundOrigin(double xz, double y) {
   if(abs(xz) < 0.00001 && abs(y) < 0.00001)
      return;
   
   rotateAroundOrigin(vec2(xz, y));
}

void Camera::rotateAroundOrigin(vec2 rotation) {
   angles += rotation;
   
   if(angles.x >= PI * 2)
      angles.x -= PI * 2;
   if(angles.x < 0)
      angles.x += PI * 2;
   if(angles.y >= PI / 2)
      angles.y  = PI / 2;
   if(angles.y < -PI / 2)
      angles.y = -PI / 2;
   
   setAngles(angles);
}

void Camera::setAngles(vec2 angle) {
   angles = angle;
   
   position.y = dist * sin(angles.y);
   
   double dist = this->dist * cos(angles.y);
   if(abs(dist) < 0.00001) {
      position.x = position.z = 0;
      up = vec3(cos(angles.x), 0, sin(angles.x));
      if(position.y > 0) up *= -1;
   }
   else {
      up = vec3(0, 1, 0);
      position.x = dist * cos(angles.x);
      position.z = dist * sin(angles.x);
   }
}

void Camera::zoom(double dx) {
   position /= dist;
   
   dist += dx;
   
   if(dist < 1) dist = 1;
   
   position *= dist;
}

vec3 Camera::getDirection() {
   vec3 dir = glm::normalize(position);
   dir *= -1;
   
   return dir;
}
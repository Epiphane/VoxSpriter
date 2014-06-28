#include "camera.h"
#include "globals.h"

using namespace glm;

Camera::Camera() {
   up = vec3(0, 1, 0);
   
   dist = 48;
   setAngles(vec2(PI / 4, PI / 7));
}

mat4 Camera::getViewMatrix() {
   return lookAt(position, vec3(0, 0, 0), up);
}

void Camera::rotateAroundOrigin(double xz, double y) {
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
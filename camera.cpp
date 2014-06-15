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
   position.x = dist * cos(angles.x);
   position.z = dist * sin(angles.x);
}

void Camera::zoom(double dx) {
   position /= dist;
   
   dist += dx;
   
   position *= dist;
}
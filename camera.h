//
//  camera.h
//  Voxels
//
//  Created by Thomas Steinke on 6/8/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __Voxels__camera__
#define __Voxels__camera__

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
   glm::vec3 position;
   glm::vec2 angles;
   double dist;
   
   glm::vec3 up;
   
   glm::vec3 chunk;
public:
   Camera();
   
   void rotateAroundOrigin(double xz, double y);
   void rotateAroundOrigin(glm::vec2 rotation);
   void setAngles(glm::vec2 angles);
   
   void move(double dx, double dy, double dz);
   void rotate(double dx, double dy);
   void zoom(double dx);
   
   glm::vec3 getPosition() { return position; }
   glm::mat4 getViewMatrix();
};

#endif /* defined(__Voxels__camera__) */

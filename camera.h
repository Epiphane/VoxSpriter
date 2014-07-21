//
//  camera.h
//  Voxels
//
//  Created by Thomas Steinke on 6/8/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __Voxels__camera__
#define __Voxels__camera__

#include "globals.h"
#include "input.h"
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
   Camera();
   
   void update();
   
   void rotateAroundOrigin(double xz, double y);
   void rotateAroundOrigin(glm::vec2 rotation);
   void setAngles(glm::vec2 angles);
   
   void move(double dx, double dy, double dz);
   void rotate(double dx, double dy);
   void zoom(double dx);
   
   glm::vec3 getPosition() { return position; }
   glm::mat4 getVPMatrix() { return vpMatrix; }
   glm::vec3 getDirection();
   
private:
   glm::vec3 position;
   glm::vec2 angles;
   glm::vec3 up;
   double dist;
   
   glm::mat4 vpMatrix;
   
   class MoveCamera : public Command {
   private:
      Camera *parent;
      int ndx;
   public:
      MoveCamera(Camera *_parent, int _ndx) : parent(_parent), ndx(_ndx) {}
      
      virtual void execute(bool press);
   };
   
   // Movement directions
   static const int UP = 0;
   static const int DOWN = 1;
   static const int LEFT = 2;
   static const int RIGHT = 3;
   static const int FORWARD = 4;
   static const int BACKWARD = 5;
   
   float movement[6];
};

#endif /* defined(__Voxels__camera__) */

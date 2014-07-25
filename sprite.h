//
//  sprite.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__sprite__
#define __VoxSpriter__sprite__

#include <fstream>
#include "palette.h"
#include "globals.h"
#include "camera.h"
#include "input.h"

class Sprite : public Observer {
public:
   Sprite(Camera *camera, Palette *palette);
   ~Sprite();
   void setBlock(int x, int y, int z, unsigned int color);
   
   void update(glm::vec3 ray, glm::vec3 direction);
   
   bool castRay(glm::vec3 ray, glm::vec3 direction, bool lastEmpty);
   
   void render();
   void createMesh();
   void colorCursor();
   
   void notify(Message message, int data);
   
   void saveSprite(const char *filename);
   bool loadSprite(const char *data, int length);
   
private:
   ThreeDimension *spriteRenderer;
   ThreeDimension *cursorRenderer;
   
   unsigned int ***blocks;
   Palette *palette;
   Camera *camera;
   
   int size[3], select[4];
   int rayCollide(glm::vec3 ray, glm::vec3 lookAhead, bool pastHalf = false);
   
   void createBlock(byte4 *vertices, byte3 *vertexRGB, int &offset,
                    unsigned int block, int x, int y, int z);
   void addFace(byte4 *vertices, byte3 *vertexRGB, int &offset,
                glm::vec3 p1, glm::vec3 p2, byte3 rgb, GLbyte n);
   
   static bool shiftPressed;
   class ShiftCommand : public Command {
   public:
      ShiftCommand(Sprite *_parent) : parent(_parent) {};
      
      virtual void execute(bool press) { shiftPressed = press; parent->colorCursor(); }
   private:
      Sprite *parent;
   };
   
   class SaveCommand : public Command {
   public:
      SaveCommand(Sprite *_parent) : parent(_parent) {};
      
      virtual void execute(bool press) { if(press) parent->saveSprite("/Users/ThomasSteinke/sprite.vxp"); }
   private:
      Sprite *parent;
   };
   
   class ClickCommand : public Command {
   public:
      ClickCommand(Sprite *_parent) : parent(_parent) {};
      
      virtual void execute(double x, double y, bool press);
   private:
      Sprite *parent;
      int oldPosition[4];
   };
};

#endif /* defined(__VoxSpriter__sprite__) */

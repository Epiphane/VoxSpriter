//
//  sprite.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__sprite__
#define __VoxSpriter__sprite__

#include <iostream>
#include "palette.h"
#include "globals.h"
#include "graphics.h"

class Sprite {
private:
   unsigned int ***blocks;
   Palette *palette;
   
   int size[3], select[3];
   bool rayCollide(glm::vec3 ray);
   
   GLuint vbo, vbo_color;
   int numVertices;
   
   void createBlock(byte4 *vertices, byte3 *vertexRGB, int &offset,
                    unsigned int block, int x, int y, int z);
   void addFace(byte4 *vertices, byte3 *vertexRGB, int &offset,
                glm::vec3 p1, glm::vec3 p2, byte3 rgb, GLbyte n);
public:
   Sprite();
   ~Sprite();
   
   void click(glm::vec3 ray, glm::vec3 direction);
   bool castSelect(glm::vec3 ray, glm::vec3 direction);
   void setBlock(int x, int y, int z, unsigned int color);
   
   void render(Graphics *graphics);
   void render(Graphics *graphics, glm::vec3 cameraChunk);
   void createMesh();
};

#endif /* defined(__VoxSpriter__sprite__) */

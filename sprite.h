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
#include <fstream>
#include "palette.h"
#include "globals.h"
#include "graphics.h"
#include "input.h"

class Sprite {
private:
   GLuint vbo, vbo_color, cursor, cursor_color;
   int numVertices;
   unsigned int ***blocks;
   Palette *palette;
   
   int size[3], select[4];
   int rayCollide(glm::vec3 ray);
   int rayCollide(glm::vec3 ray, glm::vec3 lookAhead);
   
   void createBlock(byte4 *vertices, byte3 *vertexRGB, int &offset,
                    unsigned int block, int x, int y, int z);
   void addFace(byte4 *vertices, byte3 *vertexRGB, int &offset,
                glm::vec3 p1, glm::vec3 p2, byte3 rgb, GLbyte n);
public:
   Sprite();
   ~Sprite();
   
   void update(glm::vec3 ray, glm::vec3 direction);
   void mouseDown(float x, float y);
   void click(glm::vec3 ray, glm::vec3 direction, int action, int mods);
   
   bool castRay(glm::vec3 ray, glm::vec3 direction, bool lastEmpty);
   bool castSelect(glm::vec3 ray, glm::vec3 direction);
   void setBlock(int x, int y, int z, unsigned int color);
   
   void render(Graphics *graphics);
   void render(Graphics *graphics, glm::vec3 cameraChunk);
   void createMesh();
   
   void saveSprite(const char *filename);
   void loadSprite(const char *filename);
};

static const byte4 cursorVertices[] = {
   byte4(0, 0, 1, 4), byte4(1, 1, 1, 175),
   byte4(1, 0, 0, -4), byte4(0, 1, 0, -4),
   byte4(1, 0, 1, 2), byte4(1, 1, 0, 175),
   byte4(0, 0, 0, -2), byte4(0, 1, 1, -2),
   byte4(0, 1, 1, 1), byte4(1, 1, 0, 175),
   byte4(0, 0, 0, -1), byte4(1, 0, 1, -1)
};

#endif /* defined(__VoxSpriter__sprite__) */

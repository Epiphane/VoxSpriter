//
//  sprite.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "sprite.h"

const int default_size[3] = { 16, 26, 16 };

Sprite::Sprite() {
   palette = new Palette();
   
   size[0] = default_size[0];
   size[1] = default_size[1];
   size[2] = default_size[2];
   
   blocks = new unsigned int**[size[0]];
   for(int i = 0; i < size[0]; i ++) {
      blocks[i] = new unsigned int*[size[1]];
      for(int j = 0; j < size[1]; j ++) {
         blocks[i][j] = new unsigned int[size[2]]();
         for(int k = 0; k < size[2]; k ++) {
            if(rand() % 1 == 0)
               blocks[i][j][k] = 1;
         }
      }
   }
   
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &vbo_color);
   
   createMesh();
}

Sprite::~Sprite() {
   for(int i = 0; i < size[0]; i ++) {
      for(int j = 0; j < size[1]; j ++) {
         delete [] blocks[i][j];
      }
      delete [] blocks[i];
   }
   delete [] blocks;
}

void Sprite::render(Graphics *graphics) {
   glm::vec3 position = glm::vec3(-size[0] / 2, -size[1] / 2, -size[2] / 2);
   graphics->renderSprite(vbo, vbo_color, numVertices, position);
}

void Sprite::createMesh() {
   int vertSize = size[0] * size[1] * size[2] * 12;
   byte4 *vertices = new byte4[vertSize];
   byte3 *vertexRGB = new byte3[vertSize];
   numVertices = 0;
   
   for(int x = 0; x < size[0]; x ++) {
      for(int y = 0; y < size[1]; y ++) {
         for(int z = 0; z < size[2]; z ++) {
            if(blocks[x][y][z] != 0) {
               // numVertices is automatically incremented to next empty spot
               createBlock(vertices, vertexRGB, numVertices, blocks[x][y][z], x, y, z);
            }
         }
      }
   }
   
   std::cout << vertices[0] << std::endl;
   std::cout << vertices[1] << std::endl;
   
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(byte4),
                vertices, GL_STATIC_DRAW);
   
   glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
   glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(byte3),
                vertexRGB, GL_STATIC_DRAW);
   
   delete[] vertices;
   delete[] vertexRGB;
}

void Sprite::createBlock(byte4 *vertices, byte3 *vertexRGB, int &offset,
                        unsigned int block, int x, int y, int z) {
   glm::vec3 p1 = glm::vec3(x    , y    , z + 1);
   glm::vec3 p2 = glm::vec3(x + 1, y    , z + 1);
   glm::vec3 p3 = glm::vec3(x + 1, y + 1, z + 1);
   glm::vec3 p4 = glm::vec3(x    , y + 1, z + 1);
   glm::vec3 p5 = glm::vec3(x + 1, y    , z    );
   glm::vec3 p6 = glm::vec3(x    , y    , z    );
   glm::vec3 p7 = glm::vec3(x    , y + 1, z    );
   glm::vec3 p8 = glm::vec3(x + 1, y + 1, z    );
   
   byte3 rgb = palette->getRGB(block);
   
   // Front
   if(z == size[2] - 1 || blocks[x][y][z + 1] == 0)
   addFace(vertices, vertexRGB, offset, p1, p3, rgb, 4);
   
   // Back
   if(z == 0 || blocks[x][y][z - 1] == 0)
   addFace(vertices, vertexRGB, offset, p5, p7, rgb, -4);
   
   // Right
   if(x == size[0] - 1 || blocks[x + 1][y][z] == 0)
   addFace(vertices, vertexRGB, offset, p2, p8, rgb, 2);
   
   // Left
   if(x == 0 || blocks[x - 1][y][z] == 0)
   addFace(vertices, vertexRGB, offset, p6, p4, rgb, -2);
   
   // Top
   if(y == size[1] - 1 || blocks[x][y + 1][z] == 0)
   addFace(vertices, vertexRGB, offset, p4, p8, rgb, 1);
   
   // Bottom
   if(y == 0 || blocks[x][y - 1][z] == 0)
   addFace(vertices, vertexRGB, offset, p6, p2, rgb, -1);
}

void Sprite::addFace(byte4 *vertices, byte3 *vertexRGB, int &offset,
                    glm::vec3 p1, glm::vec3 p2, byte3 rgb, GLbyte n) {
   vertexRGB[offset] = rgb;
   vertices[offset++] = byte4(p1.x, p1.y, p1.z, n);
   vertexRGB[offset] = rgb;
   vertices[offset++] = byte4(p2.x, p2.y, p2.z, 127);
}
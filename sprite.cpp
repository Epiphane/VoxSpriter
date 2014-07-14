//
//  sprite.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "sprite.h"

const int default_size[3] = { 24, 24, 24 };

Sprite::Sprite() {
   palette = new Palette();
   
   size[0] = default_size[0];
   size[1] = default_size[1];
   size[2] = default_size[2];
   select[0] = select[1] = select[2] = -1;
   select[3] = Palette::ADD;
   
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &vbo_color);
   glGenBuffers(1, &cursor);
   glGenBuffers(1, &cursor_color);
   
   glBindBuffer(GL_ARRAY_BUFFER, cursor);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices),
                cursorVertices, GL_STATIC_DRAW);
   
   glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
   glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                palette->getCursorColor(select[3]), GL_STATIC_DRAW);
   
   loadSprite("sprite.vxp");
   
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
   glm::vec3 position;
   for(int i = 0; i < 3; i ++)
      position[i] = -size[i] / 2;
   graphics->renderSprite(vbo, vbo_color, numVertices, position);
   
   if(select[0] != -1) {
      for(int i = 0; i < 3; i ++)
         position[i] = select[i] - size[i] / 2;
      graphics->renderSprite(cursor, cursor_color, 12, position);
   }
   
   palette->render(graphics);
}

bool spacePressed, bPressed;
void Sprite::update(glm::vec3 ray, glm::vec3 direction) {
   for(int key = GLFW_KEY_1; key < GLFW_KEY_9; key ++) {
      if(Input::keyState(key, GLFW_PRESS)) {
         palette->setColor(key - GLFW_KEY_0);
         glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
         glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                      palette->getCursorColor(select[3]), GL_STATIC_DRAW);
      }
   }
   
   if(!spacePressed && Input::keyState(GLFW_KEY_SPACE, GLFW_PRESS)) {
      palette->incrementGrade(Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS) ? -1 : 1);
      glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
      glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                   palette->getCursorColor(select[3]), GL_STATIC_DRAW);
      
      spacePressed = true;
   }
   else if(Input::keyState(GLFW_KEY_SPACE, GLFW_RELEASE))
      spacePressed = false;
   
   if(!bPressed && Input::keyState(GLFW_KEY_V, GLFW_PRESS)) {
      palette->incrementColor(Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_PRESS) ? -1 : 1);
      glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
      glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                   palette->getCursorColor(select[3]), GL_STATIC_DRAW);
      
      bPressed = true;
   }
   else if(Input::keyState(GLFW_KEY_V, GLFW_RELEASE))
      bPressed = false;
   
   bool offset = Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE);
   int original[4] = { select[0], select[1], select[2], select[3] };
   if(castRay(ray, direction, offset)) {
      select[3] = offset ? Palette::ADD : Palette::DELETE;
      
      if(select[3] != original[3]) {
         glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
         glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                   palette->getCursorColor(select[3]), GL_STATIC_DRAW);
      }
   }
   else {
      select[0] = select[1] = select[2] = -1;
   }
   for(int i = 0; i < 3; i ++) {
      if(select[i] != original[i]) {
         createMesh();
         return;
      }
   }
}

void Sprite::mouseDown(float x, float y) {
   palette->click(x, y);
   glBindBuffer(GL_ARRAY_BUFFER, cursor_color);
   glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(byte3),
                palette->getCursorColor(select[3]), GL_STATIC_DRAW);
}

void Sprite::click(glm::vec3 ray, glm::vec3 direction, int action, int mods) {
   bool offset = Input::keyState(GLFW_KEY_LEFT_SHIFT, GLFW_RELEASE);
   if(castRay(ray, direction, offset)) {
      select[3] = offset ? Palette::ADD : Palette::DELETE;
      
      if(select[3] == Palette::DELETE)
         setBlock(select[0], select[1], select[2], 0);
      else if(select[3] == Palette::ADD)
         setBlock(select[0], select[1], select[2], palette->getCurrentColor());
      
      update(ray, direction);
   }
}

// Returns true if the ray collides, and modifies select
bool Sprite::castRay(glm::vec3 ray, glm::vec3 direction, bool off) {
   double startDist = mag(ray);
   double dist = startDist;
   
   int res = 0;
   double lastDist = dist;
   while(dist <= lastDist && res == 0) {
      lastDist = dist;
      ray += direction;
      
      if(off)
         res = rayCollide(ray, direction);
      else
         res = rayCollide(ray);
      
      if(res == 2)
         return true;
      
      dist = mag(ray);
   }
   
   while(res != 0) {
      ray += direction;
      
      if(off)
         res = rayCollide(ray, direction);
      else
         res = rayCollide(ray);
      
      if(res == 2)
         return true;
   }
   
   return false;
}

int Sprite::rayCollide(glm::vec3 ray) {
   int pos[3] = { static_cast<int>(floor(ray.x) + size[0] / 2),
                  static_cast<int>(floor(ray.y) + size[1] / 2),
                  static_cast<int>(floor(ray.z) + size[2] / 2) };
   
   if(pos[0] < 0 || pos[0] >= size[0] ||
      pos[1] < 0 || pos[1] >= size[1] ||
      pos[2] < 0 || pos[2] >= size[2])
      return 0;
   
   // If we're in bounds, check for collision
   if(blocks[pos[0]][pos[1]][pos[2]] != 0) {
      if(select[0] != pos[0] || select[1] != pos[1] || select[2] != pos[2]) {
         for(int i = 0; i < 3; i ++)
            select[i] = pos[i];
      }
      return 2;
   }
   return 1;
}

int Sprite::rayCollide(glm::vec3 ray, glm::vec3 lookAhead) {
   lookAhead += ray;
   int pos[3] = { static_cast<int>(floor(ray.x) + size[0] / 2),
                  static_cast<int>(floor(ray.y) + size[1] / 2),
                  static_cast<int>(floor(ray.z) + size[2] / 2) };
   int look[3] = { static_cast<int>(floor(lookAhead.x) + size[0] / 2),
                   static_cast<int>(floor(lookAhead.y) + size[1] / 2),
                   static_cast<int>(floor(lookAhead.z) + size[2] / 2)};
   
   if(pos[0] < 0 || pos[0] >= size[0] ||
      pos[1] < 0 || pos[1] >= size[1] ||
      pos[2] < 0 || pos[2] >= size[2] ||
      look[0] < 0 || look[0] >= size[0] ||
      look[1] < 0 || look[1] >= size[1] ||
      look[2] < 0 || look[2] >= size[2])
      return 0;
   
   // If we're in bounds, check for collision
   if(blocks[look[0]][look[1]][look[2]] != 0) {
      if(select[0] != pos[0] || select[1] != pos[1] || select[2] != pos[2]) {
         for(int i = 0; i < 3; i ++)
            select[i] = pos[i];
      }
      return 2;
   }
   return 1;
}

void Sprite::setBlock(int x, int y, int z, unsigned int color) {
   blocks[x][y][z] = color;
}

void Sprite::createMesh() {
   int vertSize = size[0] * size[1] * size[2] * 12;
   byte4 *vertices = new byte4[vertSize];
   byte3 *vertexRGB = new byte3[vertSize];
   numVertices = 0;
   
   for(int x = 0; x < size[0]; x ++) {
      for(int y = 0; y < size[1]; y ++) {
         for(int z = 0; z < size[2]; z ++) {
            if(blocks[x][y][z] != 0 && (select[0] != x || select[1] != y || select[2] != z)) {
               // numVertices is automatically incremented to next empty spot
               createBlock(vertices, vertexRGB, numVertices, blocks[x][y][z], x, y, z);
            }
         }
      }
   }
   
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

void Sprite::saveSprite(const char *filename) {
   //std::fstream::open(filename, std::fstream::out);
}

std::vector<int> *decodeString(char *frame) {
   std::vector<int> *result = new std::vector<int>;
   
   while(0);
   
   return result;
}

int decodeChar(char ch) {
   char mask[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   for(int i = 0; i < strlen(mask); i++) {
      if(mask[i] == ch)
         return i;
   }
   
   return -1;
}

void Sprite::loadSprite(const char *filename) {
   std::fstream file("sprite.vxp", std::fstream::in);
   
   if(file.is_open())
      std::cout << "Opened sprite" << std::endl;
   
   char in;
   int start[3] = {0, 0, 0};
   while(file >> std::noskipws >> in) {
      if(in == 'C' && file >> std::noskipws >> in && in == '/') {
         // Decode colors
         int currentColor = 0, ndx = 0, rgbNdx = 0;
         char color;
         byte3 rgb;
         GLubyte val = 0;
         while(file >> std::noskipws >> color && color != ':') {
            if(ndx == 0) {
               if(color >= '0' && color <= '9') {
                  val += color - '0';
               }
               else {
                  val += color - 'a' + 10;
               }
               val <<= 4;
               ndx = 1;
            }
            else { //if(ndx == 1) {
               if(color >= '0' && color <= '9') {
                  val += color - '0';
               }
               else {
                  val += color - 'a' + 10;
               }
               
               rgb[rgbNdx++] = val;
               // Ship the color
               if(rgbNdx == 3) {
                  palette->setRGB(currentColor++, rgb);
                  rgbNdx = 0;
               }
               
               ndx = val = 0;
            }
         }
      }
      else if(in == 'A' && file >> std::noskipws >> in && in == '/') {
         // Voxels
         int current[4] = {start[0], start[1], start[2], 0};
         while(file >> std::noskipws >> in && in != ':') {
            int data = decodeChar(in);
            for(int i = 0; i < 4; i ++) {
               if((data >> (3 - i)) & 1) {
                  file >> std::noskipws >> in;
                  current[i] += decodeChar(in) - 32;
               }
            }
            if((data >> 4) & 1) {
               blocks[current[0]][current[1]][current[2]] = current[3];
            }
         }
      }
      else {
         // Size
         int ndx = 0;
         do {
            int data = decodeChar(in);
            if(ndx < 3) {
               size[ndx++] = data;
            }
            else {
               start[ndx++ - 3] = data;
            }
         } while(file >> std::noskipws >> in && in != ':');
         
         blocks = new unsigned int**[size[0]];
         for(int i = 0; i < size[0]; i ++) {
            blocks[i] = new unsigned int*[size[1]];
            for(int j = 0; j < size[1]; j ++) {
               blocks[i][j] = new unsigned int[size[2]]();
            }
         }
      }
   }
   std::cout << std::endl;
}
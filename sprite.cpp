//
//  sprite.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "sprite.h"

//const int default_size[3] = { 24, 24, 24 };

using namespace glm;

bool Sprite::shiftPressed = false;

Sprite::Sprite(Camera *_camera, Palette *_palette) {
   palette = _palette;
   select[3] = palette->getCurrent();
   observe(palette);
   
   camera = _camera;
   
   spriteRenderer = new ThreeDimension();
   spriteRenderer->setCamera(camera);
   spriteRenderer->setVertexFormat(GL_UNSIGNED_BYTE);
   
   cursorRenderer = new ThreeDimension();
   cursorRenderer->setCamera(camera);
   cursorRenderer->setVertexFormat(GL_UNSIGNED_BYTE);
   cursorRenderer->setNumVertices(12);
   
   byte4 cursorVertices[] = {
      byte4(0, 0, 1, 4), byte4(1, 1, 1, 150),
      byte4(1, 0, 0, -4), byte4(0, 1, 0, -4),
      byte4(1, 0, 1, 2), byte4(1, 1, 0, 150),
      byte4(0, 0, 0, -2), byte4(0, 1, 1, -2),
      byte4(0, 1, 1, 1), byte4(1, 1, 0, 150),
      byte4(0, 0, 0, -1), byte4(1, 0, 1, -1)
   };
   cursorRenderer->bufferData(ThreeDimension::VERTEX_BUFFER, 12 * sizeof(byte4), cursorVertices);
   colorCursor();
   
   size[0] = 0;
   loadSprite(0, 0);
   
   vec3 position;
   for(int i = 0; i < 3; i ++)
      position[i] = -size[i] / 2;
   
   spriteRenderer->setPosition(position);
   
   Input::setCallback(GLFW_KEY_LEFT_SHIFT, new ShiftCommand(this));
   Input::setModCallback(GLFW_KEY_LEFT_SUPER, GLFW_KEY_S, new SaveCommand(this));
   Input::setMouseCallback(new ClickCommand(this));
   
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

void Sprite::render() {
   spriteRenderer->render();
   if(select[0] != -1)
      cursorRenderer->render();
}

void Sprite::update(vec3 ray, vec3 direction) {
   bool offset = !shiftPressed;
   int original[4] = { select[0], select[1], select[2], select[3] };
   if(castRay(ray, direction, offset)) {
      cursorRenderer->setPosition(vec3(select[0] - size[0] / 2, select[1] - size[1] / 2, select[2] - size[2] / 2));
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

void Sprite::notify(Message message, int data) {
   if(message == PALETTE_SELECT_CHANGE) {
      select[3] = data;
      colorCursor();
   }
   else if(message == PALETTE_COLOR_CHANGE) {
      colorCursor();
   }
}

void Sprite::ClickCommand::execute(double x, double y, bool press) {
   if(press) {
      vec3 ray, direction;
      parent->camera->computeRayAndDir(&ray, &direction);
   
      bool offset = !shiftPressed;
      if(parent->castRay(ray, direction, offset)) {
         bool moved = false;
         for(int i = 0; i < 4; i ++) {
            moved |= oldPosition[i] != parent->select[i];
         }
      
         if(!moved)
            return;
      
         if(offset) { // ADD
            parent->setBlock(parent->select[0], parent->select[1], parent->select[2], parent->select[3]);
         }
         else { // DELETE
            parent->setBlock(parent->select[0], parent->select[1], parent->select[2], 0);
         }
         
         parent->update(ray, direction);
         for(int i = 0; i < 4; i ++)
            oldPosition[i] = parent->select[i];
      }
   }
   else {
      for(int i = 0; i < 4; i ++)
         oldPosition[i] = -1;
   }
}

// Returns true if the ray collides, and modifies select
bool Sprite::castRay(vec3 ray, vec3 direction, bool off) {
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
         res = rayCollide(ray, vec3(0, 0, 0));
      
      if(res == 2)
         return true;
      
      dist = mag(ray);
   }
   
   while(res != 2) {
      ray += direction;
      
      if(off)
         res = rayCollide(ray, direction, true);
      else
         res = rayCollide(ray, vec3(0, 0, 0), true);
   }
   
   return true;
}

int Sprite::rayCollide(vec3 ray, vec3 look, bool pastHalf) {
   bool offset = look == vec3(0, 0, 0);
   
   look += ray;
   int pos[3] = { static_cast<int>(floor(look.x) + size[0] / 2),
                   static_cast<int>(floor(look.y) + size[1] / 2),
                   static_cast<int>(floor(look.z) + size[2] / 2)};
   
   bool outOfBounds = pos[0] < 0 || pos[1] < 0 ||  pos[0] >= size[0] || pos[1] >= size[1] || pos[2] < 0 || pos[2] >= size[2];
   if(!pastHalf && outOfBounds)
      return 0;
   
   // If we're in bounds, check for collision
   if(pastHalf && outOfBounds || blocks[pos[0]][pos[1]][pos[2]] != 0) {
      int pos[3] = { static_cast<int>(floor(ray.x) + size[0] / 2), static_cast<int>(floor(ray.y) + size[1] / 2), static_cast<int>(floor(ray.z) + size[2] / 2) };
      
      if(select[0] != pos[0] || select[1] != pos[1] || select[2] != pos[2]) {
         
         for(int i = 0; i < 3; i ++)
            select[i] = pos[i];
         
         // Let's hide it if it's out of bound and we're deleting
         if(outOfBounds && offset) {
            select[0] = -1;
         }
      }
      return 2;
   }
   return 1;
}

void Sprite::setBlock(int x, int y, int z, unsigned int color) {
   if(x < 0 || x >= size[0] || y < 0 || y >= size[1] || z < 0 || z >= size[2]) {
      return;
   }
   
   blocks[x][y][z] = color;
}

void Sprite::colorCursor() {
   if(shiftPressed)
      cursorRenderer->bufferData(ThreeDimension::COLOR_BUFFER, 12 * sizeof(byte3), palette->getColorMesh(Palette::DELETE));
   else
      cursorRenderer->bufferData(ThreeDimension::COLOR_BUFFER, 12 * sizeof(byte3), palette->getColorMesh(select[3]));
}

void Sprite::createMesh() {
   int vertSize = size[0] * size[1] * size[2] * 12 + size[0] * size[2] * 2;
   byte4 *vertices = new byte4[vertSize];
   byte3 *vertexRGB = new byte3[vertSize];
   int numVertices = 0;
   
   // Grid
   byte3 gridColor = byte3(10, 160, 160);
   for(int x = 0; x < size[0]; x ++) {
      for(int z = 0; z < size[2]; z ++) {
         if(x % 2 == z % 2) {
            addFace(vertices, vertexRGB, numVertices, vec3(x, 0, z + 1), vec3(x + 1, 0, z), gridColor, 0);
            
            addFace(vertices, vertexRGB, numVertices, vec3(x + 1, size[1], z + 1), vec3(x, size[1], z), gridColor, 0);
         }
      }
      for(int y = 0; y < size[1]; y ++) {
         if(x % 2 == y % 2) {
            addFace(vertices, vertexRGB, numVertices, vec3(x + 1, y + 1, 0), vec3(x, y, 0), gridColor, 0);
            
            addFace(vertices, vertexRGB, numVertices, vec3(x + 1, y, size[2]), vec3(x, y + 1, size[2]), gridColor, 0);
         }
      }
   }
   for(int y = 0; y < size[1]; y ++) {
      for(int z = 0; z < size[2]; z ++) {
         if(y % 2 == z % 2) {
            addFace(vertices, vertexRGB, numVertices, vec3(0, y, z + 1), vec3(0, y + 1, z), gridColor, 0);
            
            addFace(vertices, vertexRGB, numVertices, vec3(size[0], y + 1, z + 1), vec3(size[0], y, z), gridColor, 0);
         }
      }
   }
   
   
   // Cubes
   for(int x = 0; x < size[0]; x ++) {
      for(int z = 0; z < size[2]; z ++) {
         for(int y = 0; y < size[1]; y ++) {
            if(blocks[x][y][z] != 0 && (select[0] != x || select[1] != y || select[2] != z)) {
               // numVertices is automatically incremented to next empty spot
               createBlock(vertices, vertexRGB, numVertices, blocks[x][y][z], x, y, z);
            }
         }
      }
   }
   
   spriteRenderer->setNumVertices(numVertices);
   spriteRenderer->bufferData(ThreeDimension::VERTEX_BUFFER, numVertices * sizeof(byte4), vertices);
   spriteRenderer->bufferData(ThreeDimension::COLOR_BUFFER, numVertices * sizeof(byte3), vertexRGB);

   delete[] vertices;
   delete[] vertexRGB;
}

void Sprite::createBlock(byte4 *vertices, byte3 *vertexRGB, int &offset, unsigned int block, int x, int y, int z) {
   vec3 p1 = vec3(x    , y    , z + 1);
   vec3 p2 = vec3(x + 1, y    , z + 1);
   vec3 p3 = vec3(x + 1, y + 1, z + 1);
   vec3 p4 = vec3(x    , y + 1, z + 1);
   vec3 p5 = vec3(x + 1, y    , z    );
   vec3 p6 = vec3(x    , y    , z    );
   vec3 p7 = vec3(x    , y + 1, z    );
   vec3 p8 = vec3(x + 1, y + 1, z    );
   
   byte3 rgb = palette->getColor(block);
   
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
                    vec3 p1, vec3 p2, byte3 rgb, GLbyte n) {
   vertexRGB[offset] = rgb;
   vertices[offset++] = byte4(p1.x, p1.y, p1.z, n);
   vertexRGB[offset] = rgb;
   vertices[offset++] = byte4(p2.x, p2.y, p2.z, 127);
}

// -----------------------
// FILE STUFF
// -----------------------

// Can encode -32 to 31. Don't overflow pls
char *encode(std::vector<int> data) {
   //char mask[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   
   char *result = new char[data.size()];
   for(int ndx = 0; ndx < data.size(); ndx ++) {
      result[ndx] = data[ndx];//mask[data[ndx] + 32];
   }
   
   return result;
}

void Sprite::saveSprite(const char *filename) {
   std::fstream outputFile(filename, std::fstream::out);
   
   if(outputFile.is_open())
      std::cout << "Saving " << filename << std::endl;
   
   // Size data
   char sizeData[3];
   for(int i = 0; i < 3; i ++)
      sizeData[i] = size[i];
   outputFile.write(sizeData, 3);
   
   // Palette data
   char *paletteData = new char[numColors * 3];
   for(int i = 0; i < numColors; i ++) {
      byte3 rgb = palette->getColor(i);
      paletteData[i * 3    ] = rgb.r;
      paletteData[i * 3 + 1] = rgb.g;
      paletteData[i * 3 + 2] = rgb.b;
   }
   outputFile.write(":C/", 3);
   outputFile.write(paletteData, numColors * 3);
   delete paletteData;
   
   // Voxel data
   std::vector<int> data;
   int last[4] = { 0, 0, 0, 0 };
   int current[4] = {0, 0, 0, 0 };
   for(current[0] = 0; current[0] < size[0]; current[0] ++) {
      for(current[1] = 0; current[1] < size[1]; current[1] ++) {
         for(current[2] = 0; current[2] < size[2]; current[2] ++) {
            if(current[3] = blocks[current[0]][current[1]][current[2]]) {
               int code = 0;
               int codes[4] = { 0, 0, 0, 0 };
               
               for(int i = 0; i < 4; i ++) {
                  if(current[i] != last[i]) {
                     code += (1 << i);
                     codes[i] = current[i] - last[i];
                     last[i] = current[i];
                  }
               }
               data.push_back(code);
               for(int i = 0; i < 4; i ++) {
                  if(code & (1 << i)) {
                     data.push_back(codes[i]);
                  }
               }
            }
         }
      }
   }
   
   char *dataStr = encode(data);
   outputFile.write(":A/", 3);
   outputFile.write(dataStr, data.size());
   delete dataStr;
   
   outputFile.close();
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

bool Sprite::loadSprite(const char *data, int length) {
   if(size[0]) {
      for(int i = 0; i < size[0]; i ++) {
         for(int j = 0; j < size[1]; j ++) {
            delete [] blocks[i][j];
         }
         delete [] blocks[i];
      }
      delete [] blocks;
   }
   
   if(data == 0) {
      // Default: just set up size
      for(int i = 0; i < 3; i ++)
         size[i] = 24;
      
      blocks = new unsigned int**[size[0]];
      for(int i = 0; i < size[0]; i ++) {
         blocks[i] = new unsigned int*[size[1]];
         for(int j = 0; j < size[1]; j ++) {
            blocks[i][j] = new unsigned int[size[2]]();
         }
      }
      
      return true;
   }
   
   char in;
   int dNdx = 0;
   int start[3] = {0, 0, 0};
   while(dNdx < length) {
      in = data[dNdx++];
      if(in == 'C' && data[dNdx++] == '/') {
         // Decode colors
         int currentColor = 0, rgbNdx = 0;
         char color;
         byte3 rgb;
         while(dNdx < length) {
            color = data[dNdx++];
            if(color == ':')
               break;
            
            rgb[rgbNdx++] = color;
            // Ship the color
            if(rgbNdx == 3) {
               palette->setColor(currentColor++, rgb);
               rgbNdx = 0;
            }
         }
      }
      else if(in == 'A' && data[dNdx++] == '/') {
         // Voxels
         int current[4] = {start[0], start[1], start[2], 0};
         while(dNdx < length && (in = data[dNdx++]) && in != ':') {
            int tData = in;
            for(int i = 0; i < 4; i ++) {
               if(tData & (1 << i)) {
                  in = data[dNdx++];
                  current[i] += in;
               }
            }
            blocks[current[0]][current[1]][current[2]] = current[3];
         }
      }
      else {
         // Size
         int ndx = 0;
         do {
            if(ndx < 3) {
               size[ndx++] = in;
            }
         } while((in = data[dNdx++]) && in != ':');
         
         blocks = new unsigned int**[size[0]];
         for(int i = 0; i < size[0]; i ++) {
            blocks[i] = new unsigned int*[size[1]];
            for(int j = 0; j < size[1]; j ++) {
               blocks[i][j] = new unsigned int[size[2]]();
            }
         }
      }
   }
   
   return true;
}
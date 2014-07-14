//
//  palette.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__palette__
#define __VoxSpriter__palette__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "globals.h"
#include "graphics.h"

struct PaletteColor {
//   unsigned int ID;
   byte3 RGB;
};

const int gradeSize = 7;
const int numColors = 12 * gradeSize;

class Palette {
private:
   PaletteColor palette[numColors];
   
   void makeColor(int ndx, byte3 rgb);
   void makeGradient(int ndx, float h, float s, float v);
   void makeGradient(int ndx, float h, float sFrom, float sTo, float vFrom, float vTo);
   int size;
   int currentColor;
   
   GLuint SVSquareTexture, hBarTexture;
   float hueSelect;
public:
   Palette();
   
   static const unsigned int DEFAULT = UINT_MAX;
   static const unsigned int DELETE = UINT_MAX - 1;
   static const unsigned int ADD = UINT_MAX - 2;
   
   byte3 *getCursorColor(int cursor);
   
   byte3 getCurrentColorRGB();
   int getCurrentColor() { return currentColor + 1; }
   void setColor(int color);
   void incrementColor(int amount);
   void incrementGrade(int amount);
   byte3 getRGB(unsigned int colorID);
   void setRGB(unsigned int colorID, byte3 rgb);
   
   void render(Graphics *graphics);
   void click(int x, int y);
};

const byte3 defaultPalette[] = {
   byte3(0, 0, 0), byte3(60, 60, 60),
   byte3(128, 128, 128), byte3(180, 180, 180),
   byte3(200, 200, 200), byte3(220, 220, 220),
   byte3(255, 255, 255),
   byte3(51, 21, 0), byte3(87, 47, 10),
   byte3(124, 77, 32), byte3(153, 106, 62),
   byte3(194, 147, 102), byte3(206, 164, 125),
   byte3(237, 200, 167),
   byte3(148, 57, 13), byte3(181, 84, 37),
   byte3(210, 115, 70), byte3(246, 161, 120),
   byte3(255, 187, 153), byte3(255, 200, 167),
   byte3(255, 225, 200),
   
   byte3(0, 102, 0), byte3(0, 153, 0),
   byte3(0, 204, 0), byte3(0, 225, 0),
   byte3(51, 255, 51), byte3(102, 255, 102),
   byte3(153, 255, 153),
   byte3(102, 102, 0), byte3(153, 153, 0),
   byte3(204, 204, 0), byte3(225, 225, 0),
   byte3(255, 255, 51), byte3(255, 255, 102),
   byte3(255, 255, 153),
   byte3(102, 0, 0), byte3(153, 0, 0),
   byte3(204, 0, 0), byte3(225, 0, 0),
   byte3(255, 51, 51), byte3(255, 102, 102),
   byte3(255, 153, 153),
   byte3(102, 0, 102), byte3(153, 0, 153),
   byte3(204, 0, 204), byte3(225, 0, 225),
   byte3(255, 51, 255), byte3(255, 102, 255),
   byte3(255, 153, 255),
   byte3(0, 0, 102), byte3(0, 0, 153),
   byte3(0, 0, 204), byte3(0, 0, 225),
   byte3(51, 51, 255), byte3(102, 102, 255),
   byte3(153, 153, 255),
   byte3(0, 102, 102), byte3(0, 153, 153),
   byte3(0, 204, 204), byte3(0, 225, 225),
   byte3(51, 255, 255), byte3(102, 255, 255),
   byte3(153, 255, 255)
};

#endif /* defined(__VoxSpriter__palette__) */

//
//  palette.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "palette.h"

using namespace glm;

PaletteColor *createColor(GLubyte r, GLubyte g, GLubyte b) {
   PaletteColor *newColor = new PaletteColor;
   newColor->RGB = byte3(r, g, b);
   return newColor;
}

Palette::Palette() {
   // Set up default palette
   palette.push_back(createColor(255, 255, 255));
   palette.push_back(createColor(50, 50, 255));
   palette.push_back(createColor(255, 50, 50));
   palette.push_back(createColor(50, 255, 50));
   palette.push_back(createColor(50, 255, 255));
   palette.push_back(createColor(255, 255, 50));
   palette.push_back(createColor(255, 50, 255));
   
   currentColor = 1;
}

void Palette::render(Graphics *graphics) {
   float squareHeight = w_height * 2 / 7;
   float squareWidth = squareHeight / w_width;
   squareHeight /= w_height;
   for(int i = 0; i < 7; i ++) {
      vec3 pos;
      pos.x = 1.0f - squareWidth * 3 / 4;
      pos.y = -1 + i * squareHeight + squareHeight / 4;
      pos.z = 0;
      graphics->renderSquareFlat(palette[i]->RGB, pos, squareWidth / 2, squareHeight / 2);
   }
}

byte3 Palette::getRGB(unsigned int colorID) {
   if(colorID == DEFAULT)
      return byte3(200, 200, 200);
   
   if(colorID < palette.size()) {
      return palette[colorID]->RGB;
   }
   
   return byte3(0, 0, 0);
}

const byte3 BLACK = byte3(0, 0, 0);
const byte3 RED = byte3(255, 50, 50);
const byte3 GREEN = byte3(50, 255, 50);

byte3 Palette::getCurrentColorRGB() {
   return getRGB(currentColor);
}

byte3 *Palette::getCursorColor(int cursor) {
   byte3 *cursorColor = new byte3[12];
   byte3 color;
   
   if(cursor == DELETE)
      color = RED;
   else if(cursor == ADD)
      color = palette[currentColor]->RGB * byte3(2, 2, 2);
   
   if(color.r > 255) color.r = 255;
   if(color.g > 255) color.g = 255;
   if(color.b > 255) color.b = 255;
   
   for(int i = 0; i < 12; i ++) {
      cursorColor[i] = color;
   }
   
   return cursorColor;
}

void Palette::setColor(int color) {
   if(color >= palette.size())
      return;
   currentColor = color;
}
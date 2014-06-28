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

class Palette {
private:
   std::vector<PaletteColor *> palette;
   
   int size;
   int currentColor;
public:
   Palette();
   
   static const unsigned int DEFAULT = UINT_MAX;
   static const unsigned int DELETE = UINT_MAX - 1;
   static const unsigned int ADD = UINT_MAX - 2;
   
   byte3 *getCursorColor(int cursor);
   
   byte3 getCurrentColorRGB();
   int getCurrentColor() { return currentColor; }
   void setColor(int color);
   byte3 getRGB(unsigned int colorID);
   
   void render(Graphics *graphics);
};

#endif /* defined(__VoxSpriter__palette__) */

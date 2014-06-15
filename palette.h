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

struct PaletteColor {
   unsigned int ID;
   glm::vec3 RGB;
};

class Palette {
private:
   std::vector<PaletteColor *> palette;
   
   int size;
public:
   byte3 getRGB(unsigned int colorID);
};

#endif /* defined(__VoxSpriter__palette__) */

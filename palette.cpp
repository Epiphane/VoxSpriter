//
//  palette.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "palette.h"

using namespace glm;

byte3 Palette::getRGB(unsigned int colorID) {
   switch(colorID) {
      case 1:
         return byte3(0, 255, 0);
      default:
         return byte3(255, 127, 0);
   }
}
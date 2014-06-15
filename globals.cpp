//
//  globals.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "globals.h"

const int w_width = 1024;
const int w_height = 768;
const char *w_title = "VoxSpriter";
const double PI = 3.14159265;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
   out << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
   return out;
}

std::ostream &operator<< (std::ostream &out, const byte4 &vec) {
   out << "{" << static_cast<int>(vec.x) << ", " << static_cast<int>(vec.y) << ", " << static_cast<int>(vec.z) << ", " << static_cast<int>(vec.w) << "}";
   return out;
}

std::ostream &operator<< (std::ostream &out, const byte3 &vec) {
   out << "{" << static_cast<int>(vec.x) << ", " << static_cast<int>(vec.y) << ", " << static_cast<int>(vec.z) << "}";
   return out;
}
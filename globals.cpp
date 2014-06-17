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

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
   for(int i = 0; i < 4; i ++) {
      out << "| ";
      for(int j = 0; j < 4; j ++)
         out << mat[i][j] << " ";
      out << "|" << std::endl;
   }
   return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
   out << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
   return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
   out << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
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

bool operator==(const glm::vec3 &vecA, const glm::vec3 &vecB)
{
   const double epsilion = 0.0001;  // choose something apprpriate.
   
   return   fabs(vecA[0] - vecB[0]) < epsilion
         && fabs(vecA[1] - vecB[1]) < epsilion
         && fabs(vecA[2] - vecB[2]) < epsilion;
}

bool operator!= (const glm::vec3 &vecA, const glm::vec3 &vecB) {
   return !(vecA == vecB);
}

double mag(const glm::vec3 &vec) {
   return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
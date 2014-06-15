#ifndef ____globals__
#define ____globals__

#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern const int w_width;
extern const int w_height;
extern const char *w_title;

extern const double PI;

typedef glm::detail::tvec4<GLubyte> byte4;
typedef glm::detail::tvec3<GLubyte> byte3;
typedef glm::detail::tvec2<GLfloat> byte2;

std::ostream &operator<< (std::ostream &out, const byte4 &vec);
std::ostream &operator<< (std::ostream &out, const byte3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);

#endif

#ifndef ____globals__
#define ____globals__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Constant values */
extern const int w_width;
extern const int w_height;
extern const char *w_title;
extern const float PI;
extern const int FRAMES_PER_SEC;
extern const double SEC_PER_FRAME;

/* Vector operators */
bool operator== (const glm::vec3 &vecA, const glm::vec3 &vecB);
bool operator!= (const glm::vec3 &vecA, const glm::vec3 &vecB);
double mag(const glm::vec3 &vec);

/* Variable typedefs */
typedef glm::detail::tvec3<GLubyte> byte3;
typedef glm::detail::tvec4<GLubyte> byte4;

/* Debugging functions */
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);
std::ostream &operator<< (std::ostream &out, const byte3 &vec);
std::ostream &operator<< (std::ostream &out, const byte4 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat);

#endif

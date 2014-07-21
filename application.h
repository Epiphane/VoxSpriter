//
//  application.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/12/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__application__
#define __VoxSpriter__application__

#include "graphics.h"
#include "globals.h"
#include "input.h"
#include "interface.h"
#include "sprite.h"

class Application {
public:
   Application() :
   graphics(), running(false) {};
   
   int init();
   void update();
   void render();
   void quit();
   
   void mouseMove(double x, double y);
   void click(int button, int action, int mods);
   
   bool isRunning() { return running; }
   
private:
   Graphics *graphics;
   Camera *camera;
   Sprite *sprite;
   UI *ui;
   
   bool running;
   
   void computeRayAndDir(glm::vec3 *ray, glm::vec3 *direction);
};

#endif /* defined(__VoxSpriter__application__) */

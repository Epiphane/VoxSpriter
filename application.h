//
//  application.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/12/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__application__
#define __VoxSpriter__application__

#include <iostream>
#include "graphics.h"
#include "globals.h"
#include "input.h"
#include "sprite.h"

class Application {
private:
   Graphics *graphics;
   Camera *camera;
   Sprite *sprite;
   
   bool running;
public:
   Application() :
   graphics(), running(false) {};
   
   int init();
   void update();
   void render();
   void quit();
   
   void mouseMove(double x, double y);
   
   bool isRunning() { return running; }
};

#endif /* defined(__VoxSpriter__application__) */

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
   
   bool isRunning() { return running; }
   
private:
   Graphics *graphics;
   Camera *camera;
   Palette *palette;
   Sprite *sprite;
   UI *ui;
   
   bool running;
};

#endif /* defined(__VoxSpriter__application__) */

//
//  observer.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 7/20/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__observer__
#define __VoxSpriter__observer__

#include "globals.h"

enum Message {
   PALETTE_COLOR_CHANGE,
   PALETTE_SELECT_CHANGE
};

class Observed;

class Observer {
public:
   void observe(Observed *obs);
   void cutTies();
   virtual void notify(Message message, int data = 0) = 0;
   
   ~Observer();
private:
   Observed *target;
};

class Observed {
public:
   void removeObserver(Observer *obs);
   void addObserver(Observer *obs);
   void send(Message message, int data = 0);
private:
   std::vector<Observer *> observers;
};

#endif /* defined(__VoxSpriter__observer__) */

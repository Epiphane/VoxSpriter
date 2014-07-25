//
//  observer.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 7/20/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "observer.h"

using namespace std;

Observer::~Observer() {
   cutTies();
}

void Observer::observe(Observed *obs) {
   obs->addObserver(this);
   target = obs;
}

void Observer::cutTies() {
   target->removeObserver(this);
   target = 0;
}

void Observed::addObserver(Observer *obs) {
   observers.push_back(obs);
}

void Observed::removeObserver(Observer *obs) {
   vector<Observer *>::iterator it = observers.begin();
   
   while(it < observers.end()) {
      if(*it == obs)
         observers.erase(it);
      else
         it ++;
   }
}

void Observed::send(Message message, int data) {
   vector<Observer *>::iterator it = observers.begin();
   
   while(it < observers.end()) {
      (*it)->notify(message, data);
      it ++;
   }
}
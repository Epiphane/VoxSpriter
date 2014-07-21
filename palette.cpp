//
//  palette.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include <string>
#include "palette.h"

const byte3 BLACK = byte3(0, 0, 0);
const byte3 RED = byte3(255, 50, 50);
const byte3 GREEN = byte3(50, 255, 50);

Palette::Palette() {
   currentColor = 0;
   
   for(int i = 0; i < sizeof(defaultPalette) / sizeof(byte3); i ++) {
      makeColor(i, defaultPalette[i]);
   }
   
   currentColor = gradeSize / 2;
}

void Palette::makeGradient(int ndx, float h, float sFrom, float sTo, float vFrom, float vTo) {
   ndx *= gradeSize;
   
   float s = sFrom, v = vFrom;
   
   float ds = (sTo - sFrom) / gradeSize;
   float dv = (vTo - vFrom) / gradeSize;
   
   for(int i = 0; i < gradeSize; i ++) {
      palette[ndx++] = HSVtoRGB(h, s, v);
      s += ds;
      v += dv;
   }
}

byte3 HSVtoRGB(float h, float s, int v) {
   GLubyte r, g, b;
   
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		return byte3(v, v, v);
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial posture of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}
   
   return byte3(r, g, b);
}

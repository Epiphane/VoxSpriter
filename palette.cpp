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
      setColor(i, defaultPalette[i]);
   }
   
   currentColor = gradeSize / 2;
}

byte3 *Palette::getColorMesh(int colorID) {
   byte3 *colorMesh = new byte3[12];
   byte3 color;
   
   if(colorID == DELETE)
      color = RED;
   else
      color = palette[colorID];
   
   for(int i = 0; i < 12; i ++) {
      colorMesh[i] = color;
   }
   
   return colorMesh;
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

void Palette::setCurrent(int _current) {
   if(currentColor != _current) {
      currentColor = _current;
      send(PALETTE_SELECT_CHANGE, currentColor);
   }
}

void Palette::setCurrentColor(byte3 rgb) {
   setColor(currentColor, rgb);
}

void Palette::setColor(int colorID, byte3 rgb) {
   if(palette[colorID] != rgb) {
      palette[colorID] = rgb;
      send(PALETTE_COLOR_CHANGE, colorID);
   }
}

byte3 HSVtoRGB(float h, float s, int v) {
   h *= 360;
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

using namespace glm;

vec3 RGBtoHSV(byte3 rgb) {
   vec3 hsv;
   double min, max, delta;
   
   min = rgb.r < rgb.g ? rgb.r : rgb.g;
   min = min   < rgb.b ? min   : rgb.b;
   
   max = rgb.r > rgb.g ? rgb.r : rgb.g;
   max = max   > rgb.b ? max   : rgb.b;
   
   hsv.b = max;                                // v
   delta = max - min;
   if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
      hsv.g = (delta / max);                  // s
   } else {
      // if max is 0, then r = g = b = 0
      // s = 0, v is undefined
      hsv.g = 0.0;
      hsv.r = NAN;                            // its now undefined
      return hsv;
   }
   if( rgb.r >= max )                           // > is bogus, just keeps compilor happy
      hsv.r = ( rgb.g - rgb.b ) / delta;        // between yellow & magenta
   else
      if( rgb.g >= max )
         hsv.r = 2.0 + ( rgb.b - rgb.r ) / delta;  // between cyan & yellow
      else
         hsv.r = 4.0 + ( rgb.r - rgb.g ) / delta;  // between magenta & cyan
   
   hsv.r /= 6.0;
   
   if( hsv.r < 0.0 )
      hsv.r += 1.0;
   
   return hsv;
}
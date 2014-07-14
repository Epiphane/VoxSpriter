//
//  palette.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 6/13/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include <string>
#include "texture.h"
#include "palette.h"

using namespace glm;

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

Palette::Palette() {
   currentColor = hueSelect = 0;
   
   for(int i = 0; i < sizeof(defaultPalette) / sizeof(byte3); i ++) {
   //   makeColor(i, defaultPalette[i]);
   }
   
   currentColor = gradeSize / 2;
   
   SVSquareTexture = loadImage("img/saturation.png");
   hBarTexture = loadImage("img/hue.png");
}

void Palette::render(Graphics *graphics) {
   vec3 pos;
   byte3 rgb;
   
   float squareHeight = w_height * 8 / numColors;
   float squareWidth = squareHeight / w_width;
   
   squareHeight /= w_height;
   for(int i = 0; i < numColors; i ++) {
      rgb = palette[i].RGB;
      pos.x = 1.0 - squareWidth * (gradeSize - i % gradeSize);
      pos.y = 1.0 - (i / gradeSize + 1) * squareHeight;
      pos.z = 0;
      if(i == currentColor)
         graphics->renderSquareFlat(rgb, pos + vec3(squareWidth / 8, squareHeight / 8, 0), squareWidth * 6 / 8, squareHeight * 6 / 8);
      else
         graphics->renderSquareFlat(rgb, pos, squareWidth, squareHeight);
   }
   
   pos.x = 1.0 - squareWidth * (gradeSize - currentColor % gradeSize - 3.0f / 16);
   pos.y = 1.0 - squareHeight * (currentColor / gradeSize + 13.0f / 16);
   pos.z = 0;
   graphics->renderSquareFlat(byte3(0, 0, 0), pos, squareWidth * 5 / 8, squareHeight * 5 / 8);
   pos -= vec3(squareWidth / 16, squareHeight / 16, 0);
   graphics->renderSquareFlat(byte3(255,255,255), pos, squareWidth * 3 / 4, squareHeight * 3 / 4);
   
   float w = squareHeight * w_height * gradeSize / 2;
   
   graphics->renderFlatSprite(SVSquareTexture, vec2(0, 0), 1, 1, vec2(w_width - w, 0), w, w, hueSelect);
   
   graphics->renderFlatSprite(hBarTexture, vec2(0, 0), 1, 1, vec2(w_width - w, w), w, 25);
   
   graphics->renderSquareFlat(byte3(0, 0, 0), vec3(1 - squareWidth * gradeSize, -1, 0), squareWidth * gradeSize, 2);
}

void Palette::click(int x, int y) {
   float width = w_height * 4 * gradeSize / numColors;
   float squareHeight = w_height * 4 / numColors;
   
   x -= w_width - width;
   if(x >= 0 && x < width) {
      if(y > w_height || y < 0) {
         return;
      }
      if(y >= w_height - width) {
         y -= w_height - width;
         float sat = x / width;
         int val = 255 - y * 255 / width;
         
         palette[currentColor].RGB = HSVtoRGB(hueSelect * 360, sat, val);
      }
      else if(y >= w_height - width - 25) {
         hueSelect = x / width;
      }
      else if(y <= squareHeight * numColors / gradeSize) {
         y /= squareHeight;
         x /= squareHeight;
         currentColor = x + y * gradeSize;
      }
   }
}

byte3 Palette::getRGB(unsigned int colorID) {
   if(colorID == DEFAULT)
      return byte3(127, 127, 127);
   
   if(colorID <= numColors && colorID > 0) {
      return palette[colorID - 1].RGB;
   }
   
   return byte3(0, 0, 0);
}

void Palette::setRGB(unsigned int colorID, byte3 rgb) {
   if(colorID <= numColors && colorID > 0) {
      palette[colorID - 1].RGB = rgb;
   }
}

const byte3 BLACK = byte3(0, 0, 0);
const byte3 RED = byte3(255, 50, 50);
const byte3 GREEN = byte3(50, 255, 50);

byte3 Palette::getCurrentColorRGB() {
   return getRGB(currentColor);
}

byte3 *Palette::getCursorColor(int cursor) {
   byte3 *cursorColor = new byte3[12];
   byte3 color;
   
   if(cursor == DELETE)
      color = RED;
   else if(cursor == ADD) {
      color = palette[currentColor].RGB;
   }
   
   if(color.r > 255) color.r = 255;
   if(color.g > 255) color.g = 255;
   if(color.b > 255) color.b = 255;
   
   for(int i = 0; i < 12; i ++) {
      cursorColor[i] = color;
   }
   
   return cursorColor;
}

void Palette::makeColor(int ndx, byte3 rgb) {
   palette[ndx].RGB = rgb;
}

vec2 getSV(float d) {
   return vec2(0, 0);
}

void Palette::makeGradient(int ndx, float h, float s, float v) {
   // Translate to SV circle (clip off corners)
   if(s * s + v * v > 1.0f) {
      float rad = atan2f(v, s);
      s = cos(rad);
      v = sin(rad);
   }
   
   ndx *= gradeSize;
   
   float d = 0.25f;
   for(int i = 0; i < gradeSize; i ++) {
      vec2 sv = getSV(d);
      palette[ndx++].RGB = HSVtoRGB(h, sv[0], sv[1]);
      d += 0.5f / gradeSize;
   }
}

void Palette::makeGradient(int ndx, float h, float sFrom, float sTo, float vFrom, float vTo) {
   ndx *= gradeSize;
   
   float s = sFrom, v = vFrom;
   
   float ds = (sTo - sFrom) / gradeSize;
   float dv = (vTo - vFrom) / gradeSize;
   
   for(int i = 0; i < gradeSize; i ++) {
      palette[ndx++].RGB = HSVtoRGB(h, s, v);
      s += ds;
      v += dv;
   }
}

void Palette::setColor(int color) {
   if(color >= numColors)
      return;
   currentColor = (color - 1) * 4;
}

void Palette::incrementColor(int amount) {
   currentColor += amount;
   if(currentColor >= numColors)
      currentColor -= numColors;
}

void Palette::incrementGrade(int amount) {
   currentColor /= gradeSize;
   currentColor += amount;
   currentColor = currentColor * gradeSize + gradeSize / 2;
}
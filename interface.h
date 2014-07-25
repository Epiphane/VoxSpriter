//
//  interface.h
//  VoxSpriter
//
//  Created by Thomas Steinke on 7/14/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#ifndef __VoxSpriter__interface__
#define __VoxSpriter__interface__

#include "globals.h"
#include "texture.h"
#include "palette.h"
#include "input.h"
#include "graphics.h"

class UI;

typedef void elementCallBack(UI *ui, double x, double y);

class UIElement {
public:
   virtual int elementType() { return 0; }
   static const int COLOR_ELEMENT = 1;
   static const int IMAGE_ELEMENT = 2;
   
   virtual void click(double x, double y);
   
   UIElement *init(float _x, float _y, float _z, float _w, float _h, UI *_callBackUI, elementCallBack *callback);
   void setPosition(float _x, float _y) { x = _x; y = _y; }
protected:
   float x, y, z, w, h;
   UI *callBackUI;
   elementCallBack *callback;
};

class ColorElement : public UIElement {
public:
   virtual int elementType() { return COLOR_ELEMENT; }
   
   void setColor(byte3 _color) { color = _color; }
   virtual void pushVertices(glm::vec4 *vertices, byte3 *colors, int &off);
   
protected:
   byte3 color;
};

class PaletteElement : public ColorElement {
private:
   bool current;
public:
   PaletteElement() : current(false) {};
   
   void click(double x, double y);
   
   bool isCurrent() { return current; }
   void setCurrent(bool _current) { current = _current; }
   void pushVertices(glm::vec4 *vertices, byte3 *colors, int &off);
};

class ImageElement : public UIElement {
public:
   virtual int elementType() { return IMAGE_ELEMENT; }
   
   void setUV(float _x, float _y, float _w, float _h);
   void pushVertices(glm::vec3 *vertices, glm::vec2 *uvs, int &off);
   
private:
   float imageX, imageY, imageW, imageH;
};

class UI : public Observer {
public:
   constexpr static const double BAR_X = 0.5;
   constexpr static const double BAR_Y = -1;
   constexpr static const double BAR_W = 0.5;
   constexpr static const double BAR_H = 2;
   constexpr static const double HUE_H = 0.1;
   
   UI();
   void setPalette(Palette *_palette);
   
   void buildVertices();
   void render();
   
   virtual void notify(Message message, int data);
private:
   ThreeDimension *colorRenderer;
   ImageProgram *imageRenderer;
   
   Palette *palette;
   
   class ClickInterfaceCommand : public Command {
   private:
      UI *parent;
   public:
      ClickInterfaceCommand(UI *_parent) : parent(_parent) {}
      
      virtual void execute(double x, double y, bool press);
   };
   
   float hue, sat, val;
   PaletteElement *paletteElements;
   int selectedPalette;
   
   ImageElement *satValElem;
   std::vector<UIElement *> elements;
   GLuint numColorElements, numImageElements;

   void addColorElement(float x, float y, float z, float w, float h, byte3 color, elementCallBack *callback = 0);
   void addImageElement(float x, float y, float z, float w, float h, glm::vec2 bottomLeft, glm::vec2 topRight, elementCallBack *callback = 0);
   
   // Callbacks
   static void selectHue(UI *ui, double x, double y);
   static void selectSatVal(UI *ui, double x, double y);
   static void selectElement(UI *ui, double x, double y);
};

#endif /* defined(__VoxSpriter__interface__) */

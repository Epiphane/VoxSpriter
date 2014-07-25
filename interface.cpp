//
//  interface.cpp
//  VoxSpriter
//
//  Created by Thomas Steinke on 7/14/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#include "interface.h"

using namespace glm;

// SIDE NOTE: MAKE EVERYTHING ON UI COME FROM THE SAME IMAGE

UI::UI() {
   colorRenderer = new ThreeDimension();
   colorRenderer->setVertexFormat(GL_FLOAT);
   
   imageRenderer = new ImageProgram();
   imageRenderer->setTexture("img/ui.png");
   
   elements.clear();
   numColorElements = numImageElements = 0;
   
   paletteElements = new PaletteElement[numColors];
   double square[2];
   square[0] = BAR_W / gradeSize;
   square[1] = square[0] * w_width / w_height;
   for(int i = 0; i < numColors; i ++) {
      paletteElements[i].init(BAR_X + (i % gradeSize) * square[0], 1 - (i / gradeSize + 1) * square[1], 0.1, square[0], square[1], this, UI::selectElement);
      elements.push_back(&paletteElements[i]);
      numColorElements ++;
   }
   palette = 0;
   selectedPalette = -1;
   
   // Set up backdrop
   addColorElement(BAR_X, BAR_Y, 0, BAR_W, BAR_H, byte3(0, 0, 0));
   
   addImageElement(BAR_X, BAR_Y, 0.2, BAR_W, BAR_W * w_width / w_height, vec2(0, 0), vec2(100.0 / 128, 100.0 / 128), UI::selectSatVal);
   addImageElement(BAR_X, BAR_Y + BAR_W * w_width / w_height, 0.2, BAR_W, HUE_H, vec2(0, 100.0 / 128), vec2(100.0 / 128, 101.0 / 128), UI::selectHue);

   // Add satValElem
   {
      float w = 5;
      satValElem = new ImageElement();
      satValElem->init(BAR_X, BAR_Y, 0.3, w / w_width, w / w_height, this, 0);
      satValElem->setUV(100.0 / 128, 100.0 / 128, (100.0 + w) / 128, (100.0 + w) / 128);
      elements.push_back(satValElem);
      numImageElements ++;
      
      sat = val = 0;
   }
   
   hue = 0;
   imageRenderer->setTint(hue);

   Input::setMouseCallback(new ClickInterfaceCommand(this));
   
   buildVertices();
}

void UI::setPalette(Palette *_palette) {
   observe(_palette);
   palette = _palette;
   
   if(selectedPalette != -1) {
      paletteElements[selectedPalette].setCurrent(false);
   }
   for(int i = 0; i < numColors; i ++) {
      paletteElements[i].setColor(palette->getColor(i));
   }
   selectedPalette = palette->getCurrent();
   paletteElements[selectedPalette].setCurrent(true);
   
   buildVertices();
}

void UI::notify(Message message, int data) {
   if(message == PALETTE_COLOR_CHANGE) {
      paletteElements[data].setColor(palette->getColor(data));
      buildVertices();
   }
   else if(message == PALETTE_SELECT_CHANGE) {
      if(selectedPalette != -1) {
         paletteElements[selectedPalette].setCurrent(false);
      }
      
      selectedPalette = palette->getCurrent();
      paletteElements[selectedPalette].setCurrent(true);
      buildVertices();
   }
}

void UI::render() {
   colorRenderer->render();
   imageRenderer->render();
}

void UI::buildVertices() {
   GLuint numColorVertices = numColorElements * 2;
   GLuint numImageVertices = numImageElements * 2;
   
   vec4 *cVertices = new vec4[numColorVertices];
   byte3 *cColors = new byte3[numColorVertices];
   int cOffset = 0;
   
   vec3 *iVertices = new vec3[numImageVertices];
   vec2 *iColors = new vec2[numImageVertices];
   int iOffset = 0;
   
   std::vector<UIElement *>::iterator it = elements.begin();
   while(it < elements.end()) {
      int type = (*it)->elementType();
      if(type == UIElement::COLOR_ELEMENT)
         ((ColorElement *)(*it))->pushVertices(cVertices, cColors, cOffset);
      else if(type == UIElement::IMAGE_ELEMENT)
         ((ImageElement *)(*it))->pushVertices(iVertices, iColors, iOffset);
      
      it++;
   }
   
   colorRenderer->setNumVertices(numColorVertices);
   colorRenderer->bufferData(ThreeDimension::VERTEX_BUFFER, sizeof(vec4) * numColorVertices, cVertices);
   colorRenderer->bufferData(ThreeDimension::COLOR_BUFFER, sizeof(byte3) * numColorVertices, cColors);
   
   imageRenderer->setNumVertices(numImageVertices);
   imageRenderer->bufferData(ImageProgram::VERTEX_BUFFER, sizeof(vec3) * numImageVertices, iVertices);
   imageRenderer->bufferData(ImageProgram::UV_BUFFER, sizeof(vec2) * numImageVertices, iColors);
   
   delete[] cVertices;
   delete[] cColors;
   delete[] iVertices;
   delete[] iColors;
}

void UI::addColorElement(float x, float y, float z, float w, float h, byte3 color, elementCallBack *callback) {
   ColorElement *element = new ColorElement();
   element->init(x, y, z, w, h, this, callback);
   element->setColor(color);
   
   elements.push_back(element);
   numColorElements ++;
}

void UI::addImageElement(float x, float y, float z, float w, float h, vec2 bottomLeft, vec2 topRight, elementCallBack *callback) {
   ImageElement *element = new ImageElement();
   element->init(x, y, z, w, h, this, callback);
   
   topRight -= bottomLeft;
   element->setUV(bottomLeft.x, bottomLeft.y, topRight.x, topRight.y);
   elements.push_back(element);
   
   numImageElements ++;
}

// ---------------------------------
// ------------Callbacks------------
// ---------------------------------

void UI::selectHue(UI *ui, double x, double y) {
   ui->hue = x;
   ui->imageRenderer->setTint(ui->hue);
   ui->palette->setCurrentColor(HSVtoRGB(ui->hue, ui->sat, ui->val * 255));
}

void UI::selectSatVal(UI *ui, double x, double y) {
   ui->sat = x;
   ui->val = y;
   ui->palette->setCurrentColor(HSVtoRGB(ui->hue, ui->sat, ui->val * 255));
   ui->satValElem->setPosition(x * BAR_W + BAR_W, BAR_Y + y * BAR_W * w_width / w_height);
}

void UI::selectElement(UI *ui, double x, double y) {
   double square[2];
   square[0] = BAR_W / gradeSize;
   square[1] = square[0] * w_width / w_height;
   
   int newSelect = floor(x / square[0]) + floor(y / square[1]) * gradeSize;
   
   ui->palette->setCurrent(newSelect);
}

void UI::ClickInterfaceCommand::execute(double x, double y, bool press) {
   if(x >= BAR_X && x <= BAR_X + BAR_W && y >= BAR_Y && y <= BAR_Y + BAR_H) {
      std::vector<UIElement *>::iterator it;
      for(it = parent->elements.begin(); it < parent->elements.end(); it ++) {
         (*it)->click(x, y);
      }
   }
}

// ---------------------------------
// ------------UI Element-----------
// ---------------------------------

UIElement *UIElement::init(float _x, float _y, float _z, float _w, float _h, UI *_callBackUI, elementCallBack *_callback) {
   x = _x;
   y = _y;
   // Multiply z by -1 because it's easier to visualize + above -
   z = _z * -1;
   w = _w;
   h = _h;
   
   callBackUI = _callBackUI;
   callback = _callback;
   
   return this;
}

void UIElement::click(double _x, double _y) {
   _x -= x;
   _y -= y;
   if(callback && _x >= 0 && _x <= w && _y >= 0 && _y <= h) {
      callback(callBackUI, _x / w, _y / h);
   }
}

// ---------------------------------
// ----------Color Element----------
// ---------------------------------

void ColorElement::pushVertices(vec4 *vertices, byte3 *colors, int &off) {
   colors[off] = color;
   vertices[off++] = vec4(x, y, z, 1);
   colors[off] = color;
   vertices[off++] = vec4(x + w, y + h, z, 255);
}

// ---------------------------------
// ---------Palette Element---------
// ---------------------------------

void PaletteElement::pushVertices(vec4 *vertices, byte3 *colors, int &off) {
   if(current) {
      colors[off] = color;
      vertices[off++] = vec4(x + w / 8, y + h / 8, z, 1);
      colors[off] = color;
      vertices[off++] = vec4(x + w * 7 / 8, y + h * 7 / 8, z, 255);
   }
   else {
      ColorElement::pushVertices(vertices, colors, off);
   }
}

void PaletteElement::click(double _x, double _y) {
   _x -= x;
   _y -= y;
   if(callback && _x >= 0 && _x <= w && _y >= 0 && _y <= h) {
      callback(callBackUI, _x + x - UI::BAR_X, 1 - (_y + y));
   }
}

// ---------------------------------
// ----------Image Element----------
// ---------------------------------

void ImageElement::setUV(float _x, float _y, float _w, float _h) {
   imageX = _x;
   imageY = 1 - _y - _h;
   imageW = _w;
   imageH = _h;
}

void ImageElement::pushVertices(vec3 *vertices, vec2 *uvs, int &off) {
   uvs[off] = vec2(imageX, imageY);
   vertices[off++] = vec3(x, y, z);
   uvs[off] = vec2(imageX + imageW, imageY + imageH);
   vertices[off++] = vec3(x + w, y + h, z);
}
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

void UI::ClickInterfaceCommand::execute(double x, double y) {
   if(x >= BAR_X && x <= BAR_X + BAR_W && y >= BAR_Y && y <= BAR_Y + BAR_H) {
      std::vector<UIElement *>::iterator it;
      for(it = parent->elements.begin(); it < parent->elements.end(); it ++) {
         (*it)->click(x, y);
      }
   }
}

UI::UI() {
   colorRenderer = new ThreeDimension();
   colorRenderer->setVertexFormat(GL_FLOAT);
   
   imageRenderer = new ImageProgram();
   imageRenderer->setTexture("img/ui.png");
   
   elements.clear();
   numColorElements = numImageElements = 0;
   
   // Set up backdrop
   addColorElement(BAR_X, BAR_Y, BAR_Z, BAR_W, BAR_H, byte3(0, 0, 0));
   
   addImageElement(BAR_X, BAR_Y, BAR_Z + 0.1, BAR_W, BAR_W * w_width / w_height, vec2(0, 0), vec2(100.0 / 128, 100.0 / 128), UI::selectSatVal);
   addImageElement(BAR_X, BAR_Y + BAR_W * w_width / w_height, BAR_Z + 0.1, BAR_W, BAR_W / 8 * w_width / w_height, vec2(0, 100.0 / 128), vec2(100.0 / 128, 101.0 / 128), UI::selectHue);
   
   Input::setMouseCallback(new ClickInterfaceCommand(this));
   
   buildVertices();
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
   
   imageRenderer->setTint(0.25);
   
   delete[] cVertices;
   delete[] cColors;
   delete[] iVertices;
   delete[] iColors;
}

void UI::render() {
   colorRenderer->render();
   imageRenderer->render();
}

void UI::selectHue(UI *ui, double x, double y) {
   ui->imageRenderer->setTint(x / 2 / BAR_W);
}

void UI::selectSatVal(UI *ui, double x, double y) {
   //ui->imageRenderer->setTint(x / 2 / BAR_W);
}

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

void ColorElement::pushVertices(vec4 *vertices, byte3 *colors, int &off) {
   colors[off] = color;
   vertices[off++] = vec4(x, y, z, 1);
   colors[off] = color;
   vertices[off++] = vec4(x + w, y + h, z, 255);
}

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
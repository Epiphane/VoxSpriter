#include "application.h"

using namespace glm;

int Application::init() {
   Input::clear();
   
   graphics = new Graphics();
   Input::init(graphics->getWindow());
   
   if(graphics->isValid()) {
      camera = graphics->getCamera();
      sprite = new Sprite(camera);
      ui = new UI();
      
      running = true;
      
      return 0;
   }
   return 1;
}

void Application::render() {
   graphics->beginRender();
   
   sprite->render();
   ui->render();
   
   graphics->endRender();
}

void Application::update() {
   Input::tick();
   
   camera->update();
   
   if(Input::shouldExit())
      running = false;
}

/*void Application::click(int button, int action, int mods) {
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      vec3 ray, direction;
      computeRayAndDir(&ray, &direction);
      sprite->click(ray, direction, action, mods);
   }
}

void Application::computeRayAndDir(glm::vec3 *ray, glm::vec3 *dir) {
   double x, y;
   
   mat4 vpMatrix = camera->getVPMatrix();
   mat4 invProjection = glm::inverse(vpMatrix);
   
   Input::getMousePos(&x, &y);
   x = (x / static_cast<double>(w_width) - 0.5f) * 2.0f;
   y = ((w_height - y)/static_cast<double>(w_height) - 0.5f) * 2.0f;
   
   glm::vec4 lRayStart_NDC(x, y, -1.0, 1.0f);
   glm::vec4 lRayEnd_NDC(x, y, 0.0, 1.0f);
   lRayStart_NDC = invProjection * lRayStart_NDC;
   lRayEnd_NDC = invProjection * lRayEnd_NDC;
   lRayStart_NDC /= lRayStart_NDC.w;
   lRayEnd_NDC /= lRayEnd_NDC.w;
   
   vec4 d = glm::normalize(lRayEnd_NDC - lRayStart_NDC);
   *dir = vec3(d.x, d.y, d.z);
   *dir /= 10;

   *ray = camera->getPosition();
}*/

void Application::quit() {
   graphics->terminate();
   sprite->~Sprite();
}
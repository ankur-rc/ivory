#include "ivory/window_manager.h"

#include <iostream>

#include "ivory/opengl.h"

namespace ivory {

WindowManager* WindowManager::GetSingleton() {
  if (!initialized_) {
    window_manager_.reset(new WindowManager);
    if (!window_manager_->Initialize()) {
      window_manager_.reset();
    } else {
      initialized_ = true;
    }
  }
  return window_manager_.get();
}

Window* WindowManager::CreateWindow(size_t width, size_t height, std::string_view title) {
  std::unique_ptr<Window> window;
  window.reset(new Window());

  auto id = WindowManager::NextId();
  if (!window->Create(id, width, height, title)) {
    return nullptr;
  }

  registry_.push_back(std::move(window));

  return registry_.back().get();
}

bool WindowManager::Initialize() {
  // initialise GLFW
  if (!glfwInit()) {
    std::cout << "[WindowManager::Initialize] GLFW failed to initialize\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  return true;
}

WindowManager::~WindowManager() {
  initialized_ = false;

  glfwTerminate();
}

}  // namespace ivory
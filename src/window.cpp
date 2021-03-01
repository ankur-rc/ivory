#include "ivory/window.h"

#include <ivory/opengl.h>

#include <iostream>

namespace ivory {
bool Window::Create(const int32_t id, const std::size_t width, const std::size_t height,
                    std::string_view title) {
  // create window
  window_ = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (window_ == nullptr) {
    std::cout << "Failed to create GLFW window\n";
    return false;
  }

  glfwMakeContextCurrent(this->window_);

  // initialise GLEW
  glewExperimental = GL_TRUE;
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    std::cout << "[Window::Create] GLEW failed to initialize: " << glewGetErrorString(res) << "\n";
    return false;
  }

  id_ = id;
  std::cout << "[Window::Create] Created window with id: " << id_ << " and pointer address: " << this << "\n";
  is_closed_ = false;

  return true;
}

void Window::Update(std::function<void()> update_fn) {
  if (!IsClosed()) {
    glfwMakeContextCurrent(this->window_);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    update_fn();
    glfwPollEvents();
    glfwSwapBuffers(this->window_);
  } else {
    if (!is_closed_) {
      glfwDestroyWindow(this->window_);
      is_closed_ = true;
    }
  }
}

int32_t Window::Id() const { return id_; }

bool Window::IsClosed() const { return glfwWindowShouldClose(this->window_); }

void Window::Close() { glfwSetWindowShouldClose(this->window_, GL_TRUE); }

Window::~Window() {
  std::cout << "[Window::~Window] Destroying window with id: " << id_ << " and pointer address: " << this
            << "\n";
}

}  // namespace ivory
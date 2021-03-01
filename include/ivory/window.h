#pragma once
#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>

class GLFWwindow;

namespace ivory {

class Window {
  friend class WindowManager;

public:
  void Update(std::function<void()> update_fn);
  void Close();
  bool IsClosed() const;
  int32_t Id() const;
  ~Window();

private:
  Window() = default;
  bool Create(const int32_t id, const std::size_t width, const std::size_t height, std::string_view title);
  bool initialized_ = false;
  GLFWwindow* window_ = nullptr;
  std::atomic<bool> is_closed_;
  int32_t id_ = -1;
};
}  // namespace ivory
#pragma once

#include <memory>
#include <vector>

#include "window.h"

namespace ivory {
class WindowManager final {
public:
  static WindowManager* GetSingleton();
  Window* CreateWindow(size_t width, size_t height, std::string_view title);
  ~WindowManager();

private:
  WindowManager() = default;
  static int32_t NextId() { return next_id_++; };
  static bool Initialize();

  inline static std::vector<std::unique_ptr<Window>> registry_;
  inline static bool initialized_ = false;
  inline static std::shared_ptr<WindowManager> window_manager_;
  inline static int32_t next_id_ = 0;
};
}  // namespace ivory
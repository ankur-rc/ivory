#include <ivory/window_manager.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

template <typename T>
static void CloseWindow(ivory::Window* window, const T duration) {
  std::this_thread::sleep_for(duration);
  window->Close();
}

int main(int argc, char** argv) {
  auto window_manager = ivory::WindowManager::GetSingleton();
  if (!window_manager) {
    return -1;
  }

  // create windows
  auto window1 = window_manager->CreateWindow(800, 600, "Window1");
  auto window2 = window_manager->CreateWindow(800, 600, "Window2");

  using namespace std::chrono_literals;

  auto duration1 = 3000ms;
  auto duration2 = 5000ms;
  std::thread close_thread1 = std::thread(CloseWindow<decltype(duration1)>, window1, duration1);
  std::thread close_thread2 = std::thread(CloseWindow<decltype(duration2)>, window2, duration2);

  while (!window1->IsClosed() || !window2->IsClosed()) {
    window1->Update([]() {});
    window2->Update([]() {});
  }

  if (close_thread1.joinable() && close_thread2.joinable()) {
    close_thread1.join();
    close_thread2.join();
  }

  return 0;
}
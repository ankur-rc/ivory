#include <iostream>
#include <random>
#include <vector>

#include "ivory/opengl.h"
#include "ivory/shader_utils.h"
#include "ivory/window_manager.h"

int main() {
  auto window = ivory::WindowManager::GetSingleton()->CreateWindow(480, 360, "Hello Triangle");

  // Create vertices
  const std::vector<float> vertex_data = {
      0.0,  0.25,  0.0,  // top
      -0.5, -0.25, 0.0,  // left
      0.5,  -0.25, 0.0,  // right
  };

  // Create VAO and VBO
  GLuint vao;
  GLuint vbo;
  {
    // Generate VAO
    glGenVertexArrays(1, &vao);
    // Bind the VAO
    glBindVertexArray(vao);

    // Generate Buffer
    glGenBuffers(1, &vbo);
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

    // Enable the vertex attribte pointer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

  // Create shaders
  auto vertex_shader = ivory::Shader("basic.vs", GL_VERTEX_SHADER);
  auto fragment_shader = ivory::Shader("basic.fs", GL_FRAGMENT_SHADER);
  auto shader_program = ivory::ShaderProgram(vertex_shader, fragment_shader);

  // Render
  glm::vec4 color = {1.F, 0.5F, 0.F, 1.F};
  float i = 0;
  while (!window->IsClosed()) {
    window->Update([&]() {
      i += 0.01;
      color.z = std::sin(i) * std::sin(i);
      shader_program.activate();
      shader_program.setVec4f("color", color);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    });
  }

  // De-allocate
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  return 0;
}
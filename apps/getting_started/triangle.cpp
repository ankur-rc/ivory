#include <iostream>
#include <vector>

#include "ivory/opengl.h"
#include "ivory/shader_utils.h"
#include "ivory/window_manager.h"

int main() {
  auto window = ivory::WindowManager::GetSingleton()->CreateWindow(480, 360, "Hello Traingle");

  // Create vertices
  const std::vector<float> vertex_data = {
      0.0,  0.25,  0.0,  // top
      -0.5, -0.25, 0.0,  // left
      0.5,  -0.25, 0.0,  // right
  };

  // Create VBO
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
  while (!window->IsClosed()) {
    window->Update([&]() {
      shader_program.activate();
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    });
  }

  return 0;
}
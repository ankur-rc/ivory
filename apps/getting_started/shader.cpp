#include <iostream>
#include <random>
#include <vector>

#include "ivory/opengl.h"
#include "ivory/shader_utils.h"
#include "ivory/window_manager.h"

int main() {
  auto window = ivory::WindowManager::GetSingleton()->CreateWindow(480, 360, "Hello Triangle -> Set color");

  // Create vertices and colors
  struct Vertex {
    float x, y, z;
  };

  struct Color {
    float r, g, b;
  };

  struct ColoredVertex {
    Vertex v;
    Color c;
  };

  const std::vector<ColoredVertex> data = {
      {{0.0, 0.25, 0.0}, {1., 0., 0.}},    // top
      {{-0.5, -0.25, 0.0}, {0., 1., 0.}},  // left
      {{0.5, -0.25, 0.0}, {0., 0., 1.}}    // right
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * data.size(), data.data(), GL_STATIC_DRAW);

    // Enable the vertex attribute pointer.
    glEnableVertexAttribArray(0);  // << Signal the vertex shader to get this attribute value from an array as
                                   // opposed to one value for all vertices (set through glVertexAttribXX)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);  // Vertex

    glEnableVertexAttribArray(1);
    const size_t colorOffsetPtr = sizeof(float)*3;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(ColoredVertex), (void*)colorOffsetPtr);  // Color
  }

  // Create shaders
  auto vertex_shader = ivory::Shader("basic_color.vert", GL_VERTEX_SHADER);
  auto fragment_shader = ivory::Shader("basic_color.frag", GL_FRAGMENT_SHADER);
  auto shader_program = ivory::ShaderProgram(vertex_shader, fragment_shader);

  // Render
  while (!window->IsClosed()) {
    window->Update([&]() {
      shader_program.activate();
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    });
  }

  // De-allocate
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  return 0;
}
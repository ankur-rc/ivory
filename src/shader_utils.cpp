/**
 * Common utilities to load a shader from disk.
 */

#include "ivory/shader_utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace ivory {

std::optional<std::string> LoadFromDisk(const std::string& name) {
  namespace fs = std::filesystem;
  const fs::path full_path = fs::path(SHADER_ROOT) / name;
  if (!fs::exists(full_path)) {
    std::cout << "Shader source not found at: " << full_path;
    return {};
  }

  std::ifstream file_stream(full_path);
  std::stringstream shader_source;
  std::string line;
  while (std::getline(file_stream, line)) {
    shader_source << line << "\n";
  }

  file_stream.close();

  return shader_source.str();
}

Shader::Shader(const std::string& name, GLenum shader_type)
    : name_(name)
    , source_(LoadFromDisk(name).value_or(""))
    , type_(shader_type) {
  // Create shader
  id_ = glCreateShader(shader_type);

  // Upload shader source
  const char* csource = source_.c_str();
  glShaderSource(id_, 1, &csource, NULL);

  // Compile shader
  glCompileShader(id_);

  if (!LogCompileStatus()) {
    throw std::runtime_error("Failed to compile shader!");
  }
}

int Shader::LogCompileStatus() {
  int success;
  char infoLog[512];
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(id_, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED -> Name: " << name_ << " ID:" << id_ << "\n"
              << infoLog << "\n";
  } else {
    std::cout << "INFO::SHADER::COMPILATION SUCCESS -> Name: " << name_ << " ID:" << id_ << "\n";
  }

  return success;
}

ShaderProgram::ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader) {
  if ((vertex_shader.type() != GL_VERTEX_SHADER) || (fragment_shader.type() != GL_FRAGMENT_SHADER)) {
    throw std::runtime_error("Shader type mismatch!");
  }
  // Create program
  id_ = glCreateProgram();

  // Attach shaders
  glAttachShader(id_, vertex_shader.id());
  glAttachShader(id_, fragment_shader.id());

  // Link program
  glLinkProgram(id_);

  if (!LogLinkStatus()) {
    throw std::runtime_error("Failed to link program!");
  }
}

void ShaderProgram::activate() {
  glUseProgram(id_);
  activated_ = true;
}

ShaderProgram::~ShaderProgram() {
  if (activated_) {
    glUseProgram(0);
  }
}

int ShaderProgram::LogLinkStatus() {
  int success;
  char infoLog[512];
  glGetProgramiv(id_, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(id_, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED ID:" << id_ << "\n" << infoLog << "\n";
  } else {
    std::cout << "INFO::SHADER_PROGRAM::LINKING SUCCESS ID:" << id_ << "\n";
  }

  return success;
}

}  // namespace ivory
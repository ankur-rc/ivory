/**
 * Common utilities to load and manage shaders.
 */

#include "ivory/shader_utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ivory/assert.h"

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

  Assert(logAndReturnCompileStatus(), "Failed to compile shader!");
}

int Shader::logAndReturnCompileStatus() {
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

  Assert(logAndReturnLinkStatus(), "Failed to link program!");
}

void ShaderProgram::activate() {
  activated_program_id_ = id_;
  glUseProgram(id_);
}

ShaderProgram::~ShaderProgram() {
  if (ShaderProgram::activated_program_id_ == id_) {
    ShaderProgram::activated_program_id_ = 0;
    glUseProgram(0);
  }

  glDeleteProgram(id_);
}

int ShaderProgram::logAndReturnLinkStatus() {
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

void ShaderProgram::setBool(const std::string& name, bool value) const {
  Assert(activated_program_id_ == id_, "Current shader program is not active!");
  glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
  Assert(activated_program_id_ == id_, "Current shader program is not active!");
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
  Assert(activated_program_id_ == id_, "Current shader program is not active!");
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void ShaderProgram::setVec4f(const std::string& name, const glm::vec4& value) const {
  Assert(activated_program_id_ == id_, "Current shader program is not active!");
  glUniform4f(glGetUniformLocation(id_, name.c_str()), value.x, value.y, value.z, value.w);
}

}  // namespace ivory
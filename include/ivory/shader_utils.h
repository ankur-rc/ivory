/**
 * Common utilites to load shader files from disk.
 */

#pragma once
#ifndef SHADER_ROOT
#define SHADER_ROOT ""
#endif

#include <atomic>
#include <optional>
#include <string>

#include "ivory/opengl.h"

namespace ivory {
/**
 * @brief Load a shader source from disk into a string.
 * @name shader name
 * @return shader source if successfully loaded
 */
std::optional<std::string> LoadFromDisk(const std::string& name);

/**
 * @brief Class to load, bind and compile shaders.
 */
class Shader {
public:
  Shader(const std::string& name, GLenum shader_type);

  GLenum type() const { return type_; }
  GLuint id() const { return id_; }

private:
  const std::string name_;
  std::string source_;
  GLuint id_;
  GLenum type_;

  int logAndReturnCompileStatus();
};

/**
 * @brief Class to link vertex and fragment shaders and use them in a program.
 */
class ShaderProgram {
public:
  ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader);

  void activate();
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec4f(const std::string& name, const glm::vec4& value) const;

  ~ShaderProgram();

private:
  GLuint id_;
  int logAndReturnLinkStatus();
  inline static std::atomic<GLuint> activated_program_id_ = 0;
};

}  // namespace ivory
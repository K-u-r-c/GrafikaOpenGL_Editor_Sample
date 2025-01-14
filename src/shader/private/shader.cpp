#include "../public/shader.h"

#include <sstream>
#include <iostream>

shader::shader(const std::string &vertexPath, const std::string &fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // * This ensures that ifstream objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure const &) {
    // ! Here we throw an exception
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  uint32_t vertex, fragment;
  int32_t success;
  char infoLog[512];

  // Vertex shader --------------------------------------------------------------------
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);

  // Print compile errors
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment shader ------------------------------------------------------------------
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);

  // Print compile errors
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // shader program -------------------------------------------------------------------
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

shader::shader(const shader &shader) { ID = shader.ID; }

shader::~shader() { glDeleteProgram(ID); }

void shader::use() const { glUseProgram(ID); }

uint32_t shader::getID() const { return ID; }

void shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int32_t) value);
}

void shader::setInt(const std::string &name, int32_t value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string &name, float_t value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void shader::setVec2(const std::string &name, float_t x, float_t y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void shader::setVec3(const std::string &name, float_t x, float_t y, float_t z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void shader::setVec4(const std::string &name, float_t x, float_t y, float_t z, float_t w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
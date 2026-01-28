#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Shader {
public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath) {
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    // read shader file
    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (ifstream::failure &e) {
      cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
      cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
      cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
           << infoLog << endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  };

  // use/activate the shader
  void use() { glUseProgram(ID); };

  // utility uniform functions
  void setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  };

  void setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  };
  void setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  };
  void setVec3(const string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,
                 glm::value_ptr(value));
  };
  void setMat4(const string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
  };
};

#endif

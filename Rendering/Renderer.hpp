//
// Created by Nikita Kruk on 03.07.18.
//

#ifndef SPRMOVIE_RENDERER_HPP
#define SPRMOVIE_RENDERER_HPP

#include "../Definitions.hpp"
#include "../Parameters/ParameterContainer.hpp"
#include "ScreenshotHandler.hpp"
#include "../Models/Model.hpp"
#include "../Models/SimulationModel.hpp"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H

#include <fstream>
#include <vector>
#include <string>

class Renderer
{
 public:

  Renderer(Model *model, ParameterContainer *parameter_container);
  Renderer(SimulationModel &model, ParameterContainer &parameter_container);
  ~Renderer();

  void Start();

 private:

  ScreenshotHandler screenshot_handler_;
  ParameterContainer *parameter_container_;
  Model *model_;

  static bool stop_flag_;
  static bool pause_flag_;
  static bool take_screenshot_flag_;
  static int screenshot_count_;
  static int frame_speed_; // 1 - the basic frame rate
  static GLfloat x_shift_;
  static GLfloat y_shift_;
  static GLfloat z_scale_;
  static int t_start_;

//  FT_Library ft_; // FreeType library object
//  FT_Face face_;  // FreeType face object

  static void ErrorCallback(int error, const char *description);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  void InitShaders(GLuint shader_program[]);
  void SetShaderParameter(GLuint shader_program, GLfloat parameter_value, const std::string &parameter_name_in_shader);
  void SetShaderParameter(GLuint shader_program, GLint parameter_value, const std::string &parameter_name_in_shader);
  void FinFunc();
  void ReadNewState();
  void DisplayFunc(GLuint vao[], GLuint vbo[], GLuint shader_program[]);
  void RenderBorder(GLuint vao, GLuint vbo, GLuint shader_program);
  void RenderRods(GLuint vao, GLuint vbo, GLuint shader_program);
  void RenderRodsWithColor(GLuint vao, GLuint vbo, GLuint shader_program);
//  void RenderText(GLuint vao, GLuint vbo, GLuint shader_program,
//                  const std::string &text, GLfloat x, GLfloat y, GLfloat sx, GLfloat sy);
  void ReadShaderSource(const std::string &fname, std::vector<char> &buffer);
  GLuint LoadAndCompileShader(const std::string &fname, GLenum shader_type);
  GLuint CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
  GLuint CreateProgramFromShader(const std::string &vertex_shader_path,
                                 const std::string &geometry_shader_path,
                                 const std::string &fragment_shader_path);
};

#endif //SPRMOVIE_RENDERER_HPP

//
// Created by Nikita Kruk on 03.07.18.
//

#include "Renderer.hpp"

#include <iostream>
#include <cmath>
#include <map>
#include <cassert>
#include <algorithm>    // std::copy
#include <iterator>     // std::istream_iterator

bool Renderer::stop_flag_ = true;
bool Renderer::pause_flag_ = true;
bool Renderer::take_screenshot_flag_ = false;
int Renderer::screenshot_count_ = 0;
int Renderer::frame_speed_ = 1; // 1 - the basic frame rate
GLfloat Renderer::x_shift_ = 0.0f;
GLfloat Renderer::y_shift_ = 0.0f;
GLfloat Renderer::z_scale_ = 1.0f;
int Renderer::t_start_ = 0;

Renderer::Renderer(Model *model, ParameterContainer *parameter_container) :
    screenshot_handler_(),
    parameter_container_(parameter_container),
    model_(model)
{
  std::cout << "Renderer Created" << std::endl;

  model_->SkipTimeUnits(t_start_, parameter_container_->ParametersDictionaryValue("output_delta_t"));

//  active_passive_.resize((unsigned long) (parameter_container_.ParametersDictionaryValue("N")), 1.0f);
//  active_passive_file_.open(parameter_container_.GetActivePassiveFileName(), std::ios::in);
//  assert(active_passive_file_.is_open());
//  std::copy(std::istream_iterator<Real>(active_passive_file_), std::istream_iterator<Real>(), active_passive_.begin());

  // open a simulation file
//  rods_.resize((unsigned long) (
//      parameter_container_.GetNumberOfStateVariables() * parameter_container_.ParametersDictionaryValue("N")), 0.0f);
//  simulation_file_.open(parameter_container_.GetSimulationFileName(), std::ios::binary | std::ios::in);
//  assert(simulation_file_.is_open());
//  simulation_file_.seekg(
//      int(parameter_container_.GetTStart() / parameter_container_.ParametersDictionaryValue("output_delta_t"))
//          * (1 + parameter_container_.GetNumberOfStateVariables() * parameter_container_.ParametersDictionaryValue("N"))
//          * sizeof(Real), std::ios::beg);
}

Renderer::Renderer(SimulationModel &model, ParameterContainer &parameter_container) :
    screenshot_handler_(),
    parameter_container_(&parameter_container),
    model_(&model)
{
  std::cout << "Renderer Created" << std::endl;

  model_->SkipTimeUnits(t_start_, parameter_container_->ParametersDictionaryValue("output_delta_t"));
}

Renderer::~Renderer()
{
  std::cout << "Renderer Deleted" << std::endl;
}

void Renderer::Start()
{
  GLFWwindow *window;

//  void (Renderer::*error_callback_ptr)(int, const char *);
//  error_callback_ptr = &Renderer::ErrorCallback;
//  glfwSetErrorCallback(this->*error_callback_ptr);
  glfwSetErrorCallback(Renderer::ErrorCallback);

  if (!glfwInit())
  {
    std::cerr << "Initialization of GLFW failure" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);//MSAA

  window = glfwCreateWindow(1024, 1024, "Self-propelled Rods", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    std::cerr << "Window opening failure" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, Renderer::KeyCallback);

  int major, minor, rev;
  major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
  minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
  rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
  std::cout << "OpenGL = " << major << "." << minor << "." << rev << std::endl;

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "GLEW initialization failure" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // contexts for border, rods, text
  GLuint vao[3] = {0};
  GLuint vbo[3] = {0};
  glGenVertexArrays(3, &vao[0]);
  glGenBuffers(3, &vbo[0]);

  GLuint shader_program[3] = {0};
  InitShaders(shader_program);

//  if (FT_Init_FreeType(&ft_))
//  {
//    std::cerr << "Could not init freetype library" << std::endl;
//  }
//  if (FT_New_Face(ft, "/System/Library/Fonts/HelveticaNeueDeskInterface.ttc", 0, &face_))
//  {
//    std::cerr << "Could not open font" << std::endl;
//  }
//  FT_Set_Pixel_Sizes(face, 0, 48);

  while (!glfwWindowShouldClose(window))
  {
    SetShaderParameter(shader_program[1], x_shift_, "x_shift");
    SetShaderParameter(shader_program[1], y_shift_, "y_shift");
    SetShaderParameter(shader_program[1], z_scale_, "z_scale");
    SetShaderParameter(shader_program[1], (GLint) parameter_container_->ParametersDictionaryValue("n"), "n");

    DisplayFunc(vao, vbo, shader_program);

    if (take_screenshot_flag_)
    {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      screenshot_handler_.TakeScreenshotPng(width, height, screenshot_count_++);
      take_screenshot_flag_ = false;
//      if (screenshot_count_ >= 1000)
//      {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//      }
    }

//		if (t_global > 2000.0)
//		{
//			glfwSetWindowShouldClose(window, GL_TRUE);
//		}

    ReadNewState();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  FinFunc();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Renderer::ErrorCallback(int error, const char *description)
{
  std::cerr << description << std::endl;
}

void Renderer::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (GLFW_PRESS == action)
  {
    switch (key)
    {
      case GLFW_KEY_ESCAPE:
//			case GLFW_PRESS:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;

      case GLFW_KEY_S:stop_flag_ = !stop_flag_;
        break;

      case GLFW_KEY_P:
        if (stop_flag_)
        {
          pause_flag_ = !pause_flag_;
        }
        break;

      case GLFW_KEY_F13:take_screenshot_flag_ = true;
        break;

      case GLFW_KEY_O:++frame_speed_;
        break;

      case GLFW_KEY_L:
        if (frame_speed_ > 1)
        {
          --frame_speed_;
        }
        break;

      case GLFW_KEY_1:x_shift_ += 0.1f;
        break;

      case GLFW_KEY_2:x_shift_ -= 0.1f;
        break;

      case GLFW_KEY_3:y_shift_ += 0.1f;
        break;

      case GLFW_KEY_4:y_shift_ -= 0.1f;
        break;

      case GLFW_KEY_5:z_scale_ /= 1.25f;
        break;

      case GLFW_KEY_6:z_scale_ *= 1.25f;
        break;

      default:break;
    }
  }
}

void Renderer::InitShaders(GLuint *shader_program)
{
  shader_program[0] =
      CreateProgramFromShader(std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/border_vertex_shader.shader"),
                              std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/border_fragment_shader.shader"));
  shader_program[1] =
      CreateProgramFromShader(std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/rod_vertex_shader.shader"),
                              std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/rod_geometry_shader_v1.shader"),
                              std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/rod_fragment_shader_v1.shader"));
  SetShaderParameter(shader_program[1], (GLfloat) parameter_container_->ParametersDictionaryValue("L"), "L");
  SetShaderParameter(shader_program[1], (GLfloat) parameter_container_->ParametersDictionaryValue("lambda"), "lambda");
  SetShaderParameter(shader_program[1], (GLfloat) parameter_container_->ParametersDictionaryValue("l"), "l");

  shader_program[2] =
      CreateProgramFromShader(std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/text_vertex_shader.shader"),
                              std::string("/Users/nikita/CLionProjects/sprMovie/Shaders/text_fragment_shader.shader"));
  GLuint texture;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLint tex_location = glGetUniformLocation(shader_program[2], "tex");
  glUniform1i(tex_location, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Renderer::SetShaderParameter(GLuint shader_program,
                                  GLfloat parameter_value,
                                  const std::string &parameter_name_in_shader)
{
  GLint location = glGetUniformLocation(shader_program, parameter_name_in_shader.c_str());
  if (-1 != location)
  {
    glUniform1f(location, parameter_value);
  }
}

void Renderer::SetShaderParameter(GLuint shader_program,
                                  GLint parameter_value,
                                  const std::string &parameter_name_in_shader)
{
  GLint location = glGetUniformLocation(shader_program, parameter_name_in_shader.c_str());
  if (-1 != location)
  {
    glUniform1i(location, parameter_value);
  }
}

void Renderer::FinFunc()
{

}

void Renderer::ReadNewState()
{
  if (!stop_flag_ || !pause_flag_)
  {
    model_->ReadNewState();
    // speed up or down the simulation output
    model_->SkipTimeUnits(frame_speed_ - 1, 1);
    pause_flag_ = true;
  }
}

void Renderer::DisplayFunc(GLuint *vao, GLuint *vbo, GLuint *shader_program)
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
//  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_MULTISAMPLE);//MSAA

//  glUseProgram(shader_program[0]);
//  RenderBorder(vao[0], vbo[0], shader_program[0]);

  glUseProgram(shader_program[1]);
  if (model_->GetColors().size() == 0)
  {
    RenderRods(vao[1], vbo[1], shader_program[1]);
  } else
  {
    RenderRodsWithColor(vao[1], vbo[1], shader_program[1]);
  }
}

void Renderer::RenderBorder(GLuint vao, GLuint vbo, GLuint shader_program)
{
  glBindVertexArray(vao);

  GLfloat border_vertices[8] =
      {
          0.0f, 0.0f,
          1.0f, 0.0f,
          1.0f, 1.0f,
          0.0f, 1.0f
      };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), border_vertices, GL_STATIC_DRAW);

  GLint position_attribute = glGetAttribLocation(shader_program, "position");
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_attribute);

  glBindVertexArray(vao);
  glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void Renderer::RenderRods(GLuint vao, GLuint vbo, GLuint shader_program)
{
  const std::vector<GLfloat> &system_state = model_->GetCurrentState();
  const std::vector<GLfloat> &active_passive = model_->GetActivePassiveState();

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

//	glBufferData(GL_ARRAY_BUFFER, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), &rods[0], GL_DYNAMIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER,
               (parameter_container_->GetNumberOfStateVariables() + 1)
                   * parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
               NULL,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  parameter_container_->GetNumberOfStateVariables()
                      * parameter_container_->ParametersDictionaryValue("N")
                      * sizeof(GLfloat),
                  &system_state[0]);
  glBufferSubData(GL_ARRAY_BUFFER,
                  parameter_container_->GetNumberOfStateVariables()
                      * parameter_container_->ParametersDictionaryValue("N")
                      * sizeof(GLfloat),
                  parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
                  &active_passive[0]);

  GLint position_attribute = glGetAttribLocation(shader_program, "position");
  glVertexAttribPointer(position_attribute,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        parameter_container_->GetNumberOfStateVariables() * sizeof(GLfloat),
                        0);
  glEnableVertexAttribArray(position_attribute);

  GLint direction_attibute = glGetAttribLocation(shader_program, "direction");
  glVertexAttribPointer(direction_attibute,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        parameter_container_->GetNumberOfStateVariables() * sizeof(GLfloat),
                        (GLvoid *) (2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(direction_attibute);

  GLint active_passive_attribute = glGetAttribLocation(shader_program, "active_passive");
  glVertexAttribPointer(active_passive_attribute,
                        1,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat),
                        (GLvoid *) (parameter_container_->GetNumberOfStateVariables()
                            * size_t(parameter_container_->ParametersDictionaryValue("N")) * sizeof(GLfloat)));
  glEnableVertexAttribArray(active_passive_attribute);

  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, (GLsizei) parameter_container_->ParametersDictionaryValue("N"));
//	glDrawArrays(GL_POINTS, 3990, 1);

  glDisableVertexAttribArray(position_attribute);
  glDisableVertexAttribArray(direction_attibute);
  glDisableVertexAttribArray(active_passive_attribute);
}

void Renderer::RenderRodsWithColor(GLuint vao, GLuint vbo, GLuint shader_program)
{
  const std::vector<GLfloat> &system_state = model_->GetCurrentState();
  const std::vector<GLfloat> &active_passive = model_->GetActivePassiveState();
  const std::map<int, std::vector<GLfloat>> &colors = model_->GetColors();
  const std::vector<int> &indexes = model_->GetIndexes();
  std::vector<GLfloat> transformed_colors(3 * indexes.size(), 0.0f);
  for (int i = 0; i < indexes.size(); ++i)
  {
    transformed_colors[3 * i] = colors.at(indexes[i])[0];
    transformed_colors[3 * i + 1] = colors.at(indexes[i])[1];
    transformed_colors[3 * i + 2] = colors.at(indexes[i])[2];
  }

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

//	glBufferData(GL_ARRAY_BUFFER, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), &rods[0], GL_DYNAMIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER,
               (parameter_container_->GetNumberOfStateVariables() + 1 + 3)
                   * parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
               NULL,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  parameter_container_->GetNumberOfStateVariables()
                      * parameter_container_->ParametersDictionaryValue("N")
                      * sizeof(GLfloat),
                  &system_state[0]);
  glBufferSubData(GL_ARRAY_BUFFER,
                  parameter_container_->GetNumberOfStateVariables()
                      * parameter_container_->ParametersDictionaryValue("N")
                      * sizeof(GLfloat),
                  parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
                  &active_passive[0]);
  glBufferSubData(GL_ARRAY_BUFFER,
                  (parameter_container_->GetNumberOfStateVariables() + 1)
                      * parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
                  3 * parameter_container_->ParametersDictionaryValue("N") * sizeof(GLfloat),
                  &transformed_colors[0]);

  GLint position_attribute = glGetAttribLocation(shader_program, "position");
  glVertexAttribPointer(position_attribute,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        parameter_container_->GetNumberOfStateVariables() * sizeof(GLfloat),
                        0);
  glEnableVertexAttribArray(position_attribute);

  GLint direction_attibute = glGetAttribLocation(shader_program, "direction");
  glVertexAttribPointer(direction_attibute,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        parameter_container_->GetNumberOfStateVariables() * sizeof(GLfloat),
                        (GLvoid *) (2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(direction_attibute);

  GLint active_passive_attribute = glGetAttribLocation(shader_program, "active_passive");
  glVertexAttribPointer(active_passive_attribute,
                        1,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat),
                        (GLvoid *) (parameter_container_->GetNumberOfStateVariables()
                            * size_t(parameter_container_->ParametersDictionaryValue("N"))
                            * sizeof(GLfloat))); // TODO: correct last argument?
  glEnableVertexAttribArray(active_passive_attribute);

  GLint color_r_attribute = glGetAttribLocation(shader_program, "color_r");
  glVertexAttribPointer(color_r_attribute,
                        1,
                        GL_FLOAT,
                        GL_FALSE,
                        3 * sizeof(GLfloat),
                        (GLvoid *) ((parameter_container_->GetNumberOfStateVariables() + 1)
                            * size_t(parameter_container_->ParametersDictionaryValue("N")) * sizeof(GLfloat)));
  glEnableVertexAttribArray(color_r_attribute);
  GLint color_g_attribute = glGetAttribLocation(shader_program, "color_g");
  glVertexAttribPointer(color_g_attribute,
                        1,
                        GL_FLOAT,
                        GL_FALSE,
                        3 * sizeof(GLfloat),
                        (GLvoid *) ((parameter_container_->GetNumberOfStateVariables() + 1)
                            * size_t(parameter_container_->ParametersDictionaryValue("N")) * sizeof(GLfloat)
                            + sizeof(GLfloat)));
  glEnableVertexAttribArray(color_g_attribute);
  GLint color_b_attribute = glGetAttribLocation(shader_program, "color_b");
  glVertexAttribPointer(color_b_attribute,
                        1,
                        GL_FLOAT,
                        GL_FALSE,
                        3 * sizeof(GLfloat),
                        (GLvoid *) ((parameter_container_->GetNumberOfStateVariables() + 1)
                            * size_t(parameter_container_->ParametersDictionaryValue("N")) * sizeof(GLfloat)
                            + 2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(color_b_attribute);

  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, (GLsizei) parameter_container_->ParametersDictionaryValue("N"));

  glDisableVertexAttribArray(position_attribute);
  glDisableVertexAttribArray(direction_attibute);
  glDisableVertexAttribArray(active_passive_attribute);
  glDisableVertexAttribArray(color_r_attribute);
  glDisableVertexAttribArray(color_g_attribute);
  glDisableVertexAttribArray(color_b_attribute);
}

//void Renderer::RenderText(GLuint vao,
//                          GLuint vbo,
//                          GLuint shader_program,
//                          const std::string &text,
//                          GLfloat x,
//                          GLfloat y,
//                          GLfloat sx,
//                          GLfloat sy)
//{
//  glBindVertexArray(vao);
//  glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//  GLint coord_location = glGetAttribLocation(shader_program, "generic_coord");
//  glVertexAttribPointer(coord_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(coord_location);
//
//  FT_GlyphSlot glyph_slot = face_->glyph;
//
//  for (size_t i = 0; i < text.length(); ++i)
//  {
//    if (FT_Load_Char(face_, text[i], FT_LOAD_RENDER))
//    {
//      continue;
//    }
//
//    glTexImage2D(GL_TEXTURE_2D,
//                 0,
//                 GL_RED,
//                 glyph_slot->bitmap.width,
//                 glyph_slot->bitmap.rows,
//                 0,
//                 GL_RED,
//                 GL_UNSIGNED_BYTE,
//                 glyph_slot->bitmap.buffer);
//
//    GLfloat x2 = x + glyph_slot->bitmap_left * sx;
//    GLfloat y2 = -y - glyph_slot->bitmap_top * sy;
//    GLfloat w = glyph_slot->bitmap.width * sx;
//    GLfloat h = glyph_slot->bitmap.rows * sy;
//
//    GLfloat box[4][4] =
//        {
//            {x2, -y2, 0.0f, 0.0f},
//            {x2 + w, -y2, 1.0f, 0.0f},
//            {x2, -y2 - h, 0.0f, 1.0f},
//            {x2 + w, -y2 - h, 1.0f, 1.0f}
//        };
//
//    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    x += (glyph_slot->advance.x / 64) * sx;
//    y += (glyph_slot->advance.y / 64) * sy;
//  }
//}

void Renderer::ReadShaderSource(const std::string &fname, std::vector<char> &buffer)
{
  std::ifstream in;
  in.open(fname, std::ios::binary | std::ios::in);

  if (in.is_open())
  {
    in.seekg(0, std::ios::end);
    size_t length = (size_t) in.tellg();

    in.seekg(0, std::ios::beg);

    buffer.resize(length + 1);
    in.read((char *) &buffer[0], length);
    buffer[length] = '\0';

    in.close();
  } else
  {
    std::cerr << "Unabla to read the shader file \"" << fname << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }
}

GLuint Renderer::LoadAndCompileShader(const std::string &fname, GLenum shader_type)
{
  std::vector<char> buffer;
  ReadShaderSource(fname, buffer);
  const char *src = &buffer[0];

  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint compilation_test;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_test);
  if (!compilation_test)
  {
    std::cerr << "Shader compilation failed with the following message: " << std::endl;
    std::vector<char> compilation_log(512, '\0');
    glGetShaderInfoLog(shader, (GLsizei) compilation_log.size(), NULL, &compilation_log[0]);
    std::cerr << &compilation_log[0] << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  return shader;
}

GLuint Renderer::CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &fragment_shader_path)
{
  GLuint vertex_shader = LoadAndCompileShader(vertex_shader_path, GL_VERTEX_SHADER);
  GLuint fragment_shader = LoadAndCompileShader(fragment_shader_path, GL_FRAGMENT_SHADER);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  return shader_program;
}

GLuint Renderer::CreateProgramFromShader(const std::string &vertex_shader_path,
                                         const std::string &geometry_shader_path,
                                         const std::string &fragment_shader_path)
{
  GLuint vertex_shader = LoadAndCompileShader(vertex_shader_path, GL_VERTEX_SHADER);
  GLuint geometry_shader = LoadAndCompileShader(geometry_shader_path, GL_GEOMETRY_SHADER);
  GLuint fragment_shader = LoadAndCompileShader(fragment_shader_path, GL_FRAGMENT_SHADER);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, geometry_shader);
  glAttachShader(shader_program, fragment_shader);

  glDeleteShader(vertex_shader);
  glDeleteShader(geometry_shader);
  glDeleteShader(fragment_shader);

  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  return shader_program;
}
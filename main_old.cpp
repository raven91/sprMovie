#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <algorithm>    // std::copy
#include <iterator>     // std::istream_iterator

//#define CLUSTER_COLORING

typedef double Real; // type of the simulation data

const int sprNumberOfStateVariables = 4;
const int sprTStart = 0;

const std::string sprParameterString("_phi_0.063_a_4_U0_250_k_0");
const std::string sprParametersFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/SyntheticData/100/parameters" + sprParameterString + ".txt");
const std::string sprSimulationFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/SyntheticData/100/spr_simulation" + sprParameterString + ".bin");
const std::string sprActivePassiveFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/SyntheticData/100/active_passive" + sprParameterString + ".txt");
//const std::string sprParametersFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/ExperimentalData/parameters_rank_0.txt");
//const std::string sprSimulationFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/ExperimentalData/spr_simulation_rank_0.bin");
//const std::string sprActivePassiveFileName("/Users/nikita/Documents/spr/sprApproximateBayesianComputation/ExperimentalData/active_passive_rank_0.txt");
std::ifstream parameters_file;
std::ifstream simulation_file;
std::ifstream active_passive_file;

std::map<std::string, GLfloat> parameters_dictionary;
std::vector<GLfloat> rods;
std::vector<Real> rods_real;//file-to-container buffer
std::vector<GLfloat> active_passive;
#ifdef CLUSTER_COLORING
std::vector<GLfloat> clustering_identity;
#endif
Real t_global = 0.0f;

GLboolean stop_flag = true;
GLboolean pause_flag = true;
GLboolean take_screenshot_flag = false;
int screenshot_count = 0;
int sprFrameSpeed = 1;//1 - the basic frame rate
GLfloat x_shift = 0.0f, y_shift = 0.0f, z_scale = 1.0f;

void ErrorCallback(int error, const char *description);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void DisplayFunc(GLFWwindow *window, GLuint vao[], GLuint vbo[], GLuint shader_program[]);
void RenderBorder(GLuint vao, GLuint vbo, GLuint shader_program);
void RenderRods(GLuint vao, GLuint vbo, GLuint shader_program);
void InitFunc(GLFWwindow *window, GLuint vao[], GLuint vbo[], GLuint shader_program[]);
void FinFunc();
void ReadNewState();
void ReadShaderSource(const std::string &fname, std::vector<char> &buffer);
GLuint LoadAndCompileShader(const std::string &fname, GLenum shader_type);
GLuint CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
GLuint CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &geometry_shader_path, const std::string &fragment_shader_path);

void IdentifyClusters(std::vector<GLfloat> &clustering_identity);

void TakeScreenshotPpm(int width, int height);
void FreePpm();
void TakeScreenshotPng(unsigned int width, unsigned int height, int image_index);
void FreePng();

int main(int argc, const char * argv[])
{
    GLFWwindow *window;

    glfwSetErrorCallback(ErrorCallback);

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

    glfwSetKeyCallback(window, KeyCallback);

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

    //contexts for border, rods
    GLuint vao[2] = {0};
    GLuint vbo[2] = {0};
    GLuint shader_program[2] = {0};

    InitFunc(window, vao, vbo, shader_program);

    while (!glfwWindowShouldClose(window))
    {
        GLint x_shift_location = glGetUniformLocation(shader_program[1], "x_shift");
        if (-1 != x_shift_location)
        {
            glUniform1f(x_shift_location, x_shift);
        }
        GLint y_shift_location = glGetUniformLocation(shader_program[1], "y_shift");
        if (-1 != y_shift_location)
        {
            glUniform1f(y_shift_location, y_shift);
        }
        GLint z_scale_location = glGetUniformLocation(shader_program[1], "z_scale");
        if (-1 != z_scale_location)
        {
            glUniform1f(z_scale_location, z_scale);
        }

        DisplayFunc(window, vao, vbo, shader_program);

        if (take_screenshot_flag)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            TakeScreenshotPng(width, height, screenshot_count++);

            take_screenshot_flag = false;
        }

//		if (t_global > 2000.0)
//		{
//			glfwSetWindowShouldClose(window, GL_TRUE);
//		}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    FinFunc();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

void ErrorCallback(int error, const char *description)
{
    std::cerr << description << std::endl;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (GLFW_PRESS == action)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
//			case GLFW_PRESS:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_S:
                stop_flag = !stop_flag;
                break;

            case GLFW_KEY_P:
                if (stop_flag)
                {
                    pause_flag = !pause_flag;
                }
                break;

            case GLFW_KEY_F13:
                take_screenshot_flag = true;
                break;

            case GLFW_KEY_O:
                ++sprFrameSpeed;
                break;

            case GLFW_KEY_L:
                if (sprFrameSpeed > 1)
                {
                    --sprFrameSpeed;
                }
                break;

            case GLFW_KEY_1:
                x_shift += 0.1f;
                break;

            case GLFW_KEY_2:
                x_shift -= 0.1f;
                break;

            case GLFW_KEY_3:
                y_shift += 0.1f;
                break;

            case GLFW_KEY_4:
                y_shift -= 0.1f;
                break;

            case GLFW_KEY_5:
                if (z_scale > 1.0f)
                {
                    z_scale -= 0.1f;
                }
                break;

            case GLFW_KEY_6:
                z_scale += 0.1f;
                break;

            default:
                break;
        }
    }
}

void InitFunc(GLFWwindow *window, GLuint vao[], GLuint vbo[], GLuint shader_program[])
{
//	glEnable(GL_BLEND);
//	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendEquation(GL_FUNC_ADD);

//	glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);//MSAA

    //read parameters
    parameters_file.open(sprParametersFileName, std::ios::in);
    assert(parameters_file.is_open());
    std::string key;
    GLfloat value;
    while (parameters_file >> key >> value)
    {
        parameters_dictionary[key] = value;
    }
    parameters_file.close();
    rods.resize(sprNumberOfStateVariables * parameters_dictionary["N"], 0.0f);
    rods_real.resize(sprNumberOfStateVariables * parameters_dictionary["N"], 0.0);
    active_passive.resize(parameters_dictionary["N"], 1.0f);
#ifdef CLUSTER_COLORING
    clustering_identity.resize(parameters_dictionary["N"], 0.0f);
#endif

    //open a simulation file
    simulation_file.open(sprSimulationFileName, std::ios::binary | std::ios::in);
    assert(simulation_file.is_open());
    simulation_file.seekg(int(sprTStart / parameters_dictionary["output_delta_t"]) * (1 + sprNumberOfStateVariables * parameters_dictionary["N"]) * sizeof(Real), std::ios::beg);
    active_passive_file.open(sprActivePassiveFileName, std::ios::in);
    assert(active_passive_file.is_open());
    std::copy(std::istream_iterator<float>(active_passive_file), std::istream_iterator<float>(), active_passive.begin());

    glGenVertexArrays(2, &vao[0]);
    glGenBuffers(2, &vbo[0]);

    shader_program[0] = CreateProgramFromShader(std::string("/Users/nikita/Projects/sprABCVisualization/sprABCVisualization/Shaders/border_vertex_shader.shader"), std::string("/Users/nikita/Projects/sprABCVisualization/sprABCVisualization/Shaders/border_fragment_shader.shader"));
    shader_program[1] = CreateProgramFromShader(std::string("/Users/nikita/Projects/sprABCVisualization/sprABCVisualization/Shaders/rod_vertex_shader.shader"), std::string("/Users/nikita/Projects/sprABCVisualization/sprABCVisualization/Shaders/rod_geometry_shader_v1.shader"), std::string("/Users/nikita/Projects/sprABCVisualization/sprABCVisualization/Shaders/rod_fragment_shader_v1.shader"));

    GLint L_location = glGetUniformLocation(shader_program[1], "L");
    if (-1 != L_location)
    {
        glUniform1i(L_location, parameters_dictionary["L"]);
    }
    GLint lambda_location = glGetUniformLocation(shader_program[1], "lambda");
    if (-1 != lambda_location)
    {
        glUniform1f(lambda_location, parameters_dictionary["lambda"]);
    }
    GLint l_location = glGetUniformLocation(shader_program[1], "l");
    if (-1 != l_location)
    {
        glUniform1f(l_location, parameters_dictionary["l"]);
    }
    GLint n_location = glGetUniformLocation(shader_program[1], "n");
    if (-1 != n_location)
    {
        glUniform1i(n_location, parameters_dictionary["n"]);
    }
}

void FinFunc()
{
    if (simulation_file.is_open())
    {
        simulation_file.close();
    }
    if (active_passive_file.is_open())
    {
        active_passive_file.close();
    }

//	FreePpm();
    FreePng();
}

void ReadNewState()
{
    if (!stop_flag || !pause_flag)
    {
        Real t = 0.0f;
        simulation_file.read((char *)&t, sizeof(Real));
        simulation_file.read((char *)&rods_real[0], sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(Real));
        std::copy(rods_real.begin(), rods_real.end(), rods.begin());
#ifdef CLUSTER_COLORING
        IdentifyClusters(clustering_identity);
#endif

        t_global = t;
        std::cout << "t = " << t << std::endl;

        //speed up or down the silumation output
        for (int n = 0; n < sprFrameSpeed - 1; ++n)
        {
            simulation_file.seekg((1 + sprNumberOfStateVariables * parameters_dictionary["N"]) * sizeof(Real), std::ios::cur);
        }

        pause_flag = true;
    }
}

void DisplayFunc(GLFWwindow *window, GLuint vao[], GLuint vbo[], GLuint shader_program[])
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glUseProgram(shader_program[1]);
    RenderRods(vao[1], vbo[1], shader_program[1]);

//	glUseProgram(shader_program[0]);
//	RenderBorder(vao[0], vbo[0], shader_program[0]);
}

void RenderBorder(GLuint vao, GLuint vbo, GLuint shader_program)
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

void RenderRods(GLuint vao, GLuint vbo, GLuint shader_program)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

#ifdef CLUSTER_COLORING
    glBufferData(GL_ARRAY_BUFFER, (sprNumberOfStateVariables + 2) * parameters_dictionary["N"] * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), &rods[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), parameters_dictionary["N"] * sizeof(GLfloat), &active_passive[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (sprNumberOfStateVariables + 1) * parameters_dictionary["N"] * sizeof(GLfloat), parameters_dictionary["N"] * sizeof(GLfloat), &clustering_identity[0]);
#else
//	glBufferData(GL_ARRAY_BUFFER, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), &rods[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, (sprNumberOfStateVariables + 1) * parameters_dictionary["N"] * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), &rods[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sprNumberOfStateVariables * parameters_dictionary["N"] * sizeof(GLfloat), parameters_dictionary["N"] * sizeof(GLfloat), &active_passive[0]);
#endif

    GLint position_attribute = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, sprNumberOfStateVariables * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(position_attribute);

    GLint direction_attibute = glGetAttribLocation(shader_program, "direction");
    glVertexAttribPointer(direction_attibute, 2, GL_FLOAT, GL_FALSE, sprNumberOfStateVariables * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(direction_attibute);

    GLint active_passive_attribute = glGetAttribLocation(shader_program, "active_passive");
    glVertexAttribPointer(active_passive_attribute, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid *)(sprNumberOfStateVariables * size_t(parameters_dictionary["N"]) * sizeof(GLfloat)));
    glEnableVertexAttribArray(active_passive_attribute);

#ifdef CLUSTER_COLORING
    GLint cluster_color_attribute = glGetAttribLocation(shader_program, "cluster_color");
	glVertexAttribPointer(cluster_color_attribute, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid *)((sprNumberOfStateVariables + 1) * size_t(parameters_dictionary["N"]) * sizeof(GLfloat)));
	glEnableVertexAttribArray(cluster_color_attribute);
#endif

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, parameters_dictionary["N"]);
//	glDrawArrays(GL_POINTS, 3990, 1);

    glDisableVertexAttribArray(position_attribute);
    glDisableVertexAttribArray(direction_attibute);
    glDisableVertexAttribArray(active_passive_attribute);
#ifdef CLUSTER_COLORING
    glDisableVertexAttribArray(cluster_color_attribute);
#endif

    ReadNewState();
}

void ReadShaderSource(const std::string &fname, std::vector<char> &buffer)
{
    std::ifstream in;
    in.open(fname, std::ios::binary | std::ios::in);

    if (in.is_open())
    {
        in.seekg(0, std::ios::end);
        size_t length = (size_t)in.tellg();

        in.seekg(0, std::ios::beg);

        buffer.resize(length + 1);
        in.read((char *)&buffer[0], length);
        buffer[length] = '\0';

        in.close();
    }
    else
    {
        std::cerr << "Unabla to read the shader file \"" << fname << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
}

GLuint LoadAndCompileShader(const std::string &fname, GLenum shader_type)
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
        glGetShaderInfoLog(shader, (GLsizei)compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    return shader;
}

GLuint CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &fragment_shader_path)
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

GLuint CreateProgramFromShader(const std::string &vertex_shader_path, const std::string &geometry_shader_path, const std::string &fragment_shader_path)
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
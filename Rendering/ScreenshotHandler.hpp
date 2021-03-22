//
// Created by Nikita Kruk on 03.07.18.
//

#ifndef SPRMOVIE_SCREENSHOTHANDLER_HPP
#define SPRMOVIE_SCREENSHOTHANDLER_HPP

#include "../Definitions.hpp"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sstream>

extern "C"
{
#include <png.h>
}

class ScreenshotHandler
{
 public:

  ScreenshotHandler();
  ~ScreenshotHandler();

  void TakeScreenshotPpm(int width, int height);
  void TakeScreenshotPng(unsigned int width, unsigned int height, int image_index);

};

#endif //SPRMOVIE_SCREENSHOTHANDLER_HPP

//
// Created by Nikita Kruk on 05.07.18.
//

#ifndef SPRMOVIE_MODEL_HPP
#define SPRMOVIE_MODEL_HPP

#include "../Definitions.hpp"
#include "../Parameters/ParameterContainer.hpp"

#include <GLEW/glew.h>

#include <vector>
#include <fstream>
#include <map>

class Model
{
 public:

  Model(ParameterContainer *parameter_container);
  virtual ~Model();

  virtual void ReadNewState() = 0;
  virtual void SkipTimeUnits(int t, Real delta_t) = 0;

  const std::vector<GLfloat> &GetCurrentState();
  const std::vector<GLfloat> &GetActivePassiveState();
  const std::map<int, std::vector<GLfloat>> &GetColors();
  const std::vector<int> &GetIndexes();

 protected:

  ParameterContainer *parameter_container_;
  std::vector<GLfloat> system_state_;
  std::vector<GLfloat> active_passive_;
  std::map<int, std::vector<GLfloat>> colors_;
  std::vector<int> indexes_;
  int number_of_agents_;
  int number_of_state_variables_;

};

#endif //SPRMOVIE_MODEL_HPP

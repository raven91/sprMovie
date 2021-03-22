//
// Created by Nikita Kruk on 03.07.18.
//

#ifndef SPRMOVIE_PARAMETERCONTAINER_HPP
#define SPRMOVIE_PARAMETERCONTAINER_HPP

#include "../Definitions.hpp"

#include <GLEW/glew.h>

#include <string>
#include <map>
#include <fstream>
#include <string>

class ParameterContainer
{
 public:

  ParameterContainer();
  virtual ~ParameterContainer();

  Real ParametersDictionaryValue(const std::string &name);
  int GetNumberOfStateVariables();

  const std::string &GetSimulationFolder();
  const std::string &GetTrackingFolder();
  const std::string &GetParameterFileName();
  const std::string &GetModelFileName();
  const std::string &GetActivePassiveFileName();
  const std::map<std::string, Real> &GetParameterDictionary();

 protected:

  std::string simulation_folder_;
  std::string tracking_folder_;
  std::string parameters_file_name_;
  std::string model_file_name_;
  std::string active_passive_file_name_;
  std::ifstream parameters_file_;
  std::map<std::string, Real> parameters_dictionary_;

 private:

  int number_of_state_variables_;

};

#endif //SPRMOVIE_PARAMETERCONTAINER_HPP

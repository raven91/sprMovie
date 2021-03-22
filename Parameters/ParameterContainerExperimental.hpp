//
// Created by Nikita Kruk on 12.08.18.
//

#ifndef SPRMOVIE_PARAMETERCONTAINEREXPERIMENTAL_HPP
#define SPRMOVIE_PARAMETERCONTAINEREXPERIMENTAL_HPP

#include "../Definitions.hpp"
#include "ParameterContainer.hpp"

class ParameterContainerExperimental : public ParameterContainer
{
 public:

  ParameterContainerExperimental();
  virtual ~ParameterContainerExperimental();

  void SetNumberOfAgents(int new_number_of_agents);

};

#endif //SPRMOVIE_PARAMETERCONTAINEREXPERIMENTAL_HPP

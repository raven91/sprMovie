//
// Created by Nikita Kruk on 05.07.18.
//

#ifndef SPRMOVIE_PARAMETERCONTAINERWITHMISDETECTIONS_HPP
#define SPRMOVIE_PARAMETERCONTAINERWITHMISDETECTIONS_HPP

#include "../Definitions.hpp"
#include "ParameterContainer.hpp"

class ParameterContainerWithMisdetections : public ParameterContainer
{
 public:

  void SetNumberOfAgents(int new_number_of_agents);

};

#endif //SPRMOVIE_PARAMETERCONTAINERWITHMISDETECTIONS_HPP

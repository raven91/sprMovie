//
// Created by Nikita Kruk on 05.07.18.
//

#include "ParameterContainerWithMisdetections.hpp"

void ParameterContainerWithMisdetections::SetNumberOfAgents(int new_number_of_agents)
{
  parameters_dictionary_["N"] = new_number_of_agents;
}
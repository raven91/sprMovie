//
// Created by Nikita Kruk on 05.07.18.
//

#include "Model.hpp"

#include <string>
#include <sstream>
#include <cassert>
#include <iostream>

Model::Model(ParameterContainer *parameter_container) :
    parameter_container_(parameter_container),
    system_state_(),
    active_passive_(),
    colors_(),
    indexes_(),
    number_of_agents_((int)parameter_container->ParametersDictionaryValue("N")),
    number_of_state_variables_(parameter_container->GetNumberOfStateVariables())
{

}

Model::~Model()
{

}

void Model::ReadNewState()
{

}

void Model::SkipTimeUnits(int t, Real delta_t)
{

}

const std::vector<GLfloat> &Model::GetCurrentState()
{
  return system_state_;
}

const std::vector<GLfloat> &Model::GetActivePassiveState()
{
  return active_passive_;
}

const std::map<int, std::vector<GLfloat>>& Model::GetColors()
{
  return colors_;
}

const std::vector<int>& Model::GetIndexes()
{
  return indexes_;
}
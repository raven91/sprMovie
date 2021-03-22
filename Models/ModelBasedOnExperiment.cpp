//
// Created by Nikita Kruk on 12.08.18.
//

#include "ModelBasedOnExperiment.hpp"

#include <sstream>
#include <string>
#include <cassert>
#include <iostream>
#include <cmath>

#include <eigen3/Eigen/Dense>

ModelBasedOnExperiment::ModelBasedOnExperiment(ParameterContainerExperimental *parameter_container) :
    Model(parameter_container)
{
  std::string
      experimental_file_name = parameter_container_->GetSimulationFolder() + parameter_container_->GetModelFileName();
  data_file_.open(experimental_file_name, std::ios::in);
  assert(data_file_.is_open());

  int time_idx = 0;
  int agent_idx = 0;
  Eigen::VectorXd agent(8);
  data_file_ >> time_idx >> number_of_agents_;
  parameter_container->SetNumberOfAgents(number_of_agents_);
  system_state_.resize(number_of_state_variables_ * number_of_agents_, 0.0f);
  indexes_.resize(number_of_agents_, 0);
  for (int i = 0; i < number_of_agents_; ++i)
  {
    data_file_ >> agent_idx >>
               agent(0) >> agent(1) >> agent(2) >> agent(3) >> agent(4) >> agent(5) >> agent(6) >> agent(7);
    int vec_idx = number_of_state_variables_ * i;
    system_state_[vec_idx] = agent(0);
    system_state_[vec_idx + 1] = agent(1);
//    system_state_[vec_idx + 2] = agent(2);
//    system_state_[vec_idx + 3] = agent(3);
    system_state_[vec_idx + 2] = std::cos(agent(5));
    system_state_[vec_idx + 3] = std::sin(agent(5));
    indexes_[i] = agent_idx;
  }
  std::cout << "t:" << time_idx << ", n:" << number_of_agents_ << std::endl;

  active_passive_.resize(number_of_agents_, 1.0f);
}

ModelBasedOnExperiment::~ModelBasedOnExperiment()
{
  data_file_.close();
}

void ModelBasedOnExperiment::ReadNewState()
{
  int time_idx = 0;
  int agent_idx = 0;
  Eigen::VectorXd agent(8);
  data_file_ >> time_idx >> number_of_agents_;
  ParameterContainerExperimental
      *parameter_container_experimental = dynamic_cast<ParameterContainerExperimental *>(parameter_container_);
  parameter_container_experimental->SetNumberOfAgents(number_of_agents_);
  system_state_.resize(number_of_state_variables_ * number_of_agents_, 0.0f);
  active_passive_.resize(number_of_agents_, 1.0f);
  indexes_.resize(number_of_agents_, 0);
  for (int i = 0; i < number_of_agents_; ++i)
  {
    data_file_ >> agent_idx >>
               agent(0) >> agent(1) >> agent(2) >> agent(3) >> agent(4) >> agent(5) >> agent(6) >> agent(7);
    int vec_idx = number_of_state_variables_ * i;
    system_state_[vec_idx] = agent(0);
    system_state_[vec_idx + 1] = agent(1);
    system_state_[vec_idx + 2] = agent(2);
    system_state_[vec_idx + 3] = agent(3);
    indexes_[i] = agent_idx;
  }

  std::cout << "t:" << time_idx << ", n:" << number_of_agents_ << std::endl;
}

void ModelBasedOnExperiment::SkipTimeUnits(int t, Real delta_t)
{
  std::string line;
  for (int i = 0; i < int(t / delta_t); ++i)
  {
    std::getline(data_file_, line);
  }
}


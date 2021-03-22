//
// Created by Nikita Kruk on 05.07.18.
//

#include "SimulationModelAfterTracking.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

SimulationModelAfterTracking::SimulationModelAfterTracking(ParameterContainerWithMisdetections *parameter_container) :
    Model(parameter_container),
    mersenne_twister_generator_(std::random_device{}()),
    unif_color_dist_(0.0f, 1.0f)
{
  Real phi = 0.1;
  Real a = 4;
  Real U0 = 250;
  Real k = 0;
  Real percentage_of_misdetections = 0.5;
  std::ostringstream modified_solution_file_name_buffer;
  modified_solution_file_name_buffer << parameter_container_->GetTrackingFolder() << "kalman_filter_output_" << phi
                                     << "_a_" << a << "_U0_" << U0 << "_k_" << k << "_pom_"
                                     << percentage_of_misdetections << ".txt";
  std::string simulation_file_name = modified_solution_file_name_buffer.str();
  data_file_.open(simulation_file_name, std::ios::in);
  assert(data_file_.is_open());

  int time_idx = 0;
  int agent_idx = 0;
  data_file_ >> time_idx >> number_of_agents_;
  parameter_container->SetNumberOfAgents(number_of_agents_);
  system_state_.resize(number_of_state_variables_ * number_of_agents_, 0.0f);
  indexes_.resize(number_of_agents_, 0);
  for (int i = 0; i < number_of_agents_; ++i)
  {
    int vec_idx = number_of_state_variables_ * i;
    data_file_ >> agent_idx >> system_state_[vec_idx] >> system_state_[vec_idx + 1] >> system_state_[vec_idx + 2]
               >> system_state_[vec_idx + 3];
    AddAgentColor(agent_idx);
    indexes_[i] = agent_idx;
  }
  std::cout << "t:" << time_idx << ", n:" << number_of_agents_ << std::endl;

  // TODO: remove active/passive subdivision from majority of Model-classes
  active_passive_.resize(number_of_agents_, 1.0f);
}

SimulationModelAfterTracking::~SimulationModelAfterTracking()
{
  data_file_.close();
}

void SimulationModelAfterTracking::ReadNewState()
{
  int time_idx = 0;
  int agent_idx = 0;
  data_file_ >> time_idx >> number_of_agents_;
  ParameterContainerWithMisdetections *parameter_container_with_misdetections =
      dynamic_cast<ParameterContainerWithMisdetections *>(parameter_container_);
  parameter_container_with_misdetections->SetNumberOfAgents(number_of_agents_);
  system_state_.resize(number_of_state_variables_ * number_of_agents_, 0.0f);
  active_passive_.resize(number_of_agents_, 1.0f);
  indexes_.resize(number_of_agents_, 0);
  for (int i = 0; i < number_of_agents_; ++i)
  {
    int vec_idx = number_of_state_variables_ * i;
    data_file_ >> agent_idx >> system_state_[vec_idx] >> system_state_[vec_idx + 1] >> system_state_[vec_idx + 2]
               >> system_state_[vec_idx + 3];
    AddAgentColor(agent_idx);
    indexes_[i] = agent_idx;
  }

  std::cout << "t:" << time_idx << ", n:" << number_of_agents_ << std::endl;
}

void SimulationModelAfterTracking::SkipTimeUnits(int t, Real delta_t)
{
  std::string line;
  for (int i = 0; i < int(t / delta_t); ++i)
  {
    std::getline(data_file_, line);
  }
}

void SimulationModelAfterTracking::AddAgentColor(int agent_idx)
{
  if (colors_.find(agent_idx) == colors_.end())
  {
    std::vector<GLfloat> agent_color(3);
    agent_color[0] = unif_color_dist_(mersenne_twister_generator_);
    agent_color[1] = unif_color_dist_(mersenne_twister_generator_);
    agent_color[2] = unif_color_dist_(mersenne_twister_generator_);
    colors_[agent_idx] = agent_color;
  }
}
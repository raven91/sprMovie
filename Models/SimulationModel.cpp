//
// Created by Nikita Kruk on 05.07.18.
//

#include "SimulationModel.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

SimulationModel::SimulationModel(ParameterContainer *parameter_container) :
    Model(parameter_container)
{
//  std::string folder = "/Users/nikita/Documents/Projects/spr/SelfPropelledRodsModelSimulation/100/";
//  Real phi = 0.1;
//  Real a = 5;
//  Real U0 = 3e-20;
//  Real k = 0;
  std::ostringstream simulation_file_name_buffer;
  simulation_file_name_buffer << parameter_container_->GetSimulationFolder() <<
                              "spr_simulation_N_" << parameter_container_->ParametersDictionaryValue("N")
                              << "_phi_" << parameter_container_->ParametersDictionaryValue("phi")
                              << "_a_" << parameter_container_->ParametersDictionaryValue("a")
                              << "_U0_" << parameter_container_->ParametersDictionaryValue("U_0")
                              << "_k_" << parameter_container_->ParametersDictionaryValue("k")
                              << ".bin";
//  simulation_file_name_buffer
//      << "/Users/nikita/Documents/Projects/spr/sprApproximateBayesianComputation/CandidateDatasets/candidate_dataset.bin";
  std::string simulation_file_name = simulation_file_name_buffer.str();
  data_file_.open(simulation_file_name, std::ios::binary | std::ios::in);
  assert(data_file_.is_open());
  system_state_.resize(number_of_state_variables_ * number_of_agents_, 0.0f);

//  std::ostringstream active_passive_file_name_buffer;
//  active_passive_file_name_buffer << parameter_container_->GetSimulationFolder() << "active_passive_phi_" << phi
//                                  << "_a_" << a << "_U0_" << U0 << "_k_" << k << ".txt";
//  std::string active_passive_file_name = active_passive_file_name_buffer.str();
//  active_passive_file_.open(active_passive_file_name, std::ios::in);
//  assert(active_passive_file_.is_open());
  active_passive_.resize(number_of_agents_, 1.0f);
//  std::copy(std::istream_iterator<Real>(active_passive_file_), std::istream_iterator<Real>(), active_passive_.begin());
//  active_passive_file_.close();
}

SimulationModel::~SimulationModel()
{
  data_file_.close();
}

void SimulationModel::ReadNewState()
{
  Real t = 0.0f;
  std::vector<Real> vec(number_of_state_variables_ * number_of_agents_, 0.0);
  data_file_.read((char *) &t, sizeof(Real));
  data_file_.read((char *) &vec[0], number_of_state_variables_ * number_of_agents_ * sizeof(Real));
  std::copy(vec.begin(), vec.end(), system_state_.begin());

  std::cout << "t:" << t << std::endl;
}

void SimulationModel::SkipTimeUnits(int t, Real delta_t)
{
  data_file_.seekg(int(t / delta_t) * (1 + number_of_state_variables_ * number_of_agents_) * sizeof(Real),
                   std::ios::cur);
}

const std::vector<GLfloat> &SimulationModel::GetActivePassiveState()
{
  return active_passive_;
}
//
// Created by Nikita Kruk on 03.07.18.
//

#include "ParameterContainer.hpp"

ParameterContainer::ParameterContainer() :
    number_of_state_variables_(4),
    parameters_dictionary_(),
    simulation_folder_("/Users/nikita/Documents/Projects/spr/SelfPropelledRodsModelSimulation/"),
    tracking_folder_("/Users/nikita/Documents/Projects/spr/MultiTargetTracking/Synthetic/")
{
  std::string parameter_string = "_N_1000_phi_0.75_a_15_U0_3e-20_k_0";
  parameters_file_name_ = simulation_folder_ + "parameters" + parameter_string + ".txt";
//  parameters_file_name_ = "/Users/nikita/Documents/Projects/spr/sprApproximateBayesianComputation/CandidateDatasets/candidate_dataset_parameters.txt";
  model_file_name_ = "spr_simulation" + parameter_string + ".bin";
  active_passive_file_name_ = "active_passive" + parameter_string + ".txt";

  // read parameters
  parameters_file_.open(parameters_file_name_, std::ios::in);
  assert(parameters_file_.is_open());
  std::string key;
  Real value;
  while (parameters_file_ >> key >> value)
  {
    parameters_dictionary_[key] = value;
  }
  parameters_file_.close();
}

ParameterContainer::~ParameterContainer()
{

}

Real ParameterContainer::ParametersDictionaryValue(const std::string &name)
{
  return parameters_dictionary_[name];
}

int ParameterContainer::GetNumberOfStateVariables()
{
  return number_of_state_variables_;
}

const std::string& ParameterContainer::GetSimulationFolder()
{
  return simulation_folder_;
}

const std::string& ParameterContainer::GetTrackingFolder()
{
  return tracking_folder_;
}

const std::string &ParameterContainer::GetParameterFileName()
{
  return parameters_file_name_;
}

const std::string &ParameterContainer::GetModelFileName()
{
  return model_file_name_;
}

const std::string &ParameterContainer::GetActivePassiveFileName()
{
  return active_passive_file_name_;
}

const std::map<std::string, Real>& ParameterContainer::GetParameterDictionary()
{
  return parameters_dictionary_;
}
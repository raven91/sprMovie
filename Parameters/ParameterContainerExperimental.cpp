//
// Created by Nikita Kruk on 12.08.18.
//

#include "ParameterContainerExperimental.hpp"

ParameterContainerExperimental::ParameterContainerExperimental() : ParameterContainer()
{
  simulation_folder_ = "/Volumes/Kruk/Swarming/20170720/100x_01-BF0_1to1600_yepd_1ms_6.2nm_v_1/DataAnalysis/";
  tracking_folder_ = "";
  parameters_file_name_ = "/Volumes/Kruk/Swarming/20170720/100x_01-BF0_1to1600_yepd_1ms_6.2nm_v_1/ConfigExperimental.cfg";
  model_file_name_ = "kalman_filter_output.txt";
  active_passive_file_name_ = "";

  // read parameters
  parameters_file_.open(parameters_file_name_, std::ios::in);
  assert(parameters_file_.is_open());
  std::map<std::string, std::string> experimental_configuration;
  std::string key, value;
  while (parameters_file_ >> key >> value)
  {
    experimental_configuration[key] = value;
  }
  parameters_file_.close();
  // adjust some parameters to those from a simulation model
  parameters_dictionary_.clear();
  parameters_dictionary_["output_delta_t"] = 1;
  parameters_dictionary_["n"] = 5;
  parameters_dictionary_["L"] = std::stod(experimental_configuration["subimage_x_size"]);
  parameters_dictionary_["lambda"] = 0.5 / kMicroMetersPerPixel;
  parameters_dictionary_["a"] = 5;
  parameters_dictionary_["l"] = parameters_dictionary_["a"] * parameters_dictionary_["lambda"];
}

ParameterContainerExperimental::~ParameterContainerExperimental()
{

}

void ParameterContainerExperimental::SetNumberOfAgents(int new_number_of_agents)
{
  parameters_dictionary_["N"] = new_number_of_agents;
}
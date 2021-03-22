#include "Rendering/Renderer.hpp"
#include "Models/Model.hpp"
#include "Models/SimulationModel.hpp"
#include "Models/SimulationModelWithMisdetections.hpp"
#include "Models/SimulationModelAfterTracking.hpp"
#include "Models/ModelBasedOnExperiment.hpp"
#include "Parameters/ParameterContainer.hpp"
#include "Parameters/ParameterContainerWithMisdetections.hpp"
#include "Parameters/ParameterContainerExperimental.hpp"

int main(int argc, const char *argv[])
{
  ParameterContainer parameter_container;
  SimulationModel model(&parameter_container);
//  Model *model_ptr = &model;
  Renderer renderer(model, parameter_container);
  renderer.Start();

//  ParameterContainerWithMisdetections parameter_container_with_misdetections;
//  SimulationModelWithMisdetections simulation_model_with_misdetections(&parameter_container_with_misdetections);
//  Renderer renderer(&simulation_model_with_misdetections, &parameter_container_with_misdetections);
//  renderer.Start();

//  ParameterContainerWithMisdetections parameter_container_with_misdetections;
//  SimulationModelAfterTracking simulation_model_after_tracking(&parameter_container_with_misdetections);
//  Renderer renderer(&simulation_model_after_tracking, &parameter_container_with_misdetections);
//  renderer.Start();

//  ParameterContainerExperimental parameter_container_experimental;
//  ModelBasedOnExperiment model_based_on_experiment(&parameter_container_experimental);
//  Renderer renderer(&model_based_on_experiment, &parameter_container_experimental);
//  renderer.Start();

  return 0;
}
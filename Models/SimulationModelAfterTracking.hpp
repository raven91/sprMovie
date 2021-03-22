//
// Created by Nikita Kruk on 05.07.18.
//

#ifndef SPRMOVIE_SIMULATIONMODELAFTERTRACKING_HPP
#define SPRMOVIE_SIMULATIONMODELAFTERTRACKING_HPP

#include "../Definitions.hpp"
#include "Model.hpp"
#include "../Parameters/ParameterContainerWithMisdetections.hpp"

#include <random>

class SimulationModelAfterTracking : public Model
{
 public:

  explicit SimulationModelAfterTracking(ParameterContainerWithMisdetections *parameter_container);
  ~SimulationModelAfterTracking();

  virtual void ReadNewState();
  virtual void SkipTimeUnits(int t, Real delta_t = Real(0.0));

 private:

  std::ifstream data_file_;
  std::mt19937 mersenne_twister_generator_;
  std::uniform_real_distribution<GLfloat> unif_color_dist_;

  void AddAgentColor(int agent_idx);

};

#endif //SPRMOVIE_SIMULATIONMODELAFTERTRACKING_HPP

//
// Created by Nikita Kruk on 05.07.18.
//

#ifndef SPRMOVIE_SIMULATIONMODELWITHMISDETECTIONS_HPP
#define SPRMOVIE_SIMULATIONMODELWITHMISDETECTIONS_HPP

#include "../Definitions.hpp"
#include "Model.hpp"
#include "../Parameters/ParameterContainerWithMisdetections.hpp"

class SimulationModelWithMisdetections : public Model
{
 public:

  explicit SimulationModelWithMisdetections(ParameterContainerWithMisdetections *parameter_container);
  ~SimulationModelWithMisdetections();

  virtual void ReadNewState();
  virtual void SkipTimeUnits(int t, Real delta_t = Real(0.0));

 private:

  std::ifstream data_file_;

};

#endif //SPRMOVIE_SIMULATIONMODELWITHMISDETECTIONS_HPP

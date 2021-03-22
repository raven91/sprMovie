//
// Created by Nikita Kruk on 05.07.18.
//

#ifndef SPRMOVIE_SIMULATIONMODEL_HPP
#define SPRMOVIE_SIMULATIONMODEL_HPP

#include "../Definitions.hpp"
#include "Model.hpp"
#include "../Parameters/ParameterContainer.hpp"

class SimulationModel : public Model
{
 public:

  SimulationModel(ParameterContainer *parameter_container);
  ~SimulationModel();

  virtual void ReadNewState();
  virtual void SkipTimeUnits(int t, Real delta_t = Real(1.0));

  const std::vector<GLfloat> &GetActivePassiveState();

 private:

  std::ifstream data_file_;
  std::ifstream active_passive_file_;

};

#endif //SPRMOVIE_SIMULATIONMODEL_HPP

//
// Created by Nikita Kruk on 12.08.18.
//

#ifndef SPRMOVIE_MODELBASEDONEXPERIMENT_HPP
#define SPRMOVIE_MODELBASEDONEXPERIMENT_HPP

#include "../Definitions.hpp"
#include "Model.hpp"
#include "../Parameters/ParameterContainerExperimental.hpp"

class ModelBasedOnExperiment : public Model
{
 public:

  explicit ModelBasedOnExperiment(ParameterContainerExperimental *parameter_container);
  ~ModelBasedOnExperiment();

  virtual void ReadNewState();
  virtual void SkipTimeUnits(int t, Real delta_t = Real(0.0));

 private:

  std::ifstream data_file_;

};

#endif //SPRMOVIE_MODELBASEDONEXPERIMENT_HPP

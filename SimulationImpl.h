#ifndef WIRELESS_LAN_SIMULATION_IMPL_H__
#define WIRELESS_LAN_SIMULATION_IMPL_H__

#include "Simulation.h"

namespace Framework {

inline void Simulation::SetSeed(int seed) {
  seed_ = seed;
}

inline void Simulation::SetRunNumber(int run) {
  run_ = run;
}

} // namespace Framework

#endif // WIRELESS_LAN_SIMULATION_IMPL_H__

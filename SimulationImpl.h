#ifndef INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__
#define INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__

#include "Simulation.h"

namespace Framework {

inline void Simulation::SetSeed(int seed) {
  seed_ = seed;
}

inline void Simulation::SetRunNumber(int run) {
  run_ = run;
}

} // namespace Framework

#endif // INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__

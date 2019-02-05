#ifndef INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__
#define INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__

#include "Simulation.h"

namespace Framework {

inline int Simulation::GetSeed() const {
  return seed_;
}

inline int Simulation::GetRunNumber() const {
  return run_;
}

} // namespace Framework

#endif // INCLUDED_WIRELESS_LAN_SIMULATION_IMPL_H__

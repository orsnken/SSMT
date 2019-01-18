#ifndef WIRELESS_LAN_SIMULATION_H__
#define WIRELESS_LAN_SIMULATION_H__

namespace Framework {

class Simulation {
public:
  Simulation();
  void Init(int argc, char* argv[]);
    // USER DEFINE
  void Run();
    // USER DEFINE
  
  void SetSeed(int seed);
  void SetRunNumber(int run);
private:
  int seed_;
  int run_;
};

} // namespace WirelessLan

#include "SimulationImpl.h"

#endif // WIRELESS_LAN_SIMULATION_H__

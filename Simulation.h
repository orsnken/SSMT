#ifndef INCLUDED_WIRELESS_LAN_SIMULATION_H__
#define INCLUDED_WIRELESS_LAN_SIMULATION_H__

namespace Framework {

class Simulation {
public:
  Simulation();
  void Init(int argc, char* argv[]);
    // USER DEFINE
  void Run();
    // USER DEFINE
  
  void Setup(int seed, int run);

  int GetSeed() const;
  int GetRunNumber() const;
private:
  int seed_;
  int run_;
};

} // namespace WirelessLan

#include "SimulationImpl.h"

#endif // INCLUDED_WIRELESS_LAN_SIMULATION_H__

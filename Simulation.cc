#include "ns3.h"
#include "Simulation.h"

#include <iostream>
#include <random>

namespace Framework {

using namespace ns3;

Simulation::Simulation() {
  std::random_device rnd;
  std::mt19937 mt(rnd());
  seed_ = mt() + 1;
  run_  = mt() + 1;
}

void Simulation::Setup() {
  RngSeedManager::SetSeed(seed_);
  RngSeedManager::SetRun(run_);
}

} // namespace WirelessLan

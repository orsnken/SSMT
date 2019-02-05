#include "ns3.h"
#include "Simulation.h"

#include <iostream>
#include <random>

namespace Framework {

using namespace ns3;

Simulation::Simulation() {
  std::random_device rnd;
  std::mt19937 mt(rnd());
  std::uniform_int_distribution<> rand1000(0, 1000);
  seed_ = rand1000(mt) + 1;
  run_  = rand1000(mt) + 1;
}

void Simulation::Setup(int seed = 0, int run = 0) {
  if (seed != 0) {
    seed_ = seed;
  }
  if (run != 0) {
    run_ = run;
  }
  RngSeedManager::SetSeed(seed_);
  RngSeedManager::SetRun(run_);
}

} // namespace WirelessLan

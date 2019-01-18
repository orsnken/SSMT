#include "ns3.h"

#include <random>

using namespace ns3;

int main(int argc, char *argv[]) {
  Framework::Simulation sim;
  sim.Init(argc, argv);
  sim.Setup();
  sim.Run();
}

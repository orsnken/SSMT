#include "ns3.h"
#include "Simulation.h"

#include <random>

using namespace ns3;

int main(int argc, char *argv[]) {
  Framework::Simulation sim;
  sim.Init(argc, argv);
  sim.Run();
}

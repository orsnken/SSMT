#include "ns3.h"
#include "Simulation.h"

#include <iostream>

void Framework::Simulation::Init(int argc, char* argv[]) {
  CommandLine cmd;
  cmd.Parse(argc, argv);
  
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(100000));
}

void Framework::Simulation::Run() {
  std::cout << "Hello world!" << std::endl;
  Simulator::Stop(1);
  Simulator::Run();
  Simulator::Destroy();
}

#include "ns3.h"
#include "Domain.h"
#include "Simulation.h"

#include <iostream>

using namespace ns3;
using namespace WirelessLan;

void Framework::Simulation::Init(int argc, char* argv[]) {
  CommandLine cmd;
  cmd.Parse(argc, argv);
  
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(100000));

  Domain::Init();
}

void Framework::Simulation::Run() {
  std::cout << "Hello world!" << std::endl;

  Domain network1("Network 1", "192.168.1.0", "255.255.255.0", 3);
  network1.ConfigureMobility(Vector3D(0.0, 0.0, 0.0), 10.0);

  Domain network2("Network 2", "192.168.2.0", "255.255.255.0", 4);
  network2.ConfigureMobility(Vector3D(0.0, 50.0, 0.0), 10.0);

  Domain network3("Network 3", "192.168.3.0", "255.255.255.0", 5);
  network3.ConfigureMobility(Vector3D(50.0, 25.0, 0.0), 10.0);

  Simulator::Stop(Seconds(1));
  Simulator::Run();
  Simulator::Destroy();
}

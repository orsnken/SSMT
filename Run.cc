#include "ns3.h"
#include "Domain.h"
#include "Simulation.h"

#include <iostream>

using namespace ns3;
using namespace WirelessLan;

namespace {
/*
ApplicationContainer SetApplication(
  Ptr<Node> src,
  Ptr<Node> dest,
  int port
) {
  Ipv4InterfaceAddress dest_addr = dest->GetObject<Ipv4>()->GetAddress(1, 0);
  InetSocketAddress remote_sock_addr(dest_addr.GetLocal(), port);

  BulkSendHelper ftp("ns3::TcpSocketFactory", Address());
  ftp.SetAttribute("Remote", AddressValue(remote_sock_addr));
  ApplicationContainer app = ftp.Install(src);
  PacketSinkHelper sink_helper("ns3::TcpSocketFactory", Address(remote_sock_addr));
  app.Add(sink_helper.Install(dest));

  return app;
}
*/

ApplicationContainer SetOnOffApplication(
  Ptr<Node> src,
  Ptr<Node> dest,
  int port,
  std::string protocol
) {
  Ipv4InterfaceAddress dest_addr = dest->GetObject<Ipv4>()->GetAddress(1, 0);
  InetSocketAddress remote_sock_addr(dest_addr.GetLocal(), port);

  OnOffHelper ftp(protocol, Address());
  ftp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  ftp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  ftp.SetAttribute("DataRate", StringValue("54Mbps"));
  ftp.SetAttribute("Remote", AddressValue(remote_sock_addr));
  ApplicationContainer app = ftp.Install(src);
  PacketSinkHelper sink_helper(protocol, Address(remote_sock_addr));
  app.Add(sink_helper.Install(dest));

  return app;
}

ApplicationContainer SetApplication(
  const Domain& domain,
  int port,
  double start_sec,
  double stop_sec
) {
  ApplicationContainer app;
  for (int i = 0, n = domain.GetN() - 1; i < n; i++) {
    app.Add(SetOnOffApplication(domain.GetStaNode(i), domain.GetApNode(), port + i, "ns3::UdpSocketFactory"));
  }
  app.Start(Seconds(start_sec));
  app.Stop(Seconds(stop_sec));
  
  return app;
}

} // namespace

void Framework::Simulation::Init(int argc, char* argv[]) {
  CommandLine cmd;
  cmd.Parse(argc, argv);
  
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(2200));

  // LogComponentEnable("BulkSendApplication", LOG_LEVEL_INFO);
  // LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

  Domain::Init();
}

void Framework::Simulation::Run() {
  std::cout << "Hello world!" << std::endl;

  Domain network1("Network 1", "192.168.1.0", "255.255.255.0", 3);
  network1.ConfigureMobility(Vector3D(0.0, 0.0, 0.0), 10.0);
  SetApplication(network1, 1001, 0.1, 1.0);

  Domain network2("Network 2", "192.168.2.0", "255.255.255.0", 4);
  network2.ConfigureMobility(Vector3D(0.0, 50.0, 0.0), 10.0);
  SetApplication(network2, 2001, 0.1, 1.0);

  Domain network3("Network 3", "192.168.3.0", "255.255.255.0", 5);
  network3.ConfigureMobility(Vector3D(50.0, 25.0, 0.0), 10.0);
  SetApplication(network3, 3001, 0.1, 1.0);

  AnimationInterface anim("SSMT-anim.xml");
  anim.EnablePacketMetadata();

  Simulator::Stop(Seconds(1));
  Simulator::Run();
  Simulator::Destroy();
}

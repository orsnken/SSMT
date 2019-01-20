#include "ns3.h"
#include "Domain.h"
#include "Simulation.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace ns3;
using namespace WirelessLan;

namespace {

void output(FlowMonitorHelper& flowMonitor, Ptr<FlowMonitor> fm) {
  fm->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitor.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = fm->GetFlowStats();
  std::cout << "--------------------------------------" << std::endl;
  double avr = 0.0;
  int i = 0;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); iter++) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
    double throughputKbps = iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024;
    NS_LOG_UNCOND("Flow ID: " << iter->first << " src addr " << t.sourceAddress << " dest addr " << t.destinationAddress);
    NS_LOG_UNCOND("TP:" << throughputKbps << ", Tx:" << iter->second.txBytes << ", Rx:" << iter->second.rxBytes << ", LP:" << iter->second.lostPackets);
    avr +=  throughputKbps;
    i++;
  }
  NS_LOG_UNCOND("SUM.: " << avr);
  NS_LOG_UNCOND("AVR.: " << avr / i);
}

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
  ftp.SetAttribute("DataRate", StringValue("50Mbps"));
  ftp.SetAttribute("PacketSize", UintegerValue(1464));
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
    // app.Add(SetApplication(domain.GetStaNode(i), domain.GetApNode(), port + i));
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

  LogComponentEnable("SsmtTxop", LOG_INFO);

  Domain::Init();
}

void Framework::Simulation::Run() {
  std::cout << "Hello world!" << std::endl;

  Domain network1("Network 1", "192.168.1.0", "255.255.255.0", 2);
  network1.ConfigureMobility(Vector3D(0.0, 0.0, 0.0), 0.5);
  SetApplication(network1, 1001, 0.1, 4.9);

  Domain network2("Network 2", "192.168.2.0", "255.255.255.0", 2);
  network2.ConfigureMobility(Vector3D(10.0, 0.0, 0.0), 0.5);
  SetApplication(network2, 2001, 0.1, 4.9);

  Domain network3("Network 3", "192.168.3.0", "255.255.255.0", 2);
  network3.ConfigureMobility(Vector3D(0.0, 10.0, 0.0), 0.5);
  SetApplication(network3, 3001, 0.1, 4.9);

  Domain network4("Network 4", "192.168.4.0", "255.255.255.0", 2);
  network4.ConfigureMobility(Vector3D(10.0, 10.0, 0.0), 0.5);
  SetApplication(network4, 4001, 0.1, 4.9);

  // Domain network5("Network 5", "192.168.5.0", "255.255.255.0", 1);
  // network5.ConfigureMobility(Vector3D(20.0, 0.0, 0.0), 0.5);
  // SetApplication(network5, 5001, 0.1, 4.9);

  // AnimationInterface anim("SSMT-anim.xml");
  // anim.EnablePacketMetadata();

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(5));
  Simulator::Run();
  Simulator::Destroy();

  output(flowMonitor, fm);
}

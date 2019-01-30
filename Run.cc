#include "ns3.h"
#include "Domain.h"
#include "Simulation.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace ns3;
using namespace WirelessLan;

namespace {

double gSsmtAlpha = 0.5;
double gSsmtPsi   = 4.0;
double gSsmtZeta  = 0.974;

double gDistanceWlans     = 10.0;
double gDistanceTerminals = 0.5;

std::string gMacType = "SSMT_CI";

void InitMacWithSsmt() {
  Config::SetDefault("ns3::SsmtTxop::Alpha", DoubleValue(gSsmtAlpha));
  Config::SetDefault("ns3::SsmtTxop::Psi"  , DoubleValue(gSsmtPsi));
  Config::SetDefault("ns3::SsmtTxop::Zeta" , DoubleValue(gSsmtZeta));

  Domain::Init();
}

void InitMacWithSrb() {
  exit(1);
}

void InitMacWithCwOnly() {
  exit(1);
}

void InitMacWithDcf() {
  exit(1);
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

void Output(FlowMonitorHelper& flowMonitor, Ptr<FlowMonitor> fm) {
  fm->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitor.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = fm->GetFlowStats();
  std::cout << "--------------------------------------" << std::endl;
  std::vector<double> throughputs_kbps;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); iter++) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
    double throughput_kbps = iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024;
    NS_LOG_UNCOND("Flow ID: " << iter->first << " src addr " << t.sourceAddress << " dest addr " << t.destinationAddress);
    NS_LOG_UNCOND("TP:" << throughput_kbps << ", Tx:" << iter->second.txBytes << ", Rx:" << iter->second.rxBytes << ", LP:" << iter->second.lostPackets);
    throughputs_kbps.push_back(throughput_kbps);
  }
  
  double fairness = 0.0;
  double total_throughput_kbps = 0.0;
  
  for (double v: throughputs_kbps) {
    fairness += v * v;
    total_throughput_kbps += v;
  }
  fairness = total_throughput_kbps * total_throughput_kbps / (static_cast<double>(throughputs_kbps.size()) * fairness);

  NS_LOG_UNCOND("SUM.: " << total_throughput_kbps);
  NS_LOG_UNCOND("AVR.: " << total_throughput_kbps / static_cast<double>(throughputs_kbps.size()));
  NS_LOG_UNCOND("FRN.: " << fairness);

}


} // namespace

void Framework::Simulation::Init(int argc, char* argv[]) {
  CommandLine cmd;
  cmd.AddValue("distanceWlans",
              "The distance between each Wireless LANs.",
              gDistanceWlans);
  cmd.AddValue("distanceTerminals",
               "The distance between AP and STAs.",
               gDistanceTerminals);
  cmd.AddValue("macType",
               "The MAC algorithm applied with each terminals (SSMT_CI/SRB/CW_ONLY/DCF).",
               gMacType);
  cmd.AddValue("ssmtAlpha",
               "The parameter associated with a successful transmission behavior for SSMT/CI.",
               gSsmtAlpha);
  cmd.AddValue("ssmtPsi",
               "The parameter associated with a failed transmission behavior for SSMT/CI.",
               gSsmtPsi);
  cmd.AddValue("ssmtZeta",
               "The upper limit of deterministic back-off probability for SSMT/CI.",
               gSsmtZeta);
  cmd.Parse(argc, argv);
  
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(2200));
  
  LogComponentEnable("SsmtTxop", LOG_INFO);
  LogComponentEnable("SsmtTxop", LOG_WARN);

  if (gMacType == "SSMT_CI") {
    InitMacWithSsmt();
  } else if (gMacType == "SRB") {
    InitMacWithSrb();
  } else if (gMacType == "CW_ONLY") {
    InitMacWithCwOnly();
  } else if (gMacType == "DCF") {
    InitMacWithDcf();
  } else {
    NS_LOG_UNCOND("The unknown MAC type \"" << gMacType << "\". Simulatinon aborted.");
    exit(1);
  }
}

void Framework::Simulation::Run() {
  std::cout << "Hello world!" << std::endl;

  Domain network1("Network 1", "192.168.1.0", "255.255.255.0", 1);
  network1.ConfigureMobility(Vector3D(0.0, 0.0, 0.0), gDistanceTerminals);
  SetApplication(network1, 1001, 0.1, 4.9);

  Domain network2("Network 2", "192.168.2.0", "255.255.255.0", 1);
  network2.ConfigureMobility(Vector3D(gDistanceWlans, 0.0, 0.0), gDistanceTerminals);
  SetApplication(network2, 2001, 0.1, 4.9);

  Domain network3("Network 3", "192.168.3.0", "255.255.255.0", 1);
  network3.ConfigureMobility(Vector3D(gDistanceWlans / 2.0, gDistanceWlans / 2.0 * 1.7320508, 0.0), gDistanceTerminals);
  SetApplication(network3, 3001, 0.1, 4.9);

  // AnimationInterface anim("SSMT-anim.xml");
  // anim.EnablePacketMetadata();

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(5));
  Simulator::Run();
  Simulator::Destroy();

  Output(flowMonitor, fm);
}

#include "ns3.h"
#include "Domain.h"
#include "PhyParameters.h"
#include "Simulation.h"
#include "SsmtTxop.h"
#include "WifiCeHelper.h"

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

bool gEnableLogComponents = false;

int gSeed = 0;
int gRun  = 0;

double gDistanceWlans     = 10.0;
double gDistanceTerminals = 0.5;

double gSsmtAlpha = 0.8;
double gSsmtPsi   = 4.0;
double gSsmtZeta  = 0.974;

ns3::WifiCeHelper          gWifiCeHelper;
ns3::YansWifiPhyHelper     gWifiPhyHelper;
ns3::YansWifiChannelHelper gWifiChannelHelper;

std::string gMacType = "SSMT_CI";

std::vector<Ptr<Domain>> gDomains;

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
  int port
) {
  ApplicationContainer app;
  for (int i = 0, n = domain.GetN() - 1; i < n; i++) {
    // app.Add(SetApplication(domain.GetStaNode(i), domain.GetApNode(), port + i));
    app.Add(SetOnOffApplication(domain.GetStaNode(i), domain.GetApNode(), port + i, "ns3::UdpSocketFactory"));
  }
  
  return app;
}

void InitMac() {
  gWifiCeHelper.SetRemoteStationManager(
    "ns3::ConstantRateWifiManager",
    "DataMode"   , StringValue(kRemoteStationDataMode),
    "ControlMode", StringValue(kRemoteStationControlMode)
  );
  gWifiCeHelper.SetStandard(kWifiPhyStandard);

  gWifiChannelHelper.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  gWifiChannelHelper.AddPropagationLoss(
    "ns3::LogDistancePropagationLossModel",
    "Exponent"         , DoubleValue(kLogDistancePropagationLossExponent),
    "ReferenceDistance", DoubleValue(kLogDistancePropagationLossReferenceDistance),
    "ReferenceLoss"    , DoubleValue(kLogDistancePropagationLossReferenceLoss)
  );
  gWifiChannelHelper.AddPropagationLoss(
    "ns3::NakagamiPropagationLossModel",
    "Distance1", DoubleValue(kNakagamiPropagationLossDistance1),
    "Distance2", DoubleValue(kNakagamiPropagationLossDistance2),
    "m0", DoubleValue(kNakagamiPropagationLossM0),
    "m1", DoubleValue(kNakagamiPropagationLossM1),
    "m2", DoubleValue(kNakagamiPropagationLossM2)
  );

  gWifiPhyHelper = YansWifiPhyHelper::Default();
  gWifiPhyHelper.SetPcapDataLinkType(kPhyPcapDlt);
  gWifiPhyHelper.SetChannel(gWifiChannelHelper.Create());
  gWifiPhyHelper.Set("EnergyDetectionThreshold", DoubleValue(kPhyEnergyDetectionThreshold));
  gWifiPhyHelper.Set("CcaMode1Threshold"       , DoubleValue(kPhyCcaMode1Threshold));
  gWifiPhyHelper.Set("TxPowerStart"            , DoubleValue(kPhyTxPowerStart));
  gWifiPhyHelper.Set("TxPowerEnd"              , DoubleValue(kPhyTxPowerEnd));
  gWifiPhyHelper.Set("ChannelNumber", UintegerValue(1));
}

void InitDomains(
  const std::string& mac_ap_type,
  WifiMacHelper& mac_ap,
  const std::string& mac_sta_type,
  WifiMacHelper& mac_sta) {
  Ptr<Domain> network = 0;

  network = Create<Domain>("Network 1", "192.168.1.0", "255.255.255.0", 1);
  network->Construct(gWifiCeHelper, gWifiPhyHelper, mac_ap_type, mac_ap, mac_sta_type, mac_sta);
  gDomains.push_back(network);

  network = Create<Domain>("Network 2", "192.168.2.0", "255.255.255.0", 1);
  network->Construct(gWifiCeHelper, gWifiPhyHelper, mac_ap_type, mac_ap, mac_sta_type, mac_sta);
  gDomains.push_back(network);

  // network = Create<Domain>("Network 3", "192.168.3.0", "255.255.255.0", 1);
  // network->Construct(gWifiCeHelper, gWifiPhyHelper, mac_ap_type, mac_ap, mac_sta_type, mac_sta);
  // gDomains.push_back(network);

  ApplicationContainer app;
  gDomains[0]->ConfigureMobility(Vector3D(0.0, 0.0, 0.0), gDistanceTerminals);
  SetApplication(*(gDomains[0]), 1001).Start(Seconds(0.5));
  gDomains[1]->ConfigureMobility(Vector3D(gDistanceWlans, 0.0, 0.0), gDistanceTerminals);
  SetApplication(*(gDomains[1]), 2001).Start(Seconds(0.1));
  // gDomains[2]->ConfigureMobility(Vector3D(gDistanceWlans / 2.0, gDistanceWlans / 2.0 * 1.7320508, 0.0), gDistanceTerminals);
  // app.Add(SetApplication(*(gDomains[2]), 3001));
  
  // Ptr<UniformRandomVariable> rv = CreateObject<UniformRandomVariable>();
  // rv->SetAttribute("Min", DoubleValue(0.01));
  // rv->SetAttribute("Max", DoubleValue(1.00));
  // app.StartWithJitter(Seconds(0.1), rv);
  // app.Stop(Seconds(9.9));
}

void InitMacWithSsmt() {
  const std::string kMacApType  = "ns3::SsmtApWifiMac";
  const std::string kMacStaType = "ns3::SsmtStaWifiMac";

  Config::SetDefault("ns3::SsmtTxop::Alpha", DoubleValue(gSsmtAlpha));
  Config::SetDefault("ns3::SsmtTxop::Psi"  , DoubleValue(gSsmtPsi));
  Config::SetDefault("ns3::SsmtTxop::Zeta" , DoubleValue(gSsmtZeta));
  
  WifiMacHelper mac_ap;
  mac_ap.SetType(
    kMacApType
  );
  WifiMacHelper mac_sta;
  mac_sta.SetType(
    kMacStaType,
    "ActiveProbing", BooleanValue(false)
  );

  InitDomains(kMacApType, mac_ap, kMacStaType, mac_sta);

  // ----
  // Direct Configure Process
  // ----
  // for (int i = 0, n = gDomains.size(); i < n; i++) {
  //   Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(gDomains[i]->GetStaNode(0)->GetDevice(0));
  //   Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
  //   Ptr<SsmtTxop> txop;
  //   PointerValue ptr;
  //   wifi_mac->GetAttribute("Txop", ptr);
  //   txop = ptr.Get<SsmtTxop>();
  //   txop->SetAlpha(0.0);
  // }
}

void InitMacWithSrb() {
  const std::string kMacApType  = "ns3::SsmtApWifiMac";
  const std::string kMacStaType = "ns3::SsmtStaWifiMac";

  Config::SetDefault("ns3::SsmtTxop::Alpha", DoubleValue(0.0));

  WifiMacHelper mac_ap;
  mac_ap.SetType(
    kMacApType
  );
  WifiMacHelper mac_sta;
  mac_sta.SetType(
    kMacStaType,
    "ActiveProbing", BooleanValue(false)
  );

  InitDomains(kMacApType, mac_ap, kMacStaType, mac_sta);
}

void InitMacWithCwOnly() {
  const std::string kMacApType  = "ns3::VcwApWifiMac";
  const std::string kMacStaType = "ns3::VcwStaWifiMac";
  
  WifiMacHelper mac_ap;
  mac_ap.SetType(
    kMacApType
  );
  WifiMacHelper mac_sta;
  mac_sta.SetType(
    kMacStaType,
    "ActiveProbing", BooleanValue(false),
    "VcwMinCw", IntegerValue(1),
    "VcwMaxCw", IntegerValue(2)
  );

  InitDomains(kMacApType, mac_ap, kMacStaType, mac_sta);

  // ----
  // Direct Configure Process
  // ----
  for (int i = 0, n = gDomains.size(); i < n; i++) {
    Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(gDomains[i]->GetApNode()->GetDevice(0));
    Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
    Ptr<Txop> txop;
    PointerValue ptr;
    wifi_mac->GetAttribute("Txop", ptr);
    txop = ptr.Get<Txop>();
    txop->SetMinCw(1);
  }
}

void InitMacWithDcf() {
  const std::string kMacApType  = "ns3::ApWifiMac";
  const std::string kMacStaType = "ns3::StaWifiMac";
  
  WifiMacHelper mac_ap;
  mac_ap.SetType(
    kMacApType
  );
  WifiMacHelper mac_sta;
  mac_sta.SetType(
    kMacStaType,
    "ActiveProbing", BooleanValue(false)
  );

  InitDomains(kMacApType, mac_ap, kMacStaType, mac_sta);
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
  cmd.AddValue("DistanceWlans",
              "The distance between each Wireless LANs.",
              gDistanceWlans);
  cmd.AddValue("DistanceTerminals",
               "The distance between AP and STAs.",
               gDistanceTerminals);
  cmd.AddValue("EnableLogComponents",
               "Enable log function.",
               gEnableLogComponents);
  cmd.AddValue("MacType",
               "The MAC algorithm applied with each terminals (SSMT_CI/SRB/CW_ONLY/DCF).",
               gMacType);
  cmd.AddValue("SimRun",
               "The run number for RndManager. Set to 0 if you want to initialize with a random stream.",
               gRun);
  cmd.AddValue("SimSeed",
               "The seed for RndManager. Set to 0 if you want to initialize with a random stream.",
               gSeed);
  cmd.AddValue("SsmtAlpha",
               "The parameter associated with a successful transmission behavior for SSMT/CI.",
               gSsmtAlpha);
  cmd.AddValue("SsmtPsi",
               "The parameter associated with a failed transmission behavior for SSMT/CI.",
               gSsmtPsi);
  cmd.AddValue("SsmtZeta",
               "The upper limit of deterministic back-off probability for SSMT/CI.",
               gSsmtZeta);
  cmd.Parse(argc, argv);
  
  Simulation::Setup(gSeed, gRun);

  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue(2200));
  
  if (gEnableLogComponents) {
    LogComponentEnable("SsmtTxop", LOG_INFO);
    LogComponentEnable("SsmtTxop", LOG_WARN);
    LogComponentEnable("Txop", LOG_INFO);
    LogComponentEnable("VcwStaWifiMac", LOG_INFO);
  }

  InitMac();
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
  std::cout << "SEED " << GetSeed() << "  RUN " << GetRunNumber() << std::endl;

  // AnimationInterface anim("SSMT-anim.xml");
  // anim.EnablePacketMetadata();

  FlowMonitorHelper flowMonitor;
  Ptr<FlowMonitor> fm = flowMonitor.InstallAll();

  Simulator::Stop(Seconds(10));
  Simulator::Run();
  Simulator::Destroy();

  Output(flowMonitor, fm);
}

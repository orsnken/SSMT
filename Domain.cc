#include "ns3.h"
#include "Domain.h"
#include "PhyParameters.h"
#include "WifiCeHelper.h"

#include <cmath>

using namespace ns3;

namespace {

ns3::WifiCeHelper          gWifiCeHelper;
ns3::YansWifiPhyHelper     gPhyHelper;
ns3::YansWifiChannelHelper gChannelHelper;

const int gChanneNumber = 1;

} // namespace *

namespace WirelessLan {

void Domain::Init() {
  gWifiCeHelper.SetRemoteStationManager(
    "ns3::ConstantRateWifiManager",
    "DataMode"   , StringValue(kRemoteStationDataMode),
    "ControlMode", StringValue(kRemoteStationControlMode)
  );
  gWifiCeHelper.SetStandard(kWifiPhyStandard);

  gChannelHelper.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  gChannelHelper.AddPropagationLoss(
    "ns3::LogDistancePropagationLossModel",
    "Exponent"         , DoubleValue(kLogDistancePropagationLossExponent),
    "ReferenceDistance", DoubleValue(kLogDistancePropagationLossReferenceDistance),
    "ReferenceLoss"    , DoubleValue(kLogDistancePropagationLossReferenceLoss)
  );
  gChannelHelper.AddPropagationLoss(
    "ns3::NakagamiPropagationLossModel",
    "Distance1", DoubleValue(kNakagamiPropagationLossDistance1),
    "Distance2", DoubleValue(kNakagamiPropagationLossDistance2),
    "m0", DoubleValue(kNakagamiPropagationLossM0),
    "m1", DoubleValue(kNakagamiPropagationLossM1),
    "m2", DoubleValue(kNakagamiPropagationLossM2)
  );

  gPhyHelper = YansWifiPhyHelper::Default();
  gPhyHelper.SetPcapDataLinkType(kPhyPcapDlt);
  gPhyHelper.SetChannel(gChannelHelper.Create());
  gPhyHelper.Set("EnergyDetectionThreshold", DoubleValue(kPhyEnergyDetectionThreshold));
  gPhyHelper.Set("CcaMode1Threshold"       , DoubleValue(kPhyCcaMode1Threshold));
  gPhyHelper.Set("TxPowerStart"            , DoubleValue(kPhyTxPowerStart));
  gPhyHelper.Set("TxPowerEnd"              , DoubleValue(kPhyTxPowerEnd));
}

Domain::Domain(
  std::string ssid,
  std::string naddr,
  std::string smask,
  int n
) : naddr_(naddr), smask_(smask) {
  ssid_ = Ssid(ssid);
  apNodes_.Create(1);
  staNodes_.Create(n);

  gPhyHelper.Set("ChannelNumber", UintegerValue(gChanneNumber));

  WifiMacHelper mac_sta;
  mac_sta.SetType(
    "ns3::SsmtStaWifiMac",
    "ActiveProbing", BooleanValue(false),
    "Ssid", SsidValue(ssid_)
  );
  staDevs_ = gWifiCeHelper.Install(gPhyHelper, mac_sta, staNodes_);

  WifiMacHelper mac_ap;
  mac_ap.SetType(
    "ns3::SsmtApWifiMac",
    "Ssid", SsidValue(ssid_)
  );
  apDevs_ = gWifiCeHelper.Install(gPhyHelper, mac_ap, apNodes_);

  InternetStackHelper stack;
  stack.Install(apNodes_);
  stack.Install(staNodes_);

  Ipv4AddressHelper ipv4Network;
  ipv4Network.SetBase(naddr_.c_str(), smask_.c_str());
  ipv4Network.Assign(apDevs_);
  ipv4Network.Assign(staDevs_);
}

void Domain::ConfigureMobility(
  ns3::Vector3D base,
  std::vector<ns3::Vector3D> rpstas
) {
  Ptr<ListPositionAllocator> positionAp = CreateObject<ListPositionAllocator>();
  positionAp->Add(base);
  Ptr<ListPositionAllocator> positionStas = CreateObject<ListPositionAllocator>();
  for (Vector3D v: rpstas) {
    positionStas->Add(v + base);
  }
  MobilityHelper mobility;
  mobility.SetPositionAllocator(positionAp);
  mobility.Install(apNodes_);
  mobility.SetPositionAllocator(positionStas);
  mobility.Install(staNodes_);
}

void Domain::ConfigureMobility(
  ns3::Vector3D base,
  double r
) {
  Ptr<ListPositionAllocator> position_ap = CreateObject<ListPositionAllocator>();
  position_ap->Add(base);
  Ptr<ListPositionAllocator> position_sta = CreateObject<ListPositionAllocator>();
  for (int i = 0, n = staNodes_.GetN(); i < n; i++) {
    double th = 2.0 * 3.141592 / n * i;
    Vector3D v(r * std::cos(th), r * std::sin(th), 0.0);
    position_sta->Add(v + base);
  }
  MobilityHelper mobility;
  mobility.SetPositionAllocator(position_ap);
  mobility.Install(apNodes_);
  mobility.SetPositionAllocator(position_sta);
  mobility.Install(staNodes_);
}

} // namespace WirelessLan

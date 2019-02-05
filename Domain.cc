#include "ns3.h"
#include "Domain.h"
#include "WifiCeHelper.h"

#include <cmath>

using namespace ns3;

namespace WirelessLan {

Domain::Domain(
  std::string ssid,
  std::string naddr,
  std::string smask,
  int n
) : naddr_(naddr), smask_(smask) {
  ssid_ = Ssid(ssid);
  apNodes_.Create(1);
  staNodes_.Create(n);
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

void Domain::Construct(
  ns3::WifiCeHelper&  wifi,
  ns3::WifiPhyHelper& phy,
  const std::string& mac_ap_type,
  ns3::WifiMacHelper& mac_ap,
  const std::string& mac_sta_type,
  ns3::WifiMacHelper& mac_sta
) {
  mac_ap.SetType(
    mac_ap_type.c_str(),
    "Ssid", SsidValue(ssid_)
  );
  apDevs_ = wifi.Install(phy, mac_ap, apNodes_);

  mac_sta.SetType(
    mac_sta_type.c_str(),
    "Ssid", SsidValue(ssid_)
  );
  staDevs_ = wifi.Install(phy, mac_sta, staNodes_);

  InternetStackHelper stack;
  stack.Install(apNodes_);
  stack.Install(staNodes_);

  Ipv4AddressHelper ipv4Network;
  ipv4Network.SetBase(naddr_.c_str(), smask_.c_str());
  ipv4Network.Assign(apDevs_);
  ipv4Network.Assign(staDevs_);
}

} // namespace WirelessLan

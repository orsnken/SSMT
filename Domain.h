#ifndef INCLUDED_WIRELESS_LAN_H__
#define INCLUDED_WIRELESS_LAN_H__

#include "ns3.h"
#include "WifiCeHelper.h"

#include <string>
#include <vector>

namespace WirelessLan {

class Domain : public ns3::Object {
public:
  Domain(
    std::string ssid,
    std::string network_addr,
    std::string subnet_mask,
    int num_of_nodes
  );

  void ConfigureMobility(
    ns3::Vector3D base,
    std::vector<ns3::Vector3D> relative_position_of_stations
  );

  void ConfigureMobility(
    ns3::Vector3D base,
    double radius
  );

  ns3::Ptr<ns3::Node> GetApNode() const;

  ns3::Ptr<ns3::Node> GetStaNode(int index) const;

  int GetN() const;

  void Construct(
    ns3::WifiCeHelper&  wifi,
    ns3::WifiPhyHelper& phy,
    const std::string& mac_ap_type,
    ns3::WifiMacHelper& mac_ap,
    const std::string& mac_sta_type,
    ns3::WifiMacHelper& mac_sta
  );
private:
  int ch_;
  std::string naddr_;
  std::string smask_;
  ns3::Ssid ssid_;
  ns3::NodeContainer apNodes_;
  ns3::NodeContainer staNodes_;
  ns3::NetDeviceContainer apDevs_;
  ns3::NetDeviceContainer staDevs_;
};

// ------------------------------------------------
// inline functions
// ------------------------------------------------

inline ns3::Ptr<ns3::Node> Domain::GetApNode() const {
  return apNodes_.Get(0);
}

inline ns3::Ptr<ns3::Node> Domain::GetStaNode(int index) const {
  return staNodes_.Get(index);
}

inline int Domain::GetN() const {
  return apNodes_.GetN() + staNodes_.GetN();
}

// ------------------------------------------------

} // namespace WirelessLan

#endif // INCLUDED_WIRELESS_LAN_H__

#ifndef WIRELESS_LAN_H__
#define WIRELESS_LAN_H__

#include "ns3.h"

#include <string>
#include <vector>

namespace WirelessLan {

class Domain {
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

  ns3::Ptr<ns3::Node> GetApNode();

  ns3::Ptr<ns3::Node> GetStaNode(int index);

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

inline ns3::Ptr<ns3::Node> Domain::GetApNode() {
  return apNodes_.Get(0);
}

inline ns3::Ptr<ns3::Node> Domain::GetStaNode(int index) {
  return staNodes_.Get(index);
}

// ------------------------------------------------

} // namespace WirelessLan

#endif // WIRELESS_LAN_H__

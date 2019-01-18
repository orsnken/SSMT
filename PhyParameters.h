#ifndef WIRELESS_LAN_PHY_PARAMETERS_H__
#define WIRELESS_LAN_PHY_PARAMETERS_H__

#include "ns3.h"

#include <string>

namespace WirelessLan {

static const std::string kRemoteStationDataMode;
  // Transmission mode for data.
  // Recommend -> ErpOfdmRate54Mbps

static const std::string kRemoteStationControlMode;
  // Transmission mode for control messages.
  // Recommend -> ErpOfdmRate54Mbps

static const ns3::WifiPhyStandard kWifiPhyStandard;
  // The standard for a wireless LAN.
  // ---- List
  //   WIFI_PHY_STANDARD_80211a
  //   WIFI_PHY_STANDARD_80211b
  //   WIFI_PHY_STANDARD_80211g
  //   WIFI_PHY_STANDARD_80211_10MHZ
  //   WIFI_PHY_STANDARD_80211_5MHZ
  //   WIFI_PHY_STANDARD_holland
  //   WIFI_PHY_STANDARD_80211n_2_4GHZ
  //   WIFI_PHY_STANDARD_80211n_5GHZ
  //   WIFI_PHY_STANDARD_80211ac
  //   WIFI_PHY_STANDARD_80211ax_2_4GHZa
  //   WIFI_PHY_STANDARD_80211ax_5GHZ
  //   WIFI_PHY_STANDARD_UNSPECIFIED

static const double kLogDistancePropagationLossExponent;
  // Loss exponent for Log Distance Propagation Loss Model.
  // Higher value means noisy environment.
  // In ideal space, this value is equal to 2.0.
  // Recommend -> 3.0

static const double kLogDistancePropagationLossReferenceDistance;
  // Reference distance for Log Distance Propagation Loss Model.
  // Recommend -> 1.0

static const double kLogDistancePropagationLossReferenceLoss;
  // Reference loss for Log Distance Propagation Loss Model.
  // Recommend -> 40.045997

static const double kNakagamiPropagationLossDistance1;
static const double kNakagamiPropagationLossDistance2;
static const double kNakagamiPropagationLossM0;
static const double kNakagamiPropagationLossM1;
static const double kNakagamiPropagationLossM2;

static const ns3::WifiPhyHelper::SupportedPcapDataLinkTypes kPhyPcapDlt;
  // Data link type.
  // ---- List
  //   DLT_IEEE802_11
  //   DLT_PRISM_HEADER
  //   DLT_IEEE802_11_RADIO

static const double kPhyEnergyDetectionThreshold;
  // Carrer sense threshold.

static const double kPhyCcaMode1Threshold;
  // CCA threshold.

static const double kPhyTxPowerStart;
  // 10[dBm](10[mW]) is maximum transmission power of wireless LANs in Japan.
  // Recommend -> 10[dBm]

static const double kPhyTxPowerEnd;
  // 10[dBm](10[mW]) is maximum transmission power of wireless LANs in Japan.
  // Recommend -> 10[dBm]

} // namesapce WirelessLan

#endif // WIRELESS_LAN_PHY_PARAMETERS_H__

#ifndef WIRELESS_LAN_PHY_PARAMETERS_H__
#define WIRELESS_LAN_PHY_PARAMETERS_H__

#include "ns3.h"

#include <string>

namespace WirelessLan {

static const std::string kRemoteStationDataMode;
static const std::string kRemoteStationControlMode;

static const ns3::WifiPhyStandard kWifiPhyStandard;
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
static const double kLogDistancePropagationLossReferenceDistance;
static const double kLogDistancePropagationLossReferenceLoss;

static const double kNakagamiPropagationLossDistance1;
static const double kNakagamiPropagationLossDistance2;
static const double kNakagamiPropagationLossM0;
static const double kNakagamiPropagationLossM1;
static const double kNakagamiPropagationLossM2;

static const ns3::WifiPhyHelper::SupportedPcapDataLinkTypes kPhyPcapDlt;
  // ---- List
  //   DLT_IEEE802_11
  //   DLT_PRISM_HEADER
  //   DLT_IEEE802_11_RADIO
static const double kPhyEnergyDetectionThreshold;
static const double kPhyCcaMode1Threshold;
static const double kPhyTxPowerStart;
static const double kPhyTxPowerEnd;

} // namesapce WirelessLan

#endif // WIRELESS_LAN_PHY_PARAMETERS_H__

#include "PhyParameters.h"

namespace WirelessLan {

// ------------------------------------------------
// Wifi Setting
// ------------------------------------------------
const std::string kRemoteStationDataMode    = "ErpOfdmRate54Mbps";

const std::string kRemoteStationControlMode = "ErpOfdmRate54Mbps";

const ns3::WifiPhyStandard kWifiPhyStandard = ns3::WIFI_PHY_STANDARD_80211g;
// ------------------------------------------------


// ------------------------------------------------
// Phy Setting
// ------------------------------------------------
const ns3::WifiPhyHelper::SupportedPcapDataLinkTypes kPhyPcapDlt
    = ns3::DLT_IEEE802_11_RADIO;

const double kPhyEnergyDetectionThreshold = -82.0;

const double kPhyCcaMode1Threshold        = -82.0;

const double kPhyTxPowerStart             = 10.0;

const double kPhyTxPowerEnd               = 10.0;
// ------------------------------------------------


// ------------------------------------------------
// Log Distance Propagation Model Setting
// ------------------------------------------------
const double kLogDistancePropagationLossExponent          = 3.0;

const double kLogDistancePropagationLossReferenceDistance = 1.0;

const double kLogDistancePropagationLossReferenceLoss     = 40.045997;
// ------------------------------------------------


// ------------------------------------------------
// Nakagami Propagation Model Setting
// ------------------------------------------------
const double kNakagamiPropagationLossDistance1 = 80.0;

const double kNakagamiPropagationLossDistance2 = 200.0;

const double kNakagamiPropagationLossM0        = 1.0;

const double kNakagamiPropagationLossM1        = 1.0;

const double kNakagamiPropagationLossM2        = 1.0;
// ------------------------------------------------

} // namespace WirelessLan
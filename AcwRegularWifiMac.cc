#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "ns3/regular-wifi-mac.h"
#include "ns3/wifi-phy.h"
#include "ns3/mac-rx-middle.h"
#include "ns3/mac-tx-middle.h"
#include "ns3/mac-low.h"
#include "ns3/msdu-aggregator.h"
#include "ns3/mpdu-aggregator.h"
#include "ns3/wifi-utils.h"
#include "ns3/mgt-headers.h"
#include "ns3/amsdu-subframe-header.h"

#include "AcwRegularWifiMac.h"
#include "AcwTxop.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("AcwRegularWifiMac");
NS_OBJECT_ENSURE_REGISTERED(AcwRegularWifiMac);

AcwRegularWifiMac::AcwRegularWifiMac() : RegularWifiMac() {
  NS_LOG_FUNCTION (this);
  // m_txop->Dispose();
  // m_txop = 0;

  parent_txop_expired_ = m_txop;
    // To use AcwTxop, remove parent txop. 

  m_txop = CreateObject<AcwTxop> ();
  m_txop->SetMacLow(m_low);
  m_txop->SetChannelAccessManager(m_channelAccessManager);
  m_txop->SetTxMiddle(m_txMiddle);
  m_txop->SetTxOkCallback (MakeCallback (&AcwRegularWifiMac::TxOk, this));
  m_txop->SetTxFailedCallback (MakeCallback (&AcwRegularWifiMac::TxFailed, this));
  m_txop->SetTxDroppedCallback (MakeCallback (&RegularWifiMac::NotifyTxDrop, this));
}

AcwRegularWifiMac::~AcwRegularWifiMac() {
  NS_LOG_FUNCTION (this);
}

void AcwRegularWifiMac::DoDispose() {
  NS_LOG_FUNCTION(this);
  parent_txop_expired_->Dispose();
  parent_txop_expired_ = 0;
  RegularWifiMac::DoDispose();
}

} // namespace ns3


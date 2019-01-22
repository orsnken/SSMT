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

#include "SsmtRegularWifiMac.h"
#include "SsmtTxop.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("SsmtRegularWifiMac");
NS_OBJECT_ENSURE_REGISTERED(SsmtRegularWifiMac);

SsmtRegularWifiMac::SsmtRegularWifiMac() : RegularWifiMac() {
  NS_LOG_FUNCTION (this);
  // m_txop->Dispose();
  // m_txop = 0;

  parent_txop_expired_ = m_txop;
    // To use SsmtTxop, remove parent txop. 

  m_txop = CreateObject<SsmtTxop> ();
  m_txop->SetMacLow(m_low);
  m_txop->SetChannelAccessManager(m_channelAccessManager);
  m_txop->SetTxMiddle(m_txMiddle);
  m_txop->SetTxOkCallback (MakeCallback (&SsmtRegularWifiMac::TxOk, this));
  m_txop->SetTxFailedCallback (MakeCallback (&SsmtRegularWifiMac::TxFailed, this));
  m_txop->SetTxDroppedCallback (MakeCallback (&RegularWifiMac::NotifyTxDrop, this));
}

SsmtRegularWifiMac::~SsmtRegularWifiMac() {
  NS_LOG_FUNCTION (this);
}

void SsmtRegularWifiMac::DoDispose() {
  NS_LOG_FUNCTION(this);
  parent_txop_expired_->Dispose();
  parent_txop_expired_ = 0;
  RegularWifiMac::DoDispose();
}

} // namespace ns3


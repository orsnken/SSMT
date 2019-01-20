#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "ns3/txop.h"
#include "ns3/channel-access-manager.h"
#include "ns3/wifi-mac-queue.h"
#include "ns3/mac-tx-middle.h"
#include "ns3/mac-low.h"
#include "ns3/wifi-remote-station-manager.h"

#include "SsmtTxop.h"

#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("SsmtTxop");
NS_OBJECT_ENSURE_REGISTERED(SsmtTxop);

SsmtTxop::SsmtTxop() : numf_(0), trr_(0.2), trr_c_(0.8) {
  NS_LOG_FUNCTION(this);
}

SsmtTxop::~SsmtTxop() {
  NS_LOG_FUNCTION(this);
}

void SsmtTxop::MissedCts () {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("missed cts");
  if (!NeedRtsRetransmission (m_currentPacket, m_currentHdr)) {
    NS_LOG_DEBUG ("Cts Fail");
    m_stationManager->ReportFinalRtsFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
    if (!m_txFailedCallback.IsNull ()) {
      m_txFailedCallback (m_currentHdr);
    }
    //to reset the dcf.
    m_currentPacket = 0;
    ResetCw ();
    ResetTrr();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  } else {
    UpdateFailedCw ();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  }
  RestartAccessIfNeeded ();
}

void SsmtTxop::GotAck() {
  NS_LOG_FUNCTION (this);
  if (!NeedFragmentation() || IsLastFragment()) {
    NS_LOG_DEBUG ("got ack. tx done.");
    if (!m_txOkCallback.IsNull ()) {
      m_txOkCallback (m_currentHdr);
    }
    /* we are not fragmenting or we are done fragmenting
     * so we can get rid of that packet now.
     */
    m_currentPacket = 0;
    ResetCw();
    CalcTrr(true);
    StartBackoffNow((GetMinCw() + 1) / 2);
    RestartAccessIfNeeded();
  } else {
    NS_LOG_WARN ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
  }
}

void SsmtTxop::MissedAck() {
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr)) {
    NS_LOG_DEBUG ("Ack Fail");
    m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                             m_currentPacket->GetSize ());
    if (!m_txFailedCallback.IsNull ()) {
      m_txFailedCallback (m_currentHdr);
    }
    //to reset the dcf.
    m_currentPacket = 0;
    ResetCw ();
    ResetTrr();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  } else {
    NS_LOG_DEBUG ("Retransmit");
    m_currentHdr.SetRetry ();
    UpdateFailedCw ();
    if (CalcTrr(false) > m_rng->GetValue(0.0, 1.0)) {
      StartBackoffNow((GetMinCw() + 1) / 2);
      NS_LOG_INFO ("[" << Simulator::Now() << "]SSMT Try " << numf_ << " +" << m_backoffSlots);
    } else {
      ResetTrr();
      StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
      NS_LOG_INFO ("[" << Simulator::Now() << "]SSMT Chg " << numf_ << " +" << m_backoffSlots);
    }
  }
  RestartAccessIfNeeded ();
}

void SsmtTxop::NotifyCollision() {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("NotifyCollision");
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  RestartAccessIfNeeded ();
}

void SsmtTxop::EndTxNoAck () {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("a transmission that did not require an ACK just finished");
  m_currentPacket = 0;
  ResetCw ();
  StartBackoffNow(m_rng->GetInteger (0, GetCw ()));
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
  StartAccessIfNeeded ();
}

double SsmtTxop::CalcTrr(bool succeeded) {
  if (succeeded) {
    trr_ = (1.0 - trr_c_) * trr_ + trr_c_;
    // trr_ = std::min(trr_ + 0.3, 1.0);
    // trr_ = 1.0;
  } else {
    trr_ = std::max(0.0, trr_c_ * trr_ * trr_ * trr_ * trr_);
    // trr_ = 0.0;
  }
  return trr_;
}

double SsmtTxop::ResetTrr() {
  trr_ = 1.0 - trr_c_;
  // trr_ = 0.7;
  return trr_;
}

void SsmtTxop::ResetCw() {
  NS_LOG_FUNCTION (this);
  numf_ = 0;
  m_cw = m_cwMin;
}

void SsmtTxop::UpdateFailedCw() {
  NS_LOG_FUNCTION (this);
  numf_++;
  //see 802.11-2012, section 9.19.2.5
  m_cw = std::min ( 2 * (m_cw + 1) - 1, m_cwMax);
}

} // namespace ns3

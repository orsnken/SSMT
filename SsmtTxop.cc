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

#undef  NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("SsmtTxop");
NS_OBJECT_ENSURE_REGISTERED(SsmtTxop);

SsmtTxop::SsmtTxop() :
numf_(0),
trr_(0.0),
trr_alpha_(0.0),
trr_phi_(0.0),
trr_psi_(0.0),
trr_zeta_(0.0) {
  NS_LOG_FUNCTION(this);
}

SsmtTxop::~SsmtTxop() {
  NS_LOG_FUNCTION(this);
}

TypeId SsmtTxop::GetTypeId() {
  static TypeId tid = TypeId("ns3::SsmtTxop")
    .SetParent<ns3::Txop>()
    .SetGroupName("Wifi")
    .AddConstructor<SsmtTxop>()
    .AddAttribute ("Alpha", "The coefficient for a successful transmission.",
                   DoubleValue(0.8),
                   MakeDoubleAccessor(&SsmtTxop::SetAlpha,
                                      &SsmtTxop::GetAlpha),
                   MakeDoubleChecker<double>())
    .AddAttribute ("Psi", "The exponent value for a failed transmission.",
                   DoubleValue(6.0),
                   MakeDoubleAccessor(&SsmtTxop::SetPsi,
                                      &SsmtTxop::GetPsi),
                   MakeDoubleChecker<double>())
    .AddAttribute ("Zeta", "The deterministic back-off probability for the first failed transmission.",
                   DoubleValue(0.9),
                   MakeDoubleAccessor(&SsmtTxop::SetZeta,
                                      &SsmtTxop::GetZeta),
                   MakeDoubleChecker<double>());
  return tid;
}

void SsmtTxop::DoDispose() {
  NS_LOG_FUNCTION(this);
  Txop::DoDispose();
}

void SsmtTxop::DoInitialize() {
  NS_LOG_FUNCTION(this);
  Txop::DoInitialize();
}

void SsmtTxop::MissedCts() {
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
    NS_LOG_DEBUG ("[" << Simulator::Now() << "]SSMT Got Ack " << numf_ << " +" << m_backoffSlots);
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
    NS_LOG_INFO ("[" << Simulator::Now() << "]SSMT Gave up " << numf_);
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
      StartBackoffNow((GetMinCw() + 1) / 2 * pow(2.0, numf_));
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

void SsmtTxop::NotifyInternalCollision() {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("NotifyInternalCollision");
  NotifyCollision();
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

void SsmtTxop::CalcPhi() {
  NS_LOG_FUNCTION(this);
  trr_phi_ = std::pow(trr_zeta_, 1.0 / trr_psi_);
}

double SsmtTxop::CalcTrr(bool succeeded) {
  if (succeeded) {
    trr_ = (1.0 - trr_alpha_) * trr_ + trr_alpha_ * trr_phi_;
  } else {
    trr_ = std::pow(trr_, trr_psi_);
  }
  return trr_;
}

double SsmtTxop::ResetTrr() {
  trr_ = 0.0;
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

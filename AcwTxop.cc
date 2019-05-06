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

#include "AcwTxop.h"

#include <cmath>

#undef  NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("AcwTxop");
NS_OBJECT_ENSURE_REGISTERED(AcwTxop);

AcwTxop::AcwTxop() {
  NS_LOG_FUNCTION(this);
}

AcwTxop::~AcwTxop() {
  NS_LOG_FUNCTION(this);
}

void AcwTxop::DoDispose() {
  NS_LOG_FUNCTION(this);
  Txop::DoDispose();
}

void AcwTxop::DoInitialize() {
  NS_LOG_FUNCTION(this);
  Txop::DoInitialize();
}

void AcwTxop::MissedCts() {
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
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  } else {
    UpdateFailedCw ();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  }
  RestartAccessIfNeeded ();
}

void AcwTxop::GotAck() {
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
    if (0 < m_cw && m_cw <= m_cwMin) {
      if (1 < m_cw) {
        m_cw--;
      } else {
        m_cw = 1;
      }
    } else {
      ResetCw();
    }
    NS_ASSERT(m_cw >= 1);
    StartBackoffNow (m_rng->GetInteger (0, m_cw));
    RestartAccessIfNeeded();
  } else {
    NS_LOG_WARN ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
  }
}

void AcwTxop::MissedAck() {
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr)) {
    m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                             m_currentPacket->GetSize ());
    if (!m_txFailedCallback.IsNull ()) {
      m_txFailedCallback (m_currentHdr);
    }
    //to reset the dcf.
    m_currentPacket = 0;
    ResetCw ();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  } else {
    NS_LOG_DEBUG ("Retransmit");
    m_currentHdr.SetRetry ();
    UpdateFailedCw ();
    StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  }
  RestartAccessIfNeeded ();
}

void AcwTxop::NotifyCollision() {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("NotifyCollision");
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  RestartAccessIfNeeded ();
}

void AcwTxop::NotifyInternalCollision() {
  NS_LOG_FUNCTION (this);
  NS_LOG_WARN ("NotifyInternalCollision");
  NotifyCollision();
}


void AcwTxop::EndTxNoAck () {
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

void AcwTxop::ResetCw() {
  NS_LOG_FUNCTION (this);
  m_cw = m_cwMin;
}

void AcwTxop::UpdateFailedCw() {
  NS_LOG_FUNCTION (this);
  //see 802.11-2012, section 9.19.2.5
  if (m_cw < m_cwMin) {
    m_cw = m_cwMin;
  } else {
    m_cw = std::min ( 2 * (m_cw + 1) - 1, m_cwMax);
  }
  m_cw = m_cwMin;
}

} // namespace ns3

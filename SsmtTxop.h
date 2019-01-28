#ifndef INCLUDED_SSMT_TXOP_H__
#define INCLUDED_SSMT_TXOP_H__

#include "ns3/assert.h"
#include "ns3/txop.h"

#include <cmath>

namespace ns3 {

class SsmtTxop : public Txop {
public:
  SsmtTxop();
  virtual ~SsmtTxop();

  static TypeId GetTypeId();

  // @Override
  virtual void MissedCts();

  // @Override
  virtual void GotAck();

  // @Override
  virtual void MissedAck();
  
  // @Override
  virtual void NotifyCollision();

  // @Override
  virtual void NotifyInternalCollision();

  // @Override
  virtual void EndTxNoAck();

  // @Oveeride
  virtual void DoDispose();

  // @Override
  virtual void DoInitialize();

  void SetAlpha(double alpha);

  void SetPsi(double psi);

  void SetZeta(double zeta);

  double GetAlpha() const;

  double GetPsi() const;

  double GetZeta() const;
private:
  int numf_;

  // ----------------------------------------------------------------
  // 
  // trr_ = (1 - trr_alpha_) * trr_ + trr_alpha_ * trr_phi_
  //   For a succeffsul transmission
  //
  // trr_ = trr_ ^ trr_psi_
  //   For a failed transmission
  //
  // trr_zeta_ means the deterministic back-off probability for the first time.
  // So, the initial value of trr_phi_ is calculated with the following formula.
  //   trr_phi_ = trr_zeta_ ^ (1.0 / trr_psi_)
  //
  // ----------------------------------------------------------------
  double trr_;
  double trr_alpha_;
  double trr_phi_;
  double trr_psi_;
  double trr_zeta_;
  
  void CalcPhi();
  double CalcTrr(bool succeeded);
  double ResetTrr();

  // NOT Override!!
  void ResetCw();
  // NOT Override!!
  void UpdateFailedCw();
};

inline void SsmtTxop::SetAlpha(double alpha) {
  NS_ASSERT(0.0 <= alpha && alpha <= 1.0);
  trr_alpha_ = alpha;
}

inline void SsmtTxop::SetPsi(double psi) {
  NS_ASSERT(1.0 <= psi);
  trr_psi_ = psi;
  CalcPhi();
}

inline void SsmtTxop::SetZeta(double zeta) {
  NS_ASSERT(0.0 <= zeta && zeta <= 1.0);
  trr_zeta_ = zeta;
  CalcPhi();
};

inline double SsmtTxop::GetAlpha() const {
  return trr_alpha_;
}

inline double SsmtTxop::GetPsi() const {
  return trr_psi_;
}

inline double SsmtTxop::GetZeta() const {
  return trr_zeta_;
}

} // namespace ns3

#endif // INCLUDED_SSMT_TXOP_H__

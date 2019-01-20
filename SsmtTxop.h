#ifndef INCLUDED_SSMT_TXOP_H__
#define INCLUDED_SSMT_TXOP_H__

#include "ns3/txop.h"

namespace ns3 {

class SsmtTxop : public Txop {
public:
  SsmtTxop();
  virtual ~SsmtTxop();

  // @Override
  virtual void MissedCts();

  // @Override
  virtual void GotAck();

  // @Override
  virtual void MissedAck();
  
  // @Override
  virtual void NotifyCollision();

  // @Override
  virtual void EndTxNoAck();
private:
  int numf_;
  double trr_;
  double trr_c_;
  
  double CalcTrr(bool succeeded);
  double ResetTrr();

  // NOT Override!!
  void ResetCw();
  // NOT Override!!
  void UpdateFailedCw();
};

}

#endif // INCLUDED_SSMT_TXOP_H__

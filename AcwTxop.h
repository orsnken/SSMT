#ifndef INCLUDED_ACW_TXOP_H__
#define INCLUDED_ACW_TXOP_H__

#include "ns3/assert.h"
#include "ns3/txop.h"

#include <cmath>

namespace ns3 {

class AcwTxop : public Txop {
public:
  AcwTxop();
  virtual ~AcwTxop();

  // @Override
  virtual void MissedCts();

  // @Override
  virtual void GotAck();

  // @Override
  virtual void MissedAck();

  // @Override
  virtual void NotifyInternalCollision();

  // @Override
  virtual void EndTxNoAck();

  // @Oveeride
  virtual void DoDispose();

  // @Override
  virtual void DoInitialize();

protected:
  // @Override
  virtual void NotifyCollision();

private:
  // NOT Override!!
  void ResetCw();
  // NOT Override!!
  void UpdateFailedCw();
};

} // namespace ns3

#endif // INCLUDED_ACW_TXOP_H__

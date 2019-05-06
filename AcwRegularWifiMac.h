#ifndef INCLUDED_ACW_REGULAR_WIFI_MAC_H__
#define INCLUDED_ACW_REGULAR_WIFI_MAC_H__

#include "ns3/regular-wifi-mac.h"

namespace ns3 {

class AcwRegularWifiMac : public RegularWifiMac {
public:
  AcwRegularWifiMac();
  virtual ~AcwRegularWifiMac();
  
  //@Override
  virtual void DoDispose();
protected:
  // @Override
  virtual void TxOk (const WifiMacHeader &hdr) {
    RegularWifiMac::TxOk(hdr);
  }

  // @Override
  virtual void TxFailed (const WifiMacHeader &hdr) {
    RegularWifiMac::TxFailed(hdr);
  }

private:
  Ptr<Txop> parent_txop_expired_;
};

}

#endif // INCLUDED_ACW_REGULAR_WIFI_MAC_H__

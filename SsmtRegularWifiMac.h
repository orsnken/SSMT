#ifndef INCLUDED_SSMT_REGULAR_WIFI_MAC_H__
#define INCLUDED_SSMT_REGULAR_WIFI_MAC_H__

#include "ns3/regular-wifi-mac.h"

namespace ns3 {

class SsmtRegularWifiMac : public RegularWifiMac {
public:
  SsmtRegularWifiMac();
  virtual ~SsmtRegularWifiMac();
protected:
  // @Override
  virtual void TxOk (const WifiMacHeader &hdr) {
    RegularWifiMac::TxOk(hdr);
  }

  // @Override
  virtual void TxFailed (const WifiMacHeader &hdr) {
    RegularWifiMac::TxFailed(hdr);
  }
};

}

#endif // INCLUDED_SSMT_REGULAR_WIFI_MAC_H__

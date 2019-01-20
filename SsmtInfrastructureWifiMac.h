#ifndef INCLUDED_SSMT_INFRASTRUCTURE_WIFI_MAC_H__
#define INCLUDED_SSMT_INFRASTRUCTURE_WIFI_MAC_H__

#include "SsmtRegularWifiMac.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 * The Wifi MAC high model for a STA or AP in a BSS.
 */
class SsmtInfrastructureWifiMac : public SsmtRegularWifiMac
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  SsmtInfrastructureWifiMac ();
  virtual ~SsmtInfrastructureWifiMac ();

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to) = 0;
  /**
   * Enable or disable QoS support for the device.
   *
   * \param enable whether QoS is supported
   */
  void SetQosSupported (bool enable);

  /**
   * Enable or disable PCF support for the device.
   *
   * \param enable whether PCF is supported
   */
  void SetPcfSupported (bool enable);
  /**
   * Return whether the device supports PCF.
   *
   * \return true if PCF is supported, false otherwise
   */
  bool GetPcfSupported () const;


private:
  /**
   * This Boolean is set \c true iff this WifiMac support PCF
   */
  bool m_pcfSupported;
};

} //namespace ns3


#endif // INCLUDED_SSMT_INFRASTRUCTURE_WIFI_MAC_H__

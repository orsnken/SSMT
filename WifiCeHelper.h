/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#ifndef INCLUDED_WIFI_CE_HELPER_H
#define INCLUDED_WIFI_CE_HELPER_H

#include "ns3/trace-helper.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-mac-helper.h"
#include "ns3/wifi-helper.h"

namespace ns3 {

class WifiNetDevice;
class Node;
class RadiotapHeader;

/**
 * \brief helps to create WifiNetDevice objects with Frame Capture Effect
 *
 * This class can help to create a large set of similar
 * WifiNetDevice objects and to configure a large set of
 * their attributes during creation.
 */
class WifiCeHelper
{
public:
  virtual ~WifiCeHelper ();

  /**
   * Create a Wifi helper in an empty state: all its parameters
   * must be set before calling ns3::WifiCeHelper::Install
   *
   * The default state is defined as being an Adhoc MAC layer with an ARF rate control algorithm
   * and both objects using their default attribute values.
   * By default, configure MAC and PHY for 802.11a.
   */
  WifiCeHelper ();

  /**
   * \param type the type of ns3::WifiRemoteStationManager to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void SetRemoteStationManager (std::string type,
                                std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                                std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                                std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                                std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                                std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                                std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                                std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                                std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param first lower bound on the set of nodes on which a wifi device must be created
   * \param last upper bound on the set of nodes on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  NetDeviceContainer
  virtual Install (const WifiPhyHelper &phy,
                   const WifiMacHelper &mac,
                   NodeContainer::Iterator first,
                   NodeContainer::Iterator last) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param c the set of nodes on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, NodeContainer c) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param node the node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, Ptr<Node> node) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param nodeName the name of node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, std::string nodeName) const;
  /**
   * \param standard the phy standard to configure during installation
   *
   * This method sets standards-compliant defaults for WifiMac
   * parameters such as sifs time, slot time, timeout values, etc.,
   * based on the standard selected.  It results in
   * WifiMac::ConfigureStandard(standard) being called on each
   * installed mac object.
   *
   * The default standard of 802.11a will be applied if SetStandard()
   * is not called.
   *
   * Note that WifiMac::ConfigureStandard () will overwrite certain
   * defaults in the attribute system, so if a user wants to manipulate
   * any default values affected by ConfigureStandard() while using this
   * helper, the user should use a post-install configuration such as
   * Config::Set() on any objects that this helper creates, such as:
   * \code
   * Config::Set ("/NodeList/0/DeviceList/0/$ns3::WifiNetDevice/Mac/Slot", TimeValue (MicroSeconds (slot)));
   * \endcode
   *
   * \sa WifiMac::ConfigureStandard
   * \sa Config::Set
   */
  virtual void SetStandard (WifiPhyStandard standard);

  /**
   * Helper to enable all WifiNetDevice log components with one statement
   */
  static void EnableLogComponents (void);

  /**
  * Assign a fixed random variable stream number to the random variables
  * used by the Phy and Mac aspects of the Wifi models.  Each device in
  * container c has fixed stream numbers assigned to its random variables.
  * The Wifi channel (e.g. propagation loss model) is excluded.
  * Return the number of streams (possibly zero) that
  * have been assigned. The Install() method should have previously been
  * called by the user.
  *
  * \param c NetDeviceContainer of the set of net devices for which the
  *          WifiNetDevice should be modified to use fixed streams
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (NetDeviceContainer c, int64_t stream);


protected:
  ObjectFactory m_stationManager; ///< station manager
  WifiPhyStandard m_standard; ///< wifi standard
};

} //namespace ns3

#endif /* INCLUDED_WIFI_CE_HELPER_H */

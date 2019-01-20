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

#include "ns3/wifi-net-device.h"
#include "ns3/minstrel-wifi-manager.h"
#include "ns3/minstrel-ht-wifi-manager.h"
#include "ns3/ap-wifi-mac.h"
#include "ns3/ampdu-subframe-header.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/radiotap-header.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/wifi-helper.h"

#include "ns3/simple-frame-capture-model.h" 
  // USER DEFINE

#include "WifiCeHelper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiCeHelper");

WifiCeHelper::~WifiCeHelper ()
{
}

WifiCeHelper::WifiCeHelper ()
  : m_standard (WIFI_PHY_STANDARD_80211a)
{
  SetRemoteStationManager ("ns3::ArfWifiManager");
}

void
WifiCeHelper::SetRemoteStationManager (std::string type,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7)
{
  m_stationManager = ObjectFactory ();
  m_stationManager.SetTypeId (type);
  m_stationManager.Set (n0, v0);
  m_stationManager.Set (n1, v1);
  m_stationManager.Set (n2, v2);
  m_stationManager.Set (n3, v3);
  m_stationManager.Set (n4, v4);
  m_stationManager.Set (n5, v5);
  m_stationManager.Set (n6, v6);
  m_stationManager.Set (n7, v7);
}

void
WifiCeHelper::SetStandard (WifiPhyStandard standard)
{
  m_standard = standard;
}

NetDeviceContainer
WifiCeHelper::Install (const WifiPhyHelper &phyHelper,
                     const WifiMacHelper &macHelper,
                     NodeContainer::Iterator first,
                     NodeContainer::Iterator last) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = first; i != last; ++i)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = macHelper.Create ();
      Ptr<WifiPhy> phy = phyHelper.Create (node, device);

      Ptr<SimpleFrameCaptureModel> frameCaptureModel = CreateObject<SimpleFrameCaptureModel>();
        // USER DEFINE
      frameCaptureModel->SetMargin(0.5);
        // USER DEFINE
      phy->SetFrameCaptureModel(frameCaptureModel);
        // USER DEFINE

      mac->SetAddress (Mac48Address::Allocate ());
      mac->ConfigureStandard (m_standard);
      phy->ConfigureStandard (m_standard);
      device->SetMac (mac);
      device->SetPhy (phy);
      device->SetRemoteStationManager (manager);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node=" << node << ", mob=" << node->GetObject<MobilityModel> ());
    }
  return devices;
}

NetDeviceContainer
WifiCeHelper::Install (const WifiPhyHelper &phyHelper,
                     const WifiMacHelper &macHelper, NodeContainer c) const
{
  return Install (phyHelper, macHelper, c.Begin (), c.End ());
}

NetDeviceContainer
WifiCeHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, Ptr<Node> node) const
{
  return Install (phy, mac, NodeContainer (node));
}

NetDeviceContainer
WifiCeHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (phy, mac, NodeContainer (node));
}

void
WifiCeHelper::EnableLogComponents (void)
{
  LogComponentEnable ("AarfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("AarfcdWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("AdhocWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("AmrrWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ApWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("AparfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ArfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("BlockAckAgreement", LOG_LEVEL_ALL);
  LogComponentEnable ("BlockAckCache", LOG_LEVEL_ALL);
  LogComponentEnable ("BlockAckManager", LOG_LEVEL_ALL);
  LogComponentEnable ("CaraWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ConstantRateWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("Txop", LOG_LEVEL_ALL);
  LogComponentEnable ("ChannelAccessManager", LOG_LEVEL_ALL);
  LogComponentEnable ("DsssErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("QosTxop", LOG_LEVEL_ALL);
  LogComponentEnable ("IdealWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("InfrastructureWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("InterferenceHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("MacLow", LOG_LEVEL_ALL);
  LogComponentEnable ("MacRxMiddle", LOG_LEVEL_ALL);
  LogComponentEnable ("MacTxMiddle", LOG_LEVEL_ALL);
  LogComponentEnable ("MinstrelHtWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("MinstrelWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("MpduAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("MsduAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("NistErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("OnoeWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ParfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("RegularWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("RraaWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("RrpaaWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("SpectrumWifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("StaWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("SupportedRates", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiMacQueueItem", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhyStateHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiRadioEnergyModel", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiRemoteStationManager", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiSpectrumPhyInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiSpectrumSignalParameters", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiTxCurrentModel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_ALL);
}

int64_t
WifiCeHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<WifiNetDevice> wifi = DynamicCast<WifiNetDevice> (netDevice);
      if (wifi)
        {
          //Handle any random numbers in the PHY objects.
          currentStream += wifi->GetPhy ()->AssignStreams (currentStream);

          //Handle any random numbers in the station managers.
          Ptr<WifiRemoteStationManager> manager = wifi->GetRemoteStationManager ();
          Ptr<MinstrelWifiManager> minstrel = DynamicCast<MinstrelWifiManager> (manager);
          if (minstrel)
            {
              currentStream += minstrel->AssignStreams (currentStream);
            }

          Ptr<MinstrelHtWifiManager> minstrelHt = DynamicCast<MinstrelHtWifiManager> (manager);
          if (minstrelHt)
            {
              currentStream += minstrelHt->AssignStreams (currentStream);
            }

          //Handle any random numbers in the MAC objects.
          Ptr<WifiMac> mac = wifi->GetMac ();
          Ptr<RegularWifiMac> rmac = DynamicCast<RegularWifiMac> (mac);
          if (rmac)
            {
              PointerValue ptr;
              rmac->GetAttribute ("Txop", ptr);
              Ptr<Txop> txop = ptr.Get<Txop> ();
              currentStream += txop->AssignStreams (currentStream);

              rmac->GetAttribute ("VO_Txop", ptr);
              Ptr<QosTxop> vo_txop = ptr.Get<QosTxop> ();
              currentStream += vo_txop->AssignStreams (currentStream);

              rmac->GetAttribute ("VI_Txop", ptr);
              Ptr<QosTxop> vi_txop = ptr.Get<QosTxop> ();
              currentStream += vi_txop->AssignStreams (currentStream);

              rmac->GetAttribute ("BE_Txop", ptr);
              Ptr<QosTxop> be_txop = ptr.Get<QosTxop> ();
              currentStream += be_txop->AssignStreams (currentStream);

              rmac->GetAttribute ("BK_Txop", ptr);
              Ptr<QosTxop> bk_txop = ptr.Get<QosTxop> ();
              currentStream += bk_txop->AssignStreams (currentStream);

              //if an AP, handle any beacon jitter
              Ptr<ApWifiMac> apmac = DynamicCast<ApWifiMac> (rmac);
              if (apmac)
                {
                  currentStream += apmac->AssignStreams (currentStream);
                }
            }
        }
    }
  return (currentStream - stream);
}

} //namespace ns3

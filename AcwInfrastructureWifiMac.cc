/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "AcwInfrastructureWifiMac.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AcwInfrastructureWifiMac");

NS_OBJECT_ENSURE_REGISTERED (AcwInfrastructureWifiMac);


TypeId
AcwInfrastructureWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AcwInfrastructureWifiMac")
    .SetParent<RegularWifiMac> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("PcfSupported",
                   "This Boolean attribute is set to enable PCF support at this STA",
                   BooleanValue (false),
                   MakeBooleanAccessor (&AcwInfrastructureWifiMac::SetPcfSupported,
                                        &AcwInfrastructureWifiMac::GetPcfSupported),
                   MakeBooleanChecker ())
  ;
  return tid;
}

AcwInfrastructureWifiMac::AcwInfrastructureWifiMac ()
  : m_pcfSupported (0)
{
  NS_LOG_FUNCTION (this);
}

AcwInfrastructureWifiMac::~AcwInfrastructureWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
AcwInfrastructureWifiMac::SetQosSupported (bool enable)
{
  NS_ASSERT_MSG (!(GetPcfSupported () && enable), "QoS cannot be enabled when PCF support is activated (not supported)");
  RegularWifiMac::SetQosSupported (enable);
}

void
AcwInfrastructureWifiMac::SetPcfSupported (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  NS_ASSERT_MSG (!(GetQosSupported () && enable), "PCF cannot be enabled when QoS support is activated (not supported)");
  m_pcfSupported = enable;
  if (m_stationManager != 0)
    {
      m_stationManager->SetPcfSupported (enable);
    }
}

bool
AcwInfrastructureWifiMac::GetPcfSupported () const
{
  return m_pcfSupported;
}

} //namespace ns3

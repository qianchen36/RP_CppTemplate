/**
 * @file    device.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "device.hpp"

namespace device {

/* Devices List */
std::map<uint8_t, DEVICE_c *> DeviceList;

void DEVICE_c::AddDevice(DEVICE_c *dev)
{
  if (dev == nullptr || dev->devID == NULL)
    return;

  DeviceList.insert(std::pair<uint8_t, DEVICE_c *>(dev->devID, dev));
}



void DEVICE_c::DelDevice(DEVICE_c *dev)
{
  if (dev == nullptr || dev->devID == NULL)
    return;

  DeviceList.erase(dev->devID);
}

}

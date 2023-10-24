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



/**
 * @brief  Construct a new device::DEVICE_c object
 * 
 * @retval None
 */
DEVICE_c::DEVICE_c()
{
  devID    = NULL;
  devType  = DEV_UNDEF;
  devComm  = comm::COMM_UNDEF;
  devState = DEV_RESET;

  hComm_ = nullptr;
}



/**
 * @brief Destroy the device::DEVICE_c object
 * 
 * @retval None
 */
DEVICE_c::~DEVICE_c()
{
  if (devID != NULL)
    DelDevice(this);
}



/**
 * @brief 
 * 
 * @param id 
 * @param hComm 
 * @retval None
 */
void DEVICE_c::Init(uint8_t id, comm::COMM_c *hComm)
{
  devID = id;
  devComm = hComm->comType;

  hComm_ = hComm;

  AddDevice(this);

  devState = DEV_OFFLINE;
}



/**
 * @brief  Add a new device to the DeviceList
 * 
 * @param  dev Pointer to the device object
 * @retval None
 */
void DEVICE_c::AddDevice(DEVICE_c *dev)
{
  if (dev == nullptr || dev->devID == NULL)
    return;

  DeviceList.insert(std::pair<uint8_t, DEVICE_c *>(dev->devID, dev));
}



/**
 * @brief  Delete a device from the DeviceList
 * 
 * @param  dev Pointer to the device object
 * @retval None
 */
void DEVICE_c::DelDevice(DEVICE_c *dev)
{
  if (dev == nullptr || dev->devID == NULL)
    return;

  DeviceList.erase(dev->devID);
}



/**
 * @brief  Check status of all devices
 * 
 * @retval None
 */
void DEVICE_Heartbeat(void)
{
  for (auto it = DeviceList.begin(); it != DeviceList.end(); it++)
    it->second->Heartbeat();
}

} // namespace device

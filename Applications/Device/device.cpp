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
 * @brief  Construct a new device::DEV_InitParam_s structure
 * 
 * @return None
 */
_DEV_InitParam::_DEV_InitParam()
{
  devID = NULL;
  hComm = nullptr;
}



/**
 * @brief  Construct a new device::DEVICE_c object
 * 
 * @return None
 */
DEVICE_c::DEVICE_c()
{
  devID    = NULL;
  devType  = DEV_UNDEF;
  devComm  = comm::COMM_UNDEF;
  devState = DEV_RESET;

  hComm_ = nullptr;
  lastHartbeatTime_ = 0;
}



/**
 * @brief Destroy the device::DEVICE_c object
 * 
 * @return None
 */
DEVICE_c::~DEVICE_c()
{
  if (devID != NULL)
    DelDevice(this);
}



/**
 * @brief 
 * 
 * @param  id Set the device ID
 * @param  hComm Set the communication interface
 * @param  pStruct Set the device specific parameters
 * @return None
 */
void DEVICE_c::InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct)
{
  devID = id;
  devComm = hComm->comType;

  hComm_ = hComm;

  AddDevice(this);

  devState = DEV_OFFLINE;
}



/**
 * @brief  Update the device
 * 
 * @return None
 */
void DEVICE_c::UpdateDevice(void)
{
  if (devState == DEV_OFFLINE)
    return;

}



/**
 * @brief  Check the heartbeat of the device

 * @return None
 */
void DEVICE_c::HeartbeatDevice(void)
{
  if (devState == DEV_OFFLINE)
    return;

  if (HAL_GetTick() - lastHartbeatTime_ > 50)   // 50ms
    devState = DEV_OFFLINE;
}



/**
 * @brief  Get the object handle of the device
 * 
 * @return DEVICE_c* Pointer to the device object
 */
DEVICE_c *DEVICE_c::GetObjectHandler(void)
{
  return this;
}



/**
 * @brief  Add a new device to the DeviceList
 * 
 * @param  dev Pointer to the device object
 * @return None
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
 * @return None
 */
void DEVICE_c::DelDevice(DEVICE_c *dev)
{
  if (dev == nullptr || dev->devID == NULL)
    return;

  DeviceList.erase(dev->devID);
}



/**
 * @brief  Trigger heartbeat check of all devices
 * 
 * @return None
 */
void DEVICE_Heartbeat(void)
{
  for (auto it : DeviceList)
    it.second->HeartbeatDevice();

}

} // namespace device

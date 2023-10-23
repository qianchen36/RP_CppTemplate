/**
 * @file    device.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include "main.h"
#include "device_def.hpp"

#include <map>

namespace device {

class DEVICE_c
{
protected:
  void AddDevice(DEVICE_c *dev);
  void DelDevice(DEVICE_c *dev);

public:
  uint8_t         devID;
  DEVICE_Type_e   devType;
  DEVICE_Status_e devState;

};

extern std::map<uint8_t, DEVICE_c *> DeviceList;

}

#endif // __DEVICE_HPP__

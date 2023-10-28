/**
 * @file    device_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __DEVICE_DEF_HPP__
#define __DEVICE_DEF_HPP__

namespace device {

typedef enum
{
  DEV_RESET,

  DEV_OFFLINE,
  DEV_ONLINE,

  DEV_ERROR,

} DEVICE_Status_e;

typedef enum
{
  DEV_UNDEF,

  DEV_RC,
  DEV_IMU,
  DEV_MTR,

} DEVICE_Type_e;

} // namespace device

#endif // __DEVICE_DEF_HPP__
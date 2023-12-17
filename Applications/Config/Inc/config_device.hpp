/**
 * @file    comfig_device.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __COMFIG_DEVICE_HPP__
#define __COMFIG_DEVICE_HPP__

#include "config_def.hpp"

typedef enum
{
  DEV_UNDEF,

  DEV_RC_DR16,

  DEV_CHAS_MTR1,
  DEV_CHAS_MTR2,
  DEV_CHAS_MTR3,
  DEV_CHAS_MTR4,

} DEV_ID_e;

namespace config {

void DeviceInitialize(void);

} // namespace config

#endif // __COMFIG_DEVICE_HPP__

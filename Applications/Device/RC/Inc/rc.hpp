/**
 * @file    rc.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __RC_HPP__
#define __RC_HPP__

#include "rc_def.hpp"
#include "device.hpp"

#include <map>

namespace device {

namespace rc {

class RC_ChData_c
{
protected:
  RC_ChStatus_e UpdateChState(int16_t current);

public:
  RC_ChType_e   chType;
  RC_ChStatus_e chState;
  int16_t       chValue;

  int16_t operator=(const int16_t chValue);

};

class RC_c : public DEVICE_c
{
protected:

public:
  RC_Type_e rcType;
  std::map<uint8_t, RC_ChData_c> rcData;
  // RC_ChData_s *rcData;

  RC_c();
  ~RC_c();

  // void HeartbeatDevice(void) override;
  RC_c *GetObjectHandler(void) override;

};

} // namespace rc

} // namespace device

/* Sub include */
#include "rc_dr16.hpp"

#endif // __RC_HPP__

/**
 * @file    rc.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __RC_HPP__
#define __RC_HPP__

#include "rc_def.hpp"
#include "device.hpp"

#include <map>
#include <vector>

namespace rp {

namespace device {

namespace rc {

typedef struct _RC_InitParam : public _DEV_InitParam
{
  RC_Type_e rcType;

  _RC_InitParam();

} RC_InitParam_s;



class RC_ChData_c
{
friend class RC_c;

protected:
  uint16_t cnt_;
  uint16_t cntMax_;

public:
  RC_ChType_e   chType;
  RC_ChStatus_e chState;
  int16_t       chValue;

  RC_ChData_c();
  ~RC_ChData_c();

  RC_ChData_c operator=(const int16_t value);
  bool operator==(const int16_t value);

};



class RC_c : public DEVICE_c
{
protected:

public:
  RC_Type_e                rcType;
  std::vector<RC_ChData_c> rcData;

  RC_c();
  ~RC_c();

  // void initDevice(DEV_InitParam_s *initParam) override;
  // void HeartbeatDevice(void) override;
};

} // namespace rc

} // namespace device

} // namespace rp

/* Sub include */
#include "rc_dr16.hpp"

#endif // __RC_HPP__

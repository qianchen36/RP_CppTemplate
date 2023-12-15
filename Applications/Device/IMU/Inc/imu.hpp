/**
 * @file    imu.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-24
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __IMU_HPP__
#define __IMU_HPP__

#include "device.hpp"
#include "imu_def.hpp"

#include <map>
#include <vector>

namespace device {

namespace imu {

typedef struct _IMU_InitParam : public _DEV_InitParam
{
  IMU_Type_e imuType;

  float localGravity;
  float refreshRate;

  FUNC_STATE_e useRpySolve;

  _IMU_InitParam();
  
} IMU_InitParam_s;



class IMU_c : public DEVICE_c
{
protected:
  float g_;
  float halfT_;

public:
  IMU_Type_e         imuType;
  std::vector<float> imuData;

  IMU_c();
  ~IMU_c();

  void HeartbeatDevice(void) override;
  IMU_c *GetObjectHandler(void) override;

};

} // namespace imu

} // namespace device

#endif // __IMU_HPP__

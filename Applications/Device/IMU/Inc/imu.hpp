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

namespace rp {

namespace device {

namespace imu {

typedef struct _IMU_InitParam : public _DEV_InitParam
{
  IMU_Type_e imuType;

  float32_t localGravity;
  float32_t refreshRate;

  FUNC_STATE_e useRpySolve;

  _IMU_InitParam();
  
} IMU_InitParam_s;



class IMU_c : public DEVICE_c
{
protected:
  float32_t g_;
  float32_t halfT_;

public:
  IMU_Type_e             imuType;
  std::vector<float32_t> imuData;

  IMU_c();
  ~IMU_c();

  void HeartbeatDevice(void) override;
  IMU_c *GetObjectHandler(void) override;

};

} // namespace imu

} // namespace device

} // namespace rp

#endif // __IMU_HPP__

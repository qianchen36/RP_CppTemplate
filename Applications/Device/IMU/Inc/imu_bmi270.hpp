/**
 * @file    imu_bmi270.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-14
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __IMU_BMI270_HPP__
#define __IMU_BMI270_HPP__

#include "imu.hpp"
#include "bmi270.h"

namespace device {

namespace imu {

typedef struct _IMU_BMI270_InitParam : public _IMU_InitParam
{
  _IMU_BMI270_InitParam();
  
} IMU_BMI270_InitParam_s;



class IMU_BMI270_c : public IMU_c
{
protected:
  bmi2_dev bmi2Dev_;

public:
  IMU_BMI270_c();
  ~IMU_BMI270_c();

  void InitDevice(DEV_InitParam_s *initParam) override;
  void UpdateDevice(void) override;

  IMU_BMI270_c *GetObjectHandler(void) override;

};

} // namespace imu

} // namespace device

#endif // __IMU_BMI270_HPP__

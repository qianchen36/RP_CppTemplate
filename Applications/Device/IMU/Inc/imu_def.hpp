/**
 * @file    imu_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-24
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __IMU_DEF_HPP__
#define __IMU_DEF_HPP__

#include "main.h"
#include "config.hpp"

namespace device {

namespace imu {

typedef enum
{
  IMU_UNDEF,

  IMU_BMI088,
  IMU_BMI270,
  IMU_MPU6050,

} IMU_Type_e;

typedef enum
{
  IMU_DATA_ACC_X,
  IMU_DATA_ACC_Y,
  IMU_DATA_ACC_Z,

  IMU_DATA_GYRO_X,
  IMU_DATA_GYRO_Y,
  IMU_DATA_GYRO_Z,

  IMU_DATA_PITCH,
  IMU_DATA_ROLL,
  IMU_DATA_YAW,

} IMU_DataType_e;

} // namespace imu

} // namespace device

#endif // __IMU_DEF_HPP__

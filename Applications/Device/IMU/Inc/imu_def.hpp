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

namespace device {

namespace imu {

typedef enum
{
  IMU_UNDEF,

  IMU_BMI088,
  IMU_BMI270,
  IMU_MPU6050,

} IMU_Type_e;

// typedef enum
// {
//   MEMS_UNDEF,

// } MEMS_Type_e;

typedef struct
{
  int16_t accX;
  int16_t accY;
  int16_t accZ;

  int16_t gyrX;
  int16_t gyrY;
  int16_t gyrZ;

  uint32_t timeStamp;

} IMU_ImuRawData_s;

// typedef struct
// {  
//   int16_t magX;
//   int16_t magY;
//   int16_t magZ;

// } IMU_CompRawData_s;

typedef struct
{
  float accX;
  float accY;
  float accZ;

  float roll;
  float pitch;
  float yaw;

  float rollSpeed;
  float pitchSpeed;
  float yawSpeed;

} IMU_ImuData_s;

typedef struct
{
  bool use_Compass;       // TODO: Compass support
  bool use_RpySolver;     // TODO: RPY solver support
  bool use_OutputFilter;  // TODO: Output filter support

} IMU_FuncList_s;

} // namespace imu

} // namespace device

#endif // __IMU_DEF_HPP__

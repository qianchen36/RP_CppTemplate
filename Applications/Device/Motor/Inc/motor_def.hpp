/**
 * @file    motor_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MOTOR_DEF_HPP__
#define __MOTOR_DEF_HPP__

#include "main.h"
#include "config.hpp"

namespace rp {

namespace device {

namespace motor {

typedef enum
{
  MTR_UNDEF,

  MTR_GM6020,
  MTR_M3508,
  MTR_M2006,

} MOTOR_Type_e;

typedef enum
{
  MTR_DATA_ANGLE,
  MTR_DATA_SPEED,
  MTR_DATA_VOLTAGE,
  MTR_DATA_CURRENT,
  MTR_DATA_TORQUE,
  MTR_DATA_POSIT,
  MTR_DATA_TEMP,

  MTR_DATA_ERRCODE,

} MOTOR_DataType_e;

typedef enum
{
  MTR_CTRL_UNDEF,

  MTR_CTRL_ANGLE,
  MTR_CTRL_POSIT,
  MTR_CTRL_SPEED,
  MTR_CTRL_TORQUE,
  MTR_CTRL_CURRENT,

  MTR_CTRL_CNT,

} MOTOR_CtrlType_e;

typedef enum
{
  MTR_ERR_NONE,

  MTR_ERR_STALL,

} MOTOR_ErrType_e;

} // namespace motor

} // namespace device

} // namespace rp

#endif // __MOTOR_DEF_HPP__
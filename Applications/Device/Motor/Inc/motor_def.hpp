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

namespace device {

namespace motor {

typedef enum
{
  MOTOR_UNDEF,

  MOTOR_GM6020,
  MOTOR_M3502,

} MOTOR_Type_e;

typedef enum
{
  MOTOR_DATA_UNDEF,

  MOTOR_DATA_ANGLE,
  MOTOR_DATA_SPEED,
  MOTOR_DATA_VOLTAGE,
  MOTOR_DATA_CURRENT,
  MOTOR_DATA_TORQUE,
  MOTOR_DATA_COUNT,
  MOTOR_DATA_TEMP,

} MOTOR_DataType_e;

typedef enum
{
  MOTOR_CTRL_UNDEF,

  MOTOR_CTRL_ANGLE,
  MOTOR_CTRL_SPEED,
  MOTOR_CTRL_TORQUE,
  MOTOR_CTRL_VOLTAGE,
  MOTOR_CTRL_CURRENT,

} MOTOR_CtrlType_e;

typedef struct
{
  int16_t angle;
  int16_t speed;
  int16_t voltage;
  int16_t current;
  int16_t torque;
  int32_t count;
  int16_t temp;

} MOTOR_Data_s;

} // namespace motor

} // namespace device

#endif // __MOTOR_DEF_HPP__
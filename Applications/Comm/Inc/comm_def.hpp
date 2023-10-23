/**
 * @file    comm_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __COMM_DEF_HPP__
#define __COMM_DEF_HPP__

namespace comm {

typedef enum
{
  COMM_RESET,

  COMM_STOP,
  COMM_IDLE,
  COMM_BUSY,

  COMM_ERROR,

} COMM_Status_e;

typedef enum
{
  COMM_UNDEF,

  COMM_CAN,
  COMM_I2C,
  COMM_PWM,
  COMM_SPI,
  COMM_USB,
  COMM_SDIO,
  COMM_UART,

} COMM_Type_e;

} // namespace comm

#endif // __COMM_DEF_HPP__

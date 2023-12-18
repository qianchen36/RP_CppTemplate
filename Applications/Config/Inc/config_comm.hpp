/**
 * @file    config_comm.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CONFIG_COMM_HPP__
#define __CONFIG_COMM_HPP__

#include "config_def.hpp"

typedef enum
{
  COMM_UNDEF,

  COMM_USB,

  COMM_CAN1,
  COMM_CAN2,

  COMM_DBUS,
  COMM_UART1,
  COMM_UART3,
  COMM_UART4,
  COMM_UART5,
  COMM_UART6,

  COMM_SPI1,
  COMM_SPI2,

  COMM_I2C1,
  COMM_I2C2,

  COMM_ADC1,
  COMM_ADC2,
  COMM_ADC3,

  COMM_PWM1,

} COMM_ID_e;

namespace rp {

namespace config {

void CommInitialize(void);

} // namespace config

} // namespace rp

#endif // __CONFIG_COMM_HPP__

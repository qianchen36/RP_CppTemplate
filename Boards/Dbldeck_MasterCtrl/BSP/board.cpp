/**
 * @file    board.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief
 * @version 1.0
 * @date    2023-10-21
 *
 * @copyright SZU-RobotPilots Copyright (c) 2023
 *
 */

#include "board.hpp"
#include "config.hpp"

extern "C" {

void SystemClock_Config(void);

/**
 * @brief  Main program
 * 
 * @return None
 */
int main(void)
{
  /* Syscore Initialize */
  SystemClock_Config();
  HAL_Init();

  /* Hardware Initialize */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART2_UART_Init();

  MX_USB_DEVICE_Init();   // Must be the last

  /* Application Initialize */
  rp::config::ApplicationInitialize();

  return 0;
}

} // extern "C"

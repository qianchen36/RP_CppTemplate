/**
 * @file    board.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-16
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "main.h"

#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "iwdg.h"
#include "sdio.h"
#include "usart.h"
#include "fatfs.h"
#include "usb_device.h"

#define HAL_TIMEBASE_HANLDER htim14

#endif // __BOARD_HPP__
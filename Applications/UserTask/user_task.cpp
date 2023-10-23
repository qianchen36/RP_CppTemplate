/**
 * @file user_task.cpp
 * @author Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "user_task.hpp"

#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"



/**
  * @brief  Function implementing the wakeupTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartWakeupTask(void *argument)
{
  MX_USB_DEVICE_Init();

}
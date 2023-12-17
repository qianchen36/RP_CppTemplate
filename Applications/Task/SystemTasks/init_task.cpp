/**
 * @file    init_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "system_task.hpp"
#include "user_task.hpp"

#include "usb_device.h"

namespace task {

namespace sys_task {

/* Init Task */
TaskHandle_t initTaskHandle;

/**
  * @brief  Function implementing the initTask thread.
  * 
  * @param  argument: Not used
  * @return None
  */
extern "C" void StartInitTask(void *argument)
{
  /* System initialize */
  MX_USB_DEVICE_Init();

  /* Startup system tasks */
  #if USE_TEST_TASK == 1U
  xTaskCreate(StartTestTask, "testTask", 128, NULL, 25, &testTaskHandle);
  #else
  xTaskCreate(StartHeartbeatTask, "heartbeatTask", 128, NULL, 1, &heartbeatTaskHandle)
  #endif

  /* Startup user tasks */
  #if USE_USER_TASK == 1U
  user_task::StartupUserTasks();
  #endif

  /* Delete the initTask */
  vTaskDelete(NULL);
}

} // namespace sys_task

} // namespace task

/**
 * @file    monitor_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-17
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "user_task.hpp"

namespace task {

namespace user_task {

/* Monitor task handle */
TaskHandle_t monitorTaskHandle;

/**
 * @brief  Monitor task entry function
 * 
 * @param  argument Not used
 * @return None
 */
extern "C" void StartMonitorTask(void *argument)
{

  while (1)
  {

    vTaskDelay(1);
  }
}

} // namespace user_task

} // namespace task
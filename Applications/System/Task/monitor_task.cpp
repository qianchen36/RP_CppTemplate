/**
 * @file    monitor_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-18
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "task.hpp"

namespace rp {

namespace system {

namespace task {

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

} // namespace task

} // namespace system

} // namespace rp

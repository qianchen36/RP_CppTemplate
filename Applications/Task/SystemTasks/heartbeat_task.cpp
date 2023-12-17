/**
 * @file    heartbeat_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "system_task.hpp"
#include "device.hpp"
#include "module.hpp"

namespace task {

namespace sys_task {

/* Heartbeat task handle */
TaskHandle_t heartbeatTaskHandle;

/**
  * @brief  Heartbeat task entry function
  * 
  * @param  argument: Not used
  * @return None
  */
extern "C" void StartHeartbeatTask(void *argument)
{
  while (1)
  {
    device::DEVICE_Heartbeat();
    module::MODULE_Heartbeat();

    vTaskDelay(10);
  }

}

} // namespace sys_task

} // namespace task

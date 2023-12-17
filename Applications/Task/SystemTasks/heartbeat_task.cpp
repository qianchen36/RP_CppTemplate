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

namespace task {

namespace sys_task {

TaskHandle_t heartbeatTaskHandle;

/**
  * @brief  Function implementing the heartbeatTask thread.
  * 
  * @param  argument: Not used
  * 
  * @retval None
  */
extern "C" void StartHeartbeatTask(void *argument)
{
  while (1)
  {
    device::DEVICE_Heartbeat();

    vTaskDelay(10);
  }

}

} // namespace sys_task

} // namespace task

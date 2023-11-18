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

osThreadId_t heartbeatTaskHandle;

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

    osDelay(10);
  }

}

} // namespace sys_task

} // namespace task

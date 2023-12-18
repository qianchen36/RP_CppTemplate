/**
 * @file    config_system.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-18
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "config_system.hpp"

#include "system.hpp"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

namespace rp {

namespace config {

void SystemInitialize(void)
{
  /* Creat tasks */
  xTaskCreate(system::task::StartHeartbeatTask, "Heartbeat Task", 256, NULL, 20, &system::task::heartbeatTaskHandle);
  xTaskCreate(system::task::StartMonitorTask, "Monitor Task", 256, NULL, 30, &system::task::monitorTaskHandle);

  /* FreeRTOS Initialize */
  vTaskStartScheduler();  
}

} // namespace config

} // namespace rp
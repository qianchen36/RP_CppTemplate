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
  xTaskCreate(system::task::StartMonitorTask,
              "Monitor Task",
              256, NULL, 30,
              &system::task::monitorTaskHandle);

  xTaskCreate(system::task::StartResetTask,
              "Reset Task",
              512, NULL, 20,
              &system::task::resetTaskHandle);

  xTaskCreate(system::task::StartControlTask,
              "Control Task",
              512, NULL, 10,
              &system::task::controlTaskHandle);

  xTaskCreate(system::task::StartHeartbeatTask,
              "Heartbeat Task",
              256, NULL, 16,
              &system::task::heartbeatTaskHandle);

  /* FreeRTOS Initialize */
  vTaskStartScheduler();  
}

} // namespace config

} // namespace rp
/**
 * @file    reset_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "task.hpp"

namespace rp {

namespace system {

namespace task {

/* Reset task handle */
TaskHandle_t resetTaskHandle;

/**
 * @brief  Reset task entry function
 * 
 * @param  argument Not used
 * @return None
 */
void StartResetTask(void *argument)
{
  /* Task init */

  /* Task loop */
  while (1)
  {
    
    vTaskDelay(1);
  }

}

} // namespace task

} // namespace system

} // namespace rp

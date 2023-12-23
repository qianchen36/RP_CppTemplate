/**
 * @file    control_task.cpp
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

/* Control task handle */
TaskHandle_t controlTaskHandle;

/**
 * @brief  Control task entry function
 * 
 * @param  argument Not used
 * @return None
 */
void StartControlTask(void *argument)
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

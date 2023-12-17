/**
 * @file    test_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-13
 * 
 * @copyright SZU-Robotpilots Copyright (c) 2023
 * 
 */

#include "system_task.hpp"

namespace task {

namespace sys_task {

TaskHandle_t testTaskHandle;

extern "C" void StartTestTask(void *argument)
{
  while (1)
  {

    vTaskDelay(1);
  }

}

} // namespace sys_task

} // namespace task

/**
 * @file    config_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "config_task.hpp"

#include "task.hpp"

namespace config {

void TaskInitialize(void)
{
  xTaskCreate(task::sys_task::StartInitTask, "initTask", 128, NULL, 25, &task::sys_task::initTaskHandle);
}

} // namespace config

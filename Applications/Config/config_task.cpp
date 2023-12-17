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

void UserTaskInitialize(void);

/**
 * @brief  Initialize tasks
 * 
 * @return None
 */
void TaskInitialize(void)
{
  /* Startup system tasks */
  #if USE_TEST_TASK == 1U
  xTaskCreate(task::sys_task::StartTestTask, "testTask", 128, NULL, 25, &task::sys_task::testTaskHandle);
  #else
  xTaskCreate(task::sys_task::StartHeartbeatTask, "heartbeatTask", 128, NULL, 20, &task::sys_task::heartbeatTaskHandle);
  #endif

  /* Startup user tasks */
  #if USE_USER_TASK == 1U
  UserTaskInitialize();
  #endif
}



/**
 * @brief  Initialize user tasks
 * 
 * @return None
 */
void UserTaskInitialize(void)
{
  xTaskCreate(task::user_task::StartMonitorTask, "monitorTask", 256, NULL, 30, &task::user_task::monitorTaskHandle);
}

} // namespace config

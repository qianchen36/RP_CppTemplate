/**
 * @file    system_task.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __SYSTEM_TASK_HPP__
#define __SYSTEM_TASK_HPP__

#include "task.hpp"

namespace task {

namespace sys_task {

/* Heartbeat Task */
extern osThreadId_t heartbeatTaskHandle;
extern "C" void StartHeartbeatTask(void *argument);
const osThreadAttr_t heartbeatTask_attributes = {
  .name = "heartbeatTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

#ifdef USE_TEST_TASK

/* Test Task */
extern osThreadId_t testTaskHandle;
extern "C" void StartTestTask(void *argument);
const osThreadAttr_t testTask_attributes = {
  .name = "testTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

#endif // USE_TEST_TASK

} // namespace sys_task

} // namespace task

#endif // __SYSTEM_TASK_HPP__

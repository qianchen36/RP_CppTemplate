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

extern TaskHandle_t initTaskHandle;
extern "C" void StartInitTask(void *argument);

/* Heartbeat Task */
extern TaskHandle_t heartbeatTaskHandle;
extern "C" void StartHeartbeatTask(void *argument);

#ifdef USE_TEST_TASK

/* Test Task */
extern TaskHandle_t testTaskHandle;
extern "C" void StartTestTask(void *argument);

#endif // USE_TEST_TASK

} // namespace sys_task

} // namespace task

#endif // __SYSTEM_TASK_HPP__

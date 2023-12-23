/**
 * @file    task.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-18
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "system.hpp"

namespace rp {

namespace system {

namespace task {

/* Monitor task */
extern TaskHandle_t monitorTaskHandle;
extern "C" void StartMonitorTask(void *argument);

/* Reset task */
extern TaskHandle_t resetTaskHandle;
extern "C" void StartResetTask(void *argument);

/* Control task */
extern TaskHandle_t controlTaskHandle;
extern "C" void StartControlTask(void *argument);

/* Heartbeat task */
extern TaskHandle_t heartbeatTaskHandle;
extern "C" void StartHeartbeatTask(void *argument);

} // namespace task

} // namespace system

} // namespace rp

#endif // __TASK_HPP__

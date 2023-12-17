/**
 * @file user_task.hpp
 * @author Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date 2023-11-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __USER_TASK_HPP__
#define __USER_TASK_HPP__

#include "task.hpp"

namespace task {

namespace user_task {

/* Monitor Task */
extern TaskHandle_t monitorTaskHandle;
extern "C" void StartMonitorTask(void *argument);

} // namespace user_task

} // namespace task

#endif // __USER_TASK_HPP__

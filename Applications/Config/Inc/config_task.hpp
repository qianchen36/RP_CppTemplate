/**
 * @file    config_task.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-13
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __CONFIG_TASK_HPP__
#define __CONFIG_TASK_HPP__

#include "config_def.hpp"

#define USE_TEST_TASK 1U
#define USE_USER_TASK 0U

namespace config {

void TaskInitialize(void);

} // namespace config

#endif // __CONFIG_TASK_HPP__

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

/**
 * @brief  Startup user tasks
 * 
 * @return None
 * 
 * @note   This function is called in StartInitTask() when USE_USER_TASK was defined,
 *         and it should be implemented by user.
 */
__weak void StartupUserTasks(void)
{
  
  while (1)
  {
    
    vTaskDelay(10);
  }
  
}

} // namespace user_task

} // namespace task

#endif // __USER_TASK_HPP__

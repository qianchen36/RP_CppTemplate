/**
 * @file    controller_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __CONTROLLER_DEF_HPP__
#define __CONTROLLER_DEF_HPP__

namespace algo {

namespace controller {

typedef enum
{
  CTRL_RESET,

  CTRL_IDLE,
  CTRL_BUSY,

  CTRL_ERROR,

} CTRL_Status_e;

typedef enum
{
  CTRL_UNDEF,

  CTRL_PID,

} CTRL_Type_e;

} // namespace controller

} // namespace algo

#endif // __CONTROLLER_DEF_HPP__
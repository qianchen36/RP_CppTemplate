/**
 * @file    algo_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __ALGO_DEF_HPP__
#define __ALGO_DEF_HPP__

namespace rp {

namespace algo {

typedef enum
{
  ALGO_UNDEF,

  ALGO_CTRL,
  ALGO_FL,

} ALGO_Type_e;

typedef enum
{
  ALGO_RESET,

  ALGO_IDLE,
  ALGO_BUSY,

  ALGO_ERROR,

} ALGO_Status_e;

} // namespace algo

} // namespace rp

#endif // __ALGO_DEF_HPP__

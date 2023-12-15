/**
 * @file    controller.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "controller.hpp"

#include <stdarg.h>

namespace algo {

namespace controller {

/**
 * @brief  Construct a new algo::controller::CTRL_InitParam_t struct
 * 
 * @return None
 */
_CTRL_InitParam::_CTRL_InitParam()
{
  algoType = ALGO_CTRL;

  ctrlType = CTRL_UNDEF;
}



/**
 * @brief  Construct a new algo::controller::CONTROLLER_c object
 * 
 * @return None
 */
CONTROLLER_c::CONTROLLER_c()
{
  algoType = ALGO_CTRL;

  ctrlType  = CTRL_UNDEF;
}



/**
 * @brief  Destroy the algo::controller::CONTROLLER_c object
 * 
 * @return None
 */
CONTROLLER_c::~CONTROLLER_c()
{

}



/**
 * @brief  Get the controller handler
 * 
 * @return Pointer of the controller object handler
 */
CONTROLLER_c *CONTROLLER_c::GetObjectHandler(void)
{
  return this;
}

} // namespace controller

} // namespace algo

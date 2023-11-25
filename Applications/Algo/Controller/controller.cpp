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
 * @brief  Construct a new algo::controller::CONTROLLER_c object
 * 
 * @return None
 */
CONTROLLER_c::CONTROLLER_c()
{
  ctrlType  = CTRL_UNDEF;
  ctrlState = CTRL_RESET;
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

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
 * @brief  Initialize the controller
 * 
 * @param  type (CTRL_Type_e) Controller type
 * @param  ... (va_list) Controller parameters
 * 
 * @return None
 */
void CONTROLLER_c::InitController(int type, ...)
{
  ctrlType = (CTRL_Type_e)type;

  va_list args;
  va_start(args, type);

  va_end(args);
  ctrlState = CTRL_IDLE;
}



/**
 * @brief  Update the controller
 * 
 * @param  type (CTRL_Type_e) Controller type
 * @param  ... (va_list) Controller parameters
 */
float CONTROLLER_c::UpdateController(int type, ...)
{
  if (type != ctrlType)
    return 0;

  va_list args;
  va_start(args, type);

  va_end(args);
  return 0;
}



/**
 * @brief  Reset the controller
 * 
 * @return None
 */
void CONTROLLER_c::ResetController(void)
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

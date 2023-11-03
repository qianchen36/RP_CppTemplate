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

CONTROLLER_c::CONTROLLER_c()
{
  ctrlType  = CTRL_UNDEF;
  ctrlState = CTRL_RESET;

  deltaT_ = 1.0f;   // 1ms
}

CONTROLLER_c::~CONTROLLER_c()
{

}

void CONTROLLER_c::InitController(int type, double t, ...)
{
  ctrlType = (CTRL_Type_e)type;
  deltaT_  = t;

  va_list args;
  va_start(args, t);

  va_end(args);
  ctrlState = CTRL_IDLE;
}

void CONTROLLER_c::UpdateController(int type, ...)
{
  if (type != ctrlType)
    return;

  va_list args;
  va_start(args, type);

  va_end(args);
}

void CONTROLLER_c::ResetController(void)
{
  return;
}

CONTROLLER_c *CONTROLLER_c::GetObjectHandler(void)
{
  return this;
}

} // namespace controller

} // namespace algo

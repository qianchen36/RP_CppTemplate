/**
 * @file    rc_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __RC_DEF_HPP__
#define __RC_DEF_HPP__

#include "main.h"
#include "config.hpp"

namespace rp {

namespace device {

namespace rc {

typedef enum
{
  RC_UNDEF,

  RC_DR16,

} RC_Type_e;

typedef enum
{
  RC_CH_UNDEF,

  RC_CH_BUTTON,
  RC_CH_SWITCH,
  RC_CH_SLIDER,
  RC_CH_JOYSTICK,

} RC_ChType_e;

typedef enum
{
  CH_RESET,         // Default state

  CH_PRESSED,
  CH_LONG_PRESSED,

  CH_UP,
  CH_DN,
  
} RC_ChStatus_e;

} // namespace rc

} // namespace device

} // namespace rp

#endif // __RC_DEF_HPP__

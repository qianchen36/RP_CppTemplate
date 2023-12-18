/**
 * @file    rc.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "rc.hpp"

namespace rp {

namespace device {

namespace rc {

/**
 * @brief  Construct a new device::rc::RC_InitParam_s structure
 * 
 * @return None
 */
_RC_InitParam::_RC_InitParam()
{
  rcType = RC_UNDEF;
}



/**
 * @brief  Construct a new device::rc::RC_ChData_c object
 * 
 * @return None
 */
RC_ChData_c::RC_ChData_c()
{
  chType  = RC_CH_UNDEF;
  chState = CH_RESET;
  chValue = 0;

  cnt_    = 0;
  cntMax_ = 100;
}



/**
 * @brief  Destroy the device::rc::RC_ChData_c object
 * 
 * @return None
 */
RC_ChData_c::~RC_ChData_c()
{

}



/**
 * @brief  Operator= overload for RC_ChData_c
 * 
 * @param  value RC channel value
 * @return (RC_ChData_c) RC channel data
 */
RC_ChData_c RC_ChData_c::operator=(const int16_t value)
{
  /* Update channel state */
  switch (chType)
  {
  case RC_CH_BUTTON:    // Button
    if (value)
    {
      cnt_    = (cnt_ >= cntMax_) ? cntMax_ : cnt_ + 1;
      chState = (cnt_ >= cntMax_) ? CH_LONG_PRESSED : CH_PRESSED;
    }
    else
    {
      cnt_    = 0;
      chState = CH_RESET;
    }
    
    break;

  case RC_CH_SWITCH:    // Switch
    chState = CH_RESET;
    break;

  case RC_CH_SLIDER:    // Slider
    chState = CH_RESET;
    break;

  case RC_CH_JOYSTICK:  // Joystick
    chState = value >  50 ? CH_UP : CH_RESET;
    chState = value < -50 ? CH_DN : CH_RESET;
    break;
  
  default:
    chState = CH_RESET;
    break;
  }

  /* Update channel value */
  chValue = value;

  return *this;
}



/**
 * @brief  Operator== overload for RC_ChData_c
 * 
 * @param  value RC channel value
 * @return (bool) True if equal
 */
bool RC_ChData_c::operator==(const int16_t value)
{ return chValue == value; }




/**
 * @brief  Construct a new device::rc::RC_c object
 * 
 * @return None
 */
RC_c::RC_c()
{
  devType = DEV_RC;
  rcType  = RC_UNDEF;
}



/**
 * @brief  Destroy the device::rc::RC_c object
 * 
 * @return None
 */
RC_c::~RC_c()
{

}



/**
 * @brief  Get the handler of RC_c object
 * 
 * @return (RC_c*) Pointer of the RC_c object handler
 */
RC_c *RC_c::GetObjectHandler(void)
{ return this; }

} // namespace rc

} // namespace device

} // namespace rp

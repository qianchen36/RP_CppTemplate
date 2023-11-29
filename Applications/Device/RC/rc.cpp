/**
 * @file    rc.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "rc.hpp"

namespace device {

namespace rc {

int16_t RC_ChData_c::operator=(const int16_t chValue)
{
  return 0;
}

RC_ChStatus_e RC_ChData_c::UpdateChState(int16_t current)
{
  if (chType == RC_CH_UNDEF)
    return CH_RESET;

  return CH_RESET;
}

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

/**
 * @file    fl_ave.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "fl_ave.hpp"

#include <memory>

namespace algo {

namespace filter {

/**
 * @brief  Construct a new algo::filter::_FL_AVE_Initparam struct
 * 
 * @return None
 */
_FL_AVE_Initparam::_FL_AVE_Initparam()
{
  averageSize = 3;
}



/**
 * @brief  Construct a new algo::filter::FL_AVE_c::FL_AVE_c object
 * 
 * @return None
 */
FL_AVE_c::FL_AVE_c()
{
  flType  = FL_AVE;
  flState = FL_RESET;

  averageSize_   = NULL;
  averageBuffer_ = nullptr;
}



/**
 * @brief  Destroy the algo::filter::FL_AVE_c::FL_AVE_c object
 * 
 * @return None
 */
FL_AVE_c::~FL_AVE_c()
{
  if (averageBuffer_ != nullptr)
  {
    delete averageBuffer_;
    averageBuffer_ = nullptr;
  }

}



/**
 * @brief  Initialize the filter
 * 
 * @param  initParam Pointer to the init param
 * @return None
 */
void FL_AVE_c::InitFilter(FL_InitParam_t *initParam)
{
  if (initParam == nullptr)
    return;

  /* Get params */
  initParam_ = new FL_AVE_Initparam_t;
  memcpy(initParam_, initParam, sizeof(FL_AVE_Initparam_t));

  /* Initialize */
  averageSize_   = ((FL_AVE_Initparam_t*)initParam_)->averageSize;
  averageBuffer_ = new float[averageSize_];

  /* Update State */
  flState = FL_IDLE;
}



/**
 * @brief  Update the filter
 * 
 * @param  type (FL_Type_e) Type of filter
 * @param  input (const float &) Input value
 * @return (float) Output value
 */
float FL_AVE_c::UpdateFilter(int type, const float &input)
{
  float output = 0.0f;

  UpdateFilter(type, input, &output);

  return output;
}



/**
 * @brief  Update the filter
 * 
 * @param  type (FL_Type_e) Type of filter
 * @param  input (const float &) Input value
 * @param  output (float *) Pointer to output value
 * @return None
 */
void FL_AVE_c::UpdateFilter(int type, const float &input, float *output)
{
  if (flState != FL_IDLE)
    return;

  flState = FL_BUSY;

  /* Update */
  for (uint8_t i = 0; i < averageSize_ - 1; i++)
    averageBuffer_[i] = averageBuffer_[i + 1];

  averageBuffer_[averageSize_ - 1] = input;

  /* Calculate */
  for (uint8_t i = 0; i < averageSize_; i++)
    *output += averageBuffer_[i];

  *output /= averageSize_;

  flState = FL_IDLE;
}



/**
 * @brief  Reset the filter
 * 
 * @return None
 */
void FL_AVE_c::ResetFilter(void)
{
  if (flState != FL_IDLE)
    return;

  memset(averageBuffer_, 0, averageSize_ * sizeof(float));
}



/**
 * @brief  Get the filter handler
 * 
 * @return (FL_AVE_c *) Pointer to the filter handler
 */
FL_AVE_c *FL_AVE_c::GetObjectHandler(void)
{ return this; }

} // namespace filter

} // namespace algo
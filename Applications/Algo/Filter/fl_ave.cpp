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

namespace rp {

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
void FL_AVE_c::InitAlgo(ALGO_InitParam_s *initParam)
{
  if (initParam == nullptr)
    return;

  /* Get params */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new FL_AVE_Initparam_t;
  memcpy(initParam_, initParam, sizeof(FL_AVE_Initparam_t));

  /* Initialize */
  averageSize_   = ((FL_AVE_Initparam_t*)initParam_)->averageSize;
  averageBuffer_ = new float[averageSize_];

  /* Update State */
  algoState = ALGO_IDLE;
}



/**
 * @brief  Update the filter
 * 
 * @param  input (const float32_t *) Pointer to input value
 * @return (float32_t) Output value
 */
float32_t FL_AVE_c::UpdateAlgo(const float32_t *input)
{
  float32_t output = 0.0f;

  UpdateAlgo(input, &output);

  return output;
}



/**
 * @brief  Update the filter
 * 
 * @param  input (const float32_t *) Pointer to input value
 * @param  output (float32_t *) Pointer to output value
 * @return None
 */
void FL_AVE_c::UpdateAlgo(const float32_t *input, float32_t *output)
{
  if (algoState != ALGO_IDLE)
    return;

  algoState = ALGO_BUSY;

  /* Update */
  for (uint8_t i = 0; i < averageSize_ - 1; i++)
    averageBuffer_[i] = averageBuffer_[i + 1];

  averageBuffer_[averageSize_ - 1] = *input;

  /* Calculate */
  for (uint8_t i = 0; i < averageSize_; i++)
    *output += averageBuffer_[i];

  *output /= averageSize_;

  algoState = ALGO_IDLE;
}



/**
 * @brief  Reset the filter
 * 
 * @return None
 */
void FL_AVE_c::ResetAlgo(void)
{
  if (algoState != ALGO_IDLE)
    return;

  memset(averageBuffer_, 0, averageSize_ * sizeof(float32_t));
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

} // namespace rp

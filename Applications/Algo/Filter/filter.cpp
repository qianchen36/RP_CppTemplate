/**
 * @file    filter.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "filter.hpp"

namespace rp {

namespace algo {

namespace filter {

/**
 * @brief  Construct a new algo::filter::FL_InitParam_t struct
 * 
 * @return None
 */
_FL_InitParam::_FL_InitParam()
{
  algoType = ALGO_FL;

  flType = FL_UNDEF;
}



/**
 * @brief  Construct a new algo::filter::FILTER_c object
 * 
 * @return None
 */
FILTER_c::FILTER_c()
{
  algoType = ALGO_FL;

  flType  = FL_UNDEF;

  initParam_ = nullptr;
}



/**
 * @brief  Destroy the algo::filter::FILTER_c object
 * 
 * @return None
 */
FILTER_c::~FILTER_c()
{
  if (initParam_ != nullptr)
    delete initParam_;

}



/**
 * @brief  Get the filter handler
 * 
 * @return (FILTER_c *) Pointer to the filter handler
 */
FILTER_c *FILTER_c::GetObjectHandler(void)
{ return this; }

} // namespace filter

} // namespace algo

} // namespace rp
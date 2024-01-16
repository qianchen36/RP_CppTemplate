/**
 * @file    filter_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __FILTER_DEF_HPP__
#define __FILTER_DEF_HPP__

#include "config.hpp"

namespace rp {

namespace algo {

namespace filter {

typedef enum
{
  FL_UNDEF,

  FL_AVE,
  FL_KALMAN,

} FL_Type_e;

} // namespace filter

} // namespace algo

} // namespace rp

#endif // __FILTER_DEF_HPP__

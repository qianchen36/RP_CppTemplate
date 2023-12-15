/**
 * @file    filter.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __FILTER_HPP__
#define __FILTER_HPP__

#include "filter_def.hpp"
#include "algo.hpp"

namespace algo {

namespace filter {

typedef struct _FL_InitParam : public _ALGO_InitParam
{
  FL_Type_e flType;

  _FL_InitParam();

} FL_InitParam_t;



class FILTER_c : public ALGO_c
{
protected:
  FL_InitParam_t *initParam_;

public:
  FL_Type_e   flType;

  FILTER_c();
  ~FILTER_c();

  FILTER_c *GetObjectHandler(void) override;

};

} // namespace filter

} // namespace algo

#endif // __FILTER_HPP__

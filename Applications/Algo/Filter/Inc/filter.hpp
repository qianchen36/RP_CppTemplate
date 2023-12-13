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

namespace algo {

namespace filter {

typedef struct _FL_InitParam
{

  _FL_InitParam();

} FL_InitParam_t;



class FILTER_c
{
protected:
  FL_InitParam_t *initParam_;

public:
  FL_Type_e   flType;
  FL_Status_e flState;

  FILTER_c();
  ~FILTER_c();

  virtual void InitFilter(FL_InitParam_t *initParam) = 0;
  virtual float UpdateFilter(int type, const float &input) = 0;
  virtual void UpdateFilter(int type, const float &input, float *output) = 0;
  virtual void ResetFilter(void) = 0;
  virtual FILTER_c *GetObjectHandler(void);

};

} // namespace filter

} // namespace algo

#endif // __FILTER_HPP__

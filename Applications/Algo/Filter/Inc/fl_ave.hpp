/**
 * @file    fl_ave.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-12
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __FL_AVE_HPP__
#define __FL_AVE_HPP__

#include "filter.hpp"

namespace algo {

namespace filter {

typedef struct _FL_AVE_Initparam : public _FL_InitParam
{
  uint8_t  averageSize;

  _FL_AVE_Initparam();

} FL_AVE_Initparam_t;



class FL_AVE_c : public FILTER_c
{
protected:
  uint8_t    averageSize_;
  float32_t *averageBuffer_;

public:
  FL_AVE_c();
  ~FL_AVE_c();

  void InitAlgo(ALGO_InitParam_s *initParam) override;
  float32_t UpdateAlgo(const float32_t *input) override;
  void UpdateAlgo(const float32_t *input, float32_t *output) override;
  void ResetAlgo(void) override;
  FL_AVE_c *GetObjectHandler(void) override;

};

} // namespace filter

} // namespace algo

#endif // __FL_AVE_HPP__

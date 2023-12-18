/**
 * @file    algo.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __ALGO_HPP__
#define __ALGO_HPP__

#include "algo_def.hpp"
#include "arm_math.h"

namespace rp {

namespace algo {

typedef struct _ALGO_InitParam
{
  ALGO_Type_e algoType;

  _ALGO_InitParam();

} ALGO_InitParam_s;



class ALGO_c
{
protected:
  ALGO_InitParam_s *initParam_;

public:
  ALGO_Type_e   algoType;
  ALGO_Status_e algoState;
  float32_t     algoTickRate;   // Unit: Hz

  ALGO_c();
  ~ALGO_c();

  virtual void InitAlgo(ALGO_InitParam_s *initParam);
  virtual float32_t UpdateAlgo(const float32_t *input);
  virtual void UpdateAlgo(const float32_t *input, float32_t *output);
  virtual void ResetAlgo(void);
  virtual ALGO_c *GetObjectHandler(void);

};

} // namespace algo

} // namespace rp

#endif // __ALGO_HPP__

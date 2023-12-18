/**
 * @file    controller.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

#include "controller_def.hpp"
#include "algo.hpp"

namespace rp {

namespace algo {

namespace controller {

typedef struct _CTRL_InitParam : public _ALGO_InitParam
{
  CTRL_Type_e ctrlType;

  _CTRL_InitParam();

} CTRL_InitParam_s;

class CONTROLLER_c : public ALGO_c
{
public:
  CTRL_Type_e   ctrlType;

  CONTROLLER_c();
  ~CONTROLLER_c();

  CONTROLLER_c *GetObjectHandler(void) override;

};

} // namespace controller

} // namespace algo

} // namespace rp

#endif // __CONTROLLER_HPP__

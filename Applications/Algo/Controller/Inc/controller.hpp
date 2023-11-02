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

#include "main.h"
#include "algo.hpp"
#include "controller_def.hpp"

namespace algo {

namespace controller {

class CONTROLLER_c
{
protected:
  float deltaT_;  // Unit: ms

public:
  CTRL_Type_e   ctrlType;
  CTRL_Status_e ctrlState;

  CONTROLLER_c();
  ~CONTROLLER_c();

  virtual void InitController(CTRL_Type_e type, float t, ...);
  virtual void UpdateController(CTRL_Type_e type, ...);
  virtual void ResetController(void);

};

} // namespace controller

} // namespace algo

#endif // __CONTROLLER_HPP__

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
public:
  CTRL_Type_e   ctrlType;
  CTRL_Status_e ctrlState;

  CONTROLLER_c();
  ~CONTROLLER_c();

  virtual void InitController(int type, ...);
  virtual void UpdateController(int type, ...);
  virtual void ResetController(void);
  virtual CONTROLLER_c *GetObjectHandler(void);

};

} // namespace controller

} // namespace algo

#endif // __CONTROLLER_HPP__

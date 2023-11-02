/**
 * @file    ctrl_pid.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __CTRL_PID_HPP__
#define __CTRL_PID_HPP__

#include "controller.hpp"

namespace algo {

namespace controller {

typedef enum
{
  PID_UNDEF,

  PID_POSIT,
  PID_SPEED,
  PID_DELTA,

} CTRL_PID_Type_e;

typedef struct
{
  float Kp;
  float Ki;
  float Kd;

  uint16_t deadBand;
  uint16_t maxOutput;
  uint16_t maxIntegral;

} CTRL_PID_Params_t;

class CTRL_PID_c : public CONTROLLER_c
{
protected:

public:
  CTRL_PID_Type_e   pidType;
  CTRL_PID_Params_t pidParm;

  CTRL_PID_c();
  ~CTRL_PID_c();

  void InitController(CTRL_Type_e type, float t, ...) override;
  void UpdateController(CTRL_Type_e type, ...) override;
  void ResetController(void) override;

};

} // namespace controller

} // namespace algo

#endif // __CTRL_PID_HPP__

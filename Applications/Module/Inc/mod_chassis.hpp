/**
 * @file    mod_chassis.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MOD_CHASSIS_HPP__
#define __MOD_CHASSIS_HPP__

#include "module.hpp"

#include "motor.hpp"

namespace rp {

namespace module {

namespace chassis {

typedef struct _MOD_CHAS_InitParam : public _MOD_InitParam
{
  device::motor::MOTOR_c *hChasMtr_LF;
  device::motor::MOTOR_c *hChasMtr_RF;
  device::motor::MOTOR_c *hChasMtr_LB;
  device::motor::MOTOR_c *hChasMtr_RB;

  FUNC_STATE_e useSpeedLimit;
  int16_t      maxWheelSpeed;

  FUNC_STATE_e useCurrentLimit;
  int16_t      maxWheelCurrent;

  _MOD_CHAS_InitParam();

} MOD_CHAS_InitParam_t;



class MOD_CHASSIS_c : public MODULE_c
{
protected:
  device::motor::MOTOR_c *hChasMtr_LF_;
  device::motor::MOTOR_c *hChasMtr_RF_;
  device::motor::MOTOR_c *hChasMtr_LB_;
  device::motor::MOTOR_c *hChasMtr_RB_;

  void CalcMotorOutput(float32_t vx, float32_t vy, float32_t w, int16_t *output);

public:
  MOD_CHASSIS_c();
  ~MOD_CHASSIS_c();

  void InitModule(MOD_InitParam_t *initParam) override;
  void HeartbeatModule(void) override;
  MOD_CHASSIS_c *GetObjectHandler(void) override;

};

} // namespace chassis

extern chassis::MOD_CHASSIS_c MOD_Chassis;

} // namespace module

} // namespace rp

#endif // __MOD_CHASSIS_HPP__

/**
 * @file    mod_chassis.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MOD_CHASSIS_HPP_
#define _MOD_CHASSIS_HPP_

#include "module.hpp"

#include "motor.hpp"

namespace module {

namespace chassis {

typedef struct _MOD_CHAS_InitParam : public _MOD_InitParam
{
  device::motor::MOTOR_c *mtrHandler[4];
  

  _MOD_CHAS_InitParam();

} MOD_CHAS_InitParam_t;



class MOD_CHASSIS_c : public MODULE_c
{
protected:
  device::motor::MOTOR_c *mtrHandler_[4];

  void CalcMotorOutput(float vx, float vy, float w, int16_t *output);

public:
  MOD_CHASSIS_c();
  ~MOD_CHASSIS_c();

  void InitModule(MOD_InitParam_t *initParam) override;
  void UpdateModule(void) override;
  void HeartbeatModule(void) override;
  MOD_CHASSIS_c *GetObjectHandler(void) override;

};

} // namespace chassis

} // namespace module

#endif // _MOD_CHASSIS_HPP_

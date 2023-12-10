/**
 * @file    mod_chassis.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mod_chassis.hpp"

namespace module {

namespace chassis {

_MOD_CHAS_InitParam::_MOD_CHAS_InitParam()
{
  mtrHandler[0] = nullptr;
  mtrHandler[1] = nullptr;
  mtrHandler[2] = nullptr;
  mtrHandler[3] = nullptr;
}



MOD_CHASSIS_c::MOD_CHASSIS_c()
{

}



MOD_CHASSIS_c::~MOD_CHASSIS_c()
{

}



void MOD_CHASSIS_c::InitModule(MOD_InitParam_t *initParam)
{

}



void MOD_CHASSIS_c::UpdateModule(void)
{

}



void MOD_CHASSIS_c::HeartbeatModule(void)
{

}



MOD_CHASSIS_c *MOD_CHASSIS_c::GetObjectHandler(void)
{ return this; }



void MOD_CHASSIS_c::CalcMotorOutput(float vx, float vy, float w, int16_t *output)
{
  output[0] = mtrHandler_[0]->CalcMotorController(device::motor::MTR_CTRL_SPEED, vx - vy - w);
  output[1] = mtrHandler_[1]->CalcMotorController(device::motor::MTR_CTRL_SPEED, vx + vy - w);
  output[2] = mtrHandler_[2]->CalcMotorController(device::motor::MTR_CTRL_SPEED, vx + vy + w);
  output[3] = mtrHandler_[3]->CalcMotorController(device::motor::MTR_CTRL_SPEED, vx - vy + w);
}

} // namespace chassis

} // namespace module

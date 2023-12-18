/**
 * @file    mod_chassis.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "mod_chassis.hpp"

namespace rp {

namespace module {

/* Export chassis module */
chassis::MOD_CHASSIS_c MOD_Chassis;

namespace chassis {

/**
 * @brief  Construct a new module::chassis::MOD_CHAS_InitParam_t struct
 * 
 * @return None
 */
_MOD_CHAS_InitParam::_MOD_CHAS_InitParam()
{
  mtrHandler[0]   = nullptr;
  mtrHandler[1]   = nullptr;
  mtrHandler[2]   = nullptr;
  mtrHandler[3]   = nullptr;

  useSpeedLimit   = DISABLE;
  maxWheelSpeed   = 5000;

  useCurrentLimit = DISABLE;
  maxWheelCurrent = 5000;
}



/**
 * @brief  Construct a new module::chassis::MOD_CHASSIS_c object
 * 
 * @return None
 */
MOD_CHASSIS_c::MOD_CHASSIS_c()
{
  mtrHandler_[0] = nullptr;
  mtrHandler_[1] = nullptr;
  mtrHandler_[2] = nullptr;
  mtrHandler_[3] = nullptr;
}



/**
 * @brief  Destroy the module::chassis::MOD_CHASSIS_c object
 * 
 * @return None
 */
MOD_CHASSIS_c::~MOD_CHASSIS_c()
{
  if (initParam_ != nullptr)
    delete initParam_;

}



/**
 * @brief  Initialize chassis module
 * 
 * @param  initParam Pointer to init parameter structure
 * @retval None
 */
void MOD_CHASSIS_c::InitModule(MOD_InitParam_t *initParam)
{
  /* Check pointer */
  if (initParam == nullptr)
    return;

  if (initParam->modId == NULL)
    return;

  /* Copy parameters */
  initParam_ = new MOD_CHAS_InitParam_t;
  memcpy(initParam_, initParam, sizeof(MOD_CHAS_InitParam_t));

  auto param = (MOD_CHAS_InitParam_t *)initParam_;

  /* Initialize */
  modId = param->modId;

  mtrHandler_[0] = param->mtrHandler[0];
  mtrHandler_[1] = param->mtrHandler[1];
  mtrHandler_[2] = param->mtrHandler[2];
  mtrHandler_[3] = param->mtrHandler[3];

  /* Update statue */
  modStatus = MOD_OFFLINE;
}



/**
 * @brief  Heartbeat chassis module
 * 
 * @retval None
 */
void MOD_CHASSIS_c::HeartbeatModule(void)
{

}



/**
 * @brief  Get chassis module object handler
 * 
 * @retval (MOD_CHASSIS_c*) Pointer to chassis module object
 */
MOD_CHASSIS_c *MOD_CHASSIS_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  Calculate motor output current
 * 
 * @param  vx Speed in x direction
 * @param  vy Speed in y direction
 * @param  w  Angular speed
 * @param  output  Pointer to output current array
 * @retval None
 */
void MOD_CHASSIS_c::CalcMotorOutput(float32_t vx, float32_t vy, float32_t w, int16_t *output)
{
  /* Check module state */
  if (modStatus == MOD_RESET)
    return;

  /* Get params */
  auto param = (MOD_CHAS_InitParam_t *)initParam_;

  auto maxWheelSpeed   = param->maxWheelSpeed;
  auto maxWheelCurrent = param->maxWheelCurrent;

  /* Conversion factor */
  vx *= 1.0f;
  vy *= 1.0f;
  w  *= 1.0f;

  /* Calculate Inverse Kinematics */
  float32_t targetSpd[4];

  targetSpd[0] = - vx + vy;
  targetSpd[1] = - vx - vy;
  targetSpd[2] =   vx + vy;
  targetSpd[3] =   vx - vy;

  /* Limit target speed */
  if (param->useSpeedLimit)
  {
    float32_t scale = 1.0f;

    for (uint8_t i = 0; i < 4; i ++)
    {
      if (abs(targetSpd[i] + w) > maxWheelSpeed)
      {
        float32_t s = targetSpd[i] / (maxWheelSpeed - w);
        scale = (s < scale) ? s : scale;
      }

    }

    for (uint8_t i = 0; i < 4; i ++)
      targetSpd[i] = targetSpd[i] * scale + w; 

  }

  /* Calculate output current */
  output[0] = mtrHandler_[0]->CalcMotorController(device::motor::MTR_CTRL_SPEED, targetSpd[0]);
  output[1] = mtrHandler_[1]->CalcMotorController(device::motor::MTR_CTRL_SPEED, targetSpd[1]);
  output[2] = mtrHandler_[2]->CalcMotorController(device::motor::MTR_CTRL_SPEED, targetSpd[2]);
  output[3] = mtrHandler_[3]->CalcMotorController(device::motor::MTR_CTRL_SPEED, targetSpd[3]);

  /* Limit output current */
  if (param->useCurrentLimit)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      if (abs(output[i]) > maxWheelCurrent)
        output[i] = (output[i] > 0) ? maxWheelCurrent : - maxWheelCurrent;
    }
    
  }
}

} // namespace chassis

} // namespace module

} // namespace rp

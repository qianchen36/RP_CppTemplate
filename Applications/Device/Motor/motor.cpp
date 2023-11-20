/**
 * @file    motor.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "motor.hpp"

#define ABS(x) ((x > 0) ? (x) : (-x))

namespace device {

namespace motor {

std::map<uint8_t, MOTOR_c *> MotorList;

/**
 * @brief  Construct a new device::motor::MOTOR_c object
 * 
 * @return None
 */
MOTOR_c::MOTOR_c()
{
  devType = DEV_MTR;
  mtrType = MTR_UNDEF;

  encoderRes_ = NULL;

  memset(&mtrData, 0, sizeof(MOTOR_Data_s));
}



/**
 * @brief  Destroy the device::motor::MOTOR_c object
 * 
 * @return None
 */
MOTOR_c::~MOTOR_c()
{
  if (devID != NULL)
    DelMotor(this);
}



/**
 * @brief  Get the angle sum of the motor
 * 
 * @return (int32_t) Angle sum
 */
int32_t MOTOR_c::GetAngleSum(void)
{
  if (encoderRes_ == NULL)
    return 0;

  return (mtrData.angle + mtrData.posit * encoderRes_);
}



/**
 * @brief  Check the heartbeat of the motor
 * 
 * @return None
 */
void MOTOR_c::HeartbeatDevice(void)
{
  if (devState == DEV_OFFLINE)
    return;

  if (HAL_GetTick() - lastHartbeatTime_ > 50)   // 50ms
  {
    devState = DEV_OFFLINE;

    return;
  }
  else
  {
    devState = DEV_ONLINE;

    if (mtrData.errCode != MTR_ERR_NONE)
      devState = DEV_ERROR;
      
  }

}



/**
 * @brief  Get the object handle of the motor
 * 
 * @return MOTOR_c* Pointer to the motor object
 */
MOTOR_c *MOTOR_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  Calculate the motor controller
 * 
 * @param  id Controller ID
 * @param  set Target value
 * @return (float) Controller output
 */
float MOTOR_c::CalcMotorController(uint8_t id, float set)
{
  if (id == NULL || id >= MTR_CTRL_CNT)
    return 0;

  auto target = set;
  
  switch (id)
  {
  case MTR_CTRL_ANGLE:
    target = CalcMotorController(MTR_CTRL_ANGLE, mtrData.angle, target);
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData.speed, target);
    return target;

  case MTR_CTRL_POSIT:
    target = CalcMotorController(MTR_CTRL_POSIT, GetAngleSum(), target);
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData.speed, target);
    return target;

  case MTR_CTRL_SPEED:
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData.speed, target);
    return target;

  case MTR_CTRL_TORQUE:
    target = CalcMotorController(MTR_CTRL_TORQUE, mtrData.torque, target);
    return target;

  case MTR_CTRL_CURRENT:
    target = CalcMotorController(MTR_CTRL_CURRENT, mtrData.current, target);
    return target;
  
  default:
    return 0;
  }

}



/**
 * @brief  Calculate the motor controller
 * 
 * @param  id Controller ID
 * @param  get Feedback value
 * @param  set Target value
 * @return (float) Controller output
 */
float MOTOR_c::CalcMotorController(uint8_t id, float get, float set)
{
  if (id == NULL)
    return 0;

  if (!mtrCtrlList.count(id))
    return 0;

  return mtrCtrlList[id]->UpdateController(algo::controller::CTRL_PID, get, set);
}



/**
 * @brief  Add a motor controller
 * 
 * @param  id Set the controller ID
 * @param  ctrl Handle of the controller
 * 
 * @return None
 */
void MOTOR_c::AddMotorController(uint8_t id, algo::controller::CONTROLLER_c *ctrl)
{
  if (id == NULL || ctrl == nullptr)
    return;

  mtrCtrlList.insert(std::pair<uint8_t, algo::controller::CONTROLLER_c *>(id, ctrl));
}



/**
 * @brief  Delete a motor controller
 * 
 * @param  id ID of the controller
 * 
 * @return None
 */
void MOTOR_c::DelMotorController(uint8_t id)
{
  mtrCtrlList.erase(id);
}



/**
 * @brief  Add a motor to MotorList
 * 
 * @param  mtr Handle of the motor
 * 
 * @return None
 */
void MOTOR_c::AddMotor(MOTOR_c *mtr)
{
  if (mtr == nullptr || mtr->devID == NULL)
    return;

  MotorList.insert(std::pair<uint8_t, MOTOR_c *>(mtr->devID, mtr));

  AddDevice(mtr);
}



/**
 * @brief  Delete a motor from MotorList
 * 
 * @param  mtr Handle of the motor
 * 
 * @return None
 */
void MOTOR_c::DelMotor(MOTOR_c *mtr)
{
  if (mtr == nullptr || mtr->devID == NULL)
    return;

  MotorList.erase(mtr->devID);

  DelDevice(mtr);
}



/**
 * @brief  Update the round count of the motor
 * 
 * @param  curAngle Current angle
 * @param  lstAngle Last angle
 * 
 * @return (int32_t) Round count
 */
int32_t MOTOR_c::Angle2Posit(int16_t curAngle, int16_t lstAngle)
{
  if (encoderRes_ == NULL)
    return 0;

  if (mtrData.posit == 0 && lstAngle == 0)
    return 0;
  
  int16_t error = curAngle - lstAngle;
  int32_t posit = mtrData.posit + error;

  if (ABS(error) > encoderRes_ / 2)
  {
    if (error > 0)
      posit -= encoderRes_;
    else // error < 0
      posit += encoderRes_;
      
  }

  return posit;
}

} // namespace motor

} // namespace device
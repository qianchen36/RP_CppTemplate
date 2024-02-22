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

namespace rp {

namespace device {

namespace motor {

/* Motor List */
std::map<uint8_t, MOTOR_c *> MotorList;

/**
 * @brief  Construct a new device::motor::MTR_InitParam_s structure
 * 
 * @return None
 */
_MTR_InitParam::_MTR_InitParam(void)
{
  devID   = NULL;
  devType = DEV_MTR;
  hComm   = nullptr;

  mtrType = MTR_UNDEF;

  useAngle2Position = DISABLE;
  encoderResolution = 8192;

  useStallDetect  = DISABLE;
  stallDataSource = MTR_DATA_CURRENT;
  maxStallValue   = 5000;
  maxStallCount   = 200;
}



/**
 * @brief  Construct a new device::motor::MOTOR_c object
 * 
 * @return None
 */
MOTOR_c::MOTOR_c()
{
  devType = DEV_MTR;
  mtrType = MTR_UNDEF;
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

    for (auto &it : mtrCtrlList)
      it.second->ResetAlgo();

  }
  else
  {
    devState = DEV_ONLINE;

    StallDetect();

    if (mtrData[MTR_DATA_ERRCODE] != MTR_ERR_NONE)
      devState = DEV_ERROR;

  }

}



/**
 * @brief  Calculate the motor controller
 * 
 * @param  id Controller ID
 * @param  set Target value
 * @return (float32_t) Controller output
 */
float32_t MOTOR_c::CalcMotorController(uint8_t id, float32_t set)
{
  if (id == NULL || id >= MTR_CTRL_CNT)
    return 0;

  auto target = set;
  
  switch (id)
  {
  case MTR_CTRL_ANGLE:
    target = CalcMotorController(MTR_CTRL_ANGLE, mtrData[MTR_DATA_ANGLE], target);
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData[MTR_DATA_SPEED], target);
    return target;

  case MTR_CTRL_POSIT:
    target = CalcMotorController(MTR_CTRL_POSIT, mtrData[MTR_DATA_POSIT], target);
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData[MTR_DATA_SPEED], target);
    return target;

  case MTR_CTRL_SPEED:
    target = CalcMotorController(MTR_CTRL_SPEED, mtrData[MTR_DATA_SPEED], target);
    return target;

  case MTR_CTRL_TORQUE:
    target = CalcMotorController(MTR_CTRL_TORQUE, mtrData[MTR_DATA_TORQUE], target);
    return target;

  case MTR_CTRL_CURRENT:
    target = CalcMotorController(MTR_CTRL_CURRENT, mtrData[MTR_DATA_TORQUE], target);
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
 * @return (float32_t) Controller output
 */
float32_t MOTOR_c::CalcMotorController(uint8_t id, float32_t get, float32_t set)
{
  if (id == NULL || devState == DEV_OFFLINE)
    return 0;

  if (!mtrCtrlList.count(id))
    return 0;

  float32_t input[] = {get, set};

  return mtrCtrlList[id]->UpdateAlgo(input);
}



/**
 * @brief  Add a motor controller
 * 
 * @param  id Set the controller ID
 * @param  ctrl Handle of the controller
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
 * @brief  Check if the motor is stalled
 * 
 * @return None
 */
void MOTOR_c::StallDetect(void)
{
  if (initParam_ == nullptr)
    return;
  
  auto param = (MTR_InitParam_s *)initParam_;

  if (!param->useStallDetect)
    return;

  /* Check stall */
  if (abs(mtrData[param->stallDataSource]) > param->maxStallValue
      && abs(mtrData[MTR_DATA_SPEED]) < 10)
  {
    stallCnt_++;

    if (stallCnt_ >= param->maxStallCount / 10)
    {
      mtrData[MTR_DATA_ERRCODE] = MTR_ERR_STALL;
      stallCnt_ = param->maxStallCount / 10;
    }

  }
  else
  {
    stallCnt_ = 0;
    mtrData[MTR_DATA_ERRCODE] = MTR_ERR_NONE;
  }

}



/**
 * @brief  Update the round count of the motor
 * 
 * @param  curAngle Current angle
 * @param  lstAngle Last angle
 * @return (int32_t) Round count
 */
int32_t MOTOR_c::Angle2Posit(int16_t curAngle, int16_t lstAngle)
{
  if (initParam_ == nullptr)
    return 0;
  
  auto param = (MTR_InitParam_s *)initParam_;

  if (!param->useAngle2Position)
    return 0;

  if (mtrData[MTR_DATA_POSIT] == 0 && lstAngle == 0)
    return 0;
  
  int16_t error = curAngle - lstAngle;
  int32_t posit = mtrData[MTR_DATA_POSIT] + error;

  /* Check zero crossing */
  if (abs(error) > param->encoderResolution / 2)
  {
    if (error > 0)
      posit -= param->encoderResolution;
    else // error < 0
      posit += param->encoderResolution;

  }

  return posit;
}

} // namespace motor

} // namespace device

} // namespace rp

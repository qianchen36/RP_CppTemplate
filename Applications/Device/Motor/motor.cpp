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

namespace device {

namespace motor {

std::map<uint8_t, MOTOR_c *> MotorList;

MOTOR_c::MOTOR_c()
{
  devType   = DEV_MTR;
  motorType = MOTOR_UNDEF;

  memset(&motorData, 0, sizeof(MOTOR_Data_s));
}

MOTOR_c::~MOTOR_c()
{
  if (devID != NULL)
    DelMotor(this);
}

void MOTOR_c::InitMotor(uint8_t id, comm::COMM_c *hComm, MOTOR_Type_e type, ...)
{
  devID     = id;
  devComm   = hComm->comType;
  devState  = DEV_OFFLINE;
  motorType = type;

  hComm_ = hComm;

  AddMotor(this);
}

void MOTOR_c::AddMotorController(uint8_t id, algo::controller::CONTROLLER_c *ctrl)
{
  if (id == NULL || ctrl == nullptr)
    return;

  controllerList_.insert(std::pair<uint8_t, algo::controller::CONTROLLER_c *>(id, ctrl));
}

void MOTOR_c::DelMotorController(uint8_t id)
{
  controllerList_.erase(id);
}

void MOTOR_c::AddMotor(MOTOR_c *mtr)
{
  if (mtr == nullptr || mtr->devID == NULL)
    return;

  MotorList.insert(std::pair<uint8_t, MOTOR_c *>(mtr->devID, mtr));

  AddDevice(mtr);
}

void MOTOR_c::DelMotor(MOTOR_c *mtr)
{
  if (mtr == nullptr || mtr->devID == NULL)
    return;

  MotorList.erase(mtr->devID);

  DelDevice(mtr);
}

} // namespace motor

} // namespace device
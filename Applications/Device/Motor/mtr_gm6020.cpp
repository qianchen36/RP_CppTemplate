/**
 * @file    mtr_gm6020.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-03
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "mtr_gm6020.hpp"

#include <stdarg.h>

namespace device {

namespace motor {

/**
 * @brief  Construct a new device::motor::MTR_GM6020_c object
 * 
 * @return None
 */
MTR_GM6020_c::MTR_GM6020_c()
{
  devType = DEV_MTR;
  mtrType = MTR_GM6020;
  devComm = comm::COMM_CAN;

  encoderRes_ = 8192;
}



/**
 * @brief  Destroy the device::motor::MTR_GM6020_c object
 * 
 * @return None
 */
MTR_GM6020_c::~MTR_GM6020_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelMotor(this);
    ((comm::COMM_CAN_c *)hComm_)->DelCanNode(this);
  }

}



/**
 * @brief  Initialize the GM6020 motor
 * 
 * @param  id (uint8_t) Set the device ID
 * @param  hComm (COMM_c *) Set the Comm interface
 * @param  pStruct (MTR_GM6020_InitParam_s *) GM6020 motor specific parameters
 * @return None
 */
void MTR_GM6020_c::InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct)
{
  /* Check ID */
  if (id == NULL || hComm == nullptr)
    return;

  /* Init params */
  devID       = id;
  hComm_      = hComm;
  canStdID_   = ((MTR_GM6020_InitParam_s *)pStruct)->canReceiveStdID;
  // encoderRes_ = ((MTR_GM6020_InitParam_s *)pStruct)->encoderResolution;

  /* Regist device */
  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  /* Clean up */
  memset(&mtrData, 0, sizeof(MOTOR_Data_s));

  /* Update status */
  devState  = DEV_OFFLINE;
}



/**
 * @brief  Get the object handle of the GM6020 motor
 * 
 * @return MTR_GM6020_c* Pointer to the motor object
 */
MTR_GM6020_c *MTR_GM6020_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  GM6020 motor CAN node receive callback
 * 
 * @param  dataPack CAN data pack
 * @return None
 */
void MTR_GM6020_c::CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack)
{
  /* Check CAN stdID */
  if (canStdID_ == NULL || dataPack->stdID != canStdID_)
    return;

  auto lastAngle = mtrData.angle;

  /* Unpack datapack */
  mtrData.angle     = (dataPack->data[0] << 8) | dataPack->data[1];
  mtrData.speed     = (dataPack->data[2] << 8) | dataPack->data[3];
  mtrData.voltage   =  NULL;
  mtrData.current   = (dataPack->data[4] << 8) | dataPack->data[5];
  mtrData.torque    =  NULL;
  mtrData.count     =  UpdateRoundCount(mtrData.angle, lastAngle);
  mtrData.temp      =  dataPack->data[6];
  lastHartbeatTime_ =  dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;

}

} // namespace motor

} // namespace device

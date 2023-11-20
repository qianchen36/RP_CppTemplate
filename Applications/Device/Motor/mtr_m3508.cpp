/**
 * @file    mtr_m3508.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mtr_m3508.hpp"

namespace device {

namespace motor {

/**
 * @brief  Construct a new device::motor::MTR_M3508_c object
 * 
 * @return None
 */
MTR_M3508_c::MTR_M3508_c()
{
  devType = DEV_MTR;
  mtrType = MTR_M3508;
  devComm = comm::COMM_CAN;

  encoderRes_ = 8192;
}



/**
 * @brief  Destroy the device::motor::MTR_M3508_c object
 * 
 * @return None
 */
MTR_M3508_c::~MTR_M3508_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelMotor(this);
    ((comm::COMM_CAN_c *)hComm_)->DelCanNode(this);
  }

}



/**
 * @brief  Initialize the M3508 motor
 * 
 * @param  id (uint8_t) Set the device ID
 * @param  hComm (COMM_c *) Set the Comm interface
 * @param  pStruct (MTR_M3508_InitParam_s *) M3508 motor specific parameters
 * @return None
 */
void MTR_M3508_c::InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct)
{
  /* Check ID */
  if (id == NULL || hComm == nullptr)
    return;

  /* Init params */
  devID       = id;
  hComm_      = hComm;
  canStdID_   = ((MTR_M3508_InitParam_s *)pStruct)->canReceiveStdID;
  // encoderRes_ = ((MTR_M3508_InitParam_s *)pStruct)->encoderResolution;

  /* Regist device */
  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  /* Clean up */
  memset(&mtrData, 0, sizeof(MOTOR_Data_s));

  /* Update status */
  devState  = DEV_OFFLINE;
}



/**
 * @brief  Get the object handle of the M3508 motor
 * 
 * @return MTR_M3508_c* Pointer to the motor object
 */
MTR_M3508_c *MTR_M3508_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  M3508 motor CAN node receive callback
 * 
 * @param  dataPack CAN data pack
 * @return None
 */
void MTR_M3508_c::CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack)
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

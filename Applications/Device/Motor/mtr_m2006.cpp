/**
 * @file    mtr_m2006.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mtr_m2006.hpp"

#include <stdarg.h>

namespace device {

namespace motor {

/**
 * @brief  Construct a new device::motor::MTR_M2006_c object
 * 
 * @return None
 */
MTR_M2006_c::MTR_M2006_c()
{
  devType = DEV_MTR;
  mtrType = MTR_M2006;
  devComm = comm::COMM_CAN;

  encoderRes_ = 8192;
}



/**
 * @brief  Destroy the device::motor::MTR_M2006_c object
 * 
 * @return None
 */
MTR_M2006_c::~MTR_M2006_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelMotor(this);
    ((comm::COMM_CAN_c *)hComm_)->DelCanNode(this);
  }

}



/**
 * @brief  Initialize the M2006 motor
 * 
 * @param  id (uint8_t) Set the device ID
 * @param  hComm (COMM_c *) Set the Comm interface
 * @param  pStruct (MTR_M2006_InitParam_s *) M2006 motor specific parameters
 * @return None
 */
void MTR_M2006_c::InitDevice(uint8_t id, comm::COMM_c *hComm, ...)
{
  /* Check ID */
  va_list args;
  va_start(args, hComm);

  /* Get args */
  devID = id;
  hComm_ = hComm;

  auto *pStruct = va_arg(args, MTR_M2006_InitParam_s *);
  canStdID_ = pStruct->canReceiveStdID;

  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  memset(&mtrData, 0, sizeof(MOTOR_Data_s));

  /* Clean up */
  va_end(args);
  devState  = DEV_OFFLINE;
}



/**
 * @brief  Get the object handle of the M2006 motor
 * 
 * @return MTR_M2006_c* Pointer to the motor object
 */
MTR_M2006_c *MTR_M2006_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  M2006 motor CAN node receive callback
 * 
 * @param  dataPack CAN data pack
 * @return None
 */
void MTR_M2006_c::CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack)
{
  /* Check CAN stdID */
  if (canStdID_ == NULL || dataPack->stdID != canStdID_)
    return;

  auto lastAngle = mtrData.angle;

  /* Unpack datapack */
  mtrData.angle     = (dataPack->data[0] << 8) | dataPack->data[1];
  mtrData.speed     = (dataPack->data[2] << 8) | dataPack->data[3];
  mtrData.voltage   =  NULL;
  mtrData.current   =  NULL;
  mtrData.torque    = (dataPack->data[4] << 8) | dataPack->data[5];;
  mtrData.count     =  UpdateRoundCount(mtrData.angle, lastAngle);
  mtrData.temp      =  NULL;
  lastHartbeatTime_ =  dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;
}

} // namespace motor

} // namespace device

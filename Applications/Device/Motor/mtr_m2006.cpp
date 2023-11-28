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
void MTR_M2006_c::InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct)
{
  /* Check ID */
  if (id == NULL || hComm == nullptr)
    return;

  /* Init params */
  devID       = id;
  hComm_      = hComm;
  canStdID_   = ((MTR_M2006_InitParam_s *)pStruct)->canReceiveStdID;
  // encoderRes_ = ((MTR_M2006_InitParam_s *)pStruct)->encoderResolution;

  /* Regist device */
  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  /* Update status */
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

  auto lastAngle = mtrData[MTR_DATA_ANGLE];

  /* Unpack datapack */
  mtrData[MTR_DATA_ANGLE]  = (dataPack->data[0] << 8) | dataPack->data[1];
  mtrData[MTR_DATA_SPEED]  = (dataPack->data[2] << 8) | dataPack->data[3];
  mtrData[MTR_DATA_TORQUE] = (dataPack->data[4] << 8) | dataPack->data[5];
  mtrData[MTR_DATA_POSIT]  = Angle2Posit(mtrData[MTR_DATA_ANGLE], lastAngle);
  lastHartbeatTime_        = dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;
}

} // namespace motor

} // namespace device

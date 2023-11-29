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
 * @brief Construct a new device::motor::MTR_M3508_InitParam_s structure
 * 
 */
_MTR_M3508_InitParam::_MTR_M3508_InitParam(void)
{
  mtrType = MTR_M3508;

  canReceiveStdID = NULL;
}



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
 * @param  initParam Pointer to initialization parameter structure
 * @return None
 */
void MTR_M3508_c::InitDevice(DEV_InitParam_s *initParam)
{
  /* Check input pointer */
  if (initParam == nullptr)
    return;

  if (initParam->devID == NULL || initParam->hComm == nullptr)
    return;

  /* Initialize */
  devID   = initParam->devID;

  hComm_      = initParam->hComm;
  initParam_  = initParam;
  canStdID_   = ((MTR_M3508_InitParam_s *)initParam_)->canReceiveStdID;

  /* Regist */
  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  /* Update status */
  devState = DEV_OFFLINE;
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

  auto lastAngle = mtrData[MTR_DATA_ANGLE];

  /* Unpack datapack */
  mtrData[MTR_DATA_ANGLE]   = (dataPack->data[0] << 8) | dataPack->data[1];
  mtrData[MTR_DATA_SPEED]   = (dataPack->data[2] << 8) | dataPack->data[3];
  mtrData[MTR_DATA_CURRENT] = (dataPack->data[4] << 8) | dataPack->data[5];
  mtrData[MTR_DATA_POSIT]   =  Angle2Posit(mtrData[MTR_DATA_ANGLE], lastAngle);
  mtrData[MTR_DATA_TEMP]    =  dataPack->data[6];
  lastHartbeatTime_         =  dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;
}

} // namespace motor

} // namespace device

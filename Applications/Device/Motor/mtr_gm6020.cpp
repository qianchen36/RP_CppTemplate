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

namespace device {

namespace motor {

/**
 * @brief  Construct a new device::motor::MTR_GM6020_InitParam_s structure
 * 
 * @return None
 */
_MTR_GM6020_InitParam::_MTR_GM6020_InitParam(void)
{
  mtrType = MTR_GM6020;

  canReceiveStdID = NULL;
}



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
 * @param  initParam Pointer to initialization parameter structure
 * @return None
 */
void MTR_GM6020_c::InitDevice(DEV_InitParam_s *initParam)
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
  canStdID_   = ((MTR_GM6020_InitParam_s *)initParam_)->canReceiveStdID;

  mtrData[MTR_DATA_ANGLE]   = 0;
  mtrData[MTR_DATA_SPEED]   = 0;
  mtrData[MTR_DATA_CURRENT] = 0;
  mtrData[MTR_DATA_POSIT]   = 0;
  mtrData[MTR_DATA_TEMP]    = 0;
  mtrData[MTR_DATA_ERRCODE] = 0;

  /* Regist */
  AddMotor(this);
  ((comm::COMM_CAN_c *)hComm_)->AddCanNode(this);

  /* Update status */
  devState = DEV_OFFLINE;
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

  auto lastAngle = mtrData[MTR_DATA_ANGLE];

  /* Unpack datapack */
  mtrData[MTR_DATA_ANGLE]   = (int16_t)((dataPack->data[0] << 8) | dataPack->data[1]);
  mtrData[MTR_DATA_SPEED]   = (int16_t)((dataPack->data[2] << 8) | dataPack->data[3]);
  mtrData[MTR_DATA_CURRENT] = (int16_t)((dataPack->data[4] << 8) | dataPack->data[5]);
  mtrData[MTR_DATA_POSIT]   =  Angle2Posit(mtrData[MTR_DATA_ANGLE], lastAngle);
  mtrData[MTR_DATA_TEMP]    =  dataPack->data[6];
  lastHartbeatTime_         =  dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;

}

} // namespace motor

} // namespace device

/**
 * @file    mtr_m3508.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-20
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "mtr_m3508.hpp"

namespace rp {

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
  /* Check pointer */
  if (initParam == nullptr)
    return;

  if (initParam->devID == NULL || initParam->hComm == nullptr)
    return;

  /* Copy parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new MTR_GM6020_InitParam_s;
  memcpy(initParam_, initParam, sizeof(MTR_GM6020_InitParam_s));

  auto param = (MTR_GM6020_InitParam_s *)initParam_;

  /* Initialize */
  devID     = param->devID;
  hComm_    = param->hComm;
  canStdID_ = param->canReceiveStdID;

  mtrData.resize(MTR_DATA_ERRCODE + 1);
  memset(mtrData.data(), 0, sizeof(int16_t) * (MTR_DATA_ERRCODE + 1));

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
  mtrData[MTR_DATA_ANGLE]   = ((int16_t)((dataPack->data[0] << 8) | dataPack->data[1]));
  mtrData[MTR_DATA_SPEED]   = ((int16_t)((dataPack->data[2] << 8) | dataPack->data[3]));
  mtrData[MTR_DATA_CURRENT] = ((int16_t)((dataPack->data[4] << 8) | dataPack->data[5]));
  mtrData[MTR_DATA_POSIT]   =  Angle2Posit(mtrData[MTR_DATA_ANGLE], lastAngle);
  mtrData[MTR_DATA_TEMP]    =  dataPack->data[6];
  lastHartbeatTime_         =  dataPack->timeStamp;

  /* Update device status */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;
}

} // namespace motor

} // namespace device

} // namespace rp

/**
 * @file    imu.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-24
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "imu.hpp"

namespace device {
  
namespace imu {

/**
 * @brief  Construct a new device::imu::DEVICE_IMU_c object
 * 
 * @retval None
 */
DEVICE_IMU_c::DEVICE_IMU_c()
{
  devType = DEV_IMU;

  imuType = IMU_UNDEF;
  imuData = {NULL};
  imuRawData = {NULL};
  imuFuncList = {NULL};

  GetImuRawData_ = nullptr;
}



/**
 * @brief  Destroy the device::imu::DEVICE_IMU_c object
 * 
 * @retval None
 */
DEVICE_IMU_c::~DEVICE_IMU_c()
{

}



/**
 * @brief  Initialize IMU device
 * 
 * @param  type Type of IMU device
 * @param  pFunc Pointer to get IMU raw data function
 * @retval None
 */
void DEVICE_IMU_c::InitImu(IMU_Type_e type, void *pFunc)
{
  if (type == IMU_UNDEF || pFunc == nullptr)
    return;

  imuType = type;
  GetImuRawData_ = (void (*)(comm::COMM_c *, IMU_ImuRawData_s *))pFunc;

  devState = DEV_OFFLINE;
}



/**
 * @brief 
 * 
 * @retval None
 */
void DEVICE_IMU_c::Heartbeat(void)
{
  if (devState == DEV_RESET)
    return;

  if (HAL_GetTick() - imuRawData.timeStamp >= 50)  // 50ms
    devState = DEV_OFFLINE;
  else
    devState = DEV_ONLINE;

}



/**
 * @brief 
 * 
 * @retval None
 */
void DEVICE_IMU_c::Update(void)
{
  auto lastTimeStamp = imuRawData.timeStamp;

  GetImuRawData_(hComm_, &imuRawData);

  if (imuRawData.timeStamp == lastTimeStamp)
    devState = DEV_ERROR;
}

} // namespace imu

} // namespace device

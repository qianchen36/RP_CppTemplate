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
 * @brief  Construct a new device::imu::IMU_c object
 * 
 * @retval None
 */
IMU_c::IMU_c()
{
  devType = DEV_IMU;

  imuType = IMU_UNDEF;
  imuData = {NULL};
  imuRawData = {NULL};
  // compRawData = {NULL};
  imuFuncList = {NULL};

  GetImuRawData_ = nullptr;
  // GetCompRawData_ = nullptr;
}



/**
 * @brief  Destroy the device::imu::IMU_c object
 * 
 * @retval None
 */
IMU_c::~IMU_c()
{

}



/**
 * @brief  Initialize IMU device
 * 
 * @param  type Type of IMU device
 * @param  pFunc Pointer to get IMU raw data function
 * @retval None
 */
void IMU_c::InitImu(IMU_Type_e type, void *pFunc)
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
void IMU_c::Update(void)
{
  GetImuRawData_(hComm_, &imuRawData);

  if (imuRawData.timeStamp <= lastHartbeatTime_)
    devState = DEV_ERROR;
  else
    lastHartbeatTime_ = imuRawData.timeStamp;
}

} // namespace imu

} // namespace device

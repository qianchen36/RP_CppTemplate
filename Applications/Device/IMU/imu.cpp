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

namespace rp {

namespace device {
  
namespace imu {

/**
 * @brief  Construct a new device::imu::IMU_InitParam_s struct
 * 
 * @return None
 */
_IMU_InitParam::_IMU_InitParam()
{
  devType = DEV_IMU;

  imuType = IMU_UNDEF;

  localGravity = 9.8f;
  refreshRate  = 1000.0f;

  useRpySolve = ENABLE;
}



/**
 * @brief  Construct a new device::imu::IMU_c object
 * 
 * @return None
 */
IMU_c::IMU_c()
{
  devType = DEV_IMU;

  imuType = IMU_UNDEF;

}



/**
 * @brief  Destroy the device::imu::IMU_c object
 * 
 * @return None
 */
IMU_c::~IMU_c()
{

}



/**
 * @brief  Heartbeat the IMU device
 * 
 * @return None
 */
void IMU_c::HeartbeatDevice(void)
{

}



/**
 * @brief  Get the IMU handler
 * 
 * @return (IMU_c*) Pointer to the IMU handler
 */
IMU_c *IMU_c::GetObjectHandler(void)
{ return this; }

} // namespace imu

} // namespace device

} // namespace rp

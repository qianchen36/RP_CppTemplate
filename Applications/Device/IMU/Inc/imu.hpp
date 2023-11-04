/**
 * @file    imu.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-24
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __IMU_HPP__
#define __IMU_HPP__

#include "device.hpp"
#include "imu_def.hpp"

#include <map>

namespace device {

namespace imu {

class IMU_c : public DEVICE_c
{
protected:
  std::function<void(comm::COMM_c *, IMU_ImuRawData_s *)> GetImuRawData_;
  // std::function<void(comm::COMM_c *, IMU_CompRawData_s *)> GetCompRawData_;

public:
  IMU_Type_e        imuType;
  IMU_ImuData_s     imuData;
  IMU_ImuRawData_s  imuRawData;
  // IMU_CompRawData_s compRawData;
  IMU_FuncList_s    imuFuncList;

  IMU_c();
  ~IMU_c();

  void UpdateDevice(void) override;
  
  void InitImu(IMU_Type_e type, void *pImuFunc);
  // void InitMems(MEMS_Type_e type, void *pMemsFunc);
};

} // namespace imu

} // namespace device

#endif // __IMU_HPP__

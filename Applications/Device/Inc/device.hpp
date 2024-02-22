/**
 * @file    device.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include "device_def.hpp"
#include "arm_math.h"
#include "comm.hpp"

#include <map>

namespace rp {

namespace device {

typedef struct _DEV_InitParam
{
  uint8_t       devID;
  DEV_Type_e    devType;
  comm::COMM_c *hComm;

  _DEV_InitParam(void);

} DEV_InitParam_s;

/* Device */
class DEVICE_c
{
protected:
  DEV_InitParam_s *initParam_;
  comm::COMM_c    *hComm_;
  uint32_t         lastHartbeatTime_;

  void AddDevice(DEVICE_c *dev);
  void DelDevice(DEVICE_c *dev);

public:
  uint8_t           devID;
  DEV_Type_e        devType;
  DEV_Status_e      devState;

  DEVICE_c();
  ~DEVICE_c();

  virtual void InitDevice(DEV_InitParam_s *initParam);
  virtual void UpdateDevice(void) = 0;
  virtual void HeartbeatDevice(void);

};

extern std::map<uint8_t, DEVICE_c *> DeviceList;

void DEVICE_Heartbeat(void);

} // namespace device

} // namespace rp

/* Sub include */
#include "rc.hpp"
#include "imu.hpp"
#include "motor.hpp"

#endif // __DEVICE_HPP__

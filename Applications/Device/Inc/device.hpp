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
#include "comm.hpp"

#include <map>

namespace device {

typedef struct _DEV_InitParam
{
  uint8_t       devID;
  DEVICE_Type_e devType;
  comm::COMM_c *hComm;

  _DEV_InitParam(void);

} DEV_InitParam_s;



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
  DEVICE_Type_e     devType;
  comm::COMM_Type_e devComm;
  DEVICE_Status_e   devState;

  DEVICE_c();
  ~DEVICE_c();

  virtual void InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct);
  virtual void InitDevice(DEV_InitParam_s *initParam);
  virtual void UpdateDevice(void);
  virtual void HeartbeatDevice(void);
  virtual DEVICE_c *GetObjectHandler(void);

};

extern std::map<uint8_t, DEVICE_c *> DeviceList;

void DEVICE_Heartbeat(void);

} // namespace device

#endif // __DEVICE_HPP__

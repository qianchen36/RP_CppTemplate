/**
 * @file    mtr_m2006.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-19
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MTR_M2006_HPP__
#define __MTR_M2006_HPP__

#include "motor.hpp"
#include "comm_can.hpp"

namespace rp {

namespace device {

namespace motor {

typedef struct _MTR_M2006_InitParam : _MTR_InitParam
{
  uint32_t canReceiveStdID;

  _MTR_M2006_InitParam(void);

} MTR_M2006_InitParam_s;

class MTR_M2006_c : public device::motor::MOTOR_c, public comm::COMM_CAN_Node_c
{
protected:

public:  
  MTR_M2006_c();
  ~MTR_M2006_c();

  void InitDevice(DEV_InitParam_s *initParam) override;
  MTR_M2006_c *GetObjectHandler(void) override;
  void CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack) override;

};

} // namespace motor

} // namespace device

} // namespace rp

#endif // __MTR_M2006_HPP__

/**
 * @file    mtr_m3508.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __MTR_M3508_HPP__
#define __MTR_M3508_HPP__

#include "motor.hpp"
#include "comm_can.hpp"

namespace device {

namespace motor {

typedef struct
{
  // uint16_t encoderResolution;
  uint32_t canReceiveStdID;

} MTR_M3508_InitParam_s;

class MTR_M3508_c : public device::motor::MOTOR_c, public comm::COMM_CAN_Node_c
{
protected:

public:
  MTR_M3508_c();
  ~MTR_M3508_c();

  void InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct) override;
  MTR_M3508_c *GetObjectHandler(void) override;
  void CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack) override;

};

} // namespace motor

} // namespace device

#endif // __MTR_M3508_HPP__

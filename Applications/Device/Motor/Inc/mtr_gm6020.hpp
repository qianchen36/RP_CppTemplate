/**
 * @file    mtr_gm6020.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MTR_GM6020_HPP__
#define __MTR_GM6020_HPP__

#include "motor.hpp"
#include "comm_can.hpp"

namespace device {

namespace motor {

typedef struct
{
  uint16_t encoderResolution;
  uint32_t canReceiveStdID;

} MTR_GM6020_InitParam_s;

class MTR_GM6020_c : public device::motor::MOTOR_c, public comm::COMM_CAN_Node_c
{
private:

public:  
  MTR_GM6020_c();
  ~MTR_GM6020_c();

  void InitDevice(uint8_t id, comm::COMM_c *hComm, ...) override;
  MTR_GM6020_c *GetObjectHandler(void) override;
  void CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack) override;
};

} // namespace motor

} // namespace device

#endif // __MTR_GM6020_HPP__

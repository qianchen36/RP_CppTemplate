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

namespace rp {

namespace device {

namespace motor {

typedef struct _MTR_GM6020_InitParam : _MTR_InitParam
{
  uint32_t canReceiveStdID;

  _MTR_GM6020_InitParam(void);

} MTR_GM6020_InitParam_s;



class MTR_GM6020_c : public device::motor::MOTOR_c, public comm::COMM_CAN_Node_c
{
protected:

public:  
  MTR_GM6020_c();
  ~MTR_GM6020_c();

  void InitDevice(DEV_InitParam_s *initParam) override;
  void CanNode_ReceiveCallback(comm::COMM_CAN_DataPack_s *dataPack) override;
};

} // namespace motor

} // namespace device

} // namespace rp

#endif // __MTR_GM6020_HPP__

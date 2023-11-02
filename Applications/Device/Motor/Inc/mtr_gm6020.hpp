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

class MTR_GM6020_c : public device::motor::MOTOR_c : public comm::COMM_CAN_Node_c
{

};

} // namespace motor

} // namespace device

#endif // __MTR_GM6020_HPP__

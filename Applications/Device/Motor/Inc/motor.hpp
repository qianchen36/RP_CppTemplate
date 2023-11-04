/**
 * @file    motor.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MOTOR_HPP__
#define __MOTOR_HPP__

#include "main.h"
#include "motor_def.hpp"
#include "device.hpp"
#include "controller.hpp"

#include <map>

namespace device {

namespace motor {

class MOTOR_c : public device::DEVICE_c
{
protected:
  std::map<uint8_t, algo::controller::CONTROLLER_c *> controllerList_;

  void AddMotor(MOTOR_c *mtr);
  void DelMotor(MOTOR_c *mtr);

public:
  MOTOR_Type_e motorType;
  MOTOR_Data_s motorData;

  MOTOR_c();
  ~MOTOR_c();

  virtual void InitMotor(uint8_t id, comm::COMM_c *hComm, MOTOR_Type_e type, ...);
  virtual void AddMotorController(uint8_t id, algo::controller::CONTROLLER_c *ctrl);
  virtual void DelMotorController(uint8_t id);
};

extern std::map<uint8_t, MOTOR_c *> MotorList;

} // namespace motor

} // namespace device

#endif // __MOTOR_HPP__
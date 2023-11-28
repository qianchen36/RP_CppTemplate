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

#include "motor_def.hpp"
#include "device.hpp"
#include "controller.hpp"

#include <map>

namespace device {

namespace motor {

typedef struct _MTR_InitParam : _DEV_InitParam
{
  MOTOR_Type_e mtrType;
  uint16_t     encoderResolution;
  FUNC_STATE_e useStallDetect;
  uint16_t     maxStallCount;

  _MTR_InitParam(void);

} MTR_InitParam_s;



class MOTOR_c : public device::DEVICE_c
{
protected:
  uint16_t encoderRes_;
  uint16_t maxStallCnt_;

  void AddMotor(MOTOR_c *mtr);
  void DelMotor(MOTOR_c *mtr);
  int32_t Angle2Posit(int16_t curAngle, int16_t lstAngle);

public:
  MOTOR_Type_e mtrType;
  std::map<uint8_t, int16_t> mtrData;
  std::map<uint8_t, algo::controller::CONTROLLER_c *> mtrCtrlList;

  MOTOR_c();
  ~MOTOR_c();

  void HeartbeatDevice(void) override;
  MOTOR_c *GetObjectHandler(void) override;

  virtual float CalcMotorController(uint8_t id, float set);
  virtual float CalcMotorController(uint8_t id, float get, float set);
  virtual void AddMotorController(uint8_t id, algo::controller::CONTROLLER_c *ctrl);
  virtual void DelMotorController(uint8_t id);
};

extern std::map<uint8_t, MOTOR_c *> MotorList;

} // namespace motor

} // namespace device

#endif // __MOTOR_HPP__

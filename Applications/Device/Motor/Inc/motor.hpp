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
#include <vector>

namespace rp {

namespace device {

namespace motor {

typedef struct _MTR_InitParam : _DEV_InitParam
{
  MOTOR_Type_e     mtrType;

  FUNC_STATE_e     useAngle2Position;
  uint16_t         encoderResolution;

  FUNC_STATE_e     useStallDetect;
  MOTOR_DataType_e stallDataSource;
  uint16_t         maxStallValue;
  uint16_t         maxStallCount;       // Value in ms

  _MTR_InitParam(void);

} MTR_InitParam_s;



class MOTOR_c : public device::DEVICE_c
{
protected:
  uint16_t stallCnt_;

  void AddMotor(MOTOR_c *mtr);
  void DelMotor(MOTOR_c *mtr);
  void StallDetect(void);
  int32_t Angle2Posit(int16_t curAngle, int16_t lstAngle);

public:
  MOTOR_Type_e mtrType;
  std::vector<int32_t> mtrData;
  std::map<uint8_t, algo::controller::CONTROLLER_c *> mtrCtrlList;

  MOTOR_c();
  ~MOTOR_c();

  // void InitDevice(DEV_InitParam_s *initParam) override;
  void HeartbeatDevice(void) override;

  virtual float32_t CalcMotorController(uint8_t id, float32_t set);
  virtual float32_t CalcMotorController(uint8_t id, float32_t get, float32_t set);
  virtual void AddMotorController(uint8_t id, algo::controller::CONTROLLER_c *ctrl);
  virtual void DelMotorController(uint8_t id);
};

extern std::map<uint8_t, MOTOR_c *> MotorList;

} // namespace motor

} // namespace device

} // namespace rp

/* Sub include */
#include "mtr_gm6020.hpp"
#include "mtr_m3508.hpp"
#include "mtr_m2006.hpp"

#endif // __MOTOR_HPP__

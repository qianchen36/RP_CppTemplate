/**
 * @file    ctrl_pid.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __CTRL_PID_HPP__
#define __CTRL_PID_HPP__

#include "controller.hpp"

namespace algo {

namespace controller {

typedef enum
{
  PID_UNDEF,

  PID_SPEED,
  PID_ANGLE,
  PID_POSIT,
  PID_DELTA,

} CTRL_PID_Type_e;

typedef enum
{
  PID_ERRMODE_UNDEF,

  PID_ERRMODE_ENCODE,
  PID_ERRMODE_DEGREE,

} CTRL_PID_ErrMode_e;

typedef enum
{
  LLAST,
  LAST,
  NOW,

} CTRL_PID_DataCnt_e;

typedef struct
{
  CTRL_PID_Type_e pidType;

  float Kp;
  float Ki;
  float Kd;

  uint16_t deadBand;
  uint16_t maxOutput;
  uint16_t maxIntegral;

  CTRL_PID_ErrMode_e errMode;   // For PID_ANGLE only
  uint16_t           errRange;  // For PID_ANGLE in encoder mode only
  int16_t            errOffset; // For PID_ANGLE & PID_POSIT only

} CTRL_PID_Params_t;

typedef struct
{
  float get[3];
  float set[3];
  float err[3];
  float pOut, iOut, dOut;
  float output;

} CTRL_PID_Data_t;

class CTRL_PID_c : public CONTROLLER_c
{
protected:
  CTRL_PID_Data_t pidData_;

  float PID_Calculate(float get, float set);

public:
  CTRL_PID_Type_e   pidType;
  CTRL_PID_Params_t pidParm;

  CTRL_PID_c();
  ~CTRL_PID_c();

  void InitController(int type, ...) override;
  void UpdateController(int type, ...) override;
  void ResetController(void) override;
  CTRL_PID_c *GetObjectHandler(void) override;

};

} // namespace controller

} // namespace algo

#endif // __CTRL_PID_HPP__

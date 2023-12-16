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

typedef struct
{
  float32_t get[3];
  float32_t set[3];
  float32_t err[3];
  float32_t pOut, iOut, dOut;
  float32_t output;

} CTRL_PID_Data_t;

typedef struct _CTRL_PID_InitParam : public _CTRL_InitParam
{
  CTRL_PID_Type_e pidType;

  float32_t Kp;
  float32_t Ki;
  float32_t Kd;

  uint16_t deadBand;
  uint16_t maxOutput;
  uint16_t maxIntegral;

  CTRL_PID_ErrMode_e errMode;   // For PID_ANGLE only
  uint16_t           errRange;  // For PID_ANGLE in encoder mode only
  int16_t            errOffset; // For PID_ANGLE & PID_POSIT only

  _CTRL_PID_InitParam();

} CTRL_PID_InitParam_s;



class CTRL_PID_c : public CONTROLLER_c
{
protected:
  CTRL_PID_Data_t pidData_;

  float32_t PID_Calculate(float32_t get, float32_t set);

public:
  CTRL_PID_Type_e   pidType;

  CTRL_PID_c();
  ~CTRL_PID_c();

  void InitAlgo(ALGO_InitParam_s *initParam) override;
  float32_t UpdateAlgo(const float32_t *input) override;
  void UpdateAlgo(const float32_t *input, float32_t *output) override;
  void ResetAlgo(void) override;
  CTRL_PID_c *GetObjectHandler(void) override;

};

} // namespace controller

} // namespace algo

#endif // __CTRL_PID_HPP__

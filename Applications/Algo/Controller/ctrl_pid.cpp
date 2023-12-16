/**
 * @file    ctrl_pid.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-02
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "ctrl_pid.hpp"

#include <stdarg.h>

namespace algo {

namespace controller {

typedef enum
{
  LLAST,
  LAST,
  NOW,

} CTRL_PID_DataCnt_e;



/**
 * @brief  Construct a new algo::controller::CTRL_PID_InitParam_t struct
 * 
 * @return None
 */
_CTRL_PID_InitParam::_CTRL_PID_InitParam()
{
  ctrlType = CTRL_PID;

  pidType = PID_UNDEF;

  Kp = 0.0f;
  Ki = 0.0f;
  Kd = 0.0f;

  deadBand    = 0;
  maxOutput   = 0;
  maxIntegral = 0;

  errMode   = PID_ERRMODE_UNDEF;
  errRange  = 8192;
  errOffset = 0;
}



/**
 * @brief  Construct a new alog::controller::CTRL_PID_c object
 * 
 * @return None
 */
CTRL_PID_c::CTRL_PID_c()
{
  ctrlType = CTRL_PID;

  pidType = PID_UNDEF;

  memset(&pidData_, 0, sizeof(pidData_));
}



/**
 * @brief  Destroy the alog::controller::CTRL_PID_c object
 * 
 * @return None
 */
CTRL_PID_c::~CTRL_PID_c()
{

}



/**
 * @brief  Initialize the PID controller
 * 
 * @param  initParam Pointer to the PID controller init parameter
 * @return None
 */
void CTRL_PID_c::InitAlgo(ALGO_InitParam_s *initParam)
{
  /* Check ptr */
  if (initParam == nullptr)
    return;

  if (initParam->algoType != ALGO_CTRL)
    return;

  /* Get parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new CTRL_PID_InitParam_s;
  memcpy(initParam_, initParam, sizeof(CTRL_PID_InitParam_s));

  auto param = (CTRL_PID_InitParam_s *)initParam_;

  /* Clean up */
  algoState = ALGO_IDLE;
}



/**
 * @brief  Update the PID controller
 * 
 * @param  input (float32_t *) Pointer to the input value, [0] is feedback value, [1] is target value
 * @return (float32_t) Controller output
 */
float32_t CTRL_PID_c::UpdateAlgo(const float32_t *input)
{
  /* Check state */
  if (algoState != ALGO_IDLE)
    return 0;

  algoState = ALGO_BUSY;

  /* Get args */
  float32_t get = input[0];
  float32_t set = input[1];

  /* Calculate */
  auto rtv = PID_Calculate(get, set);

  /* Clean up */
  algoState = ALGO_IDLE;

  return rtv;
}



/**
 * @brief  Update the PID controller
 * 
 * @param  input (float32_t *) Pointer to the input value, [0] is feedback value, [1] is target value
 * @param  output (float32_t *) Pointer to the output value
 * @return None
 */
void CTRL_PID_c::UpdateAlgo(const float32_t *input, float32_t *output)
{
  /* Check state */
  if (algoState != ALGO_IDLE)
    return;

  algoState = ALGO_BUSY;

  /* Get args */
  float32_t get = input[0];
  float32_t set = input[1];

  /* Calculate */
  *output = PID_Calculate(get, set);

  /* Clean up */
  algoState = ALGO_IDLE;
}



/**
 * @brief  Reset the PID controller
 * 
 * @return None 
 */
void CTRL_PID_c::ResetAlgo(void)
{
  memset(&pidData_, 0, sizeof(pidData_));
}



/**
 * @brief  Get the PID controller object handler
 * 
 * @return Pointer of the PID controller handler
 */
CTRL_PID_c *CTRL_PID_c::GetObjectHandler(void)
{
  return this;
}



/**
 * @brief  Calculate the PID controller output
 * 
 * @param  get Feedback value
 * @param  set Target value
 * @return PID controller output
 */
float32_t CTRL_PID_c::PID_Calculate(float32_t get, float32_t set)
{
  /* Check type & state */
  if (pidType == PID_UNDEF)
    return 0.0f;

  auto param = (CTRL_PID_InitParam_s *)initParam_;

  switch (pidType)
  {
  case PID_SPEED: // PID in speed mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get;
    pidData_.set[NOW] = set;
    pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];

    /* Is in deadband */
    if (param->deadBand != 0 && abs(pidData_.err[NOW]) < param->deadBand)
      return 0.0f;

    /* PID calculate */
    pidData_.pOut = param->Kp * pidData_.err[NOW];
    pidData_.iOut += param->Ki * pidData_.err[NOW];
    pidData_.dOut = param->Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (param->maxIntegral != 0 && abs(pidData_.iOut) > param->maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? param->maxIntegral : -param->maxIntegral;

    break;

  case PID_ANGLE: // PID in angle mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get - param->errOffset;
    pidData_.set[NOW] = set - param->errOffset;

    /* Get error value */
    if (param->errMode == PID_ERRMODE_UNDEF)
      return 0.0f;

    if (param->errMode == PID_ERRMODE_ENCODE)
    {
      pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];
      
      if (pidData_.err[NOW] > param->errRange / 2)
        pidData_.err[NOW] -= param->errRange;
      else if (pidData_.err[NOW] < -param->errRange / 2)
        pidData_.err[NOW] += param->errRange;
    }

    if (param->errMode == PID_ERRMODE_DEGREE)
    {
      pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];
      
      if (pidData_.err[NOW] > 180)
        pidData_.err[NOW] -= 360;
      else if (pidData_.err[NOW] < -180)
        pidData_.err[NOW] += 360;
    }

    /* Is in deadband */
    if (param->deadBand != 0 && abs(pidData_.err[NOW]) < param->deadBand)
      return 0.0f;
    
    /* PID calculate */
    pidData_.pOut = param->Kp * pidData_.err[NOW];
    pidData_.iOut += param->Ki * pidData_.err[NOW];
    pidData_.dOut = param->Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (param->maxIntegral != 0 && abs(pidData_.iOut) > param->maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? param->maxIntegral : -param->maxIntegral;

    break;

  case PID_POSIT: // PID in position mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get - param->errOffset;
    pidData_.set[NOW] = set - param->errOffset;
    pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];

    /* Is in deadband */
    if (param->deadBand != 0 && abs(pidData_.err[NOW]) < param->deadBand)
      return 0.0f;

    /* PID calculate */
    pidData_.pOut =  param->Kp * pidData_.err[NOW];
    pidData_.iOut += param->Ki * pidData_.err[NOW];
    pidData_.dOut =  param->Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (param->maxIntegral != 0 && abs(pidData_.iOut) > param->maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? param->maxIntegral : -param->maxIntegral;
    
    break;

  case PID_DELTA: // PID in delta mode
    // TODO: PID_DELTA
    break;
  
  default:
    break;
  }

  /* Calculate output */
  pidData_.output = pidData_.pOut + pidData_.iOut + pidData_.dOut;

  /* Limit output */
  if (param->maxOutput != 0 && abs(pidData_.output) > param->maxOutput)
    pidData_.output = (pidData_.output > 0) ? param->maxOutput : -param->maxOutput;

  /* Update data record */
  pidData_.get[LLAST] = pidData_.get[LAST];
  pidData_.set[LLAST] = pidData_.set[LAST];
  pidData_.err[LLAST] = pidData_.err[LAST];
  pidData_.get[LAST] = pidData_.get[NOW];
  pidData_.set[LAST] = pidData_.set[NOW];
  pidData_.err[LAST] = pidData_.err[NOW];

  return pidData_.output;
}

} // namespace controller

} // namespace algo

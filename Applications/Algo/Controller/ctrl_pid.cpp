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

#define ABS(x) ((x > 0) ? (x) : (-x))

namespace algo {

namespace controller {

/**
 * @brief  Construct a new alog::controller::CTRL_PID_c object
 * 
 * @return None
 */
CTRL_PID_c::CTRL_PID_c()
{
  pidType = PID_UNDEF;

  memset(&pidParm, 0, sizeof(pidParm));
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
 * @param  type (CTRL_Type_e) Controller type
 * @param  sParm (CTRL_PID_Params_t *) PID controller parameters structure
 * 
 * @return None
 */
void CTRL_PID_c::InitController(int type, ...)
{
  /* Check type */
  if (type != CTRL_PID)
    return;

  /* Get args */
  va_list args;
  va_start(args, type);

  auto sParm = va_arg(args, CTRL_PID_Params_t *);

  pidType = sParm->pidType;
  pidParm = *sParm;
  // memcpy(&pidParm, sParm, sizeof(pidParm));

  /* Clean up */
  va_end(args);
  ctrlState = CTRL_IDLE;
}



/**
 * @brief  Update the PID controller
 * 
 * @param  type (CTRL_Type_e) Controller type
 * @param  get (float) Get value
 * @param  set (float) Set value
 * 
 * @return None
 */
void CTRL_PID_c::UpdateController(int type, ...)
{
  /* Check type & state */
  if (type != CTRL_PID || ctrlState != CTRL_IDLE)
    return;

  ctrlState = CTRL_BUSY;

  /* Get args */
  va_list args;
  va_start(args, type);

  float get = va_arg(args, double);
  float set = va_arg(args, double);

  /* Calculate */
  PID_Calculate(get, set);

  /* Clean up */
  va_end(args);
  ctrlState = CTRL_IDLE;
}



/**
 * @brief  Reset the PID controller
 * 
 * @retval None 
 */
void CTRL_PID_c::ResetController(void)
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
 * 
 * @return PID controller output
 */
float CTRL_PID_c::PID_Calculate(float get, float set)
{
  /* Check type & state */
  if (pidType == PID_UNDEF || ctrlState != CTRL_IDLE)
    return 0.0f;

  switch (pidType)
  {
  case PID_SPEED: // PID in speed mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get;
    pidData_.set[NOW] = set;
    pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];

    /* Is in deadband */
    if (pidParm.deadBand != 0 && ABS(pidData_.err[NOW]) < pidParm.deadBand)
      return 0.0f;

    /* PID calculate */
    pidData_.pOut = pidParm.Kp * pidData_.err[NOW];
    pidData_.iOut += pidParm.Ki * pidData_.err[NOW];
    pidData_.dOut = pidParm.Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (pidParm.maxIntegral != 0 && ABS(pidData_.iOut) > pidParm.maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? pidParm.maxIntegral : -pidParm.maxIntegral;

    break;

  case PID_ANGLE: // PID in angle mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get - pidParm.errOffset;
    pidData_.set[NOW] = set - pidParm.errOffset;

    /* Get error value */
    if (pidParm.errMode == PID_ERRMODE_UNDEF)
      return 0.0f;

    if (pidParm.errMode == PID_ERRMODE_ENCODE)
    {
      pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];
      
      if (pidData_.err[NOW] > pidParm.errRange / 2)
        pidData_.err[NOW] -= pidParm.errRange;
      else if (pidData_.err[NOW] < -pidParm.errRange / 2)
        pidData_.err[NOW] += pidParm.errRange;
    }

    if (pidParm.errMode == PID_ERRMODE_DEGREE)
    {
      pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];
      
      if (pidData_.err[NOW] > 180)
        pidData_.err[NOW] -= 360;
      else if (pidData_.err[NOW] < -180)
        pidData_.err[NOW] += 360;
    }

    /* Is in deadband */
    if (pidParm.deadBand != 0 && ABS(pidData_.err[NOW]) < pidParm.deadBand)
      return 0.0f;
    
    /* PID calculate */
    pidData_.pOut = pidParm.Kp * pidData_.err[NOW];
    pidData_.iOut += pidParm.Ki * pidData_.err[NOW];
    pidData_.dOut = pidParm.Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (pidParm.maxIntegral != 0 && ABS(pidData_.iOut) > pidParm.maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? pidParm.maxIntegral : -pidParm.maxIntegral;

    break;

  case PID_POSIT: // PID in position mode
    /* Get feedback & target value */
    pidData_.get[NOW] = get - pidParm.errOffset;
    pidData_.set[NOW] = set - pidParm.errOffset;
    pidData_.err[NOW] = pidData_.set[NOW] - pidData_.get[NOW];

    /* Is in deadband */
    if (pidParm.deadBand != 0 && ABS(pidData_.err[NOW]) < pidParm.deadBand)
      return 0.0f;

    /* PID calculate */
    pidData_.pOut = pidParm.Kp * pidData_.err[NOW];
    pidData_.iOut += pidParm.Ki * pidData_.err[NOW];
    pidData_.dOut = pidParm.Kd * (pidData_.err[NOW] - pidData_.err[LAST]);

    /* Limit integral output */
    if (pidParm.maxIntegral != 0 && ABS(pidData_.iOut) > pidParm.maxIntegral)
      pidData_.iOut = (pidData_.iOut > 0) ? pidParm.maxIntegral : -pidParm.maxIntegral;
    
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
  if (pidParm.maxOutput != 0 && ABS(pidData_.output) > pidParm.maxOutput)
    pidData_.output = (pidData_.output > 0) ? pidParm.maxOutput : -pidParm.maxOutput;

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

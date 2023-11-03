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

namespace algo {

namespace controller {

CTRL_PID_c::CTRL_PID_c()
{
  pidType = PID_UNDEF;

  memset(&pidParm, 0, sizeof(pidParm));
}

CTRL_PID_c::~CTRL_PID_c()
{

}

} // namespace controller

} // namespace algo

/**
 * @file    test_task.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-13
 * 
 * @copyright SZU-Robotpilots Copyright (c) 2023
 * 
 */

#include "system_task.hpp"

#include "comm_can.hpp"
#include "comm_uart.hpp"
#include "ctrl_pid.hpp"
#include "rc_dr16.hpp"
#include "mtr_m2006.hpp"
#include "mtr_m3508.hpp"

comm::COMM_CAN_c             comm_CAN;
comm::COMM_UART_c            comm_UART;
device::motor::MTR_M2006_c   mtr_1;
device::motor::MTR_M2006_c   mtr_2;
device::motor::MTR_M2006_c   mtr_3;
device::motor::MTR_M2006_c   mtr_4;
device::motor::MTR_M2006_c   mtr_5;
device::motor::MTR_M3508_c   mtr_6;
device::motor::MTR_M3508_c   mtr_7;
device::rc::RC_DR16_c        rc_dr16;

namespace task {

namespace sys_task {

osThreadId_t testTaskHandle;

extern "C" void StartTestTask(void *argument)
{
  /* Comm init */
  comm::COMM_CAN_InitParam_s sCanConfig = {
    .FilterIdHigh         = 0x0000,
    .FilterIdLow          = 0x0000,
    .FilterMaskIdHigh     = 0x0000,
    .FilterMaskIdLow      = 0x0000,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterBank           = 0,
    .FilterMode           = CAN_FILTERMODE_IDMASK,
    .FilterScale          = CAN_FILTERSCALE_32BIT,
    .FilterActivation     = ENABLE,
    .SlaveStartFilterBank = 14,
  };
  comm_CAN.InitComm(1, &hcan1, &sCanConfig);
  comm_CAN.Start();

  comm::COMM_UART_InitParam_s sUartConfig = {
    .useAutoReceive = ENABLE,
    .rxBufferSize   = 200,
    .rxBufferCount  = 2,
    .useAutoTransmit= DISABLE,
  };
  comm_UART.InitComm(2, &huart2, &sUartConfig);
  comm_UART.Start();

  /* RC init */
  rc_dr16.InitDevice(1, &comm_UART, nullptr);

  /* Mtr init */
  device::motor::MTR_M2006_InitParam_s sMtr01Config;
  sMtr01Config.devID             = 1;
  sMtr01Config.hComm             = &comm_CAN;
  sMtr01Config.canReceiveStdID   = 0x201;
  sMtr01Config.useAngle2Position = ENABLE;
  sMtr01Config.useStallDetect    = ENABLE;
  mtr_1.InitDevice(&sMtr01Config);

  device::motor::MTR_M2006_InitParam_s sMtr02Config;
  sMtr02Config.devID             = 2;
  sMtr02Config.hComm             = &comm_CAN;
  sMtr02Config.canReceiveStdID   = 0x202;
  sMtr02Config.useAngle2Position = ENABLE;
  sMtr02Config.useStallDetect    = ENABLE;
  mtr_2.InitDevice(&sMtr02Config);

  device::motor::MTR_M2006_InitParam_s sMtr03Config;
  sMtr03Config.devID             = 3;
  sMtr03Config.hComm             = &comm_CAN;
  sMtr03Config.canReceiveStdID   = 0x203;
  sMtr03Config.useAngle2Position = ENABLE;
  sMtr03Config.useStallDetect    = ENABLE;
  mtr_3.InitDevice(&sMtr03Config);

  device::motor::MTR_M2006_InitParam_s sMtr04Config;
  sMtr04Config.devID             = 4;
  sMtr04Config.hComm             = &comm_CAN;
  sMtr04Config.canReceiveStdID   = 0x204;
  sMtr04Config.useAngle2Position = ENABLE;
  sMtr04Config.useStallDetect    = ENABLE;
  mtr_4.InitDevice(&sMtr04Config);

  device::motor::MTR_M2006_InitParam_s sMtr05Config;
  sMtr05Config.devID             = 5;
  sMtr05Config.hComm             = &comm_CAN;
  sMtr05Config.canReceiveStdID   = 0x205;
  sMtr05Config.useAngle2Position = ENABLE;
  sMtr05Config.useStallDetect    = ENABLE;
  mtr_5.InitDevice(&sMtr05Config);

  device::motor::MTR_M3508_InitParam_s sMtr06Config;
  sMtr06Config.devID             = 6;
  sMtr06Config.hComm             = &comm_CAN;
  sMtr06Config.canReceiveStdID   = 0x206;
  sMtr06Config.useAngle2Position = ENABLE;
  sMtr06Config.useStallDetect    = ENABLE;
  mtr_6.InitDevice(&sMtr06Config);

  device::motor::MTR_M3508_InitParam_s sMtr07Config;
  sMtr07Config.devID             = 7;
  sMtr07Config.hComm             = &comm_CAN;
  sMtr07Config.canReceiveStdID   = 0x207;
  sMtr07Config.useAngle2Position = ENABLE;
  sMtr07Config.useStallDetect    = ENABLE;
  mtr_7.InitDevice(&sMtr07Config);

  while (1)
  {
    device::DEVICE_Heartbeat();

    osDelay(1);
  }

}

} // namespace sys_task

} // namespace task

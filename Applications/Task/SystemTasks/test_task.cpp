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
#include "ctrl_pid.hpp"
#include "mtr_gm6020.hpp"
#include "mtr_m2006.hpp"

uint8_t FLAG_softReset = 0;
comm::COMM_CAN_c comm_CAN;
algo::controller::CTRL_PID_c ctrl_GM6020;
device::motor::MTR_GM6020_c mtr_GM6020;
algo::controller::CTRL_PID_c ctrl_M2006;
device::motor::MTR_M2006_c mtr_M2006;

namespace task {

namespace sys_task {

osThreadId_t testTaskHandle;

extern "C" void StartTestTask(void *argument)
{
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

  algo::controller::CTRL_PID_Params_t sCtrlConfig = {
    .pidType = algo::controller::PID_SPEED,
    .Kp = 0.0f,
    .Ki = 10.0f,
    .Kd = 0.0f,
    .deadBand = 0,
    .maxOutput = 20000,
    .maxIntegral = 20000,
  };
  ctrl_GM6020.InitController(1, &sCtrlConfig);

  device::motor::MTR_GM6020_InitParam_s sGM6020Config = {
    // .encoderResolution = 8192,
    .canReceiveStdID   = 0x205,
  };
  mtr_GM6020.InitDevice(1, &comm_CAN, &sGM6020Config);
  mtr_GM6020.AddMotorController(device::motor::MTR_CTRL_SPEED, &ctrl_GM6020);

  device::motor::MTR_M2006_InitParam_s sM2006Config = {
    // .encoderResolution = 8192,
    .canReceiveStdID   = 0x201,
  };
  mtr_M2006.InitDevice(2, &comm_CAN, &sM2006Config);

  while (1)
  {
    mtr_GM6020.HeartbeatDevice();
    mtr_M2006.HeartbeatDevice();

    uint8_t data[8] = {0};
    uint16_t target = mtr_GM6020.CalcMotorController(device::motor::MTR_CTRL_SPEED, 100);
    data[0] = target >> 8;
    data[1] = target;
    comm_CAN.Transmit(comm::COMM_CAN, 0x1FF, data);

    osDelay(1);
  }

}

} // namespace sys_task

} // namespace task
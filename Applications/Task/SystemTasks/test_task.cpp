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
#include "mtr_gm6020.hpp"

uint8_t FLAG_softReset = 0;
comm::COMM_CAN_c comm_CAN;
device::motor::MTR_GM6020_c mtr_GM6020;

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

  device::motor::MTR_GM6020_InitParam_s sGM6020Config = {
    .encoderResolution = 8192,
    .canReceiveStdID   = 0x206,
  };
  mtr_GM6020.InitDevice(1, &comm_CAN, &sGM6020Config);

  comm::COMM_CAN_c comm[5];

  while (1)
  {
    mtr_GM6020.HeartbeatDevice();

    osDelay(1);
  }

}

} // namespace sys_task

} // namespace task
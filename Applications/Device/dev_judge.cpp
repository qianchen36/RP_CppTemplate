/**
 * @file    dev_judge.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "dev_judge.hpp"

namespace rp {

namespace device {

/* Judge communicat frame header */
typedef struct __packed _DEV_JUDGE_FrameHeader
{
  uint8_t  SOF = 0xA5;
  uint16_t LEN;
  uint8_t  SEQ;
  uint8_t  CRC8;
  uint16_t CMDID;

} DEV_JUDGE_FrameHeader_s;



/**
 * @brief  Construct a new device::DEV_JUDGE_InitParam_s struct
 * 
 * @return None
 */
_DEV_JUDGE_InitParam::_DEV_JUDGE_InitParam()
{
  devType = device::DEV_OTHER;
}



/**
 * @brief Construct a new device::DEV_JUDGE_c object
 * 
 * @return None
 */
DEV_JUDGE_c::DEV_JUDGE_c()
{
  devType = device::DEV_OTHER;
}



/**
 * @brief  Destroy the device::DEV_JUDGE_c object
 * 
 * @return None
 */
DEV_JUDGE_c::~DEV_JUDGE_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelDevice(this);
    ((comm::COMM_UART_c *)hComm_)->DelUartNode(this);
  }

}



/**
 * @brief  Initialize the judge device
 * 
 * @param  initParam Pointer to the initialization parameters
 * @return None
 */
void DEV_JUDGE_c::InitDevice(DEV_InitParam_s *initParam)
{
  /* Check pointer */
  if (initParam == nullptr)
    return;

  if (initParam->devID == NULL || initParam->hComm == nullptr)
    return;

  /* Copy parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new DEV_JUDGE_InitParam_s;
  memcpy(initParam_, initParam, sizeof(DEV_JUDGE_InitParam_s));

  /* Initialize */
  auto param = (DEV_JUDGE_InitParam_s *)initParam_;
  devID      = param->devID;
  hComm_     = param->hComm;

  /* Regist */
  AddDevice(this);
  ((comm::COMM_UART_c *)hComm_)->AddUartNode(this);

  /* Update status */
  devState = DEV_OFFLINE;
}



/**
 * @brief  Send a judge communicate frame
 * 
 * @param  pFrame Pointer to the frame
 * @return None
 */
void DEV_JUDGE_c::SendJudgeCommFrame(DEV_JUDGE_Frame_s *pFrame)
{
  /* Check pointer */
  if (pFrame == nullptr)
    return;

  /* Fill header */
  DEV_JUDGE_FrameHeader_s header;
  header.SOF   = 0xA5;
  header.LEN   = pFrame->data.size();
  header.SEQ   = pFrame->seq;
  header.CMDID = pFrame->cmdID;
  algo::CRC8_Append((uint8_t *)&header, sizeof(DEV_JUDGE_FrameHeader_s) - 2);

  /* Fill frame */
  uint8_t frame[sizeof(DEV_JUDGE_FrameHeader_s) + pFrame->data.size() + 2];
  memcpy(frame, &header, sizeof(DEV_JUDGE_FrameHeader_s));                                    // Copy header
  memcpy(frame + sizeof(DEV_JUDGE_FrameHeader_s), pFrame->data.data(), pFrame->data.size());  // Copy data
  algo::CRC16_Append(frame, sizeof(DEV_JUDGE_FrameHeader_s) + pFrame->data.size() + 2);

  /* Transmit */
  hComm_->Transmit(comm::COMM_UART, frame, sizeof(frame));
}



/**
 * @brief  Callback function for receiving data from the UART
 * 
 * @param  pData Pointer to the data
 * @param  len Length of the data
 * @return None
 */
void DEV_JUDGE_c::UartNode_ReceiveCallback(uint8_t *pData, uint16_t len)
{
  /* Check pointer */
  if (pData == nullptr)
    return;

  if (len < sizeof(DEV_JUDGE_FrameHeader_s) + 2)
    return;

  /* Check header */
  auto header = (DEV_JUDGE_FrameHeader_s *)pData;
  if (header->SOF != 0xA5) return;
  if (header->LEN > len - sizeof(DEV_JUDGE_FrameHeader_s) - 2) return;
  if (!algo::CRC8_Verify(pData, sizeof(DEV_JUDGE_FrameHeader_s) - 2)) return;
  if (!algo::CRC16_Verify(pData, sizeof(DEV_JUDGE_FrameHeader_s) + header->LEN + 2)) return;

  /* Get frame */
  DEV_JUDGE_Frame_s frame;
  frame.seq   = header->SEQ;
  frame.cmdID = header->CMDID;
  frame.data.assign(pData + sizeof(DEV_JUDGE_FrameHeader_s), pData + sizeof(DEV_JUDGE_FrameHeader_s) + header->LEN);

  frameList.push_back(frame);
  while (frameList.size() > 10) frameList.pop_front();  // Limit frame list size

  /* Update status */
  lastHartbeatTime_ = HAL_GetTick();

  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;

}

} // namespace device

} // namespace rp

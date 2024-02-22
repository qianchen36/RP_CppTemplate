/**
 * @file    dev_board.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2024-01-31
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2024
 * 
 */

#include "dev_board.hpp"

namespace rp {

namespace device {

/* Board comm header */
typedef struct __packed _DEV_BOARD_FrameHeader
{
  uint8_t SOF = 0x55;   // Must be 0x55
  uint8_t ID;           // Frame ID
  uint8_t LEN;          // Data length
  uint8_t CRC8;         // Header CRC-8 checksum

} DEV_BOARD_FrameHeader_s;

/**
 * @brief  Construct a new device::DEV_BOARD_InitParam_s struct
 * 
 * @return None
 */
_DEV_BOARD_InitParam::_DEV_BOARD_InitParam()
{
  devType = DEV_OTHER;
}



/**
 * @brief  Construct a new device::DEV_BOARD_InitParam_s struct
 * 
 * @return None
 */
DEV_BOARD_c::DEV_BOARD_c()
{
  devType = DEV_OTHER;
}



/**
 * @brief  Destroy the device::DEV_BOARD_c object
 * 
 * @return None
 */
DEV_BOARD_c::~DEV_BOARD_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelDevice(this);
    ((comm::COMM_UART_c *)hComm_)->DelUartNode(this);
  }

}



/**
 * @brief  Initialize the board device
 * 
 * @param  initParam Pointer to the initialization parameters
 * @return None
 */
void DEV_BOARD_c::InitDevice(DEV_InitParam_s *initParam)
{
  /* Check pointer */
  if (initParam == nullptr)
    return;

  if (initParam->devID == NULL || initParam->hComm == nullptr)
    return;

  /* Copy parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new DEV_BOARD_InitParam_s;
  memcpy(initParam_, initParam, sizeof(DEV_BOARD_InitParam_s));

  /* Initialize */
  auto param = (DEV_BOARD_InitParam_s *)initParam_;
  devID   = param->devID;
  hComm_  = param->hComm;
  frameList.clear();

  /* Regist */
  AddDevice(this);
  ((comm::COMM_UART_c *)hComm_)->AddUartNode(this);

  /* Update status */
  devState = DEV_OFFLINE;
}



/**
 * @brief  Send a board communication frame
 * 
 * @param  pFrame Pointer to the frame
 * @return None
 */
void DEV_BOARD_c::SendBoardCommFrame(DEV_BOARD_Frame_s *pFrame)
{
  /* Check pointer */
  if (pFrame == nullptr)
    return;

  /* Fill header */
  DEV_BOARD_FrameHeader_s header;
  header.SOF = 0x55;
  header.ID  = pFrame->id;
  header.LEN = pFrame->data.size();
  algo::CRC8_Append((uint8_t *)&header, sizeof(DEV_BOARD_FrameHeader_s));

  /* Fill frame */
  uint8_t frame[sizeof(DEV_BOARD_FrameHeader_s) + pFrame->data.size() + 2];
  memcpy(frame, &header, sizeof(DEV_BOARD_FrameHeader_s));                                    // Header
  memcpy(frame + sizeof(DEV_BOARD_FrameHeader_s), pFrame->data.data(), pFrame->data.size());  // Data
  algo::CRC16_Append(frame, sizeof(DEV_BOARD_FrameHeader_s) + pFrame->data.size());

  /* Transmit */
  hComm_->Transmit(comm::COMM_UART, frame, sizeof(frame));
}



/**
 * @brief  Callback function for receiving data from the UART node
 * 
 * @param  pData Pointer to the data
 * @param  len Length of the data
 * @return None
 */
void DEV_BOARD_c::UartNode_ReceiveCallback(uint8_t *pData, uint16_t len)
{
  /* Check data pointer & length */
  if (pData == nullptr || len < 7)
    return;

  if (len < sizeof(DEV_BOARD_FrameHeader_s) + 2)
    return;

  /* Get header */
  auto header = (DEV_BOARD_FrameHeader_s *)pData;
  if (header->SOF != 0x55) return;
  if (header->LEN > len - sizeof(DEV_BOARD_FrameHeader_s) - 2) return;
  if (!algo::CRC8_Verify(pData, sizeof(DEV_BOARD_FrameHeader_s))) return;
  if (!algo::CRC16_Verify(pData, sizeof(DEV_BOARD_FrameHeader_s) + header->LEN + 2)) return;

  /* Get frame */
  frameList[header->ID].id        = header->ID;
  frameList[header->ID].timeStamp = HAL_GetTick();
  frameList[header->ID].data.assign(pData + sizeof(DEV_BOARD_FrameHeader_s), pData + sizeof(DEV_BOARD_FrameHeader_s) + header->LEN);

  /* Update state */
  lastHartbeatTime_ = HAL_GetTick();

  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;

}

} // namespace device

} // namespace rp

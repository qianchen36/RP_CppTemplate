/**
 * @file    dev_board.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2024-01-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __DEV_BOARD_HPP__
#define __DEV_BOARD_HPP__

#include "device.hpp"
#include "comm_uart.hpp"

namespace rp {

namespace device {

/* Board device initialize params */
typedef struct _DEV_BOARD_InitParam : public _DEV_InitParam
{
  _DEV_BOARD_InitParam();

} DEV_BOARD_InitParam_s;

/* Board communicate frame */
typedef struct _DEV_BOARD_Frame
{
  uint8_t  id;
  uint32_t timestamp;
  std::vector<uint8_t> data;

} DEV_BOARD_Frame_s;

/* Board device */
class DEV_BOARD_c : public DEVICE_c, public comm::COMM_UART_Node_c
{
private:
  void UartNode_ReceiveCallback(uint8_t *pData, uint16_t len) override;

public:
  std::list<DEV_BOARD_Frame_s> frameList;

  DEV_BOARD_c();
  ~DEV_BOARD_c();

  void InitDevice(DEV_InitParam_s *initParam) override;

  void SendBoardCommFrame(DEV_BOARD_Frame_s *pFrame);
};

} // namespace device

} // namespace rp

#endif  // __DEV_BOARD_HPP__

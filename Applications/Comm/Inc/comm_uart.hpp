/**
 * @file    comm_uart.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __COMM_UART_HPP__
#define __COMM_UART_HPP__

#include "comm.hpp"
#include "stm32f4xx_hal_uart.h"

#include <list>
#include <vector>
#include <iterator>

namespace rp {

namespace comm {

typedef struct _COMM_UART_InitParam : public _COMM_InitParam
{
  FUNC_STATE_e useAutoReceive;
  uint16_t     rxBufferSize;
  uint16_t     rxBufferCount;

  FUNC_STATE_e useAutoTransmit;
  uint16_t     txQueueLength;

  _COMM_UART_InitParam();

} COMM_UART_InitParam_s;

typedef struct
{
  uint16_t len;
  uint8_t *pData;

} COMM_UART_Buffer_s;

class COMM_UART_Node_c
{
friend class COMM_UART_c;

protected:
  virtual void UartNode_ReceiveCallback(uint8_t *pData, uint16_t len) = 0;

};

class COMM_UART_c : public COMM_c
{
private:
  std::list<COMM_UART_Node_c *>   uartNodeList_;
  std::list<COMM_UART_Buffer_s *> rxBuffer_, txBuffer_;
  std::list<COMM_UART_Buffer_s *>::iterator rxBufferIt_ , txBufferIt_;  

  FUNC_STATE_e useAutoReceive_;
  FUNC_STATE_e useAutoTransmit_;
  uint16_t     txQueueLength_;

  COMM_UART_Buffer_s *CreateBuffer(uint16_t size);
  void DeleteBuffer(COMM_UART_Buffer_s *pBuffer);

public:
  COMM_UART_c();

  void InitComm(COMM_InitParam_s *initParam) override;
  COMM_UART_c *GetObjectHandler(void) override;
  void Receive(int interfaceType, ...) override;
  void Transmit(int interfaceType, ...) override;

  void Start(void);
  void Stop(void);
  void AddUartNode(COMM_UART_Node_c *node);
  void DelUartNode(COMM_UART_Node_c *node);
  void UartAutoReceiveCallback(UART_HandleTypeDef *huart, uint16_t size);
  void UartAutoTransmitCallback(UART_HandleTypeDef *huart);

};

} // namespace comm

} // namespace rp

#endif // __COMM_UART_HPP__

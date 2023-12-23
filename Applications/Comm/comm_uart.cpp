/**
 * @file    comm_uart.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "comm_uart.hpp"

#include <stdarg.h>

namespace rp {

namespace comm {

/**
 * @brief  Construct a new comm::COMM_UART_InitParam_s struct
 * 
 * @return None
 */
_COMM_UART_InitParam::_COMM_UART_InitParam()
{
  comType = COMM_UART;

  useAutoReceive = DISABLE;
  rxBufferSize   = 2;
  rxBufferCount  = 64;

  useAutoTransmit = DISABLE;
  txQueueLength   = 5;
}

/**
 * @brief Construct a new comm::COMM_UART_c object
 * 
 */
COMM_UART_c::COMM_UART_c()
{
  comType = COMM_UART;
}



/**
 * @brief  Initialize the UART communication interface
 * 
 * @param  initParam Pointer to the initialization parameters
 * @return None
 */
void COMM_UART_c::InitComm(COMM_InitParam_s *initParam)
{
  /* Check initParam */
  if (initParam == nullptr)
    return;

  if (initParam->comID == NULL || initParam->hInterface == nullptr)
    return;

  if (initParam->comType != COMM_UART)
    return;

  Stop();

  /* Get parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new COMM_UART_InitParam_s;
  memcpy(initParam_, initParam, sizeof(COMM_UART_InitParam_s));

  auto param = (COMM_UART_InitParam_s *)initParam_;

  /* Initialize */
  comID            = param->comID;

  hInterface_      = param->hInterface;
  useAutoReceive_  = param->useAutoReceive;
  useAutoTransmit_ = param->useAutoTransmit;

  if (useAutoReceive_)  // Use DMA to receive data
  {
    for (uint16_t i = 0; i < param->rxBufferCount; i++)
    {
      auto pBuffer = CreateBuffer(param->rxBufferSize);
      rxBuffer_.push_back(pBuffer);
    }

  }

  if (useAutoTransmit_) // Use DMA to transmit data
    txQueueLength_ = param->txQueueLength;

  /* Update iterator */
  rxBufferIt_ = rxBuffer_.begin();
  txBufferIt_ = txBuffer_.begin();

  /* Regist */
  AddCommPort(this);

  /* Update state */
  comState = COMM_STOP;
}



/**
 * @brief  Get the object handler
 * 
 * @return (COMM_UART_c*) Pointer to the object handler
 */
COMM_UART_c *COMM_UART_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  Receive data from UART interface
 * 
 * @param  interfaceType Set the interface type
 * @param  pData Pointer to the data buffer
 * @param  len Length of the data buffer
 * @return None
 * 
 * @note   You should not call this function,
 *         when using auto receive mode.
 */
void COMM_UART_c::Receive(int interfaceType, ...)
{
  /* Check comState */
  if (comState == COMM_STOP)
    return;

  if (useAutoReceive_ == ENABLE)
    return;

  /* Get args */
  va_list args;
  va_start(args, interfaceType);

  auto pData = va_arg(args, uint8_t *);
  auto len = va_arg(args, int);

  /* Receive data */
  HAL_UART_Receive((UART_HandleTypeDef *)hInterface_, pData, len, 1000);
    
  /* Clean up */
  va_end(args);
}



/**
 * @brief  Transmit data through UART interface
 * 
 * @param interfaceType (COMM_Type_e) Type of the interface
 * @param pData (uint8_t *) Data pack to be transmitted
 * @param len (uint16_t) Length of the data pack
 * @return None
 */
void COMM_UART_c::Transmit(int interfaceType, ...)
{
  /* Check comState */
  if (comState == COMM_STOP)
    return;

  /* Get args */
  va_list args;
  va_start(args, interfaceType);

  auto pData = va_arg(args, uint8_t *);
  auto len = va_arg(args, int);

  if (useAutoTransmit_) // Use DMA to transmit data
  {
    if (txBuffer_.size() >= txQueueLength_)
    {
      va_end(args);
      return;
    }

    /* Add datapack to transmit queue */
    auto pBuffer = CreateBuffer(len);
    memcpy(pBuffer->pData, pData, len);
    txBuffer_.push_back(pBuffer);

    /* Start DMA transmit */
    if (txBuffer_.size() == 1)
    {
      txBufferIt_ = txBuffer_.begin();
      HAL_UART_Transmit_DMA((UART_HandleTypeDef *)hInterface_, (*txBufferIt_)->pData, (*txBufferIt_)->len);
    }

    /* Clean up */
    va_end(args);
    return;
  }

  /* Transmit data normally */
  HAL_UART_Transmit((UART_HandleTypeDef *)hInterface_, pData, len, 1000);

  /* Clean up */
  va_end(args);
}



/**
 * @brief  Start the UART interface auto receive
 * 
 * @return None
 */
void COMM_UART_c::Start(void)
{
  if (comState != COMM_STOP || useAutoReceive_ != ENABLE)
    return;

  HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *)hInterface_, (*rxBufferIt_)->pData, (*rxBufferIt_)->len);
  
  comState = COMM_RUN;
}



/**
 * @brief  Stop the UART interface auto receive & transmit
 * 
 * @return None
 */
void COMM_UART_c::Stop(void)
{
  if (comState != COMM_RUN)
    return;

  /* Stop UART interface */
  HAL_UART_Abort((UART_HandleTypeDef *)hInterface_);

  /* Clear buffer */
  for (auto it : rxBuffer_)
    DeleteBuffer(it);

  rxBuffer_.clear();

  for (auto it : txBuffer_)
    DeleteBuffer(it);

  txBuffer_.clear();

  comState = COMM_STOP;
}



/**
 * @brief  Add a UART node to auto receive callback list
 * 
 * @param node (COMM_UART_Node_c *) Pointer to the UART node
 * @return None
 */
void COMM_UART_c::AddUartNode(COMM_UART_Node_c *node)
{
  if (node == nullptr)
    return;

  for (auto it : uartNodeList_)
  {
    if (it == node)
      return;

  }

  uartNodeList_.push_back(node);
}



/**
 * @brief  Delete a UART node from auto receive callback list
 * 
 * @param node (COMM_UART_Node_c *) Pointer to the UART node
 * @return None
 */
void COMM_UART_c::DelUartNode(COMM_UART_Node_c *node)
{
  if (node == nullptr)
    return;

  for (auto it : uartNodeList_)
  {
    if (it == node)
    {
      uartNodeList_.remove(it);
      return;
    }

  }

}



/**
 * @brief  UART auto receive callback
 * 
 * @param  huart (UART_HandleTypeDef *) Pointer to the UART handler
 * @param  size (uint16_t) Size of the received data
 * @return None
 */
void COMM_UART_c::UartAutoReceiveCallback(UART_HandleTypeDef *huart, uint16_t size)
{
  if (comState != COMM_RUN || useAutoReceive_ != ENABLE)
    return;

  /* Resum DMA receive */
  auto pDataPack = *rxBufferIt_;

  if (++rxBufferIt_ == rxBuffer_.end())
    rxBufferIt_ = rxBuffer_.begin();

  HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *)hInterface_, (*rxBufferIt_)->pData, (*rxBufferIt_)->len);

  /* Notify all nodes */
  for (auto it : uartNodeList_)
    it->UartNode_ReceiveCallback(pDataPack->pData, size);

  /* Clean up */
  memset(pDataPack->pData, 0, pDataPack->len);
}



/**
 * @brief  UART auto transmit callback
 * 
 * @param  huart (UART_HandleTypeDef *) Pointer to the UART handler
 * @return None
 */
void COMM_UART_c::UartAutoTransmitCallback(UART_HandleTypeDef *huart)
{
  if (comState != COMM_RUN || useAutoTransmit_ != ENABLE)
    return;

  /* Resum DMA transmit */
  if (++txBufferIt_ != txBuffer_.end())
    HAL_UART_Transmit_DMA((UART_HandleTypeDef *)hInterface_, (*txBufferIt_)->pData, (*txBufferIt_)->len);

  /* Clean up */
  DeleteBuffer(txBuffer_.front());
  txBuffer_.pop_front();

}



/**
 * @brief  Create a new buffer
 * 
 * @param  size (uint16_t) Size of the buffer
 * @return (COMM_UART_Buffer_s*) Pointer to the buffer
 */
COMM_UART_Buffer_s *COMM_UART_c::CreateBuffer(uint16_t size)
{
  auto *pBuffer = new COMM_UART_Buffer_s;

  pBuffer->len = size;
  pBuffer->pData = new uint8_t[size];
  memset(pBuffer->pData, 0, size);

  return pBuffer;
}



/**
 * @brief  Delete a buffer
 * 
 * @param  pBuffer (COMM_UART_Buffer_s *) Pointer to the buffer
 * @return None
 */
void COMM_UART_c::DeleteBuffer(COMM_UART_Buffer_s *pBuffer)
{
  if (pBuffer == nullptr)
    return;

  delete pBuffer->pData;
  delete pBuffer;
}

} // namespace comm

} // namespace rp



extern "C" {

/**
 * @brief  Override the HAL_UARTEx_RxEventCallback
 * 
 * @param  huart (UART_HandleTypeDef *) Pointer to the UART handler
 * @param  Size (uint16_t) Size of the received data
 * @return None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  for (auto it : rp::comm::CommList)
  {
    if (it.second->GetInterfaceHandler() == huart)
    {
      auto *pComm = (rp::comm::COMM_UART_c *)it.second;
      pComm->UartAutoReceiveCallback(huart, Size);

      break;
    }

  }

}



/**
 * @brief  Override the HAL_UART_TxCpltCallback
 * 
 * @param  huart (UART_HandleTypeDef *) Pointer to the UART handler
 * @return None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  for (auto it : rp::comm::CommList)
  {
    if (it.second->GetInterfaceHandler() == huart)
    {
      auto *pComm = (rp::comm::COMM_UART_c *)it.second;
      pComm->UartAutoTransmitCallback(huart);

      break;
    }

  }

}

} // extern "C"

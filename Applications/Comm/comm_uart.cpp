/**
 * @file    comm_uart.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.1
 * @date    2023-11-24
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "comm_uart.hpp"

#include <cstdarg>

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

  /* Initialize */
  auto param       = (COMM_UART_InitParam_s *)initParam_;
  comID            = param->comID;
  hInterface_      = param->hInterface;
  useAutoReceive_  = param->useAutoReceive;
  useAutoTransmit_ = param->useAutoTransmit;
  rxQueue_.clear();
  txQueue_.clear();

  if (useAutoReceive_)  // Use DMA to receive data
    rxQueue_.resize(param->rxBufferCount, std::vector<uint8_t>(param->rxBufferSize, 0));

  if (useAutoTransmit_) // Use DMA to transmit data
    txQueueLength_ = param->txQueueLength;

  /* Update iterator */
  rxQueueIt_ = rxQueue_.begin();
  txQueueIt_ = rxQueue_.begin();

  /* Regist */
  AddCommPort(this);

  /* Update state */
  comState = COMM_STOP;
}



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
  auto len   = va_arg(args, int);
  va_end(args);

  if (useAutoTransmit_) // Use DMA to transmit data
  {
    if (txQueue_.size() >= txQueueLength_)
      return;

    /* Add datapack to transmit queue */
    txQueue_.push_back(std::vector<uint8_t>(pData, pData + len));

    /* Start DMA transmit */
    if (txQueue_.size() == 1)
      HAL_UART_Transmit_DMA((UART_HandleTypeDef *)hInterface_, txQueue_.front().data(), txQueue_.front().size());

    return;
  }

  /* Transmit data normally */
  HAL_UART_Transmit((UART_HandleTypeDef *)hInterface_, pData, len, 1000);
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

  HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *)hInterface_, (*rxQueueIt_).data(), (*rxQueueIt_).size());
  
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
  txQueue_.clear();

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
    if (it == node) return;

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
  auto pDataPack = *rxQueueIt_;

  if (++rxQueueIt_ == rxQueue_.end())
    rxQueueIt_ = rxQueue_.begin();

  HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *)hInterface_, (*rxQueueIt_).data(), (*rxQueueIt_).size());

  /* Notify all nodes */
  for (auto it : uartNodeList_)
    it->UartNode_ReceiveCallback(pDataPack.data(), size);

  /* Clean up */
  memset(pDataPack.data(), 0, pDataPack.size());
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
  if (++txQueueIt_ != txQueue_.end())
    HAL_UART_Transmit_DMA((UART_HandleTypeDef *)hInterface_, (*txQueueIt_).data(), (*txQueueIt_).size());

  /* Clean up */
  txQueue_.pop_front();
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

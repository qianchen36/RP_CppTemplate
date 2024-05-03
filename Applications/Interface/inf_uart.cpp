/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-17
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-17   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "inf_uart.hpp"

namespace robotpilots {

/**
 * @brief UART Handle Map
 */
std::map<UART_HandleTypeDef *, CUartInterface *> HalUartHandleMap;


/**
 * @brief
 */
CUartInterface::~CUartInterface() {

  UnRegisterInterface_();

  if (interfaceId != EInterfaceID::INF_NULL)
    HalUartHandleMap.erase(halUartHandle_);
}


/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CUartInterface::InitInterface(const SInfInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (interfaceState == RP_BUSY) return RP_ERROR;

  auto &param = *static_cast<const SUartInfInitParam *>(pStruct);
  interfaceId    = param.interfaceId;
  halUartHandle_ = param.halUartHandle;
  useTxHandle_   = param.useTxHandle;
  useRxHandle_   = param.useRxHandle;

  HAL_UART_Abort(halUartHandle_);
  rxCallbackList_.clear();

  if (useTxHandle_) {
    txQueueLength_ = param.txQueueLength;
    txBufferSize_  = param.txBufferSize;
    txQueue_.clear();
  }

  if (useRxHandle_) {
    rxQueueLength_ = param.rxQueueLength;
    rxBufferSize_  = param.rxBufferSize;
    rxQueue_.resize(rxQueueLength_, DataBuffer<uint8_t>(rxBufferSize_, 0));
  }

  RegisterInterface_();
  HalUartHandleMap.insert(std::make_pair(halUartHandle_, this));
  interfaceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CUartInterface::StartTransfer() {

  if (interfaceState != RP_OK) return RP_ERROR;

  StopTransfer();

  if (useRxHandle_) {
    rxQueueIt_ = rxQueue_.begin();
    if (HAL_UARTEx_ReceiveToIdle_DMA(halUartHandle_, rxQueueIt_->data(), rxQueueIt_->size()))
      return RP_ERROR;
  }

  interfaceState = RP_BUSY;

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus CUartInterface::StopTransfer() {

  if (interfaceState == RP_RESET) return RP_ERROR;

  if (HAL_UART_Abort(halUartHandle_))
    return RP_ERROR;

  if (interfaceState == RP_BUSY) {
    interfaceState = RP_OK;
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @param buffer
 * @return
 */
ERpStatus CUartInterface::Transmit(const DataBuffer<uint8_t> &buffer) {

  return Transmit(buffer.data(), buffer.size());
}

/**
 * @brief
 * @param buffer
 * @param len
 * @return
 */
ERpStatus CUartInterface::Transmit(const uint8_t *buffer, size_t len) {

  if (interfaceState == RP_RESET) return RP_ERROR;

  if (useTxHandle_) {
    if (txQueue_.size() >= txQueueLength_) return RP_ERROR;
    if (len > txBufferSize_) return RP_ERROR;

    txQueue_.emplace_back(buffer, buffer + len);

    if (txQueue_.size() == 1)
       HAL_UART_Transmit_DMA(halUartHandle_, txQueue_.front().data(), txQueue_.front().size());

    return RP_OK;
  }

  if (HAL_UART_Transmit(halUartHandle_, const_cast<uint8_t *>(buffer), len, 1000))
    return RP_ERROR;

  return RP_OK;
}


/**
 * @brief
 * @param buffer
 * @return
 */
ERpStatus CUartInterface::Receive(DataBuffer<uint8_t> &buffer) {

  return Receive(buffer.data(), buffer.size());
}


/**
 * @brief
 * @param buffer
 * @param len
 * @return
 */
ERpStatus CUartInterface::Receive(uint8_t *buffer, size_t len) {

  if (interfaceState == RP_RESET) return RP_ERROR;

  if (useRxHandle_) return RP_ERROR;

  if (HAL_UART_Receive(halUartHandle_, buffer, len, 1000))
    return RP_ERROR;

  return RP_OK;
}


/**
 * @brief
 * @param callback
 * @return
 */
ERpStatus CUartInterface::RegisterCallback(const std::function<void(DataBuffer<uint8_t> &, size_t)> &callback) {

  if (interfaceState == RP_RESET) return RP_ERROR;

  rxCallbackList_.push_back(callback);
  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CUartInterface::ClearCallbackList() {

  if (interfaceState == RP_RESET) return RP_ERROR;

  rxCallbackList_.clear();
  return RP_OK;
}


/**
 * @brief
 * @param handle
 */
void CUartInterface::_UART_HalTxCallback(UART_HandleTypeDef *handle) {

  if (interfaceState == RP_RESET || !useTxHandle_) return;

  if (useTxHandle_) {
    txQueue_.pop_front();

    if (!txQueue_.empty())
      HAL_UART_Transmit_DMA(halUartHandle_, txQueue_.front().data(), txQueue_.front().size());
  }
}


/**
 * @brief
 * @param handle
 */
void CUartInterface::_UART_HalRxCallback(UART_HandleTypeDef *handle) {

  if (interfaceState == RP_RESET || !useRxHandle_) return;

  if (useRxHandle_) {
    /* Get Rx Buffer */
    size_t len = handle->RxXferSize - handle->RxXferCount;
    auto &buffer = *rxQueueIt_;

    /* Resume Receive */
    if (++rxQueueIt_ == rxQueue_.end())
      rxQueueIt_ = rxQueue_.begin();

    HAL_UARTEx_ReceiveToIdle_DMA(halUartHandle_, rxQueueIt_->data(), rxQueueIt_->size());

    /* Callback */
    for (const auto &item: rxCallbackList_)
      item(buffer, len);

    /* Clear Buffer */
    std::fill(buffer.begin(), buffer.end(), 0);
  }
}


/**
 * @brief
 * @param handle
 */
void CUartInterface::_UART_HalErrorCallback(UART_HandleTypeDef *handle) {

  StopTransfer();
  interfaceState = RP_ERROR;
}


/**
   * @brief
   */
void CUartInterface::HeartbeatHandler_() {

  // TODO: UART Interface Health Check
  if (useRxHandle_ && interfaceState == RP_ERROR) {
    interfaceState = RP_OK;
    StartTransfer();
  }
}

} // namespace robotpilots


extern "C" {

/**
* @brief Override HAL_UART_TxCpltCallback
*
* @param huart Pointer to HAL_UART Handle
* @return None
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

  auto it = robotpilots::HalUartHandleMap.find(huart);
  if (it != robotpilots::HalUartHandleMap.end())
    it->second->_UART_HalTxCallback(huart);
}


/**
 * @brief Override HAL_UART_TxHalfCpltCallback
 *
 * @param huart Pointer to HAL_UART Handle
 * @return None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  auto it = robotpilots::HalUartHandleMap.find(huart);
  if (it != robotpilots::HalUartHandleMap.end())
    it->second->_UART_HalRxCallback(huart);
}


/**
 * @brief Override HAL_UARTEx_RxEventCallback
 *
 * @param huart Pointer to HAL_UART Handle
 * @param Size Size of Received Data
 * @return None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

  auto it = robotpilots::HalUartHandleMap.find(huart);
  if (it != robotpilots::HalUartHandleMap.end())
    it->second->_UART_HalRxCallback(huart);
}


/**
 * @brief Override HAL_UART_ErrorCallback
 *
 * @param huart Pointer to HAL_UART Handle
 * @return None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

  auto it = robotpilots::HalUartHandleMap.find(huart);
  if (it != robotpilots::HalUartHandleMap.end())
    it->second->_UART_HalErrorCallback(huart);
}

} // extern "C"

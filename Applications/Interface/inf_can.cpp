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

#include "inf_can.hpp"

namespace robotpilots {

/**
 * @brief
 */
std::map<FDCAN_HandleTypeDef *, CCanInterface *> HalFdcanHandleMap;

/**
 * @brief
 * @param infId
 * @param nodeStdId
 * @param nodeFrameType
 * @param nodeFrameDlc
 */
void CCanInterface::CCanTxNode::InitTxNode(EInterfaceID infId,
                                           uint32_t nodeStdId,
                                           ECanFrameType nodeFrameType,
                                           ECanFrameDlc nodeFrameDlc) {

  stdId = nodeStdId;
  frameType = nodeFrameType;
  dataLength = nodeFrameDlc;
  pInterface_ = static_cast<CCanInterface *>(InterfaceMap.at(infId));
  dataBuffer.resize(FrameFlc2DataLength(nodeFrameDlc), 0);

  pInterface_->RegistNode_(*this);
}

/**
 * @brief
 */
void CCanInterface::CCanTxNode::Transmit() {

  pInterface_->Transmit(*this);
}

/**
 * @brief
 * @param buffer
 */
void CCanInterface::CCanTxNode::Transmit(const uint8_t *buffer) {

  std::copy(buffer, buffer + FrameFlc2DataLength(dataLength),
            dataBuffer.begin());
  Transmit();
}

/**
 * @brief
 * @param buffer
 */
void CCanInterface::CCanTxNode::Transmit(const DataBuffer<uint8_t> &buffer) {

  std::copy(buffer.data(), buffer.data() + FrameFlc2DataLength(dataLength),
            dataBuffer.begin());
  Transmit();
}

/**
 * @brief
 * @param canInfId
 * @param nodeStdId
 * @param nodeFrameType
 * @param nodeFrameDlc
 */
void CCanInterface::CCanRxNode::InitRxNode(EInterfaceID canInfId,
                                           uint32_t nodeStdId,
                                           ECanFrameType nodeFrameType,
                                           ECanFrameDlc nodeFrameDlc) {

  stdId = nodeStdId;
  canInterfaceId = canInfId;
  frameType = nodeFrameType;
  dataLength = nodeFrameDlc;
  pInterface_ = static_cast<CCanInterface *>(InterfaceMap.at(canInfId));
  dataBuffer.resize(FrameFlc2DataLength(nodeFrameDlc), 0);

  pInterface_->RegistNode_(*this);
}

/**
 * @brief
 */
void CCanInterface::CCanRxNode::Receive() {


}

/**
 * @brief
 * @param buffer
 */
void CCanInterface::CCanRxNode::Receive(uint8_t *buffer) {


}

/**
 * @brief
 * @param buffer
 */
void CCanInterface::CCanRxNode::Receive(DataBuffer<uint8_t> &buffer) {


}

/**
 * @brief
 */
CCanInterface::~CCanInterface() {

  UnRegisterInterface_();

  if (interfaceId != EInterfaceID::INF_NULL)
    HalFdcanHandleMap.erase(halFdcanHandle_);
}

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CCanInterface::InitInterface(const SInfInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (interfaceState == RP_BUSY) return RP_ERROR;

  auto &param = *static_cast<const SCanInfInitParam *>(pStruct);
  interfaceId = param.interfaceId;
  halFdcanHandle_ = param.halFdcanHandle;

  /* Config Filter */
  HAL_FDCAN_ConfigFilter(halFdcanHandle_,
                         const_cast<FDCAN_FilterTypeDef *>(&param.fdcanFilterConfig));
  HAL_FDCAN_ConfigGlobalFilter(halFdcanHandle_,
                               FDCAN_REJECT, FDCAN_REJECT,
                               FDCAN_REJECT_REMOTE, FDCAN_FILTER_REJECT);
  HAL_FDCAN_ConfigFifoWatermark(halFdcanHandle_, FDCAN_RX_FIFO0, 1);
  HAL_FDCAN_ActivateNotification(halFdcanHandle_, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

  RegisterInterface_();
  HalFdcanHandleMap.insert(std::make_pair(halFdcanHandle_, this));
  interfaceState = RP_OK;

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus CCanInterface::StartTransfer() {

  if (interfaceState != RP_OK) return RP_ERROR;

  if (HAL_FDCAN_Start(halFdcanHandle_))
    return RP_ERROR;

  interfaceState = RP_BUSY;

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus CCanInterface::StopTransfer() {

  if (interfaceState == RP_RESET) return RP_ERROR;

  if (HAL_FDCAN_Stop(halFdcanHandle_))
    return RP_ERROR;

  if (interfaceState == RP_BUSY) {
    interfaceState = RP_OK;
    return  RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @param node
 */
ERpStatus CCanInterface::Transmit(CCanNode &node) {

  /* Check State */
  if (interfaceState != RP_BUSY) return RP_ERROR;

  /* Set Header */
  FDCAN_TxHeaderTypeDef TxHeader = {
      .Identifier          = node.stdId,
      .IdType              = FDCAN_STANDARD_ID,
      .TxFrameType         = static_cast<uint32_t>(node.frameType),
      .DataLength          = static_cast<uint32_t>(node.dataLength),
      .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
      .BitRateSwitch       = FDCAN_BRS_OFF,
      .FDFormat            = FDCAN_CLASSIC_CAN,
      .TxEventFifoControl  = FDCAN_NO_TX_EVENTS,
      .MessageMarker       = 0,
  };

  /* Flush D-Cache */
  #ifdef rp_USE_DCACHE
    SCB_CleanDCache();
  #endif

  /* Transmit Data */
  if (HAL_FDCAN_AddMessageToTxFifoQ(halFdcanHandle_, &TxHeader, node.dataBuffer.data()))
    return RP_ERROR;

  return RP_OK;
}

/**
 * @brief
 * @param node
 * @return
 */
ERpStatus CCanInterface::Receive(CCanNode &node) {

  /* Check State */
  if (interfaceState != RP_BUSY) return RP_ERROR;

  /* Receive Data */
  uint8_t data[64];
  FDCAN_RxHeaderTypeDef RxHeader;
  if (HAL_FDCAN_GetRxMessage(halFdcanHandle_, FDCAN_RX_FIFO0, &RxHeader, data))
    return RP_ERROR;

  /* Update CAN Node */
  node.stdId      = RxHeader.Identifier;
  node.frameType  = static_cast<ECanFrameType>(RxHeader.RxFrameType);
  node.dataLength = static_cast<ECanFrameDlc>(RxHeader.DataLength);
  node.timestamp  = HAL_GetTick();
  node.dataBuffer.resize(FrameFlc2DataLength(node.dataLength), 0);
  std::copy(data, data + FrameFlc2DataLength(node.dataLength), node.dataBuffer.begin());

  /* Flush D-Cache */
  #ifdef rp_USE_DCACHE
    SCB_InvalidateDCache();
  #endif

  return RP_OK;
}

/**
 * @brief
 * @param dlc
 * @return
 */
size_t CCanInterface::FrameFlc2DataLength(ECanFrameDlc dlc) {

  switch (dlc) {
    case ECanFrameDlc::DLC_0:
      return 0;
    case ECanFrameDlc::DLC_1:
      return 1;
    case ECanFrameDlc::DLC_2:
      return 2;
    case ECanFrameDlc::DLC_3:
      return 3;
    case ECanFrameDlc::DLC_4:
      return 4;
    case ECanFrameDlc::DLC_5:
      return 5;
    case ECanFrameDlc::DLC_6:
      return 6;
    case ECanFrameDlc::DLC_7:
      return 7;
    case ECanFrameDlc::DLC_8:
      return 8;
    case ECanFrameDlc::DLC_12:
      return 12;
    case ECanFrameDlc::DLC_16:
      return 16;
    case ECanFrameDlc::DLC_20:
      return 20;
    case ECanFrameDlc::DLC_24:
      return 24;
    case ECanFrameDlc::DLC_32:
      return 32;
    case ECanFrameDlc::DLC_48:
      return 48;
    case ECanFrameDlc::DLC_64:
      return 64;
    default:
      return 0;
  }
}

/**
 * @brief
 */
void CCanInterface::HeartbeatHandler_() {

}

/**
 * @brief
 * @param node
 */
ERpStatus CCanInterface::RegistNode_(CCanTxNode &node) {

  if (std::find(txNodeList_.begin(), txNodeList_.end(), &node) == txNodeList_.end()) {
    txNodeList_.push_back(&node);
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @param node
 */
ERpStatus CCanInterface::RegistNode_(CCanRxNode &node) {

  if (std::find(rxNodeList_.begin(), rxNodeList_.end(), &node) == rxNodeList_.end()) {
    rxNodeList_.push_back(&node);
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @param node
 */
ERpStatus CCanInterface::UnregistNode_(CCanTxNode &node) {

  txNodeList_.remove(&node);
  return RP_OK;
}

/**
 * @brief
 * @param node
 */
ERpStatus CCanInterface::UnregistNode_(CCanRxNode &node) {

  rxNodeList_.remove(&node);
  return RP_OK;
}

/**
 * @brief
 * @param handle
 */
void CCanInterface::_CAN_HalTxCallback(FDCAN_HandleTypeDef *handle) {

  // TODO: Transmit Callback
}

/**
 * @brief
 * @param handle
 */
void CCanInterface::_CAN_HalRxCallback(FDCAN_HandleTypeDef *handle) {

  /* Receive Data */
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t data[64];
  HAL_FDCAN_GetRxMessage(halFdcanHandle_, FDCAN_RX_FIFO0,
                         &RxHeader, data);

  /* Update CAN Node */
  for (auto &rxNode : rxNodeList_) {
    if (rxNode->stdId == RxHeader.Identifier) {
      rxNode->timestamp  = HAL_GetTick();
      std::copy(data, data + FrameFlc2DataLength(rxNode->dataLength),
                rxNode->dataBuffer.begin());
    }
  }

  /* Flush D-Cache */
#ifdef rp_USE_DCACHE
  SCB_InvalidateDCache();
#endif
}

/**
 * @brief
 * @param handle
 */
void CCanInterface::_CAN_HalErrorCallback(FDCAN_HandleTypeDef *handle) {

  // TODO: Error Process
  interfaceState = RP_ERROR;
  // Error_Handler();
}

} // namespace robotpilots


extern "C" {

/**
* @brief Override HAL_CAN_TxMailbox0CompleteCallback
*
* @param hfdcan
* @param RxFifo0ITs
*/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {

  if (RxFifo0ITs == FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
    auto it = robotpilots::HalFdcanHandleMap.find(hfdcan);
    if (it != robotpilots::HalFdcanHandleMap.end())
      it->second->_CAN_HalRxCallback(hfdcan);
  }
}

/**
 * @brief Override HAL_CAN_TxMailbox0CompleteCallback
 *
 * @param hfdcan
 * @param RxFifo1ITs
 */
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs) {

  if (RxFifo1ITs == FDCAN_IT_RX_FIFO1_NEW_MESSAGE) {
    auto it = robotpilots::HalFdcanHandleMap.find(hfdcan);
    if (it != robotpilots::HalFdcanHandleMap.end())
      it->second->_CAN_HalRxCallback(hfdcan);
  }
}

/**
 * @brief Override HAL_CAN_TxMailbox0CompleteCallback
 *
 * @param hfdcan
 */
void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan) {

  auto it = robotpilots::HalFdcanHandleMap.find(hfdcan);
  if (it != robotpilots::HalFdcanHandleMap.end())
    it->second->_CAN_HalErrorCallback(hfdcan);
}

} // extern "C"

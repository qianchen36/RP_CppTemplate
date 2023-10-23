/**
 * @file    comm_can.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "comm_can.hpp"

#include <stdarg.h>

namespace comm {

/**
 * @brief  Construct a new COMM_CAN_c::COMM_CAN_c object
 * 
 * @retval None
 */
COMM_CAN_c::COMM_CAN_c(void)
{
  comType = COMM_CAN;
}



/**
 * @brief  Get the handler object of CAN interface
 * 
 * @retval Reference of the CAN interface handler
 */
COMM_CAN_c &COMM_CAN_c::GetObjectHandler(void)
{
  return *this;
}



/**
 * @brief   Receive data from CAN interface
 * 
 * @param   interfaceType (COMM_Type_e) Type of the interface
 * @param   hcan (CAN_HandleTypeDef *) Handle of the CAN interface
 * @param   fifo (uint32_t) FIFO number of the CAN interface
 * @retval  None
 * 
 * @warning You should not call this function directly
 */
void COMM_CAN_c::Receive(int interfaceType, ...)
{
  /* Check interfaceType */
  if (interfaceType != COMM_CAN)
    return;

  /* Check comState */
  if (comState != COMM_IDLE)
    return;

  comState = COMM_BUSY;

  /* Get CAN handler from variable args */
  va_list args;
  va_start(args, interfaceType);

  auto hcan = va_arg(args, CAN_HandleTypeDef *);
  auto fifo = va_arg(args, uint32_t);

  /* Receive data */
  CAN_RxHeaderTypeDef RxHeader;
  COMM_CAN_DataPack_s rxDataPack;

  HAL_CAN_GetRxMessage(hcan, fifo, &RxHeader, rxDataPack.data);

  rxDataPack.stdID = RxHeader.StdId;
  rxDataPack.timeStamp = HAL_GetTick();

  /* Call CAN node callback */
  if (canNodeList_.find(rxDataPack.stdID) != canNodeList_.end())
    canNodeList_[rxDataPack.stdID]->CanNode_ReceiveCallback(&rxDataPack);

  /* Clean up */
  comState = COMM_IDLE;
  va_end(args);
}



/**
 * @brief  Transmit data through CAN interface
 * 
 * @param  interfaceType (COMM_Type_e) Type of the interface
 * @param  stdID (uint32_t) Standard ID of CAN frame
 * @param  data (uint8_t *) Data pack to be transmitted
 * @retval None
 */
void COMM_CAN_c::Transmit(int interfaceType, ...)
{
  /* Check interfaceType */
  if (interfaceType != COMM_CAN)
    return;

  /* Check comState */
  if (comState != COMM_IDLE)
    return;

  comState = COMM_BUSY;

  /* Get CAN datapack from variable args */
  va_list args;
  va_start(args, interfaceType);

  auto stdID = va_arg(args, uint32_t);
  auto data = va_arg(args, uint8_t *);

  /* Transmit data */
  uint32_t TxMailbox;
  CAN_TxHeaderTypeDef TxHeader = {
    .StdId = stdID,
    .IDE = CAN_ID_STD,
    .RTR = CAN_RTR_DATA,
    .DLC = 8,
    .TransmitGlobalTime = DISABLE
  };

  HAL_CAN_AddTxMessage((CAN_HandleTypeDef *)hInterface_, &TxHeader, data, &TxMailbox);

  /* Clean up */
  comState = COMM_IDLE;
  va_end(args);
}



/**
 * @brief  Set CAN receive filter
 * 
 * @param  sFilterConfig Pointer to the filter config
 * @retval None
 */
void COMM_CAN_c::ConfigFilter(CAN_FilterTypeDef *sFilterConfig)
{
  HAL_CAN_ConfigFilter((CAN_HandleTypeDef *)hInterface_, sFilterConfig);

  if (sFilterConfig->FilterFIFOAssignment == CAN_RX_FIFO0)
    HAL_CAN_ActivateNotification((CAN_HandleTypeDef *)hInterface_, CAN_IT_RX_FIFO0_MSG_PENDING);

  else if (sFilterConfig->FilterFIFOAssignment == CAN_RX_FIFO1)
    HAL_CAN_ActivateNotification((CAN_HandleTypeDef *)hInterface_, CAN_IT_RX_FIFO1_MSG_PENDING);

}



/**
 * @brief  Start the CAN interface
 * 
 * @retval None
 */
void COMM_CAN_c::Start(void)
{
  if (comState == COMM_RESET)
    return;

  HAL_CAN_Start((CAN_HandleTypeDef *)hInterface_);
}



/**
 * @brief  Stop the CAN interface
 * 
 * @retval None
 */
void COMM_CAN_c::Stop(void)
{
  if (comState == COMM_RESET)
    return;

  HAL_CAN_Stop((CAN_HandleTypeDef *)hInterface_);
}



/**
 * @brief  Add a CAN node to canNodeList
 * 
 * @param  canNode Pointer to the CAN node
 * @retval None
 */
void COMM_CAN_c::AddCanNode(COMM_CAN_Node_c *canNode)
{
  if (canNode == nullptr || canNode->canStdID == NULL)
    return;
  
  canNodeList_.insert(std::pair<uint32_t, COMM_CAN_Node_c *>(canNode->canStdID, canNode));
}



/**
 * @brief Delete a CAN node from canNodeList
 * 
 * @param  canNode Pointer to the CAN node
 * @retval None
 */
void COMM_CAN_c::DelCanNode(COMM_CAN_Node_c *canNode)
{
  if (canNode == nullptr || canNode->canStdID == NULL)
    return;

  canNodeList_.erase(canNode->canStdID);
}

} // namespace comm

extern "C" {

/**
 * @brief  Rewrite HAL_CAN_RxFifo0MsgPendingCallback
 * 
 * @param  hcan Handle of the CAN interface
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  std::map<uint8_t, comm::COMM_c *>::iterator it;

  for (it = comm::CommList.begin(); it != comm::CommList.end(); ++it)
  {
    if (it->second->GetInterfaceHandler() == hcan)
    {
      it->second->Receive(comm::COMM_CAN, hcan, CAN_RX_FIFO0);

      break;
    }

  }

}



/**
 * @brief  Rewrite HAL_CAN_RxFifo1MsgPendingCallback
 * 
 * @param  hcan Handle of the CAN interface
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  std::map<uint8_t, comm::COMM_c *>::iterator it;
  
  for (it = comm::CommList.begin(); it != comm::CommList.end(); ++it)
  {
    if (it->second->GetInterfaceHandler() == hcan)
    {
      it->second->Receive(comm::COMM_CAN, hcan, CAN_RX_FIFO1);

      break;
    }

  }

}

} // extern "C"

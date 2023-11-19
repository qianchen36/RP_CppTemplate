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
COMM_CAN_c::COMM_CAN_c()
{
  comType = COMM_CAN;
}



/**
 * @brief  Initialize the CAN interface
 * 
 * @param  id Set the communicate port ID
 * @param  hInterface Set the handle of the communication interface
 * @param  pStruct Set the CAN specific parameters
 * @return None
 */
void COMM_CAN_c::InitComm(uint8_t id, void *hInterface, ...)
{
  /* Check id and hInterface */
  if (id == NULL || hInterface == nullptr)
    return;

  /* Get args */
  va_list args;
  va_start(args, hInterface);

  comID = id;
  hInterface_ = hInterface;

  auto pStruct = va_arg(args, COMM_CAN_InitParam_s *);

  ConfigFilter(pStruct);
  AddCommPort(this);

  /* Clean up */
  va_end(args);
  comState = COMM_STOP;
}



/**
 * @brief  Get the handler object of CAN interface
 * 
 * @retval Pointer of the CAN interface handler
 */
COMM_CAN_c *COMM_CAN_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief   Receive data from CAN interface
 * 
 * @param   interfaceType (COMM_Type_e) Type of the interface
 * @param   hcan (CAN_HandleTypeDef *) Handle of the CAN interface
 * @param   fifo (uint32_t) FIFO number of the CAN interface
 * @return  None
 * 
 * @warning You should never call this function directly
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
  if (canNodeList_.count(rxDataPack.stdID))
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
 * @return None
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
 * @brief  Start the CAN interface
 * 
 * @return None
 */
void COMM_CAN_c::Start(void)
{
  if (comState == COMM_RESET)
    return;

  HAL_CAN_Start((CAN_HandleTypeDef *)hInterface_);
  comState = COMM_IDLE;
}



/**
 * @brief  Stop the CAN interface
 * 
 * @return None
 */
void COMM_CAN_c::Stop(void)
{
  if (comState == COMM_RESET)
    return;

  HAL_CAN_Stop((CAN_HandleTypeDef *)hInterface_);
  comState = COMM_STOP;
}



/**
 * @brief  Add a CAN node to canNodeList
 * 
 * @param  canNode Pointer to the CAN node
 * @return None
 */
void COMM_CAN_c::AddCanNode(COMM_CAN_Node_c *canNode)
{
  if (canNode == nullptr || canNode->canStdID_ == NULL)
    return;
  
  canNodeList_.insert(std::pair<uint32_t, COMM_CAN_Node_c *>(canNode->canStdID_, canNode));
}



/**
 * @brief Delete a CAN node from canNodeList
 * 
 * @param  canNode Pointer to the CAN node
 * @return None
 */
void COMM_CAN_c::DelCanNode(COMM_CAN_Node_c *canNode)
{
  if (canNode == nullptr || canNode->canStdID_ == NULL)
    return;

  canNodeList_.erase(canNode->canStdID_);
}



/**
 * @brief  Set CAN receive filter
 * 
 * @param  sFilterConfig Pointer to the filter config
 * @return None
 */
void COMM_CAN_c::ConfigFilter(CAN_FilterTypeDef *sFilterConfig)
{
  HAL_CAN_ConfigFilter((CAN_HandleTypeDef *)hInterface_, sFilterConfig);

  if (sFilterConfig->FilterFIFOAssignment == CAN_RX_FIFO0)
    HAL_CAN_ActivateNotification((CAN_HandleTypeDef *)hInterface_, CAN_IT_RX_FIFO0_MSG_PENDING);

  else if (sFilterConfig->FilterFIFOAssignment == CAN_RX_FIFO1)
    HAL_CAN_ActivateNotification((CAN_HandleTypeDef *)hInterface_, CAN_IT_RX_FIFO1_MSG_PENDING);

}

} // namespace comm



extern "C" {

/**
 * @brief  Rewrite HAL_CAN_RxFifo0MsgPendingCallback
 * 
 * @param  hcan Handle of the CAN interface
 * @return None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  for (auto it : comm::CommList)
  {
    if (it.second->GetInterfaceHandler() == hcan)
    {
      it.second->Receive(comm::COMM_CAN, hcan, CAN_RX_FIFO0);

      break;
    }

  }

}



/**
 * @brief  Rewrite HAL_CAN_RxFifo1MsgPendingCallback
 * 
 * @param  hcan Handle of the CAN interface
 * @return None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  for (auto it : comm::CommList)
  {
    if (it.second->GetInterfaceHandler() == hcan)
    {
      it.second->Receive(comm::COMM_CAN, hcan, CAN_RX_FIFO1);

      break;
    }

  }

}

} // extern "C"

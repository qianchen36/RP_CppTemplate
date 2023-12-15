/**
 * @file    comm.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "comm.hpp"

namespace comm {

/* Communication Port List */
std::map<uint8_t, COMM_c *> CommList;



/**
 * @brief  Construct a new comm::COMM_InitParam_s struct
 * 
 * @return None
 */
_COMM_InitParam::_COMM_InitParam()
{
  comID = NULL;
  comType = COMM_UNDEF;
  hInterface = nullptr;
}



/**
 * @brief  Construct a new COMM_c::Comm_c object
 * 
 * @return None
 */
COMM_c::COMM_c()
{
  comID = NULL;
  comState = COMM_RESET;
  comType = COMM_UNDEF;

  initParam_  = nullptr;
  hInterface_ = nullptr;
}



/**
 * @brief Destroy the COMM_c::Comm_c object
 * 
 * @return None
 */
COMM_c::~COMM_c()
{
  if (comID != 0)
    DelCommPort(this);

  if (initParam_ != nullptr)
    delete initParam_;

}



/**
 * @brief  Initialize the communication interface
 * 
 * @param  initParam Pointer to the initialization parameters
 * @return None
 */
void COMM_c::InitComm(COMM_InitParam_s *initParam)
{
  /* Check initParam */
  if (initParam == nullptr)
    return;
  
  if (initParam->comID == NULL || initParam->hInterface == nullptr)
    return;

  /* Get parameters */
  initParam_ = new COMM_InitParam_s;
  memcpy(initParam_, initParam, sizeof(COMM_InitParam_s));

  auto param = (COMM_InitParam_s *)initParam_;

  /* Initialize */
  comID       = param->comID;
  hInterface_ = param->hInterface;

  /* Regist */
  AddCommPort(this);

  /* Update state */
  comState = COMM_STOP;
}



/**
 * @brief  Get the handler of communicate object
 * 
 * @return Reference of the communicate object
 */
COMM_c *COMM_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  Get the handler of communication interface
 * 
 * @return Pointer to the communication interface
 */
void *COMM_c::GetInterfaceHandler(void)
{ return hInterface_; }



/**
 * @brief  Add a new communication interface to the CommList
 * 
 * @param  comm Pointer to the communication interface
 * @return None
 */
void COMM_c::AddCommPort(COMM_c *comm)
{
  if (comm == nullptr || comm->comID == NULL)
    return;

  CommList.insert(std::pair<uint8_t, COMM_c *>(comm->comID, comm));
}



/**
 * @brief  Delete a communication interface from the CommList
 * 
 * @param  comm Pointer to the communication interface
 * @return None
 */
void COMM_c::DelCommPort(COMM_c *comm)
{
  if (comm == nullptr || comm->comID == NULL)
    return;

  CommList.erase(comm->comID);
}

} // namespace comm

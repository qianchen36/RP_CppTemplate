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
 * @brief  Construct a new COMM_c::Comm_c object
 * 
 * @retval None
 */
COMM_c::COMM_c()
{
  comID = NULL;
  comState = COMM_RESET;
  comType = COMM_UNDEF;

  hInterface_ = nullptr;
}



/**
 * @brief Destroy the COMM_c::Comm_c object
 * 
 * @retval None
 */
COMM_c::~COMM_c()
{
  if (comID != 0)
    DelCommPort(this);
}



/**
 * @brief  Initialize the communication interface
 * 
 * @param  id Set the communicate port ID
 * @param  hInterface Set the handle of the communication interface
 * @return None
 */
void COMM_c::Init(uint8_t id, void *hInterface)
{
  comID = id;
  hInterface_ = hInterface;

  AddCommPort(this);

  comState = COMM_STOP;
}



/**
 * @brief  Get the handler of communicate object
 * 
 * @retval Reference of the communicate object
 */
COMM_c &COMM_c::GetObjectHandler(void)
{
  return *this;
}



/**
 * @brief  Get the handler of communication interface
 * 
 * @return Pointer to the communication interface
 */
void *COMM_c::GetInterfaceHandler(void)
{
  return hInterface_;
}



/**
 * @brief  Add a new communication interface to the CommList
 * 
 * @param  comm Pointer to the communication interface
 * @retval None
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
 * @retval None
 */
void COMM_c::DelCommPort(COMM_c *comm)
{
  if (comm == nullptr || comm->comID == NULL)
    return;

  CommList.erase(comm->comID);
}

} // namespace comm

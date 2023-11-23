/**
 * @file    comm_spi.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "comm_spi.hpp"

#include <stdarg.h>

namespace comm {

/**
 * @brief  Construct a new comm spi c::comm spi c object
 * 
 * @return None
 */
COMM_SPI_c::COMM_SPI_c()
{
  comType = COMM_SPI;

  nssPort_ = nullptr;
  nssPin_  = NULL;
}



/**
 * @brief  Initialize the SPI interface
 * 
 * @param  id Set the communicate port ID
 * @param  hInterface Set the handle of the communication interface
 * @param  pStruct Set the SPI specific parameters
 * @return None
 */
void COMM_SPI_c::InitComm(uint8_t id, void *hInterface, ...)
{
  /* Check id and hInterface */
  if (id == NULL || hInterface == nullptr)
    return;

  /* Get args */
  va_list args;
  va_start(args, hInterface);

  comID       = id;
  hInterface_ = hInterface;

  auto pStruct = va_arg(args, COMM_SPI_InitParam_s *);

  ConfigNssPin(pStruct->nssPort, pStruct->nssPin);
  AddCommPort(this);

  /* Clean up */
  va_end(args);
  comState = COMM_STOP;
}



/**
 * @brief  Get the handler object of SPI interface
 * 
 * @return Reference of the SPI interface handler
 */
COMM_SPI_c *COMM_SPI_c::GetObjectHandler(void)
{
  return this;
}



/**
 * @brief  Receive data from SPI interface
 * 
 * @param  interfaceType (COMM_Type_e) Type of the interface
 * @param  pData (uint8_t *) Pointer to the data buffer
 * @param  size (uint16_t) Size of the data buffer
 * @param  timeout (uint32_t) Timeout value
 * @return None
 */
void COMM_SPI_c::Receive(int interfaceType, ...)
{
  /* Check interfaceType */
  if (interfaceType != COMM_SPI)
    return;

  /* Check comState */
  if (comState != COMM_RUN)
    return;

  /* Get args */
  va_list args;
  va_start(args, interfaceType);

  auto pData = va_arg(args, uint8_t *);
  auto size = va_arg(args, int);
  auto timeout = va_arg(args, int);

  /* Receive data */
  HAL_SPI_Receive((SPI_HandleTypeDef *)hInterface_, pData, size, timeout);

  /* Clean up */
  va_end(args);
}



/**
 * @brief  Transmit data from SPI interface
 * 
 * @param  interfaceType (COMM_Type_e) Type of the interface
 * @param  pData (uint8_t *) Pointer to the data buffer
 * @param  size (uint16_t) Size of the data buffer
 * @param  timeout (uint32_t) Timeout value
 * @return None
 */
void COMM_SPI_c::Transmit(int interfaceType, ...)
{
  /* Check interfaceType */
  if (interfaceType != COMM_SPI)
    return;

  /* Check comState */
  if (comState != COMM_RUN)
    return;

  /* Get args */
  va_list args;
  va_start(args, interfaceType);

  auto pData = va_arg(args, uint8_t *);
  auto size = va_arg(args, int);
  auto timeout = va_arg(args, int);

  /* Transmit data */
  HAL_SPI_Transmit((SPI_HandleTypeDef *)hInterface_, pData, size, timeout);

  /* Clean up */
  va_end(args);
}



/**
 * @brief  Set NSS pin value
 * 
 * @param  nssState GPIO_PIN_RESET for high, GPIO_PIN_SET for low
 * @return None
 */
void COMM_SPI_c::SetNssPin(COMM_SPI_NssStatus_e nssState)
{
  if (nssPort_ == nullptr || nssPin_ == NULL)
    return;

  if (nssState == COMM_SPI_NSS_RESET)
    HAL_GPIO_WritePin(nssPort_, nssPin_, GPIO_PIN_SET);

  else if (nssState == COMM_SPI_NSS_SET)
    HAL_GPIO_WritePin(nssPort_, nssPin_, GPIO_PIN_RESET);
}



/**
 * @brief  Configure the NSS pin of SPI interface
 * 
 * @param  GPIOx Set GPIO port of NSS pin
 * @param  GPIO_Pin Set GPIO pin of NSS pin
 * @return None
 */
void COMM_SPI_c::ConfigNssPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  nssPort_ = GPIOx;
  nssPin_  = GPIO_Pin;

  HAL_GPIO_WritePin(nssPort_, nssPin_, GPIO_PIN_RESET);
}

} // namespace comm

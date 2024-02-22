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

#include <cstdarg>

namespace rp {

namespace comm {

_COMM_SPI_InitParam::_COMM_SPI_InitParam()
{
  comType = COMM_SPI;

  useNssPin = DISABLE;
  nssPort   = nullptr;
  nssPin    = NULL;
}

/**
 * @brief  Construct a new comm spi c::comm spi c object
 * 
 * @return None
 */
COMM_SPI_c::COMM_SPI_c()
{
  comType = COMM_SPI;
}



/**
 * @brief  Initialize the SPI interface
 * 
 * @param  initParam Pointer to the initialization parameters
 * @return None
 */
void COMM_SPI_c::InitComm(COMM_InitParam_s *initParam)
{
  /* Check initParam */
  if (initParam == nullptr)
    return;

  if (initParam->comID == NULL || initParam->hInterface == nullptr)
    return;

  if (initParam->comType != COMM_SPI)
    return;

  /* Get parameters */
  if (initParam_ != nullptr)
    delete initParam_;

  initParam_ = new COMM_SPI_InitParam_s;
  memcpy(initParam_, initParam, sizeof(COMM_SPI_InitParam_s));

  /* Initialize */
  auto param  = (COMM_SPI_InitParam_s *)initParam_;
  comID       = param->comID;
  hInterface_ = param->hInterface;
  useNssPin_  = param->useNssPin;

  if (useNssPin_)   // NSS pin
    HAL_GPIO_WritePin(param->nssPort, param->nssPin, GPIO_PIN_RESET);

  /* Regist */
  AddCommPort(this);

  /* Update state */
  comState = COMM_STOP;
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
  if (!useNssPin_)
    return;

  auto param = (COMM_SPI_InitParam_s *)initParam_;

  if (nssState == COMM_SPI_NSS_RESET)
    HAL_GPIO_WritePin(param->nssPort, param->nssPin, GPIO_PIN_SET);

  else if (nssState == COMM_SPI_NSS_SET)
    HAL_GPIO_WritePin(param->nssPort, param->nssPin, GPIO_PIN_RESET);
}

} // namespace comm

} // namespace rp

/**
 * @file    comm_spi.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-23
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __COMM_SPI_HPP__
#define __COMM_SPI_HPP__

#include "comm.hpp"
#include "stm32f4xx_hal_spi.h"

namespace rp {

namespace comm {

typedef enum
{
  COMM_SPI_NSS_RESET,
  COMM_SPI_NSS_SET,

} COMM_SPI_NssStatus_e;

typedef struct _COMM_SPI_InitParam : public COMM_InitParam_s
{
  FUNC_STATE_e  useNssPin;
  GPIO_TypeDef *nssPort;
  uint16_t      nssPin;

  _COMM_SPI_InitParam();

} COMM_SPI_InitParam_s;



class COMM_SPI_c : public COMM_c
{
private:
  FUNC_STATE_e  useNssPin_;

public:
  COMM_SPI_c();

  void InitComm(COMM_InitParam_s *initParam) override;
  COMM_SPI_c *GetObjectHandler(void) override;
  void Receive(int interfaceType, ...) override;
  void Transmit(int interfaceType, ...) override;

  void SetNssPin(COMM_SPI_NssStatus_e nssState);

};

} // namespace comm

} // namespace rp

#endif // __COMM_SPI_HPP__
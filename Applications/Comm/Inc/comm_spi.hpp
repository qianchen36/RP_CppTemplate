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
#include "spi.h"

namespace comm {

typedef enum
{
  COMM_SPI_NSS_RESET,
  COMM_SPI_NSS_SET,

} COMM_SPI_NssStatus_e;

class COMM_SPI_c : public COMM_c
{
private:
  GPIO_TypeDef *nssPort_;
  uint16_t      nssPin_;

public:
  COMM_SPI_c();

  COMM_SPI_c &GetObjectHandler(void) override;
  void Receive(int interfaceType, ...) override;
  void Transmit(int interfaceType, ...) override;

  void ConfigNssPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
  void SetNssPin(COMM_SPI_NssStatus_e nssState);

};

}

#endif // __COMM_SPI_HPP__
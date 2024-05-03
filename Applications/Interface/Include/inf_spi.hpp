/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-04-03
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-04-03   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_INF_SPI_HPP
#define RP_INF_SPI_HPP

#include "inf_common.hpp"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_spi_ex.h"

namespace robotpilots {

class CSpiInterface : public CInfInstance {
public:
  struct SSpiInfInitParam : public SInfInitParam {
    SPI_HandleTypeDef *halSpiHandle = nullptr;    ///<
  };

  ERpStatus InitInterface(const SInfInitParam *pStruct) override;

  ERpStatus InitInterface(const SSpiInfInitParam *pStruct);

  ERpStatus Transmit(const DataBuffer<uint8_t> &buffer);

  ERpStatus Transmit(const uint8_t *buffer, size_t len);

  ERpStatus Receive(DataBuffer<uint8_t> &buffer);

  ERpStatus Receive(uint8_t *buffer, size_t len);

  ERpStatus TransmitReceive(const DataBuffer<uint8_t> &txBuffer, DataBuffer<uint8_t> &rxBuffer);

  ERpStatus TransmitReceive(const uint8_t *txBuffer, uint8_t *rxBuffer, size_t len);

private:
  SPI_HandleTypeDef *halSpiHandle = nullptr;

  void HeartbeatHandler_() override;
};

} // namespace robotpilots

#endif // RP_INF_SPI_HPP

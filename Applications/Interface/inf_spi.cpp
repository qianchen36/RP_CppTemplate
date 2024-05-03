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

#include "inf_spi.hpp"

namespace robotpilots {

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CSpiInterface::InitInterface(const SInfInitParam *pStruct) {

  return InitInterface(static_cast<const SSpiInfInitParam *>(pStruct));
}


/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CSpiInterface::InitInterface(const SSpiInfInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (interfaceState == RP_BUSY) return RP_ERROR;

  auto &param = *pStruct;
  interfaceId = param.interfaceId;
  halSpiHandle = param.halSpiHandle;

  RegisterInterface_();

  interfaceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 * @param buffer
 * @return
 */
ERpStatus CSpiInterface::Transmit(const DataBuffer<uint8_t> &buffer) {

  return Transmit(buffer.data(), buffer.size());
}


/**
 * @brief
 * @param buffer
 * @param len
 * @return
 */
ERpStatus CSpiInterface::Transmit(const uint8_t *buffer,
                                  size_t len) {

  if (interfaceState != RP_OK) return RP_ERROR;

  interfaceState = RP_BUSY;
  HAL_SPI_Transmit(halSpiHandle, const_cast<uint8_t *>(buffer), len, 10);
  interfaceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 * @param buffer
 * @return
 */
ERpStatus CSpiInterface::Receive(DataBuffer<uint8_t> &buffer) {

  return Receive(buffer.data(), buffer.size());
}


/**
 * @brief
 * @param buffer
 * @param len
 * @return
 */
ERpStatus CSpiInterface::Receive(uint8_t *buffer, size_t len) {

  if (interfaceState != RP_OK) return RP_ERROR;

  interfaceState = RP_BUSY;
  HAL_SPI_Receive(halSpiHandle, buffer, len, 10);
  interfaceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 * @param txBuffer
 * @param rxBuffer
 * @return
 */
ERpStatus CSpiInterface::TransmitReceive(const DataBuffer<uint8_t> &txBuffer,
                                         DataBuffer<uint8_t> &rxBuffer) {

  return TransmitReceive(txBuffer.data(), rxBuffer.data(), txBuffer.size());
}


/**
 * @brief
 * @param txBuffer
 * @param rxBuffer
 * @param len
 * @return
 */
ERpStatus CSpiInterface::TransmitReceive(const uint8_t *txBuffer,
                                         uint8_t *rxBuffer,
                                         size_t len) {

  if (interfaceState != RP_OK) return RP_ERROR;

  interfaceState = RP_BUSY;
  HAL_SPI_TransmitReceive(halSpiHandle,
                          const_cast<uint8_t *>(txBuffer), rxBuffer,
                          len, 10);
  interfaceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 */
void CSpiInterface::HeartbeatHandler_() {

}

} // namespace robotpilots


extern "C" {

/**
 * @brief
 * @param hspi
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {

  UNUSED(hspi);
}


/**
 * @brief
 * @param hspi
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {

  UNUSED(hspi);
}


/**
 * @brief
 * @param hspi
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

  UNUSED(hspi);
}


/**
 * @brief
 * @param hspi
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {

  UNUSED(hspi);
}

} // extern "C"

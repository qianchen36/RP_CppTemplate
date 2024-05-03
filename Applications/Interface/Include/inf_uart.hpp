/**
 * @file        inf_uart.hpp
 * @version     1.1
 * @date        2024-04-20
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       
 *
 * @details     UART Interface Class
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-17   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * <tr><td>2024-04-20   <td>1.1         <td>Morthine Xiang  <td>Finish Tx function.
 * </table>
 */

#ifndef RP_INF_UART_HPP
#define RP_INF_UART_HPP

#include "inf_common.hpp"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_uart_ex.h"

namespace robotpilots {

/**
 * @brief UART Interface Class
 */
class CUartInterface : public CInfInstance {
public:

  /**
   * @brief
   */
  struct SUartInfInitParam : public SInfInitParam {
    UART_HandleTypeDef *halUartHandle = nullptr;
    EFuncStatus useTxHandle = false;
    size_t txQueueLength = 4;
    size_t txBufferSize = 128;
    EFuncStatus useRxHandle = false;
    size_t rxQueueLength = 2;
    size_t rxBufferSize = 128;
  };

  /**
   * @brief
   */
  CUartInterface() {interfaceType = EInterfaceType::INF_UART; }

  /**
   * @brief
   */
  ~CUartInterface() override;

  /**
   * @brief
   * @param pStruct
   * @return
   */
  ERpStatus InitInterface(const SInfInitParam *pStruct) override;

  /**
   * @brief
   * @return
   */
  ERpStatus StartTransfer() override;

  /**
   * @brief
   * @return
   */
  ERpStatus StopTransfer() override;

  /**
   * @brief
   * @param buffer
   * @return
   */
  ERpStatus Transmit(const DataBuffer<uint8_t> &buffer);

  /**
   * @brief
   * @param buffer
   * @param len
   * @return
   */
  ERpStatus Transmit(const uint8_t *buffer, size_t len);

  /**
   * @brief
   * @param buffer
   * @return
   */
  ERpStatus Receive(DataBuffer<uint8_t> &buffer);

  /**
   * @brief
   * @param buffer
   * @param len
   * @return
   */
  ERpStatus Receive(uint8_t *buffer, size_t len);

  /**
   * @brief
   * @param callback
   * @return
   */
  ERpStatus RegisterCallback(const std::function<void(DataBuffer<uint8_t> &, size_t)> &callback);

  /**
   * @brief
   * @param callback
   * @return
   */
  ERpStatus ClearCallbackList();

  /**
   * @brief
   * @param handle
   */
  void _UART_HalTxCallback(UART_HandleTypeDef *handle);

  /**
   * @brief
   * @param handle
   * @param len
   */
  void _UART_HalRxCallback(UART_HandleTypeDef *handle);

  /**
   * @brief
   * @param handle
   */
  void _UART_HalErrorCallback(UART_HandleTypeDef *handle);

private:

  /**
   * @brief
   */
  UART_HandleTypeDef *halUartHandle_ = nullptr;

  /**
   * @brief
   */
  EFuncStatus useTxHandle_ = false, useRxHandle_ = false;

  /**
   * @brief
   */
  std::deque<DataBuffer<uint8_t>> txQueue_, rxQueue_;

  /**
   * @brief
   */
  size_t txQueueLength_ = 4, txBufferSize_ = 128, rxQueueLength_ = 2, rxBufferSize_ = 128;

  /**
   * @brief
   */
  std::deque<DataBuffer<uint8_t>>::iterator txQueueIt_, rxQueueIt_;

  /**
   * @brief
   */
  std::deque<std::function<void(DataBuffer<uint8_t> &, size_t)>> rxCallbackList_;

  /**
   * @brief
   */
  void HeartbeatHandler_() override;
};

} // namespace robotpilots

#endif // RP_INF_UART_HPP

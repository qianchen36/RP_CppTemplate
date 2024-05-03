/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-31
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-31   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "System.hpp"

extern UART_HandleTypeDef huart10;

namespace robotpilots {

/**
 * @brief
 * @param arg
 */
void StartUpdateTask(void *arg) {

  while (true) {

    /* Device Update */
    for (const auto &item : DeviceMap)
      item.second->UpdateHandler_();

    /* Module Update */
    for (const auto &item : ModuleMap)
      item.second->UpdateHandler_();


    vTaskDelay(pdMS_TO_TICKS(1));  // 1000Hz
  }
}

/**
 * @brief
 * @param arg
 */
void StartHeartbeatTask(void *arg) {

  while (true) {

    /* Interface Heartbeat */
    for (const auto &item : InterfaceMap)
      item.second->HeartbeatHandler_();

    /* Device Heartbeat */
    for (const auto &item : DeviceMap)
      item.second->HeartbeatHandler_();

    /* Module Heartbeat */
    for (const auto &item : ModuleMap)
      item.second->HeartbeatHandler_();

    vTaskDelay(pdMS_TO_TICKS(10));  // 100Hz
  }
}

/**
 * @brief
 * @param arg
 */
void StartMonitorTask(void *arg) {

  while (true) {

    vTaskDelay(pdMS_TO_TICKS(50));  // 20Hz
  }
}

} // namespace robotpilots
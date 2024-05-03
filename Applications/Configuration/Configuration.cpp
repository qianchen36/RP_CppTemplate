/**
 * @file        Configuration.cpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       User Application Configuration File
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-15   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#include "Configuration.hpp"

namespace robotpilots {

TaskHandle_t UpdateTaskHandle;
TaskHandle_t HeartbeatTaskHandle;
TaskHandle_t MonitorTaskHandle;

void StartUpdateTask(void* arg);
void StartHeartbeatTask(void* arg);
void StartMonitorTask(void* arg);

/**
 * @brief Initialize User Application
 *
 * @return None
 */
void ApplicationInitialize() {

  InitializeInterface();
  InitializeDevice();
  InitializeModule();
  InitializeSystem();

  xTaskCreate(StartUpdateTask, "Update Task",
              4096, nullptr, 24, &UpdateTaskHandle);
  xTaskCreate(StartHeartbeatTask, "Heartbeat Task",
              4096, nullptr, 4, &HeartbeatTaskHandle);
  xTaskCreate(StartMonitorTask, "Monitor Task",
              256, nullptr, 4, &MonitorTaskHandle);
}

} // namespace robotpilots

/**
 * @file        conf_system.cpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-15   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#include "conf_system.hpp"
#include "System.hpp"

namespace robotpilots {

TaskHandle_t SystemCoreTaskHandle;
TaskHandle_t SystemUpdateTaskHandle;

void StartSystemCoreTask(void *arg);
void StartSystemUpdateTask(void *arg);

/**
 * @brief
 * @return
 */
ERpStatus InitializeSystem() {

  /* Create System Core Task */
  xTaskCreate(StartSystemCoreTask, "System Core Task",
              1024, nullptr, 12, &SystemCoreTaskHandle);

  /* Create System Update Task */
  xTaskCreate(StartSystemUpdateTask, "System Update Task",
              1024, nullptr, 14, &SystemUpdateTaskHandle);

  static CSysRemote sys_remote;
  CSysRemote::SSysRemoteInitParam sys_remote_initparam;
  sys_remote_initparam.systemId = ESystemID::SYS_REMOTE;
  sys_remote_initparam.remoteDevID = EDeviceID::DEV_RC_DR16;
  sys_remote.InitSystem(&sys_remote_initparam);

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus InitializeMessageQueue() {

  return RP_OK;
}

} // namespace robotpilots

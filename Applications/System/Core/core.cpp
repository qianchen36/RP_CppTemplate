/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-27
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-27   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "System.hpp"
#include "Module.hpp"

namespace robotpilots {

/**
 * @brief
 * @param arg
 */
void StartSystemCoreTask(void *arg) {

  /* Create System Core */
  CSystemCore core;

  while (true) {

    vTaskDelay(pdMS_TO_TICKS(4));   // 250Hz
  }
}


} // namespace robotpilots

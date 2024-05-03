/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-22
 * @author      qianchen
 * @email
 * @brief
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-22   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_SYSTEM_HPP
#define RP_SYSTEM_HPP

#include "sys_common.hpp"
#include "sys_remote.hpp"

namespace robotpilots {

/**
 * @brief
 */
class CSystemCore {
  friend void StartSystemCoreTask(void *arg);
private:
  CSysRemote &remote = *static_cast<CSysRemote *>(SystemMap.at(ESystemID::SYS_REMOTE));
  void ControlFromRemote();
  void ControlFromKeyboard();
};

}

#endif // RP_SYSTEM_HPP

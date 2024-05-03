/**
 * @file        Interface.cpp
 * @version     1.0
 * @date        2024-03-17
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-17   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#include "Interface.hpp"

namespace robotpilots {

std::map<EInterfaceID, CInfInstance *> InterfaceMap;

/**
 * @brief
 * @return
 */
ERpStatus CInfInstance::RegisterInterface_() {

  if (interfaceId != EInterfaceID::INF_NULL) {
    InterfaceMap.insert(std::make_pair(interfaceId, this));
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @return
 */
ERpStatus CInfInstance::UnRegisterInterface_() {

  if (interfaceId != EInterfaceID::INF_NULL) {
    InterfaceMap.erase(interfaceId);
    return RP_OK;
  }

  return RP_ERROR;
}

} // namespace robotpilots

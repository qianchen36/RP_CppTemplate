/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-18
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-18   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "mtr/motor.hpp"

namespace robotpilots {

/**
 * @brief
 */
std::map<EDeviceID, CMtrInstance *> MotorMap;

/**
 * @brief
 * @return
 */
ERpStatus CMtrInstance::RegisterMotor_() {

  if (deviceId != EDeviceID::DEV_NULL) {
    MotorMap.insert(std::make_pair(deviceId, this));
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @return
 */
ERpStatus CMtrInstance::UnregisteredMotor_() {

  UnregisteredDevice_();

  if (deviceId != EDeviceID::DEV_NULL) {
    MotorMap.erase(deviceId);
    return RP_OK;
  }

  return RP_ERROR;
}

} // namespace robotpilots

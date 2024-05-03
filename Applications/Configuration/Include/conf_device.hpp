/**
 * @file        conf_device.hpp
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

#ifndef RP_CONF_DEVICE_HPP
#define RP_CONF_DEVICE_HPP

#include "conf_common.hpp"

namespace robotpilots {

/**
 * @brief Initialize Device Objects
 *
 * @return RP_OK - Initialize Succeeded
 * @return RP_ERROR - Initialize Failed
 */
ERpStatus InitializeDevice();

} // namespace robotpilots

#endif // RP_CONF_DEVICE_HPP

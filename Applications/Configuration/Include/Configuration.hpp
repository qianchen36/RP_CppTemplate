/**
 * @file        Configuration.hpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       User Application Configuration Header
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-15   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#ifndef RP_CONFIGURATION_HPP
#define RP_CONFIGURATION_HPP

#include "conf_common.hpp"
#include "conf_interface.hpp"
#include "conf_device.hpp"
#include "conf_module.hpp"
#include "conf_system.hpp"

namespace robotpilots {

/**
 * @brief Interface ID Enum
 */
enum class EInterfaceID {
  INF_NULL = -1,      ///< NULL
  INF_TEST = 0,       ///< Only Use for Test Interface
  INF_DBUS,           ///< DBUS Interface (Using USART5)
  INF_UART7,          ///< UART7 Interface (Used for Vision)
  INF_UART10,         ///< UART10 Interface (Used for Debugging)
  INF_UART11,         ///< UART11 Interface (Used for Referee System)
  INF_CAN1,           ///< CAN1 Interface (Used for Chassis Motors)
  INF_CAN2,           ///< CAN2 Interface (Used for Gantry Motors)
  INF_CAN3,           ///< CAN3 Interface (Used for Manipulator Motors)
  INF_SPI2,           ///< SPI2 Interface (Used for BMI-088)
  INF_SPI6,           ///< SPI6 Interface
};


/**
 * @brief Device ID Enum
 */
enum class EDeviceID {
  DEV_NULL = -1,          ///< NULL
  DEV_TEST = 0,           ///< Only Use for Test Device
  DEV_RC_DR16,            ///< Remote Control Receiver (DR16)
  DEV_MEMS_BMI088,        ///< 6-axis Mems (BMI-088)
};


/**
 * @brief Module ID Enum
 */
enum class EModuleID {
  MOD_NULL = -1,      ///< NULL
  MOD_TEST = 0,       ///< Only Use for Test Module
};


enum class ESystemID {
  SYS_NULL = -1,      ///< NULL
  SYS_TEST = 0,       ///< Only Use for Test System
  SYS_REMOTE,         ///< Remote Control System
  SYS_REFEREE,        ///< RoboMaster Referee System
  SYS_VISION,         ///< Vision System
};


/**
 * @brief Initialize User Application
 *
 * @return None
 */
void ApplicationInitialize();

} // namespace robotpilots

#endif  // RP_CONFIGURATION_HPP

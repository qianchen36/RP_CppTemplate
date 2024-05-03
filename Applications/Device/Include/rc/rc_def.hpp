/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-21
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-21   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_RC_DEF_HPP
#define RP_RC_DEF_HPP

namespace robotpilots {
/**
 * @brief
 */
enum class ERcType {
  UNDEF = -1,
  DR16,
};

/**
 * @brief
 */
enum class ERcStatus {
  RESET = -1,
  OFFLINE,
  ONLINE,
};

/**
 * @brief
 */
enum class EChannelType {
  UNDEF = -1,
  SWITCH,
  LEVER,
  BUTTON,
};

/**
 * @brief
 */
enum class EChannelStatus {
  RESET = -1,   ///<
  DOWN,         ///<
  HIGH,         ///<
  PRESS,        ///<
  L_PRESS,      ///<
};

}

#endif // RP_RC_DEF_HPP

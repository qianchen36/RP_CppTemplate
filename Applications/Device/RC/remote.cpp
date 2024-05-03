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

#include "rc/remote.hpp"

namespace robotpilots {

/**
 * @brief
 */
void CRcInstance::HeartbeatHandler_() {

  if (deviceState == RP_RESET) return;

  remoteState = ((HAL_GetTick() - lastHeartbeatTime_) > 500/10) ? ERcStatus::OFFLINE : ERcStatus::ONLINE;

  if (remoteState == ERcStatus::ONLINE)
    UpdateChannel_();
}

/**
 * @brief
 * @param num
 * @param type
 * @return
 */
ERpStatus CRcInstance::InitChannel_(size_t num, EChannelType type) {

  if (num > remoteData.size()) return RP_ERROR;

  remoteData[num].chType = type;
  remoteData[num].chState = EChannelStatus::RESET;
  remoteData[num].chValue = 0;
  remoteData[num].lastState_ = EChannelStatus::RESET;
  remoteData[num].lastValue_ = 0;
  remoteData[num].count_ = 0;

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus CRcInstance::UpdateChannel_() {

  if (deviceState == RP_RESET) return RP_ERROR;

  for (auto &item: remoteData) {
    item.chState = EChannelStatus::RESET;

    switch (item.chType) {
      case EChannelType::UNDEF:
        break;

      case EChannelType::SWITCH:
        break;

      case EChannelType::LEVER:
        item.chState = (item.chValue >  50) ? EChannelStatus::HIGH : item.chState;
        item.chState = (item.chValue < -50) ? EChannelStatus::DOWN : item.chState;
        break;

      case EChannelType::BUTTON:
        if (item.chValue) {
          item.count_ = (item.count_ > 500/10) ? 500/10 : item.count_ + 1;
          item.chState = (item.count_ > 50/10) ? EChannelStatus::PRESS : item.chState;
          item.chState = (item.count_ > 500/10) ? EChannelStatus::L_PRESS : item.chState;
        } else { item.count_ = 0; }
        break;
    }
    item.lastState_ = item.chState;
    item.lastValue_ = item.chValue;
  }

  return RP_OK;
}

} // namespace robotpilots

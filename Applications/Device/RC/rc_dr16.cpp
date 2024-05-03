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

#include "rc/rc_dr16.hpp"

namespace robotpilots {

/**
 * @brief
 */
static const EChannelType channelTypeList[] = {
    EChannelType::LEVER,   ///< DR16_CH_0
    EChannelType::LEVER,   ///< DR16_CH_1
    EChannelType::LEVER,   ///< DR16_CH_2
    EChannelType::LEVER,   ///< DR16_CH_3
    EChannelType::LEVER,   ///< DR16_CH_TW
    EChannelType::SWITCH,  ///< DR16_CH_SW1
    EChannelType::SWITCH,  ///< DR16_CH_SW2
    EChannelType::LEVER,   ///< DR16_CH_MOUSE_VX
    EChannelType::LEVER,   ///< DR16_CH_MOUSE_VY
    EChannelType::LEVER,   ///< DR16_CH_MOUSE_VZ
    EChannelType::BUTTON, ///< DR16_CH_MOUSE_L
    EChannelType::BUTTON, ///< DR16_CH_MOUSE_R
    EChannelType::BUTTON, ///< DR16_CH_KEY_W
    EChannelType::BUTTON, ///< DR16_CH_KEY_S
    EChannelType::BUTTON, ///< DR16_CH_KEY_A
    EChannelType::BUTTON, ///< DR16_CH_KEY_D
    EChannelType::BUTTON, ///< DR16_CH_KEY_SHIFT
    EChannelType::BUTTON, ///< DR16_CH_KEY_CTRL
    EChannelType::BUTTON, ///< DR16_CH_KEY_Q
    EChannelType::BUTTON, ///< DR16_CH_KEY_E
    EChannelType::BUTTON, ///< DR16_CH_KEY_R
    EChannelType::BUTTON, ///< DR16_CH_KEY_F
    EChannelType::BUTTON, ///< DR16_CH_KEY_G
    EChannelType::BUTTON, ///< DR16_CH_KEY_Z
    EChannelType::BUTTON, ///< DR16_CH_KEY_X
    EChannelType::BUTTON, ///< DR16_CH_KEY_C
    EChannelType::BUTTON, ///< DR16_CH_KEY_V
    EChannelType::BUTTON, ///< DR16_CH_KEY_B
};

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CRc_DR16::InitDevice(const SDevInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;

  auto &param = *static_cast<const SRcDR16InitParam *>(pStruct);
  deviceId = param.deviceId;
  uartInterface_ = static_cast<CUartInterface *>(InterfaceMap.at(param.interfaceId));

  remoteData.resize(COUNT_);
  for (size_t i = 0; i < COUNT_; i++)
    InitChannel_(i, channelTypeList[i]);

  auto callback = [this](auto &buffer, auto len) {
    if (len != 18) return;
    std::copy(buffer.data(), buffer.data() + 18, rxBuffer_);
    rxTimestamp_ = HAL_GetTick();
  };

  RegisterDevice_();
  uartInterface_->RegisterCallback(callback);

  deviceState = RP_OK;

  return RP_OK;
}

/**
 * @brief
 */
void CRc_DR16::UpdateHandler_() {

  /* Check Device State */
  if (deviceState == RP_RESET) return;

  /* Update Channel Data */
  if (rxTimestamp_ > lastHeartbeatTime_) {
    /* Decode from Rx Buffer */
    uint16_t key = rxBuffer_[14] | rxBuffer_[15] << 8;
    remoteData[CH_0].chValue = ((rxBuffer_[0] | rxBuffer_[1] << 8) & 0x07FF) - 1024;
    remoteData[CH_1].chValue = ((rxBuffer_[1] >> 3 | rxBuffer_[2] << 5) & 0x07FF) - 1024;
    remoteData[CH_2].chValue = ((rxBuffer_[2] >> 6 | rxBuffer_[3] << 2 | rxBuffer_[4] << 10) & 0x07FF) - 1024;
    remoteData[CH_3].chValue = ((rxBuffer_[4] >> 1 | rxBuffer_[5] << 7) & 0x07FF) - 1024;
    remoteData[CH_TW].chValue = ((rxBuffer_[16] | rxBuffer_[17] << 8) & 0x07FF) - 1024;
    remoteData[CH_SW1].chValue = ((rxBuffer_[5] >> 4) & 0x000C) >> 2;
    remoteData[CH_SW2].chValue = ((rxBuffer_[5] >> 4) & 0x0003);
    remoteData[CH_MOUSE_VX].chValue = rxBuffer_[6] | rxBuffer_[7] << 8;
    remoteData[CH_MOUSE_VY].chValue = rxBuffer_[8] | rxBuffer_[9] << 8;
    remoteData[CH_MOUSE_VZ].chValue = rxBuffer_[10] | rxBuffer_[11] << 8;
    remoteData[CH_MOUSE_L].chValue = rxBuffer_[12];
    remoteData[CH_MOUSE_R].chValue = rxBuffer_[13];
    remoteData[CH_KEY_W].chValue = (key >> 0) & 0x0001;
    remoteData[CH_KEY_S].chValue = (key >> 1) & 0x0001;
    remoteData[CH_KEY_A].chValue = (key >> 2) & 0x0001;
    remoteData[CH_KEY_D].chValue = (key >> 3) & 0x0001;
    remoteData[CH_KEY_SHIFT].chValue = (key >> 4) & 0x0001;
    remoteData[CH_KEY_CTRL].chValue = (key >> 5) & 0x0001;
    remoteData[CH_KEY_Q].chValue = (key >> 6) & 0x0001;
    remoteData[CH_KEY_E].chValue = (key >> 7) & 0x0001;
    remoteData[CH_KEY_R].chValue = (key >> 8) & 0x0001;
    remoteData[CH_KEY_F].chValue = (key >> 9) & 0x0001;
    remoteData[CH_KEY_G].chValue = (key >> 10) & 0x0001;
    remoteData[CH_KEY_Z].chValue = (key >> 11) & 0x0001;
    remoteData[CH_KEY_X].chValue = (key >> 12) & 0x0001;
    remoteData[CH_KEY_C].chValue = (key >> 13) & 0x0001;
    remoteData[CH_KEY_V].chValue = (key >> 14) & 0x0001;
    remoteData[CH_KEY_B].chValue = (key >> 15) & 0x0001;
    lastHeartbeatTime_ = rxTimestamp_;
  }
}

} // namespace robotpilots

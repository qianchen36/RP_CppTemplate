/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-04-30
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-04-30   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "dev_vision.hpp"

namespace robotpilots {

/**
 * @brief
 * @param pStruct
 *
 * @return
 */
ERpStatus CDevVision::InitDevice(const SDevInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (pStruct->deviceId == EDeviceID::DEV_NULL) return RP_ERROR;

  auto &param = *static_cast<const SDevVisionInitParam *>(pStruct);
  deviceId = param.deviceId;
  uartInterface_ = static_cast<CUartInterface *>(InterfaceMap.at(param.interfaceId));

  auto callback = [this](auto &buffer, auto len) {
    if (len > 256 + sizeof(SPkgHeader)) return;
    std::copy(buffer.data(), buffer.data() + len, rxBuffer_);
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
void CDevVision::UpdateHandler_() {

  if (deviceState == RP_RESET) return;

  if (rxTimestamp_ > lastHeartbeatTime_)
    ResolveRxPackage_();
}


/**
 * @brief
 */
void CDevVision::HeartbeatHandler_() {


}


/**
 * @brief
 */
ERpStatus CDevVision::ResolveRxPackage_() {

  if (deviceState == RP_RESET) return RP_ERROR;

  /* Get Package Header */
  auto header = reinterpret_cast<SPkgHeader *>(rxBuffer_);

  if (header->SOF != 0xA5) return RP_ERROR;
  if (CCrcValidator::Crc8Verify(rxBuffer_, header->CRC8, sizeof(SPkgHeader) - 1) != RP_OK)
    return RP_ERROR;

  switch (header->id) {
    case ID_ORETANK_INFO: {
      auto oreTankInfo = reinterpret_cast<SOretankinfoPkg *>(rxBuffer_);
      if (CCrcValidator::Crc16Verify(rxBuffer_, oreTankInfo->CRC16, sizeof(SOretankinfoPkg) - 2) != RP_OK)
        break;
      visionData.isFoundOreTank = oreTankInfo->isFoundOreTank;
      visionData.posit_X = oreTankInfo->posit[0];
      visionData.posit_Y = oreTankInfo->posit[1];
      visionData.posit_Z = oreTankInfo->posit[2];
      visionData.atti_Yaw = oreTankInfo->atti[0];
      visionData.atti_Pitch = oreTankInfo->atti[1];
      visionData.atti_Roll = oreTankInfo->atti[2];
      for (size_t i = 0; i < 5; i++) {
        visionData.uiPoint_X[i] = oreTankInfo->uiPoint_X[i];
        visionData.uiPoint_Y[i] = oreTankInfo->uiPoint_Y[i];
      }
      lastHeartbeatTime_ = HAL_GetTick();
      return RP_OK;
    }

    default:
      break;
  }

  return RP_ERROR;
}

} // namespace robotpilots

/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-19
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-19   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "mtr/mtr_dji.hpp"

namespace robotpilots {

/**
   * @brief
   * @param pStruct
   * @return
   */
ERpStatus CMtrDji::InitDevice(const SDevInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (pStruct->deviceId == EDeviceID::DEV_NULL) return RP_ERROR;

  auto &param = *static_cast<const SDjiMtrInitParam *>(pStruct);
  deviceId         = param.deviceId;
  useStallMonit_   = param.useStallMonit;
  useAngleToPosit_ = param.useAngleToPosit;
  canInterface_    = static_cast<CCanInterface *>(InterfaceMap.at(param.interfaceId));

  if (useStallMonit_) {
    stallMonitDataSrc_ = param.stallMonitDataSrc;
    stallThreshold_ = param.stallThreshold;
    stallTime_ = param.stallTime;
  }

  if (useAngleToPosit_) {
    encoderResolution_ = param.encoderResolution;
  }

  canRxNode_.InitRxNode(param.interfaceId, param.canStdID,
                        CCanInterface::ECanFrameType::DATA,
                        CCanInterface::ECanFrameDlc::DLC_8);

  RegisterDevice_();
  RegisterMotor_();

  deviceState = RP_OK;
  motorState = EMotorStatus::OFFLINE;

  return RP_OK;
}

/**
 * @brief
 */
void CMtrDji::HeartbeatHandler_() {

  if (motorState != EMotorStatus::RESET) {
    if (HAL_GetTick() - lastHeartbeatTime_ > 500 / 10)
      motorState = EMotorStatus::OFFLINE;
    else {
      motorState = (abs(motorData[DATA_SPEED]) < 50) ?
          EMotorStatus::STOP : EMotorStatus::RUNNING;
      if (useStallMonit_) {
        if (abs(motorData[stallMonitDataSrc_]) > static_cast<int32_t>(stallThreshold_)
            && motorState == EMotorStatus::STOP) {    // Stall Threshold
          stallCount_++;
          if (stallCount_ >= stallTime_ / 10) {     // Stall Time
            stallCount_ = stallTime_ / 10;
            motorState = EMotorStatus::STALL;
          }
        } else {
          stallCount_ = 0;
          motorState = EMotorStatus::RUNNING;
        }
      }
    }
  }
}

/**
 * @brief
 * @return
 */
int32_t CMtrDji::getPosition_() {

  if (motorState == EMotorStatus::OFFLINE) return 0;
  if (!useAngleToPosit_) return 0;

  if (motorData[DATA_POSIT] == 0 && lastAngle_ == 0) {
    lastAngle_ = motorData[DATA_ANGLE];
    return 0;
  }

  int32_t err = motorData[DATA_ANGLE] - lastAngle_;
  if (abs(err) > static_cast<int32_t>(encoderResolution_) / 2)
    err -= static_cast<int32_t>(encoderResolution_) * (err / abs(err));

  lastAngle_ = motorData[DATA_ANGLE];

  return (motorData[DATA_POSIT] + err);
}

} // namespace robotpilots

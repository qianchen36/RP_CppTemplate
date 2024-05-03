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

#include "sys_remote.hpp"

#include "FreeRTOS.h"
#include "task.h"

namespace robotpilots {

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CSysRemote::InitSystem(SSystemInitParam *pStruct) {

  return InitSystem(static_cast<const SSysRemoteInitParam *>(pStruct));
}

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CSysRemote::InitSystem(const SSysRemoteInitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;

  auto &param = *pStruct;
  systemID = param.systemId;
  remote_ = static_cast<CRcInstance *>(DeviceMap.at(param.remoteDevID));
//  referee_ = static_cast<CRefereeInstance *>(DeviceMap.at(param.refereeDevID));

  if (remoteInfoQueueHandle != nullptr)
    vQueueDelete(remoteInfoQueueHandle);
  remoteInfoQueueHandle = xQueueCreate(1, sizeof(SRemoteInfoPackage));

  RegisterSystem();

  return RP_OK;
}

/**
 * @brief
 */
void CSysRemote::UpdateHandler_() {

  info.remoteState = (remote_->remoteState == ERcStatus::ONLINE) ? RP_OK : RP_ERROR;
  info.ctrlSource = getControlSource_();

  UpdateRemote_();
  UpdateReferee_();
  UpdateKeyboard_();

  xQueueOverwrite(remoteInfoQueueHandle, &info);
}

/**
 * @brief
 * @return
 */
ERpStatus CSysRemote::UpdateRemote_() {

  if (remote_ == nullptr) return RP_ERROR;

  if (remote_->remoteData[CRc_DR16::CH_SW1].chValue == 2
      && remote_->remoteData[CRc_DR16::CH_SW2].chValue == 2
      && info.remote.switch_R != 2
      && info.remote.switch_R != 0) {
    __set_FAULTMASK(1);
    NVIC_SystemReset();
  }

  if (info.remoteState == RP_OK) {
    info.remote.joystick_LX = remote_->remoteData[CRc_DR16::CH_2].chValue;
    info.remote.joystick_LY = remote_->remoteData[CRc_DR16::CH_3].chValue;
    info.remote.joystick_RX = remote_->remoteData[CRc_DR16::CH_0].chValue;
    info.remote.joystick_RY = remote_->remoteData[CRc_DR16::CH_1].chValue;
    info.remote.thumbWheel = remote_->remoteData[CRc_DR16::CH_TW].chValue;
    info.remote.switch_L = remote_->remoteData[CRc_DR16::CH_SW1].chValue;
    info.remote.switch_R = remote_->remoteData[CRc_DR16::CH_SW2].chValue;
  } else {
    info.remote.joystick_LX = 0;
    info.remote.joystick_LY = 0;
    info.remote.joystick_RX = 0;
    info.remote.joystick_RY = 0;
    info.remote.thumbWheel = 0;
    info.remote.switch_L = 0;
    info.remote.switch_R = 0;
  }

  return RP_OK;
}

/**
 * @brief
 * @return
 */
ERpStatus CSysRemote::UpdateKeyboard_() {

  info.keyboard.speed_X = 0;
  info.keyboard.speed_Y = 0;
  info.keyboard.speed_W = 0;
  info.keyboard.speed_pitch = 0;
  info.keyboard.speed_thumb = 0;

  /* Soft Reset */
  if (remote_->remoteData[CRc_DR16::CH_KEY_CTRL].chValue == 1 &&
      remote_->remoteData[CRc_DR16::CH_KEY_SHIFT].chValue == 1 &&
      remote_->remoteData[CRc_DR16::CH_KEY_R].chValue == 1) {
    __set_FAULTMASK(1);
    NVIC_SystemReset();
  }

  if (remote_->remoteData[CRc_DR16::CH_KEY_CTRL].chValue == 1) {
    info.keyboard.speed_X += (remote_->remoteData[CRc_DR16::CH_KEY_A].chValue == 1) ? -220 : 0;
    info.keyboard.speed_X += (remote_->remoteData[CRc_DR16::CH_KEY_D].chValue == 1) ?  220 : 0;
    info.keyboard.speed_Y += (remote_->remoteData[CRc_DR16::CH_KEY_W].chValue == 1) ?  220 : 0;
    info.keyboard.speed_Y += (remote_->remoteData[CRc_DR16::CH_KEY_S].chValue == 1) ? -220 : 0;
  } else {
    info.keyboard.speed_X += (remote_->remoteData[CRc_DR16::CH_KEY_A].chValue == 1) ? -660 : 0;
    info.keyboard.speed_X += (remote_->remoteData[CRc_DR16::CH_KEY_D].chValue == 1) ?  660 : 0;
    info.keyboard.speed_Y += (remote_->remoteData[CRc_DR16::CH_KEY_W].chValue == 1) ?  660 : 0;
    info.keyboard.speed_Y += (remote_->remoteData[CRc_DR16::CH_KEY_S].chValue == 1) ? -660 : 0;
  }

  info.keyboard.speed_W = remote_->remoteData[CRc_DR16::CH_MOUSE_VX].chValue;
  info.keyboard.speed_pitch = remote_->remoteData[CRc_DR16::CH_MOUSE_VY].chValue;
  info.keyboard.speed_thumb = remote_->remoteData[CRc_DR16::CH_MOUSE_VZ].chValue;

  info.keyboard.mouse_L = (remote_->remoteData[CRc_DR16::CH_MOUSE_L].chValue == 1);
  info.keyboard.mouse_R = (remote_->remoteData[CRc_DR16::CH_MOUSE_R].chValue == 1);
  info.keyboard.key_W = (remote_->remoteData[CRc_DR16::CH_KEY_W].chValue == 1);
  info.keyboard.key_A = (remote_->remoteData[CRc_DR16::CH_KEY_A].chValue == 1);
  info.keyboard.key_S = (remote_->remoteData[CRc_DR16::CH_KEY_S].chValue == 1);
  info.keyboard.key_D = (remote_->remoteData[CRc_DR16::CH_KEY_D].chValue == 1);
  info.keyboard.key_Q = (remote_->remoteData[CRc_DR16::CH_KEY_Q].chValue == 1);
  info.keyboard.key_E = (remote_->remoteData[CRc_DR16::CH_KEY_E].chValue == 1);
  info.keyboard.key_R = (remote_->remoteData[CRc_DR16::CH_KEY_R].chValue == 1);
  info.keyboard.key_F = (remote_->remoteData[CRc_DR16::CH_KEY_F].chValue == 1);
  info.keyboard.key_G = (remote_->remoteData[CRc_DR16::CH_KEY_G].chValue == 1);
  info.keyboard.key_Z = (remote_->remoteData[CRc_DR16::CH_KEY_Z].chValue == 1);
  info.keyboard.key_X = (remote_->remoteData[CRc_DR16::CH_KEY_X].chValue == 1);
  info.keyboard.key_C = (remote_->remoteData[CRc_DR16::CH_KEY_C].chValue == 1);
  info.keyboard.key_V = (remote_->remoteData[CRc_DR16::CH_KEY_V].chValue == 1);
  info.keyboard.key_B = (remote_->remoteData[CRc_DR16::CH_KEY_B].chValue == 1);
  info.keyboard.key_Ctrl = (remote_->remoteData[CRc_DR16::CH_KEY_CTRL].chValue == 1);
  info.keyboard.key_Shift = (remote_->remoteData[CRc_DR16::CH_KEY_SHIFT].chValue == 1);

  if (info.keyboard.key_Shift) {
    info.keyboard.speed_X = 0;
    info.keyboard.speed_Y = 0;
  }

  /* Scan Keyboard Shortcuts */
  if (remote_->remoteData[CRc_DR16::CH_KEY_CTRL].chValue == 1) {
    info.keyboard.shortCut_CtrlR = (remote_->remoteData[CRc_DR16::CH_KEY_R].chValue == 1);
    info.keyboard.shortCut_CtrlF = (remote_->remoteData[CRc_DR16::CH_KEY_F].chValue == 1);
    info.keyboard.shortCut_CtrlG = (remote_->remoteData[CRc_DR16::CH_KEY_G].chValue == 1);
    info.keyboard.shortCut_CtrlZ = (remote_->remoteData[CRc_DR16::CH_KEY_Z].chValue == 1);
    info.keyboard.shortCut_CtrlX = (remote_->remoteData[CRc_DR16::CH_KEY_X].chValue == 1);
    info.keyboard.shortCut_CtrlC = (remote_->remoteData[CRc_DR16::CH_KEY_C].chValue == 1);
    info.keyboard.shortCut_CtrlV = (remote_->remoteData[CRc_DR16::CH_KEY_V].chValue == 1);
    info.keyboard.shortCut_CtrlB = (remote_->remoteData[CRc_DR16::CH_KEY_B].chValue == 1);
  } else {
    info.keyboard.shortCut_CtrlR = false;
    info.keyboard.shortCut_CtrlZ = false;
    info.keyboard.shortCut_CtrlX = false;
    info.keyboard.shortCut_CtrlC = false;
    info.keyboard.shortCut_CtrlV = false;
    info.keyboard.shortCut_CtrlB = false;
  }

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CSysRemote::UpdateReferee_() {

  // TODO: Update Referee Info
  return RP_OK;
}


/**
 * @brief
 * @return
 */
CSysRemote::ECtrlSource CSysRemote::getControlSource_() {

  if (info.remoteState != RP_OK) return ECtrlSource::REMOTE_ONLY;

  if (info.remote.switch_L == 2 && info.remote.switch_R == 1) return ECtrlSource::KEYBOARD_REMOTE;
  if (info.remote.switch_L == 2 && info.remote.switch_R == 2) return ECtrlSource::KEYBOARD_REFEREE;

  return ECtrlSource::REMOTE_ONLY;
}

} // namespace robotpilots

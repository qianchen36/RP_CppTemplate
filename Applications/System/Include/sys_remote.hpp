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

#ifndef RP_SYS_REMOTE_HPP
#define RP_SYS_REMOTE_HPP

#include "sys_common.hpp"
#include "Device.hpp"

namespace robotpilots {

/**
 * @brief
 */
class CSysRemote : public CSysInstance {
public:
  /**
   * @brief
   */
  struct SSysRemoteInitParam : public SSystemInitParam {
    EDeviceID remoteDevID = EDeviceID::DEV_NULL;
    EDeviceID refereeDevID = EDeviceID::DEV_NULL;
  };

  /**
   * @brief
   */
  enum class ECtrlSource {
    REMOTE_ONLY,        ///< Remote control only
    KEYBOARD_REMOTE,    ///< Keyboard control from remote tunnel
    KEYBOARD_REFEREE,   ///< Keyboard control from referee tunnel
  };

  /**
   * @brief
   */
  struct SRemoteInfo {
    int16_t joystick_LX = 0;
    int16_t joystick_LY = 0;
    int16_t joystick_RX = 0;
    int16_t joystick_RY = 0;
    int16_t thumbWheel = 0;
    uint8_t switch_L = 0;
    uint8_t switch_R = 0;
  };

  /**
   * @brief
   */
  struct SKeyboardInfo {
    int16_t speed_X = 0;
    int16_t speed_Y = 0;
    int16_t speed_W = 0;
    int16_t speed_pitch = 0;
    int16_t speed_thumb = 0;
    bool mouse_L = false;
    bool mouse_R = false;
    bool key_W = false;
    bool key_A = false;
    bool key_S = false;
    bool key_D = false;
    bool key_Q = false;
    bool key_E = false;
    bool key_R = false;
    bool key_F = false;
    bool key_G = false;
    bool key_Z = false;
    bool key_X = false;
    bool key_C = false;
    bool key_V = false;
    bool key_B = false;
    bool key_Ctrl = false;
    bool key_Shift = false;
    bool shortCut_CtrlR = false;
    bool shortCut_CtrlF = false;
    bool shortCut_CtrlG = false;
    bool shortCut_CtrlZ = false;
    bool shortCut_CtrlX = false;
    bool shortCut_CtrlC = false;
    bool shortCut_CtrlV = false;
    bool shortCut_CtrlB = false;
  };

  /**
   * @brief
   */
  struct SCustomInfo {

  };

  /**
   * @brief
   */
  struct SRemoteInfoPackage {

    ERpStatus remoteState = RP_RESET;

    ERpStatus refereeState = RP_RESET;

    ECtrlSource ctrlSource = ECtrlSource::REMOTE_ONLY;

    SRemoteInfo remote;

    SKeyboardInfo keyboard;

    SCustomInfo custom;

  } info;

  QueueHandle_t remoteInfoQueueHandle = nullptr;

  /**
   * @brief
   * @param pStruct
   * @return
   */
  ERpStatus InitSystem(SSystemInitParam *pStruct) override;

  /**
   * @brief
   * @param pStruct
   * @return
   */
  ERpStatus InitSystem(const SSysRemoteInitParam *pStruct);

private:
  /**
   * @brief
   */
  CRcInstance *remote_;

//  CDevReferee *referee_;

  /**
   * @brief
   */
  void UpdateHandler_() override;

  /**
   * @brief
   * @return
   */
  ERpStatus UpdateRemote_();

  /**
   * @brief
   * @return
   */
  ERpStatus UpdateKeyboard_();

  /**
   * @brief
   * @return
   */
  ERpStatus UpdateReferee_();

  /**
   * @brief
   * @return
   */
  ECtrlSource getControlSource_();
};

} // namespace robotpilots

#endif // RP_SYS_REMOTE_HPP

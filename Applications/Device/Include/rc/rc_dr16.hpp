/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-20
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-20   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */
#ifndef RP_RC_DR16_HPP
#define RP_RC_DR16_HPP

#include "rc_common.hpp"
#include "inf_uart.hpp"

namespace robotpilots {

class CRc_DR16 : public CRcInstance {
public:

  /**
   * @brief
   */
  using SRcDR16InitParam = SRcInitParam;

  /**
   * @brief
   */
  enum EChannelList {
    CH_0 = 0,
    CH_1,
    CH_2,
    CH_3,
    CH_TW,
    CH_SW1,
    CH_SW2,
    CH_MOUSE_VX,
    CH_MOUSE_VY,
    CH_MOUSE_VZ,
    CH_MOUSE_L,
    CH_MOUSE_R,
    CH_KEY_W,
    CH_KEY_S,
    CH_KEY_A,
    CH_KEY_D,
    CH_KEY_SHIFT,
    CH_KEY_CTRL,
    CH_KEY_Q,
    CH_KEY_E,
    CH_KEY_R,
    CH_KEY_F,
    CH_KEY_G,
    CH_KEY_Z,
    CH_KEY_X,
    CH_KEY_C,
    CH_KEY_V,
    CH_KEY_B,
    COUNT_,
  };

  /**
   * @brief
   */
  CRc_DR16() { remoteType = ERcType::DR16; }

  /**
   * @brief
   */
  ~CRc_DR16() override = default;

  /**
   * @brief
   * @param pStruct
   * @return
   */
  ERpStatus InitDevice(const SDevInitParam *pStruct) override;

private:
  /**
   * @brief
   */
  CUartInterface *uartInterface_ = nullptr;

  /**
   * @brief
   */
  uint32_t rxTimestamp_ = 0;

  /**
   * @brief
   */
  uint8_t rxBuffer_[18] = {0};

  /**
   * @brief
   */
  void UpdateHandler_() override;
};

}

#endif // RP_RC_DR16_HPP

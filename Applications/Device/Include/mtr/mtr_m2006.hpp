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

#ifndef RP_MTR_M2006_HPP
#define RP_MTR_M2006_HPP

#include "mtr_dji.hpp"

namespace robotpilots {

class CMtr_M2006 : public CMtrDji {
private:
  /**
   * @brief
   */
  void UpdateHandler_() override {

    if (deviceState == RP_RESET) return;

    /* Update Motor Data */
    if (canRxNode_.timestamp >= lastHeartbeatTime_) {
      motorData[DATA_ANGLE]  = (int16_t)(canRxNode_.dataBuffer[0] << 8 | canRxNode_.dataBuffer[1]);
      motorData[DATA_SPEED]  = (int16_t)(canRxNode_.dataBuffer[2] << 8 | canRxNode_.dataBuffer[3]);
      motorData[DATA_TORQUE] = (int16_t)(canRxNode_.dataBuffer[4] << 8 | canRxNode_.dataBuffer[5]);
      motorData[DATA_POSIT]  = (useAngleToPosit_) ? getPosition_() : 0;
      lastHeartbeatTime_  = canRxNode_.timestamp;
    }
  }

public:
  /**
   * @brief
   */
  using SMtrM2006InitParam = SDjiMtrInitParam;

  /**
   * @brief
   */
  CMtr_M2006() { motorType = EMotorType::MTR_M2006; }

};

} // namespace robotpilots

#endif // RP_MTR_M2006_HPP

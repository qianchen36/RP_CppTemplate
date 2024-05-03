/**
 * @file        algo_pid.hpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-15   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#ifndef RP_ALGO_PID_HPP
#define RP_ALGO_PID_HPP

#include "algo_common.hpp"

namespace robotpilots {

/**
 * @brief PID Controller Class
 * @details By Default, The Input is Considered As Error Value
 * @note Only Implemented Positional PID
 */
class CPidController : public CAlgoInstance {
public:

  /**
   * @brief PID Controller Error Value Calculate Mode Enum
   */
  enum class EPidErrMode {
    NORMAL,   ///< Calculate Error Value Without Special Handling
    ANGLE,    ///< Error Value in the Range of 0.0f ~ 360.0f
    RING,     ///< Error Value in User Defined Range
  };

  /**
   * @brief PID Controller Initialize Parameter Structure
   * @note
   */
  struct SPidControllerInitParam : public SAlgoInitParam {
    float_t kp = 0.0f;                          ///< Proportional Factor (Default Value: 0.0f)
    float_t ki = 0.0f;                          ///< Integral Factor (Default Value: 0.0f)
    float_t kd = 0.0f;                          ///< Derivative Factor (Default Value: 0.0f)
    uint16_t deadband = 0;                      ///< Input Deadband (Default Value: 0)
    uint16_t maxInteger = 0;                    ///< Max Integral Value (Default Value: 0)
    uint16_t maxOutput = 0;                     ///< Max Output Value (Default Value: 0)
    uint16_t ringModeErrRange = 8192;           ///< Define Error Value Range When errMode=RING (Default Value: 8192)
    EPidErrMode errMode = EPidErrMode::NORMAL;  ///< Error Value Calculate Mode （Default Value: NORMAL)
  };

  /**
   * @brief Initialize PID Controller Object
   * @param pStruct[in] Pointer to Initialize Parameter Structure
   *
   * @returns RP_OK - Initialize Succeeded \n
   *          RP_ERROR - Initialize Failed
   */
  ERpStatus InitAlgorithm(const SAlgoInitParam *pStruct) override;

  /**
   * @brief Update PID Controller Output
   * @details Calculate Output
   * @note If Registered, Use Registered Input & Output Sources for Calculate. \n
   *       Else, Update Input Data Buffer Manually Before Call This Function.
   *
   * @returns RP_OK - Update Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Update Failed
   */
  ERpStatus UpdateAlgorithm() override;

  /**
   * @brief Clear PID Controller Data Buffer
   * @details Clear the Buffer to Avoid Impact in Output Data
   *
   * @returns RP_OK - Reset Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Reset Failed
   */
  ERpStatus ResetAlgorithm() override;

  /**
   * @brief Set PID Controller Input Data Buffer
   * @param input
   * @param output
   * @return
   */
  DataBuffer<float_t> UpdatePidController(const DataBuffer<float_t> &target, const DataBuffer<float_t> &measure);

  /**
   * @brief Set Maximum Output Value
   * @param value
   *
   * @return RP_OK - Set Succeeded \n
   *         RP_BUSY - Algorithm Object is Busy \n
   *         RP_ERROR - Set Failed
   */
  ERpStatus SetMaxOutputValue(uint16_t value);

  /**
   * @brief Calculate Error Value
   * @details If Calculate Failed, The Return Buffer is Empty
   * @param target[in] Target Value
   * @param measure[in] Measured Value
   *
   * @return Error Value Buffer
   */
  DataBuffer<float_t> CalculateErrorValue(const DataBuffer<float_t> &target, const DataBuffer<float_t> &measure);

private:

  /**
   * @brief PID Controller Calculate Thread Info Structure
   */
  struct SPidThreadInfo {
    float_t pOut = 0.0f;        ///< Output From Proportional
    float_t iOut = 0.0f;        ///< Output From Integral
    float_t dOut = 0.0f;        ///< Output From Derivative
    float_t integer = 0.0f;     ///< Integral Cache
    float_t derivative = 0.0f;  ///< Derivative Cache
    float_t lastErr = 0.0f;     ///< Last Error Value
  };

  /**
   * @brief PID Factors
   */
  float_t kp_ = 0.0f, ki_ = 0.0f, kd_ = 0.0f;

  /**
   * @brief PID Supplementary Parameters
   */
  uint16_t deadband_ = 0, maxInteger_ = 0, maxOutput_ = 0, ringModeErrRange_ = 8192;

  /**
   * @brief PID Controller Error Value Calculate Mode
   */
  EPidErrMode errMode_ = EPidErrMode::NORMAL;

  /**
   * @brief PID Controller Thread Info Buffer
   */
  DataBuffer<SPidThreadInfo> threadInfo_;

  /**
   * @brief Calculate PID Controller Output
   * @param err[in] Error Value
   * @param info[in|out] PID Thread Information Structure
   *
   * @return Output Value
   */
  float_t Calculate_(const float_t err, SPidThreadInfo &info);
};

} // namespace robotpilots

#endif // RP_ALGO_PID_HPP

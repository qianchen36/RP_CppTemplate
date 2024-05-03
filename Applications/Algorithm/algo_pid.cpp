/**
 * @file        algo_pid.cpp
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

#include "algo_pid.hpp"

namespace robotpilots {

/**
 * @brief Initialize PID Controller Object
   * @param pStruct[in] Pointer to Initialize Parameter Structure
 *
 * @returns RP_OK - Initialize Succeeded \n
 *          RP_ERROR - Initialize Failed
 */
ERpStatus CPidController::InitAlgorithm(const SAlgoInitParam *pStruct) {

  /* Check State */
  if (pStruct == nullptr) return RP_ERROR;
  if (algoState == RP_BUSY) return RP_ERROR;

  /* Initialize PID Controller */
  auto &param = *static_cast<const SPidControllerInitParam *>(pStruct);
  algoTickRate      = param.tickRate;
  algoThreadNum     = param.threadNum;
  kp_               = param.kp;
  ki_               = param.ki;
  kd_               = param.kd;
  deadband_         = param.deadband;
  maxInteger_       = param.maxInteger;
  maxOutput_        = param.maxOutput;
  errMode_          = param.errMode;
  ringModeErrRange_ = param.ringModeErrRange;

  /* Initialize Data Buffer */
  ResetAlgorithm();

  /* Update State */
  algoState = RP_OK;

  return RP_OK;
}

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
ERpStatus CPidController::UpdateAlgorithm() {

  /* Check PID Controller State */
  if (algoState == RP_RESET) return RP_ERROR;
  if (algoState == RP_BUSY) return  RP_BUSY;

  algoState = RP_BUSY;  // Set State

  /* Call Get Input Data Callback */
  if (callbackRegisterState_)
    input_ = getInput_();

  /* Check Input Data Size */
  if (input_.size() != algoThreadNum) return RP_ERROR;

  /* Calculate Output */
  for (size_t i = 0; i < algoThreadNum; i++)
    output_[i] = Calculate_(input_[i], threadInfo_[i]);

  /* Call Set Output Data Callback */
  if (callbackRegisterState_)
    setOutput_(output_);

  algoState = RP_OK;    // Reset State

  return RP_OK;
}

/**
 * @brief Clear PID Controller Data Buffer
 * @details Clear the Buffer to Avoid Impact in Output Data
 *
 * @returns RP_OK - Reset Succeeded \n
 *          RP_BUSY - Algorithm Object is Busy \n
 *          RP_ERROR - Reset Failed
 */
ERpStatus CPidController::ResetAlgorithm() {

  /* Check Algorithm State */
  if (algoState == RP_BUSY) return RP_BUSY;
  if (algoState == RP_RESET) return  RP_ERROR;

  /* Reset Data Buffer */
  input_      = DataBuffer<float_t>(algoThreadNum, 0.0f);
  output_     = DataBuffer<float_t>(algoThreadNum, 0.0f);
  threadInfo_ = DataBuffer<SPidThreadInfo>(algoThreadNum);

  return RP_OK;
}

/**
 * @brief Update PID Controller Output
 * @param input
 * @param output
 * @return
 */
DataBuffer<float_t> CPidController::UpdatePidController(const DataBuffer<float_t> &target,
                                                        const DataBuffer<float_t> &measure) {

  DataBuffer<float_t> output(algoThreadNum, 0.0f);

  /* Check State */
  if (algoState == RP_RESET) return output;
  if (algoState == RP_BUSY) return output;

  /* Check Input Data Size */
  if (target.size() != algoThreadNum) return output;
  if (measure.size() != algoThreadNum) return output;

  /* Calculate Error Value */
  auto error = CalculateErrorValue(target, measure);

  /* Calculate Output */
  for (size_t i = 0; i < algoThreadNum; i++)
    output[i] = Calculate_(error[i], threadInfo_[i]);

  return output;
}

/**
 * @brief Set Maximum Output Value
 * @param value
 *
 * @return RP_OK - Set Succeeded \n
 *         RP_BUSY - Algorithm Object is Busy \n
 *         RP_ERROR - Set Failed
 */
ERpStatus CPidController::SetMaxOutputValue(uint16_t value) {

  /* Check State */
  if (algoState == RP_RESET) return RP_ERROR;
  if (algoState == RP_BUSY) return RP_BUSY;

  maxOutput_ = value;

  return RP_OK;
}

/**
 * @brief Calculate Error Value
 * @details If Calculate Failed, The Return Buffer is Empty
 * @param target[in] Target Value
 * @param measure[in] Measured Value
 *
 * @return Error Value Buffer
 */
DataBuffer<float_t> CPidController::CalculateErrorValue(const DataBuffer<float_t> &target,
                                                        const DataBuffer<float_t> &measure) {

  DataBuffer<float_t> error(algoThreadNum, 0.0f);

  /* Check State */
  if (algoState == RP_RESET) return error;

  /* Check Input Data Size */
  if (target.size() != algoThreadNum) return error;
  if (measure.size() != algoThreadNum) return error;

  /* Calculate Error Value */
  switch (errMode_) {

    case EPidErrMode::NORMAL:
      for (size_t i = 0; i < algoThreadNum; i++)
        error[i] = target[i] - measure[i];
      break;

    case EPidErrMode::ANGLE:
      for (size_t i = 0; i < algoThreadNum; i++) {
        error[i] = target[i] - measure[i];
        if (abs(error[i]) > 180.0f)   // Zero Cross Handle
          error[i] -= std::copysign(360.0f, error[i]);
      }
      break;

    case EPidErrMode::RING:
      for (size_t i = 0; i < algoThreadNum; i++) {
        error[i] = target[i] - measure[i];
        if (abs(error[i]) > static_cast<float_t>(ringModeErrRange_) / 2)   // Zero Cross Handle
          error[i] -= std::copysign(static_cast<float_t>(ringModeErrRange_), error[i]);
      }
      break;
  }

  return error;
}

/**
 * @brief Calculate PID Controller Output
 * @param err[in] Error Value
 * @param info[in|out] PID Thread Information Structure
 *
 * @return Output Value
 */
float_t CPidController::Calculate_(const float_t err,
                                   SPidThreadInfo &info) {

  /* Check Deadband */
  if (deadband_ != 0 && abs(err) < static_cast<float_t>(deadband_))
    return 0.0f;

  /* Calculate Integral */
  info.integer += err / static_cast<float_t>(algoTickRate);
  info.integer = std::clamp(info.integer, static_cast<float_t>(-maxInteger_), static_cast<float_t>(maxInteger_));

  /* Calculate Derivative */
  info.derivative = (err - info.lastErr) / static_cast<float_t>(algoTickRate);

  /* Calculate Output */
  info.pOut = kp_ * err;
  info.iOut = ki_ * info.integer;
  info.dOut = kd_ * info.derivative;

  auto out = info.pOut + info.iOut + info.dOut;
  out = std::clamp(out, static_cast<float_t>(-maxOutput_), static_cast<float_t>(maxOutput_));

  /* Update Last Error Value */
  info.lastErr = err;

  return out;
}

} // namespace robotpilots

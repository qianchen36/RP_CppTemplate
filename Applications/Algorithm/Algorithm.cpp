/**
 * @file        Algorithm.cpp
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

#include "Algorithm.hpp"

namespace robotpilots {

/**
 * @brief Register Input & Output Data Source Callback Function
 * @param input Callback Function for Input
 * @param output Callback Function for Output
 *
 * @returns RP_OK - Register Succeeded \n
 *          RP_BUSY - Algorithm Object is Busy \n
 *          RP_ERROR - Register Failed
 */
ERpStatus CAlgoInstance::RegisterInputOutputDataSource(InputCallback &input,
                                                       OutputCallback &output) {

  /* Check Algorithm Object State */
  if (algoState == RP_BUSY) return RP_BUSY;
  if (algoState == RP_RESET) return RP_ERROR;

  /* Register Callbacks */
  getInput_ = input;
  setOutput_ = output;
  callbackRegisterState_ = true;

  return RP_OK;
}

/**
 * @brief Unregister Input & Output Data Source Callback Function
 *
 * @return RP_OK - Unregister Succeeded \n
 *         RP_BUSY - Algorithm Object is Busy \n
 *         RP_ERROR - Unregister Failed
 */
ERpStatus CAlgoInstance::UnregisteredInputOutputDataSource() {

  /* Clear Register Flag */
  callbackRegisterState_ = false;

  return RP_OK;
}

/**
 * @brief Set Algorithm Input Data for Next Update
 * @param in[in] Input Data Vector in float_t Type
 *
 * @returns RP_OK - Set Input Succeeded \n
 *          RP_BUSY - Algorithm Object is Busy \n
 *          RP_ERROR - Set Input Failed
 */
ERpStatus CAlgoInstance::setInput(const DataBuffer<float_t> &in) {

  /* Check Algorithm Object State */
  if (algoState == RP_RESET) return RP_ERROR;
  if (algoState == RP_BUSY) return RP_BUSY;

  /* Copy Data to Input Data Buffer */
  input_ = in;
  return RP_OK;
}

/**
 * @brief Get Algorithm Output Data in Last Update
 * @param out[out] Output Data Vector in float_t Type
 *
 * @returns RP_OK - Get Output Succeeded \n
 *          RP_BUSY - Algorithm Object is Busy \n
 *          RP_ERROR - Get Output Failed
 */
ERpStatus CAlgoInstance::getOutput(DataBuffer<float_t> &out) {

  /* Check Algorithm Object State */
  if (algoState == RP_RESET) return RP_ERROR;
  if (algoState == RP_BUSY) return RP_BUSY;

  /* Copy Data from Output Data Buffer */
  out = output_;
  return RP_OK;
}

/**
 * @brief Get Algorithm Output Data in Last Update
 *
 * @return Output Data Vector in float_t Type
 */
DataBuffer<float_t> CAlgoInstance::getOutput() {

  return output_;
}

} // namespace robotpilots

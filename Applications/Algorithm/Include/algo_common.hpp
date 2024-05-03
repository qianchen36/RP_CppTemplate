/**
 * @file        algo_common.hpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       
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

#ifndef RP_ALGO_COMMON_HPP
#define RP_ALGO_COMMON_HPP

#include "algo_def.hpp"

namespace robotpilots {

/**
 * @brief Algorithm Object Abstract Instance
 * @note Also Can be Used as a Handler
 */
class CAlgoInstance {
protected:
  /**
   * @brief Algorithm Object Initialize Parameter Structure
   * @note Inheritance & Specify for Real Algorithm Object
   */
  struct SAlgoInitParam {
    uint16_t threadNum = 1;      ///< Set Algorithm Thread Number (Default: 1)
    uint16_t tickRate = 1000;    ///< Set Algorithm Update Rate (Default: 1000Hz)
  };

  /**
   * @brief Get Input Data from Registered Function
   *
   * @return Data Vector in float_t Type
   */
  InputCallback getInput_;

  /**
   * @brief Send Output Data to Registered Function
   * @param data Reference of Output Data Vector in float_t Type
   *
   * @returns RP_OK - Set Output Succeeded
   *          RP_ERROR - Set Output Failed
   */
  OutputCallback setOutput_;

  /**
   * @brief Input & Output Callback Function Register State
   * @details false - Unregistered \n
   *          true - Register \n
   *          Default Value: false
   */
  bool callbackRegisterState_ = false;

  /**
   * @brief Data Buffers
   */
  DataBuffer<float_t> input_, output_;

public:
  /**
   * @brief Algorithm Object Current State
   * @details RP_RESET - Algorithm Object Uninitialized \n
   *          RP_ERROR - Algorithm Object Error \n
   *          RP_OK - Algorithm Object Ready \n
   *          RP_BUSY - Algorithm Object Busy \n
   *          Default Value: RP_RESET
   */
  ERpStatus algoState = RP_RESET;

  /**
   * @brief Algorithm Update Rate
   * @details Unit: Hz \n
   *          Default Value: 1000Hz
   */
  uint16_t algoTickRate = 1000;

  /**
   * @brief Algorithm Thread Number
   * @details Default Value: 1
   * @note Every Single Thread of Algorithm Object Used the Same Parameter, \n
   *       But Different in Data Input&Output Source.
   */
  uint16_t algoThreadNum = 1;

  /**
   * @brief Initialize Algorithm Object
   * @param pStruct[in] Pointer to Initialize Parameter Structure
   *
   * @returns RP_OK - Initialize Succeeded \n
   *          RP_ERROR - Initialize Failed
   */
  virtual ERpStatus InitAlgorithm(const SAlgoInitParam *pStruct) = 0;

  /**
   * @brief Update Algorithm Output
   * @details Calculate Output
   * @note If Registered, Use Registered Input & Output Sources for Calculate. \n
   *       Else, Update Input Data Buffer Manually Before Call This Function.
   *
   * @returns RP_OK - Update Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Update Failed
   */
  virtual ERpStatus UpdateAlgorithm() = 0;

  /**
   * @brief Clear Algorithm Data Buffer
   * @details Clear the Buffer to Avoid Impact in Output Data
   *
   * @returns RP_OK - Reset Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Reset Failed
   */
  virtual ERpStatus ResetAlgorithm() = 0;

  /**
   * @brief Register Input & Output Data Source Callback Function
   * @param input Callback Function for Input
   * @param output Callback Function for Output
   *
   * @returns RP_OK - Register Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Register Failed
   */
  ERpStatus RegisterInputOutputDataSource(InputCallback &input, OutputCallback &output);

  /**
   * @brief Unregister Input & Output Data Source Callback Function
   *
   * @return RP_OK - Unregister Succeeded \n
   *         RP_BUSY - Algorithm Object is Busy \n
   *         RP_ERROR - Unregister Failed
   */
  ERpStatus UnregisteredInputOutputDataSource();

  /**
   * @brief Set Algorithm Input Data for Next Update
   * @param in[in] Input Data Vector in float_t Type
   *
   * @returns RP_OK - Set Input Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Set Input Failed
   */
  ERpStatus setInput(const DataBuffer<float_t> &in);

  /**
   * @brief Get Algorithm Output Data in Last Update
   * @param out[out] Output Data Vector in float_t Type
   *
   * @returns RP_OK - Get Output Succeeded \n
   *          RP_BUSY - Algorithm Object is Busy \n
   *          RP_ERROR - Get Output Failed
   */
  ERpStatus getOutput(DataBuffer<float_t> &out);

  /**
   * @brief Get Algorithm Output Data in Last Update
   *
   * @return Output Data Vector in float_t Type
   */
  DataBuffer<float_t> getOutput();
};

} // namespace robotpilots

#endif // RP_ALGO_COMMON_HPP

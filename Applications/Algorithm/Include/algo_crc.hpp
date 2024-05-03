/**
 * @file        algo_crc.h
 * @version     1.0
 * @date        2024-03-17
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-17   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */
#ifndef RP_ALGO_CRC_HPP
#define RP_ALGO_CRC_HPP

#include "algo_common.hpp"

namespace robotpilots {

/**
 * @brief CRC Validator Class
 * @details
 * @note This Class Only Provide Static Functions
 */
class CCrcValidator {
public:

  /**
   * @brief Deleted Constructor
   */
  CCrcValidator() = delete;

  /**
   * @brief Verify CRC8 Checksum Value for Data Buffer
   * @param buffer[in] Data Buffer Vector
   * @param crc[in] CRC8 Checksum Value
   *
   * @returns RP_OK - Verify Succeeded \n
   *          RP_ERROR - Verify Failed
   */
  static ERpStatus Crc8Verify(const DataBuffer<uint8_t> &buffer, uint8_t crc);

  /**
   * @brief Verify CRC8 Value Checksum for Data Buffer
   * @param buffer[in] Data Buffer Pointer
   * @param crc[in] CRC8 Checksum Value
   * @param len[in] Bit Length of Data Buffer
   *
   * @returns RP_OK - Verify Succeeded \n
   *          RP_ERROR - Verify Failed
   */
  static ERpStatus Crc8Verify(const uint8_t *buffer, uint8_t crc, size_t len);

  /**
   * @brief Verify CRC16 Checksum Value for Data Buffer
   * @param buffer[in] Data Buffer
   * @param crc[in] CRC16 Checksum Value
   *
   * @returns RP_OK - Verify Succeeded \n
   *          RP_ERROR - Verify Failed
   */
  static ERpStatus Crc16Verify(const DataBuffer<uint8_t> &buffer, uint16_t crc);

  /**
   * @brief Verify CRC16 Checksum Value for Data Buffer
   * @param buffer[in] Data Buffer Pointer
   * @param crc[in] CRC8 Checksum Value
   * @param len[in] Bit Length of Data Buffer
   *
   * @returns RP_OK - Verify Succeeded \n
   *          RP_ERROR - Verify Failed
   */
  static ERpStatus Crc16Verify(const uint8_t *buffer, uint16_t crc, size_t len);

  /**
   * @brief Calculate CRC8 Checksum Value from Data Buffer
   * @param buffer[in] Data Buffer
   *
   * @return CRC8 Checksum Value
   */
  static uint8_t Crc8Calculate(const DataBuffer<uint8_t> &buffer);

  /**
   * @brief Calculate CRC8 Checksum Value from Data Buffer
   * @param buffer[in] Data Buffer Pointer
   * @param len[in] Bit Length of Data Buffer
   *
   * @return CRC8 Checksum Value
   */
  static uint8_t Crc8Calculate(const uint8_t *buffer, size_t len);

  /**
   * @brief Calculate CRC16 Checksum Value from Data Buffer
   * @param buffer[in] Data Buffer
   *
   * @return CRC16 Checksum Value
   */
  static uint16_t Crc16Calculate(const DataBuffer<uint8_t> &buffer);

  /**
   * @brief Calculate CRC16 Checksum Value from Data Buffer
   * @param buffer[in] Data Buffer Pointer
   * @param len[in] Bit Length of Data Buffer
   *
   * @return CRC16 Checksum Value
   */
  static uint16_t Crc16Calculate(const uint8_t *buffer, size_t len);

private:
};

} // namespace robotpilots

#endif // RP_ALGO_CRC_HPP

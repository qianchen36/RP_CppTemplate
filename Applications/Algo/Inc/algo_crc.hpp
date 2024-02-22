/**
 * @file    algo_crc.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __ALGO_CRC_HPP__
#define __ALGO_CRC_HPP__

#include "algo.hpp"

namespace rp {

namespace algo {

bool CRC8_Verify(uint8_t *pData, uint16_t len);
void CRC8_Append(uint8_t *pData, uint16_t len);
bool CRC16_Verify(uint8_t *pData, uint32_t len);
void CRC16_Append(uint8_t *pData, uint32_t len);

} // namespace algo

} // namespace rp

#endif // __ALGO_CRC_HPP__

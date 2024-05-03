/**
 * @file        conf_common.hpp
 * @brief
 * @details
 *
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @copyright   SZU-RobotPilots
 */

#ifndef RP_CONF_COMMON_HPP
#define RP_CONF_COMMON_HPP

#include "main.h"
#include "arm_math.h"

#include <ctime>

#include <functional>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <map>
#include <set>

#include "FreeRTOS.h"
#include "semaphore"
#include "timers.h"
#include "queue.h"
#include "task.h"

namespace robotpilots {

/**
 * @brief Application Status Enum
 * @note Used for Application Function as Return Value
 */
enum ERpStatus {
  RP_RESET = -1,
  RP_ERROR = 0,
  RP_BUSY,
  RP_OK,
};

using EFuncStatus = bool;

/**
 * @brief Data Buffer Template
 * @tparam T Type of Data to be Stored
 */
template<typename T> using DataBuffer = std::vector<T>;

/**
 * @brief Bit Stream Buffer Template
 * @details data - Original Data \n
 *          stream - Bit Stream
 * @tparam T Type of Data to be Convert
 */
template<typename T>
union StreamBuffer {
  static_assert(!std::is_trivial<T>(), "Input Type is Not Trivial!");
  T data; uint8_t stream[sizeof(T)];
};

enum class EInterfaceID;
enum class EDeviceID;
enum class EModuleID;

} // namespace robotpilots

#endif // RP_CONF_COMMON_HPP

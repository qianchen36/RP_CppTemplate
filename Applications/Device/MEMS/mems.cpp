/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-18
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-18   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#include "mems/mems.hpp"

namespace robotpilots {

/**
 * @brief
 */
std::map<EDeviceID, CMemsInstance *> MemsMap;

/**
 * @brief
 * @return
 */
ERpStatus CMemsInstance::RegisterMems_() {

  if (deviceId != EDeviceID::DEV_NULL) {
    MemsMap.insert(std::make_pair(deviceId, this));
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @return
 */
ERpStatus CMemsInstance::UnregisteredMems_() {

  if (deviceId != EDeviceID::DEV_NULL) {
    MemsMap.erase(deviceId);
    return RP_OK;
  }

  return RP_ERROR;
}

/**
 * @brief
 * @param us
 */
void CMemsInstance::DelayUs_(uint32_t us) {

  auto ticks = us * 480;
  auto tcnt = 0ul;
  auto tnow = SysTick->VAL;
  auto told = SysTick->VAL;
  auto reload = SysTick->LOAD;

  while (true) {
    tnow = SysTick->VAL;
    if (tnow != told) {
      tcnt += (tnow < told) ? (told - tnow) : (reload - tnow + told);
      told = tnow;

      if (tcnt >= ticks) return;
    }
  }
}

/**
 * @brief
 * @param ms
 */
void CMemsInstance::DelayMs_(uint32_t ms) {

  DelayUs_(ms * 1000);
}

} // namespace robotpilots

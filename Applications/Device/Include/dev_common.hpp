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

#ifndef RP_DEV_COMMON_HPP
#define RP_DEV_COMMON_HPP

#include "Configuration.hpp"
#include "Algorithm.hpp"

namespace robotpilots {

class CDevInstance {
  friend void StartUpdateTask(void *arg);
  friend void StartHeartbeatTask(void *arg);
protected:
  /**
   * @brief
   */
  struct SDevInitParam {
    EDeviceID deviceId = EDeviceID::DEV_NULL;
  };

  /**
   * @brief
   */
  uint32_t lastHeartbeatTime_ = 0;

  /**
   * @brief
   */
  virtual void UpdateHandler_() { }

  /**
   * @brief
   */
  virtual void HeartbeatHandler_() { }

  /**
   * @brief
   * @return
   */
  ERpStatus RegisterDevice_();

  /**
   * @brief
   * @return
   */
  ERpStatus UnregisteredDevice_();

public:
  /**
   * @brief
   */
  EDeviceID deviceId = EDeviceID::DEV_NULL;

  /**
   * @brief
   */
  enum class EDeviceType {
    DEV_UNDEF = -1,
    DEV_MEMS,
    DEV_MTR,
    DEV_RC,
    DEV_VISION,
    DEV_REFEREE,
    DEV_OTHER,
  } deviceType = EDeviceType::DEV_UNDEF;

  /**
   * @brief
   */
  ERpStatus deviceState = RP_RESET;

  /**
   * @brief
   */
  CDevInstance() = default;

  /**
   * @brief
   */
  virtual ~CDevInstance() { UnregisteredDevice_(); }

  /**
   * @brief
   * @param pStruct
   * @return
   */
  virtual ERpStatus InitDevice(const SDevInitParam *pStruct) = 0;

  /**
   * @brief
   * @return
   */
  virtual ERpStatus StartDevice() { return RP_ERROR; }

  /**
   * @brief
   * @return
   */
  virtual ERpStatus StopDevice() { return RP_ERROR; }
};

/**
 * @brief
 */
extern std::map<EDeviceID, CDevInstance *> DeviceMap;

} // namespace robotpilots

#endif // RP_DEV_COMMON_HPP

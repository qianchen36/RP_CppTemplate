/**
 * @file        inf_common.hpp
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
#ifndef RP_INF_COMMON_HPP
#define RP_INF_COMMON_HPP

#include "Configuration.hpp"

namespace robotpilots {

class CInfInstance {
  friend void StartHeartbeatTask(void* arg);
protected:
  /**
   * @brief
   */
  struct SInfInitParam {
    EInterfaceID interfaceId = EInterfaceID::INF_NULL;
  };

  /**
   * @brief
   * @return
   */
  ERpStatus RegisterInterface_();

  /**
   * @brief
   * @return
   */
  ERpStatus UnRegisterInterface_();

  /**
   * @brief
   */
  virtual void HeartbeatHandler_() { }

public:
  /**
   * @brief
   */
  EInterfaceID interfaceId = EInterfaceID::INF_NULL;

  /**
   * @brief
   */
  enum class EInterfaceType {
    INF_UNDEF = -1,
    INF_CAN,
    INF_SPI,
    INF_UART,
    INF_USB_CDC,
  } interfaceType = EInterfaceType::INF_UNDEF;

  /**
   * @brief
   */
  ERpStatus interfaceState = RP_RESET;

  virtual ~CInfInstance() { UnRegisterInterface_(); }

  /**
   * @brief
   * @param pStruct
   * @return
   */
  virtual ERpStatus InitInterface(const SInfInitParam *pStruct) = 0;

  /**
   * @brief
   * @return
   */
  virtual ERpStatus StartTransfer() { return RP_ERROR; }

  /**
   * @brief
   * @return
   */
  virtual ERpStatus StopTransfer() { return RP_ERROR; }

};

/**
 * @brief
 */
extern std::map<EInterfaceID, CInfInstance *> InterfaceMap;

} // namespace robotpilots

#endif // RP_INF_COMMON_HPP

/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-19
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-19   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_MOD_COMMON_HPP
#define RP_MOD_COMMON_HPP

#include "Configuration.hpp"
#include "Algorithm.hpp"
#include "Interface.hpp"
#include "Device.hpp"

namespace robotpilots {

/**
 * @brief
 */
class CModInstance {
  friend void StartUpdateTask(void *arg);
  friend void StartHeartbeatTask(void *arg);
protected:
  struct SModInitParam {
    EModuleID moduleId = EModuleID::MOD_NULL;
  };

  virtual void UpdateHandler_() { }
  virtual void HeartbeatHandler_() { }
  ERpStatus RegisterModule_();
  ERpStatus UnregisterModule_();

public:

  class CComponent {
  public:
    enum class EComStatus {
      STOP,
      INIT,
      NORMAL,
    } comState = EComStatus::STOP;
    bool mutex = false;
    CComponent() = default;
    virtual ~CComponent() = default;
    virtual ERpStatus InitComponent(SModInitParam *pStruct) = 0;
    virtual ERpStatus StartComponent() { comState = EComStatus::INIT; return RP_OK; }
    virtual ERpStatus StopComponent() { comState = EComStatus::STOP; return RP_OK; }
    virtual ERpStatus UpdateComponent() = 0;
  };

  EModuleID moduleId = EModuleID::MOD_NULL;
  ERpStatus moduleState = RP_RESET;
  TaskHandle_t  moduleTaskHandle = nullptr;
  QueueHandle_t moduleCmdQueueHandle = nullptr;
  QueueHandle_t moduleInfoQueueHandle = nullptr;

  CModInstance() = default;
  virtual ~CModInstance() { UnregisterModule_(); };
  virtual ERpStatus InitModule(SModInitParam *pStruct) = 0;
  virtual ERpStatus StartModule() { return RP_ERROR; }
  virtual ERpStatus StopModule() { return RP_ERROR; }
};

extern std::map<EModuleID, CModInstance *> ModuleMap;

} // namespace robotpilots

#endif // RP_MOD_COMMON_HPP

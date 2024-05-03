/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-22
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-22   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_SYS_COMMON_HPP
#define RP_SYS_COMMON_HPP

#include "Configuration.hpp"
#include "Module.hpp"

namespace robotpilots {

class CSysInstance {
  friend void StartSystemUpdateTask(void *arg);
protected:
  /**
   * @brief
   */
  virtual void UpdateHandler_() { }

  /**
   * @brief
   * @return
   */
  ERpStatus RegisterSystem();

  /**
   * @brief
   * @return
   */
  ERpStatus UnregisterSystem();

public:
  /**
   * @brief
   */
  struct SSystemInitParam {
    ESystemID systemId = ESystemID::SYS_NULL;
  };

  /**
   * @brief
   */
  ESystemID systemID = ESystemID::SYS_NULL;

  /**
   * @brief
   */
  CSysInstance() = default;

  /**
   * @brief
   */
  virtual ~CSysInstance() { UnregisterSystem(); }

  /**
   * @brief
   * @param pStruct
   * @return
   */
  virtual ERpStatus InitSystem(SSystemInitParam *pStruct) = 0;

};

/**
 * @brief
 */
extern std::map<ESystemID, CSysInstance *> SystemMap;

}

#endif // RP_SYS_COMMON_HPP

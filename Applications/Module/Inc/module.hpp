/**
 * @file    module.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef _MODULE_HPP_
#define _MODULE_HPP_

#include "module_def.hpp"

#include <map>

namespace rp {

namespace module {

typedef struct _MOD_InitParam
{
  uint8_t modId;

  _MOD_InitParam();

} MOD_InitParam_t;



class MODULE_c
{
protected:
  MOD_InitParam_t *initParam_;
  uint32_t lastHeartbeatTime_;

  void AddModule(MODULE_c *module);
  void DelModule(MODULE_c *module);

public:
  uint8_t         modId;
  MODULE_Status_e modStatus;

  MODULE_c();
  ~MODULE_c();

  virtual void InitModule(MOD_InitParam_t *initParam) = 0;
  virtual void HeartbeatModule(void) = 0;
  virtual MODULE_c *GetObjectHandler(void) = 0;

}; // class MODULE_c

extern std::map<uint8_t, MODULE_c *> ModuleList;

void MODULE_Heartbeat(void);

} // namespace module

} // namespace rp

/* Sub include */
#include "mod_chassis.hpp"

#endif // _MODULE_HPP_

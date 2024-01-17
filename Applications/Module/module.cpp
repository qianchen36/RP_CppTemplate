/**
 * @file    module.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "module.hpp"

namespace rp {

namespace module {

std::map<uint8_t, MODULE_c *> ModuleList;

_MOD_InitParam::_MOD_InitParam()
{
  modId = NULL;
}



MODULE_c::MODULE_c()
{
  modId     = NULL;
  modState = MOD_RESET;

  initParam_         = nullptr;
  lastHeartbeatTime_ = NULL;
}



MODULE_c::~MODULE_c()
{
  if (modId == NULL)
    return;

  DelModule(this);
}



void MODULE_c::AddModule(MODULE_c *module)
{
  if (module == nullptr)
    return;

  ModuleList.insert(std::pair<uint8_t, MODULE_c *>(module->modId, module));
}



void MODULE_c::DelModule(MODULE_c *module)
{
  if (module == nullptr)
    return;

  ModuleList.erase(module->modId);
}



void MODULE_Heartbeat(void)
{
  for (auto it : ModuleList)
    it.second->HeartbeatModule();

}

} // namespace module

} // namespace rp

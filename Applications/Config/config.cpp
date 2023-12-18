/**
 * @file    config.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "config.hpp"

namespace rp {

namespace config {

void ApplicationInitialize(void)
{
  CommInitialize();
  DeviceInitialize();
  ModuleInitialize();
  SystemInitialize();
}

} // namespace config

} // namespace rp
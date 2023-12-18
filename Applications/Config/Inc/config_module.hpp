/**
 * @file    config_module.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CONFIG_MODULE_HPP__
#define __CONFIG_MODULE_HPP__

#include "config_def.hpp"

typedef enum
{
  MOD_UNDEF,

  MOD_SLAVE,    // Slave
  MOD_VISION,   // Vision
  MOD_CHASSIS,  // Chassis

} MOD_ID_e;

namespace rp {

namespace config {

void ModuleInitialize(void);

} // namespace config

} // namespace rp

#endif // __CONFIG_MODULE_HPP__

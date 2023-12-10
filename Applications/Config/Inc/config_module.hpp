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

  MOD_CHASIS, // Chasis
  MOD_GANTRY, // Gantry
  MOD_ROLLER, // Roller
  MOD_MANIP,  // Manipulator

} MOD_ID_e;

#endif // __CONFIG_MODULE_HPP__

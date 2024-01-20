/**
 * @file    module_def.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-08
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __MODULE_DEF_HPP__
#define __MODULE_DEF_HPP__

#include "main.h"
#include "config.hpp"

namespace rp {

namespace module {

typedef enum
{
  MOD_RESET,

  MOD_OFFLINE,
  MOD_ONLINE,

  MOD_ERROR,

} MODULE_Status_e;

} // namespace module

} // namespace rp

#endif // __MODULE_DEF_HPP__

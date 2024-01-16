/**
 * @file    config.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include "config_def.hpp"
#include "board.hpp"

namespace rp {

namespace config {

void ApplicationInitialize(void);

} // namespace config

} // namespace rp

#include "config_comm.hpp"
#include "config_device.hpp"
#include "config_module.hpp"
#include "config_system.hpp"

#endif // __CONFIG_HPP__

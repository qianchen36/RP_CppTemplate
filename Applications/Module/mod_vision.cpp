/**
 * @file    mod_vision.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mod_vision.hpp"

namespace module {

/* Export vision module */
vision::MOD_VISION_c MOD_Vision;

namespace vision {

_MOD_VISION_InitParam::_MOD_VISION_InitParam()
{
  comHandler = nullptr;
}



MOD_VISION_c::MOD_VISION_c()
{

}



MOD_VISION_c::~MOD_VISION_c()
{

}



void MOD_VISION_c::InitModule(MOD_InitParam_t *initParam)
{

}



void MOD_VISION_c::HeartbeatModule(void)
{

}



MOD_VISION_c *MOD_VISION_c::GetObjectHandler(void)
{ return this; }

} // namespace vision

} // namespace module

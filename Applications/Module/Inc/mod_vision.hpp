/**
 * @file    mod_vision.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-10
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef _MOD_VISION_HPP_
#define _MOD_VISION_HPP_

#include "module.hpp"

#include "comm.hpp"

namespace module {

namespace vision {

typedef struct _MOD_VISION_InitParam : public MOD_InitParam_t
{
  comm::COMM_c *comHandler;

  _MOD_VISION_InitParam();

} MOD_VISION_InitParam_t;



class MOD_VISION_c : public MODULE_c
{
protected:
  comm::COMM_c *comHandler_;

public:
  MOD_VISION_c();
  ~MOD_VISION_c();

  void InitModule(MOD_InitParam_t *initParam) override;
  void HeartbeatModule(void) override;
  MOD_VISION_c *GetObjectHandler(void) override;

};

} // namespace vision

extern vision::MOD_VISION_c MOD_Vision;

} // namespace module

#endif // _MOD_VISION_HPP_

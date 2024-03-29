/**
 * @file    rc_dr16.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __RC_DR16_HPP__
#define __RC_DR16_HPP__

#include "rc.hpp"
#include "comm_uart.hpp"

namespace rp {

namespace device {

namespace rc {

typedef struct _RC_DR16_InitParam : public _RC_InitParam
{
  _RC_DR16_InitParam();

} RC_DR16_InitParam_s;

typedef enum
{
  DR16_CH_0,
  DR16_CH_1,
  DR16_CH_2,
  DR16_CH_3,
  DR16_CH_TW,
  DR16_CH_S1,
  DR16_CH_S2,

  DR16_CH_MOUSE_VX,
  DR16_CH_MOUSE_VY,
  DR16_CH_MOUSE_VZ,
  DR16_CH_MOUSE_L,
  DR16_CH_MOUSE_R,
  DR16_CH_KEY_W,
  DR16_CH_KEY_S,
  DR16_CH_KEY_A,
  DR16_CH_KEY_D,
  DR16_CH_KEY_SHIFT,
  DR16_CH_KEY_CTRL,
  DR16_CH_KEY_Q,
  DR16_CH_KEY_E,
  DR16_CH_KEY_R,
  DR16_CH_KEY_F,
  DR16_CH_KEY_G,
  DR16_CH_KEY_Z,
  DR16_CH_KEY_X,
  DR16_CH_KEY_C,
  DR16_CH_KEY_V,
  DR16_CH_KEY_B,

  DR16_CH_NUM,

} RC_DR16_ChList_e;

class RC_DR16_c : public RC_c , public comm::COMM_UART_Node_c
{
public:
  RC_DR16_c();
  ~RC_DR16_c();

  void InitDevice(DEV_InitParam_s *initParam) override;
  void UartNode_ReceiveCallback(uint8_t *pData, uint16_t len) override;
};

} // namespace rc

} // namespace device

} // namespace rp

#endif // __RC_DR16_HPP__

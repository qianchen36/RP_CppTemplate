/**
 * @file    rc_dr16.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "rc_dr16.hpp"

namespace rp {

namespace device {

namespace rc {

/* DR16 CH type list */
RC_ChType_e RC_DR16_ChType[DR16_CH_NUM] = {
  RC_CH_JOYSTICK,
  RC_CH_JOYSTICK,
  RC_CH_JOYSTICK,
  RC_CH_JOYSTICK,
  RC_CH_SWITCH,
  RC_CH_SWITCH,
  RC_CH_JOYSTICK,
  RC_CH_JOYSTICK,
  RC_CH_JOYSTICK,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
  RC_CH_BUTTON,
};



/**
 * @brief  Construct a new device::rc::RC_DR16_InitParam_s structure
 * 
 * @return None
 */
_RC_DR16_InitParam::_RC_DR16_InitParam()
{
  rcType = RC_DR16;
}



/**
 * @brief  Construct a new device::rc::RC_DR16_c object
 * 
 * @return None
 */
RC_DR16_c::RC_DR16_c()
{
  devComm = comm::COMM_UART;
  rcType  = RC_DR16;
}



/**
 * @brief  Destroy the device::rc::RC_DR16_c object
 * 
 * @return None
 */
RC_DR16_c::~RC_DR16_c()
{
  if (devID != NULL && hComm_ != nullptr)
  {
    DelDevice(this);
    ((comm::COMM_UART_c *)hComm_)->DelUartNode(this);
  }

}



/**
 * @brief  Initialize the DR16 RC receiver
 * 
 * @param  id Set the device ID
 * @param  hComm Set the handle of the communication interface
 * @param  pStruct Set the DR16 specific parameters
 * @return None
 */
void RC_DR16_c::InitDevice(DEV_InitParam_s *initParam)
{
  /* Check pointer */
  if (initParam == nullptr)
    return;

  if (initParam->devID == NULL || initParam->hComm == nullptr)
    return;

  /* Copy parameters */
  initParam_ = new RC_DR16_InitParam_s;
  memcpy(initParam_, initParam, sizeof(RC_DR16_InitParam_s));

  auto param = (RC_DR16_InitParam_s *)initParam_;

  /* Initialize */
  devID   = param->devID;
  hComm_  = param->hComm;

  rcData.resize(DR16_CH_NUM);
  for (uint8_t i = 0; i < DR16_CH_NUM; i++)
  {
    rcData[i].chType  = RC_DR16_ChType[i];
    rcData[i].chValue = 0;
    rcData[i].chState = CH_RESET;
  }

  /* Regist */
  AddDevice(this);
  ((comm::COMM_UART_c *)hComm_)->AddUartNode(this);

  /* Update status */
  devState = DEV_OFFLINE;
}



/**
 * @brief  Get the handler of RC_DR16_c object
 * 
 * @return (RC_DR16_c *) Pointer of the RC_DR16_c object handler 
 */
RC_DR16_c *RC_DR16_c::GetObjectHandler(void)
{ return this; }



/**
 * @brief  Callback function for UART receive data
 * 
 * @param  pData Pointer of the received data
 * @param  len Length of the received data
 */
void RC_DR16_c::UartNode_ReceiveCallback(uint8_t *pData, uint16_t len)
{
  /* Check data pointer & length */
  if (pData == nullptr || len < 18)
    return;

  /* Decode datapack */
  rcData[DR16_CH_0]        = ((pData[0] | pData[1] << 8) & 0x07FF) - 1024;
  rcData[DR16_CH_1]        = ((pData[1] >> 3 | pData[2] << 5) & 0x07FF) - 1024;
  rcData[DR16_CH_2]        = ((pData[2] >> 6 | pData[3] << 2 | pData[4] << 10) & 0x07FF) - 1024;
  rcData[DR16_CH_3]        = ((pData[4] >> 1 | pData[5] << 7) & 0x07FF) - 1024;
  rcData[DR16_CH_TW]       = ((pData[16] | pData[17] << 8) & 0x07FF) - 1024;
  rcData[DR16_CH_S1]       = ((pData[5] >> 4) & 0x000C) >> 2;
  rcData[DR16_CH_S2]       = ((pData[5] >> 4) & 0x0003);
  rcData[DR16_CH_MOUSE_VX] =   pData[6] | pData[7] << 8;
  rcData[DR16_CH_MOUSE_VY] =   pData[8] | pData[9] << 8;
  rcData[DR16_CH_MOUSE_VZ] =   pData[10] | pData[11] << 8;
  rcData[DR16_CH_MOUSE_L]  =   pData[12];
  rcData[DR16_CH_MOUSE_R]  =   pData[13];

  uint16_t keys = pData[14] | pData[15] << 8;
  rcData[DR16_CH_KEY_W]     = (keys >> 0) & 0x0001;
  rcData[DR16_CH_KEY_S]     = (keys >> 1) & 0x0001;
  rcData[DR16_CH_KEY_A]     = (keys >> 2) & 0x0001;
  rcData[DR16_CH_KEY_D]     = (keys >> 3) & 0x0001;
  rcData[DR16_CH_KEY_SHIFT] = (keys >> 4) & 0x0001;
  rcData[DR16_CH_KEY_CTRL]  = (keys >> 5) & 0x0001;
  rcData[DR16_CH_KEY_Q]     = (keys >> 6) & 0x0001;
  rcData[DR16_CH_KEY_E]     = (keys >> 7) & 0x0001;
  rcData[DR16_CH_KEY_R]     = (keys >> 8) & 0x0001;
  rcData[DR16_CH_KEY_F]     = (keys >> 9) & 0x0001;
  rcData[DR16_CH_KEY_G]     = (keys >> 10) & 0x0001;
  rcData[DR16_CH_KEY_Z]     = (keys >> 11) & 0x0001;
  rcData[DR16_CH_KEY_X]     = (keys >> 12) & 0x0001;
  rcData[DR16_CH_KEY_C]     = (keys >> 13) & 0x0001;
  rcData[DR16_CH_KEY_V]     = (keys >> 14) & 0x0001;
  rcData[DR16_CH_KEY_B]     = (keys >> 15) & 0x0001;

  lastHartbeatTime_ = HAL_GetTick();

  /* Update device state */
  if (devState == DEV_OFFLINE)
    devState = DEV_ONLINE;

}

} // namespace rc

} // namespace device

} // namespace rp

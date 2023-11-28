/**
 * @file    rc_dr16.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "rc_dr16.hpp"

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

};



/**
 * @brief  Construct a new device::rc::RC_DR16_c object
 * 
 * @return None
 */
RC_DR16_c::RC_DR16_c()
{
  devComm = comm::COMM_UART;
  rcType  = RC_DR16;
  rcData  = new RC_ChData_s[DR16_CH_NUM];

  for (uint8_t i = 0; i < DR16_CH_NUM; i++)
  {
    rcData[i].chType  = RC_DR16_ChType[i];
    rcData[i].chState = CH_RESET;
    rcData[i].chValue = 0;
  }

}



/**
 * @brief  Destroy the device::rc::RC_DR16_c object
 * 
 * @return None
 */
RC_DR16_c::~RC_DR16_c()
{
  delete rcData;

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
void RC_DR16_c::InitDevice(uint8_t id, comm::COMM_c *hComm, void *pStruct)
{
  if (id == NULL || hComm == nullptr)
    return;

  devID = id;
  hComm_ = hComm;
  
  AddDevice(this);
  ((comm::COMM_UART_c *)hComm_)->AddUartNode(this); 

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
  if (pData == nullptr || len < 18)
    return;

  if (rcData == nullptr)
    return;

  devState          = DEV_ONLINE;
  lastHartbeatTime_ = HAL_GetTick();

  rcData[DR16_CH_0].chValue           = ((pData[0] | pData[1] << 8) & 0x07FF) - 1024;
  rcData[DR16_CH_1].chValue           = ((pData[1] >> 3 | pData[2] << 5) & 0x07FF) - 1024;
  rcData[DR16_CH_2].chValue           = ((pData[2] >> 6 | pData[3] << 2 | pData[4] << 10) & 0x07FF) - 1024;
  rcData[DR16_CH_3].chValue           = ((pData[4] >> 1 | pData[5] << 7) & 0x07FF) - 1024;
  rcData[DR16_CH_S1].chValue          = ((pData[5] >> 4) & 0x000C) >> 2;
  rcData[DR16_CH_S2].chValue          = ((pData[5] >> 4) & 0x0003);
  rcData[DR16_CH_MOUSE_X].chValue     =   pData[6] | pData[7] << 8;
  rcData[DR16_CH_MOUSE_Y].chValue     =   pData[8] | pData[9] << 8;
  rcData[DR16_CH_MOUSE_Z].chValue     =   pData[10] | pData[11] << 8;
  rcData[DR16_CH_MOUSE_LEFT].chValue  =   pData[12];
  rcData[DR16_CH_MOUSE_RIGHT].chValue =   pData[13];
}

} // namespace rc

} // namespace device

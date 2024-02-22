/**
 * @file    dev_judge.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2024-02-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __DEV_JUDGE_HPP__
#define __DEV_JUDGE_HPP__

#include "device.hpp"

namespace rp {

namespace device {

/* Judge device initialize params */
typedef struct _DEV_JUDGE_InitParam : public _DEV_InitParam
{
  _DEV_JUDGE_InitParam();

} DEV_JUDGE_InitParam_s;

/* Judge communicat frame */
typedef struct _DEV_JUDGE_Frame
{
  uint8_t              seq;
  uint16_t             cmdID;
  std::vector<uint8_t> data;

} DEV_JUDGE_Frame_s;

/* Judge device */
class DEV_JUDGE_c : public DEVICE_c, public comm::COMM_UART_Node_c
{
private:
  void UartNode_ReceiveCallback(uint8_t *pData, uint16_t len) override;

public:
  std::list<DEV_JUDGE_Frame_s> frameList;

  DEV_JUDGE_c();
  ~DEV_JUDGE_c();

  void InitDevice(DEV_InitParam_s *initParam) override;
  void SendJudgeCommFrame(DEV_JUDGE_Frame_s *pFrame);
};

} // namespace device

} // namespace rp

#endif // __DEV_JUDGE_HPP__

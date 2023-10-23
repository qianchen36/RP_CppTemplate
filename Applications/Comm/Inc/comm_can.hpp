/**
 * @file    comm_can.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __COMM_CAN_HPP__
#define __COMM_CAN_HPP__

#include "comm.hpp"
#include "can.h"

namespace comm {

typedef struct
{
  uint32_t stdID;
  uint8_t  data[8];

  uint32_t timeStamp;

} COMM_CAN_DataPack_s;

class COMM_CAN_Node_c
{
public:
  uint32_t canStdID;
  virtual void CanNode_ReceiveCallback(COMM_CAN_DataPack_s *dataPack) = 0;

};

class COMM_CAN_c : public COMM_c
{
private:
  std::map<uint32_t, COMM_CAN_Node_c *> canNodeList_;

public:
  COMM_CAN_c(void);

  COMM_CAN_c &GetObjectHandler(void) override;
  void Receive(int interfaceType, ...) override;
  void Transmit(int interfaceType, ...) override;

  void ConfigFilter(CAN_FilterTypeDef *sFilterConfig);
  void Start(void);
  void Stop(void);
  void AddCanNode(COMM_CAN_Node_c *canNode);
  void DelCanNode(COMM_CAN_Node_c *canNode);

};

} // namespace comm

#endif // __COMM_CAN_HPP__

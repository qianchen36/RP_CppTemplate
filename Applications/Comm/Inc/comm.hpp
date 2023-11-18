/**
 * @file    comm.hpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-10-22
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#ifndef __COMM_HPP__
#define __COMM_HPP__

#include "comm_def.hpp"

#include <map>

namespace comm {

class COMM_c
{
protected:
  void *hInterface_;

  void AddCommPort(COMM_c *comm);
  void DelCommPort(COMM_c *comm);

public:
  uint8_t       comID;
  COMM_Type_e   comType;
  COMM_Status_e comState;

  COMM_c();
  ~COMM_c();

  virtual void InitComm(uint8_t id, void *hInterface, ...);
  virtual COMM_c *GetObjectHandler(void);
  virtual void *GetInterfaceHandler(void);
  virtual void Receive(int interfaceType, ...) = 0;
  virtual void Transmit(int interfaceType, ...) = 0;

};

extern std::map<uint8_t, COMM_c *> CommList;

} // namespace comm

#endif // __COMM_HPP__

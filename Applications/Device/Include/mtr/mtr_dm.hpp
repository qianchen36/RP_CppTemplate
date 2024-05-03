/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-04-17
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-04-17   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */
#ifndef RP_MTR_DM_HPP
#define RP_MTR_DM_HPP

#include "mtr_common.hpp"
#include "inf_can.hpp"

namespace robotpilots {

class CMtrDm : public CMtrInstance {
protected:
  CCanInterface *canInterface_ = nullptr;
  CCanInterface::CCanRxNode canRxNode_;
  void HeartbeatHandler_() override;
public:
  struct SMtrDmInitParam : public SMtrInitParam {
    uint32_t canStdID = 0x000;
  };
  ERpStatus InitDevice(const SDevInitParam *pStruct) override;
  ERpStatus StartDevice() override;
  ERpStatus StopDevice() override;
};

} // namespace robotpilots

#endif // RP_MTR_DM_HPP

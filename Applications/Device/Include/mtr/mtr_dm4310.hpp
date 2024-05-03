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
#ifndef RP_MTR_DM4310_HPP
#define RP_MTR_DM4310_HPP

#include "mtr_dm.hpp"

namespace robotpilots {

class CMtrDm4310 : public CMtrDm {
public:
  CMtrDm4310() { motorType = EMotorType::MTR_DM4310; }
private:
};

} // namespace robotpilots

#endif // RP_MTR_DM4310_HPP

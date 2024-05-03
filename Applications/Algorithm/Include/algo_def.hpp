/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-23
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-23   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */
#ifndef RP_ALGO_DEF_HPP
#define RP_ALGO_DEF_HPP

#include "Configuration.hpp"

namespace robotpilots {

using InputCallback = std::function<std::vector<float_t>(void)>;
using OutputCallback = std::function<ERpStatus(std::vector<float_t> &)>;

}

#endif // RP_ALGO_DEF_HPP

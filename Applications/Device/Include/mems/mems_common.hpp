/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-03-18
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-18   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */
#ifndef RP_MEMS_COMMON_HPP
#define RP_MEMS_COMMON_HPP

#include "dev_common.hpp"

namespace robotpilots {

/**
 * @brief
 */
class CMemsInstance : public CDevInstance {
protected:

  struct SMemsInitParam : public SDevInitParam {
    EInterfaceID interfaceId = EInterfaceID::INF_NULL;
  };

  ERpStatus RegisterMems_();

  ERpStatus UnregisteredMems_();

  static void DelayUs_(uint32_t us);

  static void DelayMs_(uint32_t ms);

public:

  enum class EMemsType {
    MEMS_UNDEF = -1,
    MEMS_MPU6050,
    MEMS_MPU6500,
    MEMS_BMI08x,
    MEMS_BMI088,
    MEMS_BMI270,
  } memsType = EMemsType::MEMS_UNDEF;

  enum class EMemsStatus {
    RESET = -1,
    INIT,
    NORMAL,
    ERROR,
  } memsState = EMemsStatus::RESET;

  enum EMemsDataType {
    DATA_UNDEF = -1,
    DATA_ACC_X,           ///< Acceleration in X-axis (m/s^2)
    DATA_ACC_Y,           ///< Acceleration in Y-axis (m/s^2)
    DATA_ACC_Z,           ///< Acceleration in Z-axis (m/s^2)
    DATA_GYRO_X,          ///< Angular velocity in X-axis (degree/s)
    DATA_GYRO_Y,          ///< Angular velocity in Y-axis (degree/s)
    DATA_GYRO_Z,          ///< Angular velocity in Z-axis (degree/s)
    DATA_TEMP,            ///< Temperature (degree Celsius)
    DATA_COUNT_,
  };

  std::array<float, EMemsDataType::DATA_COUNT_> memsData = {0.0f};

  ~CMemsInstance() override { UnregisteredMems_(); }

};

extern std::map<EDeviceID, CMemsInstance *> MemsMap;

} // namespace robotpilots

#endif // RP_MEMS_COMMON_HPP

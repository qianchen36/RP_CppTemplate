/**
 * @file        conf_device.cpp
 * @version     1.0
 * @date        2024-03-15
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-15   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */

#include "conf_device.hpp"
#include "Device.hpp"

extern TIM_HandleTypeDef htim3;

namespace robotpilots {

/**
 * @brief
 * @return
 */
ERpStatus InitializeDevice() {

  static CRc_DR16 rc_dr16;
  CRc_DR16::SRcDR16InitParam rc_dr16_initparam;
  rc_dr16_initparam.deviceId = EDeviceID::DEV_RC_DR16;
  rc_dr16_initparam.interfaceId = EInterfaceID::INF_DBUS;
  rc_dr16.InitDevice(&rc_dr16_initparam);

  static CMems_BMI088 mems_bmi088;
  CMems_BMI088::SBMI088InitParam mems_bmi088_initparam;
  mems_bmi088_initparam.deviceId = EDeviceID::DEV_MEMS_BMI088;
  mems_bmi088_initparam.interfaceId = EInterfaceID::INF_SPI2;
  mems_bmi088_initparam.AccelUnitCsPort = BMI_ACC_CS_GPIO_Port;
  mems_bmi088_initparam.AccelUnitCsPin = BMI_ACC_CS_Pin;
  mems_bmi088_initparam.GyroUnitCsPort = BMI_GYRO_CS_GPIO_Port;
  mems_bmi088_initparam.GyroUnitCsPin = BMI_GYRO_CS_Pin;
  mems_bmi088_initparam.useTempControl = true;
  mems_bmi088_initparam.tempTarget = 40.0f;
  mems_bmi088_initparam.halTimHandle = &htim3;
  mems_bmi088_initparam.halTimChannel = TIM_CHANNEL_4;
  mems_bmi088_initparam.tempPidParam.kp = 10.0f;
  mems_bmi088_initparam.tempPidParam.ki = 3.0f;
  mems_bmi088_initparam.tempPidParam.kd = 0.5f;
  mems_bmi088_initparam.tempPidParam.maxInteger = 20;
  mems_bmi088_initparam.tempPidParam.maxOutput = 100;
  mems_bmi088.InitDevice(&mems_bmi088_initparam);

  return RP_OK;
}

} // namespace robotpilots

/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-04-03
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-04-03   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */
#ifndef RP_MEMS_BMI088_HPP
#define RP_MEMS_BMI088_HPP

#include "mems_common.hpp"
#include "inf_spi.hpp"

#include "bmi08x.h"

namespace robotpilots {

class CMems_BMI088 : public CMemsInstance {
public:

  struct SBMI088InitParam : public SMemsInitParam {
    GPIO_TypeDef *AccelUnitCsPort = nullptr;              ///<
    uint16_t AccelUnitCsPin = 0;                          ///<
    GPIO_TypeDef *GyroUnitCsPort = nullptr;               ///<
    uint16_t GyroUnitCsPin = 0;                           ///<
    EFuncStatus useTempControl = false;                   ///<
    float_t tempTarget = 40.0f;                           ///<
    TIM_HandleTypeDef *halTimHandle = nullptr;            ///<
    uint32_t halTimChannel = 0;                           ///<
    CPidController::SPidControllerInitParam tempPidParam; ///<
  };

  CMems_BMI088() { memsType = EMemsType::MEMS_BMI088; }

  ERpStatus InitDevice(const SDevInitParam *pStruct) override;

  ERpStatus InitDevice(const SBMI088InitParam *pStruct);

  ERpStatus StartDevice() override;

  ERpStatus StopDevice() override;

private:
  bmi08_dev bmi08Dev_ = {0};
  // CI2cInterface *i2cInterface_ = nullptr;
  CSpiInterface *spiInterface_ = nullptr;
  std::array<float_t, 4> quaternion_ = {0.0f, 0.0f, 0.0f, 0.0f};
  std::array<float_t, 3> gyroBias_ = {0.0f, 0.0f, 0.0f};
  GPIO_TypeDef *AccelUnitCsPort_ = nullptr;
  uint16_t AccelUnitCsPin_ = 0;
  GPIO_TypeDef *GyroUnitCsPort_ = nullptr;
  uint16_t GyroUnitCsPin_ = 0;
  EFuncStatus useTempControl_ = false;
  float_t tempTarget_ = 40.0f;
  TIM_HandleTypeDef *halTimHandle_ = nullptr;
  uint32_t halTimChannel_ = 0;
  CPidController tempPidController_;
  float_t earthGravity_ = 9.80665f;
  float_t gx_ = 0.0f, gy_ = 0.0f, gz_ = 0.0f;

  void UpdateHandler_() override;

  void HeartbeatHandler_() override;

  inline void AccelUnitCsEnable_();

  inline void AccelUnitCsDisable_();

  inline void GyroUnitCsEnable_();

  inline void GyroUnitCsDisable_();

  inline void AccelUnitReadMultiRegisters_(uint8_t regAddr, uint8_t *buffer, size_t len);

  inline void GyroUnitReadMultiRegisters_(uint8_t regAddr, uint8_t *buffer, size_t len);

  inline float_t Lsb2Mps2_(int16_t lsb, int8_t gRange, uint8_t bitWidth);

  inline float_t Lsb2Dps_(int16_t lsb, float_t dpsRange, uint8_t bitWidth);

  static BMI08_INTF_RET_TYPE portBmiReadReg_(uint8_t regAddr, uint8_t *regData, uint32_t length, void *intfPtr);

  static BMI08_INTF_RET_TYPE portBmiWriteReg_(uint8_t regAddr, const uint8_t *regData, uint32_t length, void *intfPtr);

  inline void AccelUnitWriteSingleRegister_(uint8_t regAddr, uint8_t regData);

  inline void AccelUnitReadSingleRegister_(uint8_t regAddr, uint8_t *buffer);

  inline void GyroUnitWriteSingleRegister_(uint8_t regAddr, uint8_t regData);

  inline void GyroUnitReadSingleRegister_(uint8_t regAddr, uint8_t *buffer);

  ERpStatus AccelUnitInit_();

  ERpStatus GyroUnitInit_();

  ERpStatus UpdateAccelGyroData_();

  ERpStatus TempControl_();

  static void StartBmi088InitTask_(void *arg);
};

} // namespace robotpilots

#endif // RP_MEMS_BMI088_HPP

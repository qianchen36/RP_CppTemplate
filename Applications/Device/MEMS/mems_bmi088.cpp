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

#include <string>
#include "mems/mems_bmi088.hpp"

#define BMI088_ACCEL_3G_SEN 0.0008974358974f
#define BMI088_ACCEL_6G_SEN 0.00179443359375f
#define BMI088_ACCEL_12G_SEN 0.0035888671875f
#define BMI088_ACCEL_24G_SEN 0.007177734375f


#define BMI088_GYRO_2000_SEN 0.00106526443603169529841533860381f
#define BMI088_GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define BMI088_GYRO_500_SEN 0.00026631610900792382460383465095346f
#define BMI088_GYRO_250_SEN 0.00013315805450396191230191732547673f
#define BMI088_GYRO_125_SEN 0.000066579027251980956150958662738366f

namespace robotpilots {

/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CMems_BMI088::InitDevice(const SDevInitParam *pStruct) {

  return InitDevice(static_cast<const SBMI088InitParam *>(pStruct));
}


/**
 * @brief
 * @param pStruct
 * @return
 */
ERpStatus CMems_BMI088::InitDevice(const SBMI088InitParam *pStruct) {

  if (pStruct == nullptr) return RP_ERROR;
  if (pStruct->deviceId == EDeviceID::DEV_NULL) return RP_ERROR;

  auto &param = *const_cast<SBMI088InitParam *>(pStruct);
  deviceId = param.deviceId;
  spiInterface_ = static_cast<CSpiInterface *>(InterfaceMap.at(param.interfaceId));
  AccelUnitCsPort_ = param.AccelUnitCsPort;
  AccelUnitCsPin_ = param.AccelUnitCsPin;
  GyroUnitCsPort_ = param.GyroUnitCsPort;
  GyroUnitCsPin_ = param.GyroUnitCsPin;

  if (param.useTempControl) {
    useTempControl_ = true;
    tempTarget_ = param.tempTarget;
    halTimHandle_ = param.halTimHandle;
    halTimChannel_ = param.halTimChannel;
    param.tempPidParam.threadNum = 1;
    param.tempPidParam.tickRate = 100;
    param.tempPidParam.maxOutput = 100;
    tempPidController_.InitAlgorithm(&param.tempPidParam);
  }

  RegisterDevice_();
  RegisterMems_();

  bmi08Dev_.variant = BMI088_VARIANT;
  bmi08Dev_.intf = BMI08_SPI_INTF;

  deviceState = RP_OK;

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::StartDevice() {

  if (deviceState == RP_RESET) return RP_ERROR;

  if (memsState == EMemsStatus::RESET || memsState == EMemsStatus::ERROR) {
    xTaskCreate(StartBmi088InitTask_, "BMI088InitTask",
                512, this, 3,
                nullptr);

    return RP_OK;
  }

  return RP_ERROR;
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::StopDevice() {

  if (deviceState == RP_RESET) return RP_ERROR;

  if (memsState == EMemsStatus::NORMAL) {
    memsState = EMemsStatus::RESET;
    return RP_OK;
  }

  return RP_OK;
}


/**
 * @brief
 */
void CMems_BMI088::UpdateHandler_() {

  if (memsState == CMemsInstance::EMemsStatus::NORMAL)
    UpdateAccelGyroData_();
}


/**
 * @brief
 */
void CMems_BMI088::HeartbeatHandler_() {

  if (memsState == CMemsInstance::EMemsStatus::NORMAL)
    TempControl_();
}

/**
 * @brief
 * @param lsb
 * @param gRange
 * @param bitWidth
 * @return
 */
inline float_t CMems_BMI088::Lsb2Mps2_(int16_t lsb, int8_t gRange, uint8_t bitWidth) {

  auto power = 2.0f;

  auto half_scale = pow(power, bitWidth) / 2.0f;

  return (earthGravity_ * lsb * gRange) / half_scale;
}


/**
 * @brief
 * @param lsb
 * @param dpsRange
 * @param bitWidth
 * @return
 */
inline float_t CMems_BMI088::Lsb2Dps_(int16_t lsb, float_t dpsRange, uint8_t bitWidth) {

  auto power = 2.0f;

  auto half_scale = pow(power, bitWidth) / 2.0f;

  return (dpsRange / half_scale) * lsb;
}


/**
 * @brief
 */
inline void CMems_BMI088::AccelUnitCsEnable_() {

  HAL_GPIO_WritePin(AccelUnitCsPort_, AccelUnitCsPin_, GPIO_PIN_RESET);
}


/**
 * @brief
 */
inline void CMems_BMI088::AccelUnitCsDisable_() {

  HAL_GPIO_WritePin(AccelUnitCsPort_, AccelUnitCsPin_, GPIO_PIN_SET);
}


/**
 * @brief
 */
inline void CMems_BMI088::GyroUnitCsEnable_() {

  HAL_GPIO_WritePin(GyroUnitCsPort_, GyroUnitCsPin_, GPIO_PIN_RESET);
}


/**
 * @brief
 */
inline void CMems_BMI088::GyroUnitCsDisable_() {

  HAL_GPIO_WritePin(GyroUnitCsPort_, GyroUnitCsPin_, GPIO_PIN_SET);
}


/**
 * @brief
 * @param regAddr
 * @param regData
 */
inline void CMems_BMI088::AccelUnitWriteSingleRegister_(uint8_t regAddr,
                                                        uint8_t regData) {

  AccelUnitCsEnable_();

  uint8_t cmd1[] = {regAddr, regData};
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  AccelUnitCsDisable_();
}


/**
 * @brief
 * @param regAddr
 * @param buffer
 */
inline void CMems_BMI088::AccelUnitReadSingleRegister_(uint8_t regAddr,
                                                       uint8_t *buffer) {

  AccelUnitCsEnable_();

  uint8_t cmd1[] = { static_cast<uint8_t>(regAddr | 0x80), 0x55 };
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  uint8_t cmd2[] = { 0x55 };
  spiInterface_->TransmitReceive(cmd2, buffer, sizeof(cmd2));

  AccelUnitCsDisable_();
}


/**
 * @brief
 * @param regAddr
 * @param buffer
 * @param len
 */
inline void CMems_BMI088::AccelUnitReadMultiRegisters_(uint8_t regAddr,
                                                       uint8_t *buffer,
                                                       size_t len) {

  AccelUnitCsEnable_();

  uint8_t cmd1[] = { static_cast<uint8_t>(regAddr | 0x80) };
  spiInterface_->Transmit(cmd1, sizeof(cmd1));
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  uint8_t cmd2[len];
  std::fill(cmd2, cmd2 + len, 0x55);
  spiInterface_->TransmitReceive(cmd2, buffer, sizeof(cmd2));

  AccelUnitCsDisable_();
}


/**
 * @brief
 * @param regAddr
 * @param regData
 */
inline void CMems_BMI088::GyroUnitWriteSingleRegister_(uint8_t regAddr,
                                                       uint8_t regData) {

  GyroUnitCsEnable_();

  uint8_t cmd1[] = {regAddr, regData };
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  GyroUnitCsDisable_();
}


/**
 * @brief
 * @param regAddr
 * @param buffer
 */
inline void CMems_BMI088::GyroUnitReadSingleRegister_(uint8_t regAddr,
                                                      uint8_t *buffer) {

  GyroUnitCsEnable_();

  // uint8_t cmd1[] = { static_cast<uint8_t>(regAddr | 0x80), 0x55 };
  // spiInterface_->Transmit(cmd1, sizeof(cmd1));

  uint8_t cmd1[] = { static_cast<uint8_t>(regAddr | 0x80) };
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  uint8_t cmd2[] = { 0x55 };
  spiInterface_->TransmitReceive(cmd2, buffer, sizeof(cmd2));

  GyroUnitCsDisable_();
}


/**
 * @brief
 * @param regAddr
 * @param buffer
 * @param len
 */
inline void CMems_BMI088::GyroUnitReadMultiRegisters_(uint8_t regAddr,
                                                      uint8_t *buffer,
                                                      size_t len) {

  GyroUnitCsEnable_();

  uint8_t cmd1[] = { static_cast<uint8_t>(regAddr | 0x80) };
  spiInterface_->Transmit(cmd1, sizeof(cmd1));

  uint8_t cmd2[len];
  std::fill(cmd2, cmd2 + len, 0x55);
  spiInterface_->TransmitReceive(cmd2, buffer, sizeof(cmd2));

  GyroUnitCsDisable_();
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::AccelUnitInit_() {

  uint8_t regData[] = {0x00};

  /* Check communication */
  AccelUnitReadSingleRegister_(BMI08_REG_ACCEL_CHIP_ID, regData);
  DelayUs_(150);
  AccelUnitReadSingleRegister_(BMI08_REG_ACCEL_CHIP_ID, regData);
  DelayUs_(150);

  /* Soft reset */
  AccelUnitWriteSingleRegister_(BMI08_REG_ACCEL_SOFTRESET, BMI08_SOFT_RESET_CMD);
  vTaskDelay(pdMS_TO_TICKS(80));

  /* Check communication */
  AccelUnitReadSingleRegister_(BMI08_REG_ACCEL_CHIP_ID, regData);
  DelayUs_(150);
  AccelUnitReadSingleRegister_(BMI08_REG_ACCEL_CHIP_ID, regData);
  DelayUs_(150);

  /* Check register data */
  if (regData[0] != BMI088_ACCEL_CHIP_ID) return RP_ERROR;

  /* Set Accel unit configuration */
  uint8_t config[][2] = {
      {BMI08_REG_ACCEL_PWR_CTRL, BMI08_ACCEL_POWER_ENABLE},
      {BMI08_REG_ACCEL_PWR_CONF, BMI08_ACCEL_PM_ACTIVE},
      {BMI08_REG_ACCEL_CONF, (BMI08_ACCEL_BW_NORMAL << BMI08_ACCEL_BW_POS) | BMI08_ACCEL_ODR_800_HZ },
      {BMI08_REG_ACCEL_RANGE, BMI088_ACCEL_RANGE_3G},
  };

  for (auto & step : config) {
    AccelUnitWriteSingleRegister_(step[0], step[1]);
    DelayUs_(150);
    AccelUnitReadSingleRegister_(step[0], regData);
    DelayUs_(150);

    if (regData[0] != step[1]) return RP_ERROR;
  }

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::GyroUnitInit_() {

  uint8_t regData[] = {0x00};

  /* Check communication */
  GyroUnitReadSingleRegister_(BMI08_REG_GYRO_CHIP_ID, regData);
  DelayUs_(150);
  GyroUnitReadSingleRegister_(BMI08_REG_GYRO_CHIP_ID, regData);
  DelayUs_(150);

  /* Soft reset */
  GyroUnitWriteSingleRegister_(BMI08_REG_GYRO_SOFTRESET, BMI08_SOFT_RESET_CMD);
  vTaskDelay(pdMS_TO_TICKS(80));

  /* Check communication */
  GyroUnitReadSingleRegister_(BMI08_REG_GYRO_CHIP_ID, regData);
  DelayUs_(150);
  GyroUnitReadSingleRegister_(BMI08_REG_GYRO_CHIP_ID, regData);
  DelayUs_(150);

  /* Check register data */
  if (regData[0] != BMI08_GYRO_CHIP_ID) return RP_ERROR;

  /* Set Gyro unit configuration */
  uint8_t config[][2] = {
      {BMI08_REG_GYRO_RANGE, BMI08_GYRO_RANGE_2000_DPS},
      {BMI08_REG_GYRO_BANDWIDTH, BMI08_GYRO_BW_116_ODR_1000_HZ | BMI08_GYRO_ODR_RESET_VAL},
      {BMI08_REG_GYRO_LPM1, BMI08_GYRO_PM_NORMAL},
  };

  for (auto & step : config) {
    GyroUnitWriteSingleRegister_(step[0], step[1]);
    DelayUs_(150);
    GyroUnitReadSingleRegister_(step[0], regData);
    DelayUs_(150);

    if (regData[0] != step[1]) return RP_ERROR;
  }

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::UpdateAccelGyroData_() {

  uint8_t buffer[8] = {0x00};

  /* Read Accel Data */
  AccelUnitReadMultiRegisters_(BMI08_REG_ACCEL_X_LSB, buffer, 6);
  memsData[DATA_ACC_X] = static_cast<int16_t>(buffer[1] << 8 | buffer[0]) * BMI088_ACCEL_3G_SEN;
  memsData[DATA_ACC_Y] = static_cast<int16_t>(buffer[3] << 8 | buffer[2]) * BMI088_ACCEL_3G_SEN;
  memsData[DATA_ACC_Z] = static_cast<int16_t>(buffer[5] << 8 | buffer[4]) * BMI088_ACCEL_3G_SEN;

  /* Read Gyro Data */
  GyroUnitReadMultiRegisters_(BMI08_REG_GYRO_CHIP_ID, buffer, 8);
  if (buffer[0] == BMI08_GYRO_CHIP_ID) {
    memsData[DATA_GYRO_X] = static_cast<int16_t>(buffer[3] << 8 | buffer[2]) * BMI088_GYRO_1000_SEN - gx_;
    memsData[DATA_GYRO_Y] = static_cast<int16_t>(buffer[5] << 8 | buffer[4]) * BMI088_GYRO_1000_SEN - gy_;
    memsData[DATA_GYRO_Z] = static_cast<int16_t>(buffer[7] << 8 | buffer[6]) * BMI088_GYRO_1000_SEN - gz_;
  }

  /* Read Temperature Data */
  AccelUnitReadMultiRegisters_(BMI08_REG_TEMP_MSB, buffer, 2);
  memsData[DATA_TEMP] = static_cast<int16_t>((buffer[0] << 3) | (buffer[1] >> 5)) * 0.125f + 23.0f;

  return RP_OK;
}


/**
 * @brief
 * @return
 */
ERpStatus CMems_BMI088::TempControl_() {

  if (!useTempControl_) return RP_ERROR;

  if (memsState != EMemsStatus::NORMAL) {
    HAL_TIM_PWM_Stop(halTimHandle_, halTimChannel_);
    return RP_ERROR;
  }

  auto output = tempPidController_.UpdatePidController({tempTarget_},
                                                                       {memsData[DATA_TEMP]});

  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(halTimHandle_);
  uint32_t ccr = arr * output[0] / 100;
  __HAL_TIM_SET_COMPARE(halTimHandle_, halTimChannel_, ccr);

  return RP_OK;
}


/**
 * @brief
 * @param arg
 */
void CMems_BMI088::StartBmi088InitTask_(void *arg) {

  auto bmiDev = static_cast<CMems_BMI088 *>(arg);
  auto maxRetry = 10;
  size_t sampleTime = 200;
  auto gx = 0.0f, gy = 0.0f, gz = 0.0f;

  bmiDev->memsState = EMemsStatus::INIT;

  while (maxRetry--) {
    if (bmiDev->AccelUnitInit_() == RP_OK && bmiDev->GyroUnitInit_() == RP_OK)
      break;

    vTaskDelay(pdMS_TO_TICKS(100));
  }

  if (maxRetry <= 0) {
    bmiDev->memsState = EMemsStatus::ERROR;
    vTaskDelete(nullptr);
  }

  HAL_TIM_PWM_Start(bmiDev->halTimHandle_, bmiDev->halTimChannel_);
  bmiDev->memsState = EMemsStatus::NORMAL;
  bmiDev->gx_ = 0.0f;
  bmiDev->gy_ = 0.0f;
  bmiDev->gz_ = 0.0f;

  for (size_t i = 0; i < sampleTime; i++) {
    gx += bmiDev->memsData[DATA_GYRO_X];
    gy += bmiDev->memsData[DATA_GYRO_Y];
    gz += bmiDev->memsData[DATA_GYRO_Z];
    vTaskDelay(pdMS_TO_TICKS(5));
  }

  bmiDev->gx_ = gx / static_cast<float_t>(sampleTime);
  bmiDev->gy_ = gy / static_cast<float_t>(sampleTime);
  bmiDev->gz_ = gz / static_cast<float_t>(sampleTime);

  vTaskDelete(nullptr);
}

} // namespace robotpilots

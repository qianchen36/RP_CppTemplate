/**
 * @file        ${PACKAGE_NAME}
 * @version     1.0
 * @date        2024-04-30
 * @author      qianchen
 * @email       
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-04-30   <td>1.0         <td>qianchen  <td>First Create.
 * </table>
 */

#ifndef RP_DEV_VISION_HPP
#define RP_DEV_VISION_HPP

#include "dev_common.hpp"

#include "inf_uart.hpp"

namespace robotpilots {

class CDevVision : public CDevInstance {
public:

  struct SDevVisionInitParam : public SDevInitParam {
    EInterfaceID interfaceId = EInterfaceID::INF_NULL;
  };

  struct {
    int16_t isFoundOreTank = 0;    ///< Is Found Ore Tank (0 - Not Found, 1 - Found)
    float_t posit_X = 0.0f;        ///< Ore Tank Position X (unit: cm)
    float_t posit_Y = 0.0f;        ///< Ore Tank Position Y (unit: cm)
    float_t posit_Z = 0.0f;        ///< Ore Tank Position Z (unit: cm)
    float_t atti_Roll = 0.0f;      ///< Ore Tank Attitude Roll (unit: degree)
    float_t atti_Pitch = 0.0f;     ///< Ore Tank Attitude Pitch (unit: degree)
    float_t atti_Yaw = 0.0f;       ///< Ore Tank Attitude Yaw (unit: degree)
    float_t uiPoint_X[5] = {0};    ///< UI Point X Position (Up-Left, Up-Right, Down-Right, Down-Left, Center; unit: cm)
    float_t uiPoint_Y[5] = {0};    ///< UI Point Y Position (Up-Left, Up-Right, Down-Right, Down-Left, Center; unit: cm)
  } visionData;

  CDevVision() { deviceType = EDeviceType::DEV_VISION; }

  ERpStatus InitDevice(const SDevInitParam *pStruct) override;

private:

  enum EPackageID:uint8_t {
    ID_NULL = 0,        ///< Null Package ID
    ID_HEARTBEAT,       ///< Heartbeat Package ID
    ID_RACE_INFO,       ///< Race Information Package ID
    ID_ORETANK_INFO,    ///< Ore Tank Position Information Package ID
  };

  struct SPkgHeader {
    uint8_t SOF = 0xA5;      ///< Start of Frame (Constant: 0xA5)
    uint8_t id = ID_NULL;    ///< Package ID (Default: ID_NULL)
    uint8_t len = 0;         ///< Length of Data Segment (Default: 0)
    uint8_t CRC8 = 0x00;     ///< Header CRC8 Checksum (Default: 0x00)
  } __packed;

  struct SRaceinfoPkg {
    SPkgHeader header;          ///< Package Header
    int16_t raceState = 0;      ///< Race State (0 - Unknown, 1 - Start, 2 - Finish)
    int16_t raceCamp = 0;       ///< Race Camp ID (0 - Unknown, 1 - Red, 2 - Blue)
    uint16_t CRC16 = 0x0000;    ///< Package CRC16 Checksum (Default: 0x0000)
  } __packed;

  struct SOretankinfoPkg {
    SPkgHeader header;
    int16_t isFoundOreTank = 0;    ///< Is Found Ore Tank (0 - Not Found, 1 - Found)
    float_t atti[3] = {0};         ///< Ore Tank Attitude (Yaw, Pitch, Roll; unit: degree)
    float_t posit[3] = {0};        ///< Ore Tank Position (X, Y, Z; unit: cm)
    float_t uiPoint_X[5] = {0};    ///< UI Point X Position (Up-Left, Up-Right, Down-Right, Down-Left, Center; unit: cm)
    float_t uiPoint_Y[5] = {0};    ///< UI Point Y Position (Up-Left, Up-Right, Down-Right, Down-Left, Center; unit: cm)
    uint16_t CRC16 = 0x0000;       ///< Package CRC16 Checksum (Default: 0x0000)
  } __packed;

  CUartInterface *uartInterface_ = nullptr;

  uint8_t rxBuffer_[256 + sizeof(SPkgHeader)] = {0};

  uint32_t rxTimestamp_ = 0;

  void UpdateHandler_() override;

  void HeartbeatHandler_() override;

  ERpStatus ResolveRxPackage_();
};

} // namespace robotpilots

#endif // RP_DEV_VISION_HPP

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

#ifndef RP_DEV_REFEREE_HPP
#define RP_DEV_REFEREE_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked-bitfield-compat"

#include "dev_common.hpp"

#include "inf_uart.hpp"

namespace robotpilots {

class CDevReferee : public CDevInstance {
public:

private:

  enum ECommandID:uint16_t {
    ID_NULL = 0,
    ID_RACE_STATUS  = 0x0001,
    ID_RACE_RESULT  = 0x0002,
    ID_ROBOT_HP     = 0x0003,
    ID_VENUE_EVENT  = 0x0101,
    ID_WARNING      = 0x0104,
    ID_ROBOT_PERF   = 0x0201,
    ID_ROBOT_STATUS = 0x0202,
    ID_ROBOT_POSIT  = 0x0203,
    ID_ROBOT_BUFF   = 0x0204,
    ID_DMG_INFO     = 0x0206,
    ID_RFID_STATUS  = 0x0209,
    ID_ROBOT_MSG    = 0x0301,
    ID_CUSTOM_CTRL  = 0x0302,
    ID_NORMAL_CTRL  = 0x0304,
    ID_MAP_MARK     = 0x0308,
  };

  struct SPkgHeader {
    uint8_t SOF = 0xA5;
    uint16_t len = 0;
    uint8_t seq = 0;
    uint8_t CRC8 = 0x00;
    uint16_t cmdId = ID_NULL;
  } __packed;

  struct SRaceStatusPkg { // ID: ID_RACE_STATUS (0x0001)
    SPkgHeader header;    ///< Package Header
    uint8_t raceType:4;   ///< Race Type (1 - RMUC, 2 - Deleted, 3 - ICRA, 4 - RMUL 3v3, 5 - RMUL 1v1)
    uint8_t raceStage:4;  ///< Race Stage (1 - Prepare, 2 - 15s Self-check, 3 - 5s Countdown, 4 - Racing, 5 - Settle)
    uint16_t remainTime;  ///< Remaining Time (unit: s)
    time_t timestamp;     ///< Unix Timestamp
  } __packed;

  struct SRaceResultPkg { // ID: ID_RACE_RESULT (0x0002)
    SPkgHeader header;    ///< Package Header
    uint8_t winner;       ///< Winner (0 - Draw, 1 - Red, 2 - Blue)
  } __packed;

  struct SRobotHpPkg {    // ID: ID_ROBOT_HP (0x0003)
    SPkgHeader header;    ///< Package Header
    uint16_t robotHp_R1;  ///< Hero Robot HP (Red)
    uint16_t robotHp_R2;  ///< Engineer Robot HP (Red)
    uint16_t robotHp_R3;  ///< Infantry Robot HP (Red)
    uint16_t robotHp_R4;  ///< Infantry Robot HP (Red)
    uint16_t robotHp_R5;  ///< Infantry Robot HP (Red)
    uint16_t robotHp_R7;  ///< Sentry Robot HP (Red)
    uint16_t outpostHp_R; ///< Outpost HP (Red)
    uint16_t baseHp_R;    ///< Base HP (Red)
    uint16_t robotHp_B1;  ///< Hero Robot HP (Blue)
    uint16_t robotHp_B2;  ///< Engineer Robot HP (Blue)
    uint16_t robotHp_B3;  ///< Infantry Robot HP (Blue)
    uint16_t robotHp_B4;  ///< Infantry Robot HP (Blue)
    uint16_t robotHp_B5;  ///< Infantry Robot HP (Blue)
    uint16_t robotHp_B7;  ///< Sentry Robot HP (Blue)
    uint16_t outpostHp_B; ///< Outpost HP (Blue)
    uint16_t baseHp_B;    ///< Base HP (Blue)
  } __packed;

  struct SVenueEventPkg {                 // ID: ID_VENUE_EVENT (0x0101)
    SPkgHeader header;                    ///< Package Header
    uint8_t isOuterBloodPointOccupied:1;  ///< Is Outer Blood Point Occupied (0 - No, 1 - Yes)
    uint8_t isInnerBloodPointOccupied:1;  ///< Is Inner Blood Point Occupied (0 - No, 1 - Yes)
    uint8_t isSupplyPointOccupied:1;      ///< Is Supply Point Occupied (RMUL Only; 0 - No, 1 - Yes)
    uint8_t isPowerRunePointOccupied:1;   ///< Is Power Rune Point Occupied (0 - No, 1 - Yes)
    uint8_t isSmallPowerRuneActivate:1;   ///< Is Small Power Rune Activated (0 - No, 1 - Yes)
    uint8_t isBigPowerRuneActivate:1;     ///< Is Big Power Rune Activated (0 - No, 1 - Yes)
    uint8_t zone2OccupiedState:2;         ///< Zone 2 Occupied State (0 - Unknown, 1 - Teammate, 2 - Enemy)
    uint8_t zone3OccupiedState:2;         ///< Zone 3 Occupied State (0 - Unknown, 1 - Teammate, 2 - Enemy)
    uint8_t zone4OccupiedState:2;         ///< Zone 4 Occupied State (0 - Unknown, 1 - Teammate, 2 - Enemy)
    uint8_t baseShieldRemain:7;           ///< Base Shield Remaining Percentage (0 - 100%)
    uint16_t lastDartHitTime:9;           ///< Last Dart Hit Time (unit: s; 0~420s, Base on Race Start Time)
    uint8_t lastDartHitTarget:2;          ///< Last Dart Hit Target (0 - Unknown, 1 - Outpost, 2 - Static Base, 3 - Moving Base)
    uint8_t centerPointOccupiedState:2;   ///< Center Point Occupied State (RMUL Only; 0 - Unknown, 1 - Teammate, 2 - Enemy, 3 - Both)
  } __packed;

  struct SWarningPkg {                    // ID: ID_PENALTY (0x0104)
    SPkgHeader header;                    ///< Package Header
    uint8_t lastOffendingLevel;           ///< Last Offending Level (1 - Both Yellow, 2 - Yellow, 3 - Red, 4 - Defeat)
    uint8_t lastOffendingRobotId;         ///< Last Offending Robot ID (Both: 0, Red: 1~7, Blue: 101~107)
    uint8_t lastOffendingLevelCount;      ///< Last Offending Level Count (Default: 0)
  } __packed;

  struct SRobotPerfPkg {                  // ID: ID_ROBOT_PERF (0x0201)
    SPkgHeader header;                    ///< Package Header
    uint8_t robotId;                      ///< Robot ID (Red: 1~7, Blue: 101~107)
    uint8_t robotLevel;                   ///< Robot Level
    uint16_t robotHp;                     ///< Robot Current HP
    uint16_t robotMaxHp;                  ///< Robot HP Limit
    uint16_t robotCoolDown;               ///< Robot Cool Down Value
    uint16_t robotMaxHeat;                ///< Robot Heat Limit
    uint16_t robotMaxPower;               ///< Robot Power Limit
    uint8_t isGimbalOutputEnabled:1;      ///< Is Gimbal Power Output Enabled (0 - No, 1 - Yes)
    uint8_t isChassisOutputEnabled:1;     ///< Is Chassis Power Output Enabled (0 - No, 1 - Yes)
    uint8_t isShooterOutputEnabled:1;     ///< Is Shooter Power Output Enabled (0 - No, 1 - Yes)
  } __packed;
};

} // namespace robotpilots

#endif // RP_DEV_REFEREE_HPP

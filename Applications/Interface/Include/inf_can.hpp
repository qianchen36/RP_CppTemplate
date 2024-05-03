/**
 * @file        inf_can.hpp
 * @version     1.0
 * @date        2024-03-17
 * @author      Morthine Xiang
 * @email       xiang@morthine.com
 * @brief       
 *
 * @details
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>Version     <th>Author          <th>Description
 * <tr><td>2024-03-17   <td>1.0         <td>Morthine Xiang  <td>First Create.
 * </table>
 */
#ifndef RP_INF_CAN_HPP
#define RP_INF_CAN_HPP

#include "inf_common.hpp"
#include "stm32h7xx_hal_fdcan.h"

namespace robotpilots {

class CCanInterface : public CInfInstance {
public:
  /**
   * @brief
   */
  enum class ECanFrameType {
    DATA   = FDCAN_DATA_FRAME,
    REMOTE = FDCAN_REMOTE_FRAME,
  };

  /**
   * @brief
   */
  enum class ECanFrameDlc {
    DLC_0  = FDCAN_DLC_BYTES_0,
    DLC_1  = FDCAN_DLC_BYTES_1,
    DLC_2  = FDCAN_DLC_BYTES_2,
    DLC_3  = FDCAN_DLC_BYTES_3,
    DLC_4  = FDCAN_DLC_BYTES_4,
    DLC_5  = FDCAN_DLC_BYTES_5,
    DLC_6  = FDCAN_DLC_BYTES_6,
    DLC_7  = FDCAN_DLC_BYTES_7,
    DLC_8  = FDCAN_DLC_BYTES_8,
    DLC_12 = FDCAN_DLC_BYTES_12,
    DLC_16 = FDCAN_DLC_BYTES_16,
    DLC_20 = FDCAN_DLC_BYTES_20,
    DLC_24 = FDCAN_DLC_BYTES_24,
    DLC_32 = FDCAN_DLC_BYTES_32,
    DLC_48 = FDCAN_DLC_BYTES_48,
    DLC_64 = FDCAN_DLC_BYTES_64,
  };

  /**
   * @brief
   */
  struct SCanInfInitParam : public SInfInitParam {
    FDCAN_HandleTypeDef *halFdcanHandle = nullptr;    ///<
    FDCAN_FilterTypeDef fdcanFilterConfig = {         ///<
      .IdType       = FDCAN_STANDARD_ID,
      .FilterIndex  = 0,
      .FilterType   = FDCAN_FILTER_MASK,
      .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
      .FilterID1    = 0x0000,
      .FilterID2    = 0x0000,
    };
  };

  /**
   * @brief CAN Interface Node Class
   */
  class CCanNode {
  public:
    EInterfaceID canInterfaceId = EInterfaceID::INF_NULL;
    uint32_t stdId = 0x00000000;
    uint32_t timestamp = 0;
    ECanFrameType frameType  = ECanFrameType::DATA;
    ECanFrameDlc  dataLength = ECanFrameDlc::DLC_8;
    DataBuffer<uint8_t> dataBuffer;
  protected:
    CCanInterface *pInterface_;
  };

  /**
   * @brief CAN Interface Transmit Node Class
   */
  class CCanTxNode : public CCanNode {
  public:
    /**
     * @brief
     * @param infId
     * @param nodeStdId
     * @param nodeFrameType
     * @param nodeFrameDlc
     */
    void InitTxNode(EInterfaceID infId, uint32_t nodeStdId, ECanFrameType nodeFrameType, ECanFrameDlc nodeFrameDlc);

    /**
     * @brief
     */
    void Transmit();

    /**
     * @brief
     * @param buffer
     */
    void Transmit(const uint8_t *buffer);

    /**
     * @brief
     * @param buffer
     */
    void Transmit(const DataBuffer<uint8_t> &buffer);
  };

  /**
   * @brief CAN Interface Receive Node Class
   */
  class CCanRxNode : public CCanNode {
  public:
    /**
     * @brief
     * @param canInfId
     * @param nodeStdId
     * @param nodeFrameType
     * @param nodeFrameDlc
     */
    void InitRxNode(EInterfaceID canInfId, uint32_t nodeStdId, ECanFrameType nodeFrameType, ECanFrameDlc nodeFrameDlc);

    /**
     * @brief
     */
    void Receive();

    /**
     * @brief
     * @param buffer
     */
    void Receive(uint8_t *buffer);

    /**
     * @brief
     * @param buffer
     */
    void Receive(DataBuffer<uint8_t> &buffer);
  };

  /**
   * @brief
   */
  CCanInterface() { interfaceType = EInterfaceType::INF_CAN; }

  /**
   * @brief
   */
  ~CCanInterface() override;

  /**
   * @brief
   * @param pStruct
   * @return
   */
  ERpStatus InitInterface(const SInfInitParam *pStruct) override;

  /**
   * @brief
   * @return
   */
  ERpStatus StartTransfer() override;

  /**
   * @brief
   * @return
   */
  ERpStatus StopTransfer() override;

  /**
   * @brief
   * @param node
   */
  ERpStatus Transmit(CCanNode &node);

  /**
   * @brief
   * @param node
   * @return
   */
  ERpStatus Receive(CCanNode &node);

  /**
   * @brief
   * @param dlc
   * @return
   */
  static size_t FrameFlc2DataLength(ECanFrameDlc dlc);

  /**
   * @brief
   * @param handle
   */
  void _CAN_HalTxCallback(FDCAN_HandleTypeDef *handle);

  /**
   * @brief
   * @param handle
   */
  void _CAN_HalRxCallback(FDCAN_HandleTypeDef *handle);

  /**
   * @brief
   * @param handle
   */
  void _CAN_HalErrorCallback(FDCAN_HandleTypeDef *handle);

private:
  /**
   * @brief
   */
  FDCAN_HandleTypeDef *halFdcanHandle_ = nullptr;

  /**
   * @brief
   */
  std::list<CCanTxNode *> txNodeList_;

  /**
   * @brief
   */
  std::list<CCanRxNode *> rxNodeList_;

  /**
   * @brief
   */
  void HeartbeatHandler_() override;

  /**
   * @brief
   * @param node
   */
  ERpStatus RegistNode_(CCanTxNode &node);

  /**
   * @brief
   * @param node
   */
  ERpStatus RegistNode_(CCanRxNode &node);

  /**
   * @brief
   * @param node
   */
  ERpStatus UnregistNode_(CCanTxNode &node);

  /**
   * @brief
   * @param node
   */
  ERpStatus UnregistNode_(CCanRxNode &node);
};

} // namespace robotpilots

#endif // RP_INF_CAN_HPP

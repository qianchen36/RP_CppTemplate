/**
 * @file    algo.cpp
 * @author  Morthine Xiang (xiang@morthine.com)
 * @brief 
 * @version 1.0
 * @date    2023-12-15
 * 
 * @copyright SZU-RobotPilots Copyright (c) 2023
 * 
 */

#include "algo.hpp"

namespace algo {

/**
 * @brief  Construct a new algo::ALGO_InitParam_t struct
 * 
 * @return None
 */
_ALGO_InitParam::_ALGO_InitParam()
{
  algoType = ALGO_UNDEF;
}



/**
 * @brief  Construct a new algo::ALGO_c object
 * 
 * @return None
 */
ALGO_c::ALGO_c()
{
  algoType     = ALGO_UNDEF;
  algoState    = ALGO_RESET;
  algoTickRate = NULL;

  initParam_ = nullptr;
}



/**
 * @brief  Destroy the algo::ALGO_c object
 * 
 * @return None
 */
ALGO_c::~ALGO_c()
{
  if (initParam_ != nullptr)
    delete initParam_;

}



/**
 * @brief  Initialize the Algo
 * 
 * @param  initParam Pointer to the Algo init parameter
 * @return None
 */
void ALGO_c::InitAlgo(ALGO_InitParam_s *initParam)
{
  /* Check pointer */
  if (initParam == nullptr)
    return;

  /* Copy parameters */
  initParam_ = new ALGO_InitParam_s;
  memcpy(initParam_, initParam, sizeof(ALGO_InitParam_s));

  auto param = (ALGO_InitParam_s *)initParam_;

  /* Initialize */
  algoType = param->algoType;

  /* Set state */
  algoState = ALGO_IDLE;
}



/**
 * @brief  Update the Algo
 * 
 * @param  input Pointer to the Algo input
 * @return (float32_t) Output value
 */
float32_t ALGO_c::UpdateAlgo(const float32_t *input)
{ return NULL; }



/**
 * @brief  Update the Algo
 * 
 * @param  input Pointer to the Algo input
 * @param  output Pointer to the Algo output
 * @return None
 */
void ALGO_c::UpdateAlgo(const float32_t *input, float32_t *output)
{ return; }



/**
 * @brief  Reset the Algo
 * 
 * @return None
 */
void ALGO_c::ResetAlgo(void)
{ return; }



/**
 * @brief  Get the Algo handler
 * 
 * @retval (ALGO_c *) Pointer to the Algo handler
 */
ALGO_c *ALGO_c::GetObjectHandler(void)
{ return this; }

} // namespace algo


#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "stm32f7xx_hal_conf.h"

#define LAN8720_PHY_ADDRESS             0x00

extern u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value);
extern u8 GMAC_MdioW(u8 dev,u8 reg, u16 value);
extern uint32_t HAL_GetTick(void);
bool_t lan8720Init(u16 vlan[6])
{
    u16   phyreg = 0;
    uint32_t tickstart = 0;
    uint32_t tickNow ;
    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if((GMAC_MdioW(LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
    {
      /* Return false */
      return false;
    }
     Djy_EventDelay(PHY_RESET_DELAY*mS);
     tickstart=HAL_GetTick();
      do
      {
          GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
          tickNow=HAL_GetTick();
          if((tickNow - tickstart ) > 10000)
              return false;

      } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
      /* Enable Auto-Negotiation */
      if((GMAC_MdioW(LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
      {
        return false;
      }
      /* Get tick */
      tickstart = HAL_GetTick();
      /* Wait until the auto-negotiation will be completed */
      do
      {
          GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
          tickNow=HAL_GetTick();
          if((tickNow - tickstart ) > 10000)
              return false;
      } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

      /* Read the result of the auto-negotiation */
      if((GMAC_MdioR(LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
      {
        return false;
      }
    return true;
}


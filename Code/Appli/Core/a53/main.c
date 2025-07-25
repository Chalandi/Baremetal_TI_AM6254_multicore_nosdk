#include <stdint.h>
#include "led.h"
#include "core_macros.h"
#include "gic-500.h"


void main_core1(void);
void main_core2(void);
void main_core3(void);
void timer_isr(void);
uint32_t GetActiveCoreId(void);

//----------------------------------------------------------------------------------------
/// \brief  
///
/// \descr 
///
/// \param  
///
/// \return 
//----------------------------------------------------------------------------------------
void main(void)
{
  uint32_t ActiveCore = GetActiveCoreId();
  
  /* configure the gic */
  volatile gic500_gicdRegs* pGICD = (volatile gic500_gicdRegs* const)GICD_BASE;
  volatile gic500_gicrRegs* pGICR = (volatile gic500_gicrRegs* const)GICR_BASE;

 /* configure the GIC-500 distributor */
  pGICD->CTLR = (1 << 5) | (1 << 4);
  pGICD->CTLR = 1 | (1 << 5) | (1 << 4);

  /* configure the GIC-500 redistributor */
  /* clear GICR_WAKER.ProcessorSleep */
  pGICR->CORE[ActiveCore].CONTROL.WAKER &= ~((uint32_t)2UL);

  /* wait for GICR_WAKER.ChildrenAsleep to become zero */
  while((pGICR->CORE[ActiveCore].CONTROL.WAKER & 4) != 0)
  {
    __asm volatile("nop");
  }

  /* disable all SGI and PPI */
  pGICR->CORE[ActiveCore].SGI_PPI.ISENABLER0 = 0;

  /* assign all PPI and SGI to Group 0 */
  pGICR->CORE[ActiveCore].SGI_PPI.IGROUPR_SGI_PPI = 0;

  /* configure the GIC cpu interface */
  ARM64_WRITE_SYSREG(ICC_SRE_EL3, 0x0f);
  ARM64_WRITE_SYSREG(ICC_IGRPEN0_EL1, 1);
  ARM64_WRITE_SYSREG(ICC_IGRPEN1_EL1, 0);
  ARM64_WRITE_SYSREG(ICC_IGRPEN1_EL3, 0);
  ARM64_WRITE_SYSREG(ICC_PMR_EL1, 0xff);

  /* enable PPI29 (timer interrrupt) */
  pGICR->CORE[ActiveCore].SGI_PPI.ISENABLER0 |= (1ul << 29);

  /* Set priority of INTID 29 */
  ((volatile uint8_t*)&pGICR->CORE[ActiveCore].SGI_PPI.IPRIORITYR[0])[29] = 0; /* highest prio*/

  /* Set level-triggered */
  pGICR->CORE[ActiveCore].SGI_PPI.ICFGR0 &= ~(uint32_t)(1 << ((29 - 16) * 2)); // clear bit to set level-triggered

  /* enable global interrupt */
  arch_enable_ints();
  arch_enable_fiqs();

  /* start the timer */
  ARM64_WRITE_SYSREG(CNTPS_TVAL_EL1, 0x0BEBC200);
  ARM64_WRITE_SYSREG(CNTPS_CTL_EL1, 1);

  while(1);
}

//----------------------------------------------------------------------------------------
/// \brief  
///
/// \descr 
///
/// \param  
///
/// \return 
//----------------------------------------------------------------------------------------
void timer_isr(void)
{
  static uint32_t cpt[4] = {0};
  static uint64_t intid[4] = {0};
  uint32_t ActiveCore = GetActiveCoreId();

  intid[ActiveCore] = ARM64_READ_SYSREG(ICC_IAR0_EL1);

  if(cpt[ActiveCore] % 2 == 0)
  {
    switch(ActiveCore){
       case 0: LED_1_ON(); break;
       case 1: LED_2_ON(); break;
       case 2: LED_3_ON(); break;
       case 3: LED_4_ON(); break;
       default: break;
    }
  }
  else
  {
    switch(ActiveCore){
       case 0: LED_1_OFF(); break;
       case 1: LED_2_OFF(); break;
       case 2: LED_3_OFF(); break;
       case 3: LED_4_OFF(); break;
       default: break;
    }
  }

  ARM64_WRITE_SYSREG(CNTPS_TVAL_EL1, 0x0BEBC200);
  cpt[ActiveCore]++;
  ARM64_WRITE_SYSREG(ICC_EOIR0_EL1, intid[ActiveCore]);
}

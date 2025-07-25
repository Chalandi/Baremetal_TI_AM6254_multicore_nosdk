
#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

#define PADCFG_CTRL0_CFG0_PADCONFIG3   0x000F400Cul
#define PADCFG_CTRL0_CFG0_PADCONFIG4   0x000F4010ul
#define PADCFG_CTRL0_CFG0_PADCONFIG5   0x000F4014ul
#define PADCFG_CTRL0_CFG0_PADCONFIG6   0x000F4018ul

#define GPIO_DIR01       0x00600010ul
#define GPIO_OUT_DATA01  0x00600014ul
#define GPIO_SET_DATA01  0x00600018ul
#define GPIO_CLR_DATA01  0x0060001Cul

#define LED_1   6
#define LED_2   5
#define LED_3   4
#define LED_4   3


#define LED_INIT()  do{                                                                            \
                    *(volatile uint32_t*)(PADCFG_CTRL0_CFG0_PADCONFIG3) &= ~((uint32_t)1ul <<21);  \
                    *(volatile uint32_t*)(PADCFG_CTRL0_CFG0_PADCONFIG4) &= ~((uint32_t)1ul <<21);  \
                    *(volatile uint32_t*)(PADCFG_CTRL0_CFG0_PADCONFIG5) &= ~((uint32_t)1ul <<21);  \
                    *(volatile uint32_t*)(PADCFG_CTRL0_CFG0_PADCONFIG6) &= ~((uint32_t)1ul <<21);  \
                    *(volatile uint32_t*)(GPIO_DIR01) &= ~((uint32_t)0x78ul);                      \
                    *(volatile uint32_t*)(GPIO_CLR_DATA01) |= 0x78;                                \
                    *(volatile uint32_t*)(GPIO_OUT_DATA01) &= ~((uint32_t)0x78ul);                 \
                    }while(0)

#define LED_ON(x)      do{*(volatile uint32_t*)(GPIO_SET_DATA01) |= (1ul << x); *(volatile uint32_t*)(GPIO_OUT_DATA01) |= (1ul << x);}while(0)
#define LED_OFF(x)     do{*(volatile uint32_t*)(GPIO_CLR_DATA01) |= (1ul << x); *(volatile uint32_t*)(GPIO_OUT_DATA01) &= ~(uint32_t)(1ul << x);}while(0)


#define LED_1_ON()   LED_ON(LED_1)
#define LED_2_ON()   LED_ON(LED_2)
#define LED_3_ON()   LED_ON(LED_3)
#define LED_4_ON()   LED_ON(LED_4)

#define LED_1_OFF()  LED_OFF(LED_1)
#define LED_2_OFF()  LED_OFF(LED_2)
#define LED_3_OFF()  LED_OFF(LED_3)
#define LED_4_OFF()  LED_OFF(LED_4)

#endif /*__LED_H__*/


#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

/* GPIO MACROS */
#define GPIO0      (uintptr_t)0x00600010
#define GPIO1      (uintptr_t)0x00601010
#define MCU_GPIO0  (uintptr_t)0x04201010

#define GPIO_CONFIG_AS_OUTPUT(instance_base, pin)  do{                                                                                                             \
                    if(instance_base == GPIO0){                                                                                                                    \
                      *(volatile uint32_t*)(uintptr_t)(0x000F4000 + (pin * 4)) /* PADCFG_CTRL0_CFG0_PADCONFIG */ = 0x08014007ul;                                   \
                    }else if(instance_base == MCU_GPIO0){                                                                                                          \
                      *(volatile uint32_t*)(uintptr_t)(0x4084000 + (pin * 4)) /* MCU_PADCFG_CTRL0_CFG0_PADCONFIG */ = 0x08014007ul;                                \
                    }else{/* GPIO1 has no pad configuration */                                                                                                     \
                    }                                                                                                                                              \
                    *(volatile uint32_t*)(instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x0C /* GPIO_CLR_DATA */) |= (1ul << (pin % 32));              \
                    *(volatile uint32_t*)(instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x04 /* GPIO_OUT_DATA */) &= ~((uint32_t)(1ul << (pin % 32))); \
                    *(volatile uint32_t*)(instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x00 /* GPIO_DIR      */) &= ~((uint32_t)(1ul << (pin % 32))); \
                    }while(0)

#define OUTPUT_HIGH(instance_base, pin)   do{*(volatile uint32_t*)(instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x08 /* GPIO_SET_DATA */) = (1ul << (pin % 32));}while(0)
#define OUTPUT_LOW(instance_base, pin)    do{*(volatile uint32_t*)(instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x0C /* GPIO_CLR_DATA */) = (1ul << (pin % 32));}while(0)


/* LED MACROS */

#define LED_1   6
#define LED_2   5
#define LED_3   4
#define LED_4   3

#define LED_INIT()  do{                                  \
                    GPIO_CONFIG_AS_OUTPUT(GPIO0, LED_1); \
                    GPIO_CONFIG_AS_OUTPUT(GPIO0, LED_2); \
                    GPIO_CONFIG_AS_OUTPUT(GPIO0, LED_3); \
                    GPIO_CONFIG_AS_OUTPUT(GPIO0, LED_4); \
                    }while(0)

#define LED_1_ON()   OUTPUT_HIGH(GPIO0, LED_1)
#define LED_2_ON()   OUTPUT_HIGH(GPIO0, LED_2)
#define LED_3_ON()   OUTPUT_HIGH(GPIO0, LED_3)
#define LED_4_ON()   OUTPUT_HIGH(GPIO0, LED_4)

#define LED_1_OFF()  OUTPUT_LOW(GPIO0, LED_1)
#define LED_2_OFF()  OUTPUT_LOW(GPIO0, LED_2)
#define LED_3_OFF()  OUTPUT_LOW(GPIO0, LED_3)
#define LED_4_OFF()  OUTPUT_LOW(GPIO0, LED_4)




#if 0
instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x00 /* GPIO_DIR      */
instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x04 /* GPIO_OUT_DATA */
instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x08 /* GPIO_SET_DATA */
instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x0C /* GPIO_CLR_DATA */
instance_base + (uintptr_t)((pin/32) * 0x28) + (uintptr_t)0x10 /* GPIO_IN_DATA  */
#endif

#endif /*__GPIO_H__*/

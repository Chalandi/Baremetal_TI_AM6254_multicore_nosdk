// ***************************************************************************************
// Filename    : Startup.c
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 16.05.2025
// 
// Description : C/C++ Runtime Setup (Crt0)
// 
// ***************************************************************************************

#include <stdint.h>

//=========================================================================================
// types definition
//=========================================================================================
typedef struct {

    unsigned long  sourceAddr;  /* Source Address (section in ROM memory) */
    unsigned long  targetAddr;  /* Target Address (section in RAM memory) */
    unsigned long  size;        /* length of section (bytes) */

} runtimeCopyTable_t;

typedef struct {

    unsigned long  Addr;  /* source Address (section in RAM memory) */
    unsigned long  size;  /* length of section (bytes) */

} runtimeClearTable_t;

//=========================================================================================
// linker variables
//=========================================================================================
extern const runtimeCopyTable_t __RUNTIME_COPY_TABLE[];
extern const runtimeClearTable_t __RUNTIME_CLEAR_TABLE[];
extern unsigned long __CTOR_LIST__[];

//=========================================================================================
// defines
//=========================================================================================
#define __STARTUP_RUNTIME_COPYTABLE   (runtimeCopyTable_t*)(&__RUNTIME_COPY_TABLE[0])
#define __STARTUP_RUNTIME_CLEARTABLE  (runtimeClearTable_t*)(&__RUNTIME_CLEAR_TABLE[0])
#define __STARTUP_RUNTIME_CTORS       (unsigned long*)(&__CTOR_LIST__[0])

//=========================================================================================
// function prototype
//=========================================================================================
void Startup_Init(void);
static void Startup_InitRam(void);
static void Startup_InitCtors(void);
static void Startup_RunMulticoreApplication(void);
static void Startup_Unexpected_Exit(void);
static void Startup_InitSystemClock(void);
static void Startup_InitSystemPeripheral(void);

//=========================================================================================
// extern function prototype
//=========================================================================================
extern void main(void) __attribute__((weak));

//-----------------------------------------------------------------------------------------
/// \brief  Startup_Init function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void Startup_Init(void)
{
  /* Configure the system clock */
  Startup_InitSystemClock();

  /* Init the system peripheral */
  Startup_InitSystemPeripheral();

  /* Initialize the RAM memory */
  Startup_InitRam();

  /* Initialize the non-local C++ objects */
  Startup_InitCtors();

  /* Start the multicore application */
  Startup_RunMulticoreApplication();
}


//-----------------------------------------------------------------------------------------
/// \brief  Startup_InitRam function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_InitRam(void)
{
  unsigned long index = 0;

  /* Clear Table */
  while((__STARTUP_RUNTIME_CLEARTABLE)[index].Addr != (unsigned long)-1 && (__STARTUP_RUNTIME_CLEARTABLE)[index].size != (unsigned long)-1)
  {
    for(unsigned int cpt = 0; cpt < (__STARTUP_RUNTIME_CLEARTABLE)[index].size; cpt += sizeof(unsigned long))
    {
      *(volatile unsigned long*)((unsigned long)((__STARTUP_RUNTIME_CLEARTABLE)[index].Addr) + cpt) = 0;
    }
    index++;
  }

  /* Copy Table */
  index = 0;
  while((__STARTUP_RUNTIME_COPYTABLE)[index].sourceAddr != (unsigned long)-1 &&
        (__STARTUP_RUNTIME_COPYTABLE)[index].targetAddr != (unsigned long)-1 &&
        (__STARTUP_RUNTIME_COPYTABLE)[index].size       != (unsigned long)-1
       )
  {
    for(unsigned int cpt = 0; cpt < (__STARTUP_RUNTIME_COPYTABLE)[index].size; cpt += sizeof(unsigned long))
    {
      *(volatile unsigned long*)((unsigned long)((__STARTUP_RUNTIME_COPYTABLE)[index].targetAddr) + cpt) = 
               *(volatile unsigned long*)((unsigned long)((__STARTUP_RUNTIME_COPYTABLE)[index].sourceAddr) + cpt);
    }
    index++;
  }
}

//-----------------------------------------------------------------------------------------
/// \brief Startup_InitCtors function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_InitCtors(void)
{
  for(unsigned long entry = 1; entry <= (__STARTUP_RUNTIME_CTORS)[0]; entry++)
  {
    ((void (*)(void))((__STARTUP_RUNTIME_CTORS)[entry]))();
  }
}

//-----------------------------------------------------------------------------------------
/// \brief  Startup_RunApplication function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_RunMulticoreApplication(void)
{
  /* release the secondary cores */
  __asm volatile("sev");

  /* call the main function */
  (void)main();

  /* Catch unexpected exit from main or if main does not exist */
  Startup_Unexpected_Exit();
}

//-----------------------------------------------------------------------------------------
/// \brief  Startup_Unexpected_Exit function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_Unexpected_Exit(void)
{
  for(;;);
}

//-----------------------------------------------------------------------------------------
/// \brief  Startup_InitSystemClock function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_InitSystemClock(void)
{
   /* clock initialized by the SBL */
}

//-----------------------------------------------------------------------------------------
/// \brief  Startup_InitSystemPeripheral function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_InitSystemPeripheral(void)
{
#include "led.h"
  LED_INIT();
}
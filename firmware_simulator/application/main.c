/***********************************************************************************************************************
File: main.c                                                                

Description:
Container for the Cprimer "Server" application firmware.  
***********************************************************************************************************************/

#include "configuration.h"

extern void RefreshLedMatrix(u32 *u32Address, u32 u32Bytes);

#ifndef SOLUTION
/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/

/***********************************************************************************************************************
Main Program
***********************************************************************************************************************/

void main(void)
{
  struct UserApp1_asLedMatrixColors
  {
    u8 u8Red;
    u8 u8Grn;
    u8 u8Blu;
  };
  
  struct UserApp1_asLedMatrixColors asLedMatrixColors [20];
  
  u8 u8BitMask = 0x80;        /* Start with MSB mask for a single bit */
  u8 *pu8BytePointer = &asLedMatrixColors[0].u8Red;    /* Starting address for access pointer */
  
  asLedMatrixColors[0].u8Red = 0x9a;
  asLedMatrixColors[0].u8Grn = 0x9a;
  asLedMatrixColors[0].u8Blu = 0x9a;
  asLedMatrixColors[1].u8Red = 0x9a;
  asLedMatrixColors[1].u8Grn = 0x9a;
  asLedMatrixColors[1].u8Blu = 0x9a;

  RefreshLedMatrix((u32*)asLedMatrixColors, 60);
  
#if 0  
  for(u16 i = 0; i < 60; i++)                                  // 54, 114: 5 instruction cycles
  {
    if(u8BitMask & *pu8BytePointer)                            // 59, 122: 4 instruction cycles
    { 
      /* One-pulse is 900ns high then 300ns low */
      AT91C_BASE_PIOA->PIO_SODR = 0x01;                        // 63 need this 120, 177, 234: 4 instruction cycles, pin update on 3rd
//      for(u8 j = 0; j < 10; j++);                              // 54, 209: j x 8 instruction cycles + x
      if(u8BitMask == 0x01)                                      // 67: 7 instruction cycles (else)
      {
        u8BitMask = 0x80;                                        // 188, 343: 5 instruction cycles
        pu8BytePointer++;
      }
      else
      {
        u8BitMask >>= u8BitMask;                                 // 74: 4 instruction cycles
        asm("nop");                                              // 78: instruction cycle
        asm("nop");                                              // 1 instruction cycle
        asm("nop");                                              // 1 instruction cycle
        asm("nop");                                              // 1 instruction cycle
      }
      for(u8 j = 0; j < 2; j++);                               // 82: 24 instruction cycles
      asm("nop");                                              // 106: 1 instruction cycle
      AT91C_BASE_PIOA->PIO_CODR = 0x01;                       // Need this to be 107: 4 instruction cycles, pin update on 3rd
//      for(u8 j = 2; j > 0; j--);                               // 111: j x 12 instruction cycles + x
//      asm("nop");                                              // 135: 1 instruction cycle
    } /* end if 1-bit */
    else
    {
      /* Zero-pulse is 300ns high then 900ns low */
      AT91C_BASE_PIOA->PIO_SODR = 0x01;                        // 63, 129 need this 120: 4 instruction cycles, pin update on 3rd
      for(u8 j = 0; j < 1; j++);                               // 82: 12 instruction cycles
      asm("nop");                                              // 106: 1 instruction cycle
      asm("nop");                                              // 106: 1 instruction cycle
      AT91C_BASE_PIOA->PIO_CODR = 0x01;                       // Need this to be 107: 4 instruction cycles, pin update on 3rd
      if(u8BitMask == 0x01)                                      // 67: 7 instruction cycles (else)
      {
        u8BitMask = 0x80;                                        // 188, 343: 5 instruction cycles
        pu8BytePointer++;
      }
      else
      {
        u8BitMask >>= u8BitMask;                                 // 74: 4 instruction cycles
        asm("nop");                                              // 78: instruction cycle
        asm("nop");                                              // 1 instruction cycle
        asm("nop");                                              // 1 instruction cycle
        asm("nop");                                              // 1 instruction cycle
      }
      
      for(u8 j = 0; j < 1; j++);                                 // 82: 12 instruction cycles

    } /* end if 0-bit */
    
    /* Shift mask and reset/move pointer if necessary */
  }
#endif
  
  while(1)
  {
    
  } /* end while(1) main loop */
  
} /* end main() */


/***********************************************************************************************************************
* Function definitions
***********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* SOLUTION */
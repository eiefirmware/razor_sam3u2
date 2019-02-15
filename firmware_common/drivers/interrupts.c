/*!*********************************************************************************************************************
@file interrupts.c                                                               
@brief Definitions for main system interrupts.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- 
PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- 


***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>ISR"
***********************************************************************************************************************/
/* New variables */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword)  */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;             /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variables names shall start with "ISR_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Interrupt Service Routine Definitions
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void InterruptSetup(void)

@brief Disables and clears all NVIC interrupts and sets up interrupt priorities.

Peripheral interrupt sources should be enabled outside of this function.
As this should be the first interrupt-related function that is called in
the system, we can conclude that clearing all the pending flags should
work since no peripheral interrupt sources should be connected yet. 

Requires:
- IRQn_Type enum is the sequentially ordered interrupt values starting at 0

Promises:
- Interrupt priorities are set 
- All NVIC interrupts are disabled and all pending flags are cleared

*/
void InterruptSetup(void)
{
  static const u32 au32PriorityConfig[PRIORITY_REGISTERS] = {IPR0_INIT, IPR1_INIT, IPR2_INIT, 
                                                             IPR3_INIT, IPR4_INIT, IPR5_INIT,
                                                             IPR6_INIT, IPR7_INIT};
  
  /* Disable all interrupts and ensure pending bits are clear */
  for(u8 i = 0; i < U8_SAM3U2_INTERRUPT_SOURCES; i++)
  {
    NVIC_DisableIRQ( (IRQn_Type)i );
    NVIC_ClearPendingIRQ( (IRQn_Type)i);
  } 

  /* Set interrupt priorities */
  for(u8 i = 0; i < PRIORITY_REGISTERS; i++)
  {
    ((u32*)(AT91C_BASE_NVIC->NVIC_IPR))[i] = au32PriorityConfig[i];
  }
      
} /* end InterruptSetup(void) */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void SysTick_Handler(void)

@brief Updates the global ms timer.  

This interrupt is always enabled and running in 
the system and is essential for system timing and sleep wakeup.
This ISR should be as fast as possible!

Requires:
- NONE

Promises:
- System tick interrupt pending flag is cleared
- G_u32SystemFlags _SYSTEM_SLEEPING cleared

@param G_u32SystemTime1ms counter is incremented by 1

*/
void SysTick_Handler(void)
{
  /* Clear the sleep flag */
  G_u32SystemFlags &= ~_SYSTEM_SLEEPING;
  
  /* Update Timers */
  G_u32SystemTime1ms++;
  if( (G_u32SystemTime1ms % 1000) == 0)
  {
    G_u32SystemTime1s++;
  }
    
} /* end SysTickHandler(void) */


/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void PIOA_IrqHandler(void)

@brief Parses the PORTA GPIO interrupts and handles them appropriately.  

Note that all PORTA GPIO interrupts are ORed and will trigger this handler, 
therefore any expected interrupt that is enabled must be parsed out and handled.

Requires:
- The button IO bits match the interrupt flag locations

Promises:
- Buttons: sets the active button's debouncing flag, clears the interrupt
  and initializes the button's debounce timer.

*/
void PIOA_IrqHandler(void)
{
  u32 u32GPIOInterruptSources;
  u32 u32ButtonInterrupts;
  u32 u32CurrentButtonLocation;

  /* Grab a snapshot of the current PORTA status flags (clears all flags) */
  u32GPIOInterruptSources = AT91C_BASE_PIOA->PIO_ISR;

  /******** DO NOT set a breakpoint before this line of the ISR because the debugger
  will "read" PIO_ISR and clear the flags. ********/
  
  /* Examine button interrupts */
  u32ButtonInterrupts = u32GPIOInterruptSources & GPIOA_BUTTONS;
  
  /* Check if any port A buttons interrupted */
  if(u32ButtonInterrupts)
  {
    /* Scan through the flags looking for ones that are set */
    u32CurrentButtonLocation = 0x00000001;
    for(u8 i = 0; i < 32; i++)
    {
      /* If the bit is set, then start debouncing (also disables interrupt) */
      if(u32ButtonInterrupts & u32CurrentButtonLocation)
      {
        ButtonStartDebounce(u32CurrentButtonLocation, PORTA);
      }
      
      /* Shift the mask to look at the next bit */
      u32CurrentButtonLocation <<= 1;
    }
        
  } /* end port A button interrupt checking */
  
  /* Clear the PIOA pending flag and exit */
  NVIC_ClearPendingIRQ(IRQn_PIOA);
  
} /* end PIOA_IrqHandler() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void PIOB_IrqHandler(void)

@brief Parses the PORTB GPIO interrupts and handles them appropriately.  

Note that all PORTB GPIO interrupts are ORed and will trigger this handler, 
so any expected interrupt that is enabled must be parsed out and handled.

Requires:
- The button IO bits match the interrupt flag locations

Promises:
- Buttons: sets the active button's debouncing flag, clears the interrupt
  and initializes the button's debounce timer.

*/
void PIOB_IrqHandler(void)
{
  u32 u32GPIOInterruptSources;
  u32 u32ButtonInterrupts;
  u32 u32CurrentButtonLocation;

  /* Grab a snapshot of the current PORTB status flags (clears all flags) */
  u32GPIOInterruptSources = AT91C_BASE_PIOB->PIO_ISR;

  /******** DO NOT set a breakpoint before this line of the ISR because the debugger
  will "read" PIO_ISR and clear the flags. ********/
  
  /* Examine button interrupts */
  u32ButtonInterrupts = u32GPIOInterruptSources & GPIOB_BUTTONS;
  
  /* Check if any port B buttons interrupted */
  if(u32ButtonInterrupts)
  {
    /* Scan through the flags looking for ones that are set */
    u32CurrentButtonLocation = 0x00000001;
    for(u8 i = 0; i < 32; i++)
    {
      /* If the bit is set, then start debouncing (also disables interrupt) */
      if(u32ButtonInterrupts & u32CurrentButtonLocation)
      {
        ButtonStartDebounce(u32CurrentButtonLocation, PORTB);
      }
      
      /* Shift the mask to look at the next bit */
      u32CurrentButtonLocation <<= 1;
    }
        
  } /* end port B button interrupt checking */
  
  /* Clear the PIOB pending flag and exit */
  NVIC_ClearPendingIRQ(IRQn_PIOB);
  
} /* end PIOB_IrqHandler() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void HardFault_Handler(void)
 
@brief A non-maskable (always available) core interrupt that occurs when 
something extraordinary occurs.  

In many cases, this is referencing an invalid address, but can be other 
events of various levels of mystery.  

Requires:
- NONE

Promises:
- Red LED is on, all others off
- Code is held here for debug purposes

*/
void HardFault_Handler(void)
{
  LedOff(WHITE);
  LedOff(CYAN);
  LedOff(PURPLE);
  LedOff(ORANGE);
  LedOff(BLUE);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOn(RED);

  while(1);  /* !!!!! update to log and/or report error and/or restart */
  
} /* end HardFault_Handler() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/

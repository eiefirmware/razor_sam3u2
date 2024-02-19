/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;                             /*!< @brief The state machine function pointer */

static RGBValueType UserApp1_asLedMatrixColors[U8_TOTAL_MATRIX_LEDS];   /*!< @brief Storage for complete LED matrix refresh */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* Set up LCD display */
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, "Addressable LEDs");
  LcdMessage(LINE2_START_ADDR, "RED  GRN   BLU   RGB");
  
  /* Initialize storage array */
  for(u8 i = 0; i < U8_TOTAL_MATRIX_LEDS; i++)
  {
    UserApp1_asLedMatrixColors[i].u8Red = 0;
    UserApp1_asLedMatrixColors[i].u8Grn = 0;
    UserApp1_asLedMatrixColors[i].u8Blu = 0;
  }
  
  /* Ramp up all LEDs to white */
  RefreshLedMatrix();
  
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void RefreshLedMatrix(void)

@brief Sends all data in UserApp1_asLedMatrixColors to the LED matrix daughter board(s).
This function is blocking and interrupts must be disabled during data transfer.
Update time for a single daughter board is 480us, so 3 or more daughter boards will result
in disturbing the 1ms system timing with every refresh.

A "0" bit is a high signal for 300ns followed by a low signal for 900ns (with 150ns tolerance on each)
A "1" bit is a high signal for 900ns followed by a low signal for 300ns (with 150ns tolerance on each)

Each 24-bit value is transferred MSB first starting with red. Instruction cycles are detailed in 
each section.  At 48MHz, 1 instruction cycle is 20.8ns; a bit period is 1.2us or 57.6 instruction cycles.

Requires:
- UserApp1_asLedMatrixColors is ready to be sent
- UserApp1_asLedMatrixColors contents are sequential in memory
- Interrupts can be disabled 
- LED matrix output is pin AT91C_BASE_PIOA->PIO_SODR 

Promises:
- All bits in UserApp1_asLedMatrixColors are sequentially clocked out following the
  single wire data transfer protocol for the B3DK3BRG LEDs.

*/
void RefreshLedMatrix(void)
{
  u8 u8BitMask = 0x80;        /* Start with MSB mask for a single bit */
  u8 pu8BytePointer = &UserApp1_asLedMatrixColors.u8Red;    /* Starting address for access pointer */
  
  for(u16 i = 0; i < U16_TOTAL_LED_BYTES; i++)                  // x instruction cycles
  {
    if(u8BitMask & *pu8BytePointer)                              // x instruction cycles
    {
      AT91C_BASE_PIOA->PIO_SODR |= AT91C_BASE_PIOA->PIO_SODR;  // 3 instruction cycles
      for(u8 j = 0; j < 10; j++);                              // j x 3 instruction cycles + x
      AT91C_BASE_PIOA->PIO_CODR |= AT91C_BASE_PIOA->PIO_SODR;  // 3 instruction cycles
      for(u8 j = 0; j < 3; j++);                               // j x 3 instruction cycles + x

    }
    else
    {
      AT91C_BASE_PIOA->PIO_SODR |= AT91C_BASE_PIOA->PIO_SODR;  // 3 instruction cycles
      for(u8 j = 0; j < 3; j++);                               // j x 3 instruction cycles + x
      AT91C_BASE_PIOA->PIO_CODR |= AT91C_BASE_PIOA->PIO_SODR;  // 3 instruction cycles
      for(u8 j = 0; j < 10; j++);                              // j x 3 instruction cycles + x
    }
    
    /* Shift mask and reset if necessary */
    u8BitMask >>= u8BitMask;                                   // 1 instruction cycle
    if(u8BitMask == 0)
    {
      u8BitMask = 0x80;                                        // 1 instruction cycle
    }
    else
    {
      asm(nop);                                                // 1 instruction cycle
    }
  }
  

} /* end RefreshLedMatrix */


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!*********************************************************************************************************************
@file blade_api.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

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
- void BladeApiInitialize(void)
- void BladeApiRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>BladeApi"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32BladeApiFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "BladeApi_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type BladeApi_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 BladeApi_u32Timeout;                           /*!< @brief Timeout counter used across states */


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
@fn void BladeApiInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void BladeApiInitialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    BladeApi_pfStateMachine = BladeApiSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    BladeApi_pfStateMachine = BladeApiSM_Error;
  }

} /* end BladeApiInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void BladeApiRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void BladeApiRunActiveState(void)
{
  BladeApi_pfStateMachine();

} /* end BladeApiRunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Currently, the Blade task does not do anything beyond provide configuration
resources.  Options of an Idle state could include pinging configured blades
or monitoring conformance to system task time.  */
static void BladeApiSM_Idle(void)
{
    
} /* end BladeApiSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void BladeApiSM_Error(void)          
{
  
} /* end BladeApiSM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

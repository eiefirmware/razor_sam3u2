/*!*********************************************************************************************************************
@file blade_imu_lsm9ds1.c                                                                
@brief Driver for ST LSM9DS1 IMU.

Provides configuration service and up to 1kHz of 3-axis acceleration, gyro, and compass data.

----------------------------------------------------------------------------------------------------------------------
To start a new Blade task using this blade_lsm9ds1:
 1. Copy both blade_lsm9ds1.c and blade_lsm9ds1.h to the \application\blade folder
 2. Rename the files blade_taskname.c and blade_taskname.h where "taskname" should
    match the Blade hardware version
 3. Add blade_taskname.c and blade_taskname.h to the Application>Blades Include and 
    Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances 
    of "blade_lsm9ds1" with "blade_taskname"
 5. Use ctrl-h to find and replace all instances of "BladeLsm9ds1" with "BladeTaskName"
 6. Use ctrl-h to find and replace all instances of "BLADE_LSM9DS1" with "BLADE_NEW_TASK_NAME"
 7. If using the new task, add a call to BladeTaskNameInitialize() in the init section of main
 8. If using the new task, ad a call to BladeTaskNameRunActiveState() in the Super Loop section of main
 9. Update blade_taskname.h per the instructions at the top of blade_taskname.h
10. Delete this text (between the dashed lines) and update the description to describe your task

Note: prior to pushing the completed Blade application to the Master Branch, the
Initialization and Main loop function calls should be removed from main.  
----------------------------------------------------------------------------------------------------------------------

BLADE TASK MAIN FUNCTION CALLS
  BladeLsm9ds1Initialize();
  BladeLsm9ds1ActiveState();

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
- void BladeLsm9ds1Initialize(void)
- void BladeLsm9ds1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>BladeLsm9ds1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32BladeLsm9ds1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "BladeLsm9ds1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type BladeLsm9ds1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 BladeLsm9ds1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

// AccelOn()
// GyroOn()
// CompassOn()
// WriteRegister

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void BladeLsm9ds1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void BladeLsm9ds1Initialize(void)
{
  ErrorStatusType eErrorStatus = SUCCESS;
  bool bAccel = FALSE;
  bool bGyro = FALSE;
  
  /* Blade resource requests: I2C SCL, SDA, and IO1 interrupt line */
  eErrorStatus += BladeRequestPin(BLADE_PIN_0, DIGITAL_IN);
  eErrorStatus += BladeRequestPin(BLADE_PIN_8, PERIPHERAL);
  eErrorStatus += BladeRequestPin(BLADE_PIN_9, PERIPHERAL);
  if(eErrorStatus)
  {
    DebugPrintf("LSM9DS1 Blade pin resources not available\n\r"); 
  }
  
  /* Ping the accelerometer to check it's responding */
  
  
  /* If good initialization, set state to Idle */
  if( eErrorStatus == SUCCESS )
  {
    BladeLsm9ds1_pfStateMachine = BladeLsm9ds1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    BladeLsm9ds1_pfStateMachine = BladeLsm9ds1SM_Error;
  }

} /* end BladeLsm9ds1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void BladeLsm9ds1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void BladeLsm9ds1RunActiveState(void)
{
  BladeLsm9ds1_pfStateMachine();

} /* end BladeLsm9ds1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void BladeLsm9ds1SM_Idle(void)
{
    
} /* end BladeLsm9ds1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void BladeLsm9ds1SM_Error(void)          
{
  
} /* end BladeLsm9ds1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

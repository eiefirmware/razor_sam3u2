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
- extern from ant_api.c

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

// Globals for passing data from the ANT application to the API
extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c

extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c

extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c

extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c

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
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


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

static void UserApp1SM_WaitAntReady(void);
static void UserApp1SM_WaitChannelOpen(void);
static void UserApp1SM_ChannelOpen(void);

void UserApp1Initialize(void)
{
  AntAssignChannelInfoType sChannelInfo;

  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_UNCONFIGURED)
  {
    sChannelInfo.AntChannel = U8_ANT_CHANNEL_USERAPP;
    sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
    sChannelInfo.AntChannelPeriodHi = U8_ANT_CHANNEL_PERIOD_HI_USERAPP;
    sChannelInfo.AntChannelPeriodLo = U8_ANT_CHANNEL_PERIOD_LO_USERAPP;
    
    sChannelInfo.AntDeviceIdHi = U8_ANT_DEVICE_HI_USERAPP;
    sChannelInfo.AntDeviceIdLo = U8_ANT_DEVICE_LO_USERAPP;
    sChannelInfo.AntDeviceType = U8_ANT_DEVICE_TYPE_USERAPP;
    sChannelInfo.AntTransmissionType = U8_ANT_TRANSMISSION_TYPE_USERAPP;
    
    sChannelInfo.AntFrequency = U8_ANT_FREQUENCY_USERAPP;
    sChannelInfo.AntTxPower = U8_ANT_TX_POWER_USERAPP;
    
    sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;      
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }
    
    AntAssignChannel(&sChannelInfo);
  }
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_WaitAntReady;
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


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for Ant Channel to be configured */
static void UserApp1SM_WaitAntReady(void){
  
  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_CONFIGURED){
  
    if(AntOpenChannelNumber(U8_ANT_CHANNEL_USERAPP)){
      UserApp1_pfStateMachine = UserApp1SM_WaitChannelOpen;
    }
    else{
      UserApp1_pfStateMachine = UserApp1SM_Error;
    }
    
  
  }


}// End of UserApp1SM_waitAntReady()

/* Wait for Channel to open*/
static void UserApp1SM_WaitChannelOpen(void){
  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_OPEN){
    UserApp1_pfStateMachine = UserApp1SM_ChannelOpen;
  }



} // end of UserApp1SM_WaitChannelOpen()

static void UserApp1SM_ChannelOpen(void){
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  static u8 intermediate = 0;
  u8 au8DataContent[8] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48}; //, 0x49, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56};
  if(AntReadAppMessageBuffer()){
    
    if(G_eAntApiCurrentMessageClass == ANT_DATA){
      // We got some data
      for(int i = 0; i < ANT_DATA_BYTES; i += 2){
        intermediate = G_au8AntApiCurrentMessageBytes[i];
        intermediate += G_au8AntApiCurrentMessageBytes[i + 1] << 4;
        au8DataContent[i] = intermediate;
      }
      
      LcdMessage(LINE2_START_ADDR, au8DataContent);
      
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK){

      
      
      au8TestMessage[7]++;
      
      if(au8TestMessage[7] == 0){
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0){
          au8TestMessage[5]++;
        }
      }
      
      au8TestMessage[0] = 0x00;
      au8TestMessage[1] = 0x00;
      au8TestMessage[2] = 0x00;
      au8TestMessage[3] = 0x00;
      
      if( IsButtonPressed(BUTTON0)){
        au8TestMessage[0] = 0xff;
      }
      if( IsButtonPressed(BUTTON1)){
        au8TestMessage[1] = 0xff;
      }
      if( IsButtonPressed(BUTTON2)){
        au8TestMessage[2] = 0xff;
      }
      if( IsButtonPressed(BUTTON3)){
        au8TestMessage[3] = 0xff;
      }
      
      
      
      AntQueueBroadcastMessage(U8_ANT_CHANNEL_USERAPP, au8TestMessage);
      
      // A channel period has gone by
    }
    
    
    
    
  
  }




}
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

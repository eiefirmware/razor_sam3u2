/*!*********************************************************************************************************************
@file user_app1.h                                                                
@brief Header file for user_app1

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define U8_ANT_CHANNEL_USERAPP                  (u8)ANT_CHANNEL_0 /* Channel 0 � 7 */

#define U8_ANT_DEVICE_LO_USERAPP                (u8)0x9B /* Low byte of two-byte Device # */

#define U8_ANT_DEVICE_HI_USERAPP                (u8)0x17 /* High byte of two-byte Device # */

#define U8_ANT_DEVICE_TYPE_USERAPP              (u8)1 /* 1 � 255 */

#define U8_ANT_TRANSMISSION_TYPE_USERAPP        (u8)1 /* 1-127 (MSB is pairing bit) */

#define U8_ANT_CHANNEL_PERIOD_LO_USERAPP        (u8)0x00 /* Low byte of two-byte channel period */

#define U8_ANT_CHANNEL_PERIOD_HI_USERAPP        (u8)0x20 /* High byte of two-byte channel period */

#define U8_ANT_FREQUENCY_USERAPP                (u8)50 /* 2400MHz + this number 0 � 99 */

#define U8_ANT_TX_POWER_USERAPP RADIO_TX_POWER_4DBM /* RADIO_TX_POWER_xxx */



#endif /* __USER_APP1_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

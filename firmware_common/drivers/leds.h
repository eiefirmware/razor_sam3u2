/*!**********************************************************************************************************************
@file leds.h                                                               
@brief Header file for leds.c
**********************************************************************************************************************/

#ifndef __LEDS_H
#define __LEDS_H

#include "configuration.h"


/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/

/*! 
@enum LedModeType
@brief The mode determines how the task manages the LED */
typedef enum {LED_NORMAL_MODE, LED_BLINK_MODE, LED_PWM_MODE} LedModeType;  

/*! 
@enum LedPWMDutyType
@brief Duty cycle state when tracking PWM mode*/
typedef enum {LED_PWM_DUTY_LOW = 0, LED_PWM_DUTY_HIGH = 1} LedPWMDutyType; 

/*! 
@enum LedRateType
@brief Standard blinky values for blinking.  

Other blinking rate values may be added as required.  The labels are frequencies, 
but the values are the toggling period in ms.

*** The PWM rates are set up to allow incrementing and decrementing rates within the allowed values.  
Be careful at the edge cases. ***
*/
typedef enum {LED_0HZ = 0, LED_0_5HZ = 1000, LED_1HZ = 500, LED_2HZ = 250, LED_4HZ = 125, LED_8HZ = 63,
              LED_PWM_0 = 0,   LED_PWM_5 = 1,   LED_PWM_10 = 2,  LED_PWM_15 = 3,  LED_PWM_20 = 4, 
              LED_PWM_25 = 5,  LED_PWM_30 = 6,  LED_PWM_35 = 7,  LED_PWM_40 = 8,  LED_PWM_45 = 9, 
              LED_PWM_50 = 10, LED_PWM_55 = 11, LED_PWM_60 = 12, LED_PWM_65 = 13, LED_PWM_70 = 14, 
              LED_PWM_75 = 15, LED_PWM_80 = 16, LED_PWM_85 = 17, LED_PWM_90 = 18, LED_PWM_95 = 19, 
              LED_PWM_100 = 20
             } LedRateType;

/*! 
@struct LedControlType
@brief Required parameters for the task to track what each LED is doing. 
*/
typedef struct 
{
  LedModeType eMode;              /*!< @brief Current mode */
  LedRateType eRate;              /*!< @brief Current rate */
  u16 u16Count;                   /*!< @brief Value of current duty cycle counter */
  LedPWMDutyType eCurrentDuty;    /*!< @brief Phase of the current duty cycle */
}LedControlType;



/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void LedOn(LedNameType eLED_);
void LedOff(LedNameType eLED_);
void LedToggle(LedNameType eLED_);
void LedBlink(LedNameType eLED_, LedRateType eBlinkRate_);
void LedPWM(LedNameType eLED_, LedRateType ePwmRate_);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void LedInitialize(void);
void LedRunActiveState(void);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void LedSM_Idle(void);
static void LedSM_Error(void);


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/




#endif /* __LEDS_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

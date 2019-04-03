/*!*********************************************************************************************************************
@file blade_api.h                                                                
@brief Header file for blade_api

**********************************************************************************************************************/

#ifndef __BLADE_API_H
#define __BLADE_API_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/

/*! 
@enum BladePinIOType
@brief Available options for Blade pin configuration.  

Since the Blade pinout is standardized, "PERIPHERAL" refers to the 
designated peripheral available on a particular pin.  It is assumed that
the users understand this.

The relationship is shown here:

BLADE_PIN0: GPIO 0 or Analog 0            
BLADE_PIN1: GPIO 1 or Analog 1                    
BLADE_PIN2: GPIO 2 or UART PIMO                
BLADE_PIN3: GPIO 3 or UART POMI                
BLADE_PIN4: GPIO 4 or SPI MISO                  
BLADE_PIN5: GPIO 5 or SPI MOSI                  
BLADE_PIN6: GPIO 6 or SPI SCK                    
BLADE_PIN7: GPIO 7 or SPI CS                      
BLADE_PIN8: GPIO 8 or I2C SDA              
BLADE_PIN9: GPIO 9 or I2C SCL 

*/
typedef enum {PIN_NOT_ASSIGNED, DIGITAL_IN, DIGITAL_OUT, PERIPHERAL} BladePinIOType;


/*! 
@enum BladePinType
@brief Self-documenting names for the available Blade pins.                     

*/
typedef enum {BLADE_PIN0, BLADE_PIN1, BLADE_PIN2, BLADE_PIN3, 
              BLADE_PIN4, BLADE_PIN5, BLADE_PIN6, BLADE_PIN7, 
              BLADE_PIN8, BLADE_PIN9} BladePinType;


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
ErrorStatusType BladeRequestPin(BladePinType ePin_, BladePinIOType ePinFunction_);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void BladeApiInitialize(void);
void BladeApiRunActiveState(void);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void BladeApiSM_Idle(void);    
static void BladeApiSM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define U8_BLADE_PINS       (u8)10


#endif /* __BLADE_API_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!**********************************************************************************************************************
@file accelerometer_lis2di.c                                                                
@brief Driver file for communication to the ST LIS2DH accelerometer.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- 

CONSTANTS
- 
TYPES
-  struct ButtonStatusType

PUBLIC FUNCTIONS
- 

PROTECTED FUNCTIONS
- 

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Lis2dh"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32Lis2dhFlags;                   /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;          /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;           /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;            /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Lis2dh_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Lis2dh_pfStateMachine;               /*!< @brief The state machine function pointer */
//static fnCode_type Lis2dh_pfNextState;                /*!< @brief The next state to select after a wait state */


//static u8 Lis2dh_au8SpiTxBuffer[U8_SPIM_BUFFER_SIZE];   /*!< @brief Master out (transmit) buffer */
static u8 Lis2dh_au8SpiRxBuffer[U8_SPIM_BUFFER_SIZE];   /*!< @brief Master in (receive) buffer */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhInitialize(void)

@brief Initializes the task and its variables.

Requires:
  - SPI peripheral initialized and ready 

Promises:
  - 

*/
void Lis2dhInitialize(void)
{
  u8 au8ReadWhoAmI[] = {(U8_READ_REGISTER | U8_REGADDR_WHO_AM_I), U8_SPIM_DUMMY_BYTE};
  u8 au8WriteBuffer[] = {0,0};
  u32 u32Timer;
  
  
  /* INT1_CFG */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT1_CFG;
  //au8WriteBuffer[1] = 0x0;
  //au8WriteBuffer[1] = 0xA;
  au8WriteBuffer[1] = _INT1_CFG_ZL;
  //au8WriteBuffer[1] = _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  //au8WriteBuffer[1] = _INT1_CFG_6D | _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* Boot the device to ensure that INT config registers can be written */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG5;
  au8WriteBuffer[1] = _CTRL_REG5_BOOT;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
  u32Timer = G_u32SystemTime1ms;
  while( (G_u32SystemTime1ms - u32Timer) < 10);
 
  /* INT1_CFG */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT1_CFG;
  //au8WriteBuffer[1] = 0x0;
  //au8WriteBuffer[1] = 0xA;
  au8WriteBuffer[1] = _INT1_CFG_ZL;
  //au8WriteBuffer[1] = _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  //au8WriteBuffer[1] = _INT1_CFG_6D | _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* Read the LIS2DH WHO_AM_I register to confirm it's ready */
  
  SpiPrimaryDataTransfer(sizeof(au8ReadWhoAmI), au8ReadWhoAmI, Lis2dh_au8SpiRxBuffer);
  
  /* Return if the expected result is not read */
  if(Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT] != U8_I_AM)
  {
    Lis2dh_pfStateMachine = Lis2dhSM_Error;
    return;
  }
  
  /* Otherwise, proceed to configure the accelerometer */
  /* CTRL_REG1 */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG1;
  //au8WriteBuffer[1] = _CTRL_REG1_ODR1 | _CTRL_REG1_ODR0 | _CTRL_REG1_ENZ | _CTRL_REG1_ENY | _CTRL_REG1_ENX;
  au8WriteBuffer[1] = _CTRL_REG1_ODR1 | _CTRL_REG1_ODR0 | _CTRL_REG1_ENZ | _CTRL_REG1_ENY | _CTRL_REG1_ENX;
  //au8WriteBuffer[1] = 0x57;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* CTRL_REG2 */  
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG2;
  au8WriteBuffer[1] = 0x00;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* CTRL_REG3  */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG3;
  //au8WriteBuffer[1] = _CTRL_REG3_INT1_AOI1 | _CTRL_REG3_INT1_AIO2;
  au8WriteBuffer[1] = _CTRL_REG3_INT1_AOI1;
  //au8WriteBuffer[1] = 0x40;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* CTRL_REG4 */  
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG4;
  au8WriteBuffer[1] = 0x00;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* CTRL_REG5 */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG5;
  //au8WriteBuffer[1] = _CTRL_REG5_LIR_INT2;
  //au8WriteBuffer[1] = _CTRL_REG5_LIR_INT1 | _CTRL_REG5_LIR_INT2;
  au8WriteBuffer[1] = 0x00;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

   /* CTRL_REG6 */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG6;
  //au8WriteBuffer[1] = _CTRL_REG6_I2_INT1 | _CTRL_REG6_I2_INT2 | _CTRL_REG6_P2_ACT | _CTRL_REG6_H_LACTIVE;
  //au8WriteBuffer[1] = _CTRL_REG6_I2_INT1 | _CTRL_REG6_P2_ACT;
  au8WriteBuffer[1] = 0x02;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* INT1 THRESHOLD: set for n * 16mg force for interrupt trigger */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT1_THS;
  au8WriteBuffer[1] = U8_INT1_THRESHOLD;
  //au8WriteBuffer[1] = 0x10;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* INT1 DURATION: set for n / ODR (e.g. ODR = 10Hz, so DUR/ODR = 1s when DUR = 10? */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT1_DURATION;
  au8WriteBuffer[1] = U8_INT1_DURATION;
  //au8WriteBuffer[1] = 0x00;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
  
  
  Lis2dh_pfStateMachine = Lis2dhSM_Idle;

} /* end Lis2dhInitialize() */

#if 0
  /* INT1_CFG */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT1_CFG;
  //au8WriteBuffer[1] = 0xA;
  au8WriteBuffer[1] = _INT1_CFG_YH;
  //au8WriteBuffer[1] = _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  //au8WriteBuffer[1] = _INT1_CFG_6D | _INT1_CFG_ZH | _INT1_CFG_YH | _INT1_CFG_XH;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);


  /* INT2 THRESHOLD: set for n * 16mg force for interrupt trigger */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT2_THS;
  au8WriteBuffer[1] = U8_INT2_THRESHOLD;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* INT2 DURATION: set for n / ODR (e.g. ODR = 10Hz, so DUR/ODR = 1s when DUR = 10? */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT2_DURATION;
  au8WriteBuffer[1] = U8_INT2_DURATION;
  //au8WriteBuffer[1] = 0x00;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* INT2_CFG: set for 6 direction movement interrupt */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_INT2_CFG;
  au8WriteBuffer[1] = _INT2_CFG_ZH | _INT2_CFG_YH | _INT2_CFG_XH;
  //au8WriteBuffer[1] = _INT2_CFG_6D;
  //au8WriteBuffer[1] = 0;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
#endif

#if 0
  /* Now boot the device to write INT1_CFG? */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG5;
  au8WriteBuffer[1] = _CTRL_REG5_BOOT;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);

  /* TEST: read back register to check if it gets written */
  au8WriteBuffer[0] = U8_READ_REGISTER | U8_REGADDR_INT1_CFG;
  au8WriteBuffer[1] = U8_SPIM_DUMMY_BYTE;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
  
  
  /* TEST: read back register to check if it gets written */
  au8WriteBuffer[0] = U8_READ_REGISTER | U8_REGADDR_CTRL_REG1;
  au8WriteBuffer[1] = U8_SPIM_DUMMY_BYTE;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
 
#endif  


#if 0
  /* TEST with TP7 on INT2 */
  
  /* CTRL_REG6: activate I1 and I2 on INT2, activity interrupt, active low */
  au8WriteBuffer[0] = U8_WRITE_REGISTER | U8_REGADDR_CTRL_REG6;
  au8WriteBuffer[1] = _CTRL_REG6_I2_INT1 | _CTRL_REG6_I2_INT2 | _CTRL_REG6_P2_ACT | _CTRL_REG6_H_LACTIVE;
  SpiPrimaryDataTransfer(sizeof(au8WriteBuffer), au8WriteBuffer, Lis2dh_au8SpiRxBuffer);
#endif
  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void Lis2dhRunActiveState(void)
{
  Lis2dh_pfStateMachine();

} /* end Lis2dhRunActiveState */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhSetResolution(void)

@brief Initializes the task and its variables.

Requires:
-

Promises:
  - 

*/
void Lis2dhSetResolution(void)
{

} /* end Lis2dhSetResolution() */

// Set for 8-bit 100Hz (Low power mode)
// Bypass mode (no FIFO)?
// Read temperature
// Send and read X command
// Don't forget we might have to solder jumper the IC to pull-up SA0

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhResetTxBuffer(void)

@brief Fills the Tx Buffer with U8_SPIM_DUMMY_BYTE 

Requires:
  - 

Promises:
  - Lis2dh_au8SpiTxBuffer[all] = U8_SPIM_DUMMY_BYTE

*/
void Lis2dhResetTxBuffer(void)
{
  for(u8 i = 0; i < U8_SPIM_BUFFER_SIZE; i++)
  {
    //Lis2dh_au8SpiTxBuffer[i] = U8_SPIM_DUMMY_BYTE;
  }

} /* end Lis2dhResetTxBuffer() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhResetRxBuffer(void)

@brief Fills the Rx Buffer with U8_SPIM_DUMMY_BYTE 

Requires:
  - 

Promises:
  - Lis2dh_au8SpiRxBuffer[all] = U8_SPIM_DUMMY_BYTE

*/
void Lis2dhResetRxBuffer(void)
{
  for(u8 i = 0; i < U8_SPIM_BUFFER_SIZE; i++)
  {
    Lis2dh_au8SpiRxBuffer[i] = U8_SPIM_DUMMY_BYTE;
  }

} /* end Lis2dhResetRxBuffer() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhSetError(u8 u8ErrorCode_)

@brief Sets up the LEDs to display an error code and then 
selects the error state.

Requires:
@param u8ErrorCode_ is the error code value to setup currently
allowed to be 0 to (U8_TOTAL_LEDS - 1).

Promises:
- Lis2dh_pfStateMachine set to error state 

*/
void Lis2dhSetError(u8 u8ErrorCode_)
{
  Lis2dh_pfStateMachine = Lis2dhSM_Error; 

} /* end Lis2dhSetError() */


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
#if 0
/* What does this state do? */
static void Lis2dhSM_Configure(void)
{
    
} /* end Lis2dhSM_Configure() */
#endif

/* What does this state do? */
static void Lis2dhSM_Idle(void)
{
  static u32 u32DelayCount = 0;
  u8 u8StatusReg1;
  u8 u8StatusReg2;
//  u8 u8OutXHi;
//  u8 u8OutYHi;
//  u8 u8OutZHi;
  s8 s8OutXHi;
  s8 s8OutYHi;
  s8 s8OutZHi;
  u8 u8Int1Src;
  u8 u8Int2Src;
  u8 u8IntCounter;
  u8 au8ReadRegister[] = {(U8_READ_REGISTER | U8_REGADDR_WHO_AM_I), U8_SPIM_DUMMY_BYTE};
  
  if(++u32DelayCount == 2000)
  {
    u32DelayCount = 0;
    
    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_STATUS_REG; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    u8StatusReg1 = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];
    
    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_OUT_X_H; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    s8OutXHi = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_OUT_Y_H; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    s8OutYHi = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_OUT_Z_H; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    s8OutZHi = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_INT1_SOURCE; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    u8Int1Src = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_INT2_SOURCE; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    u8Int2Src = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_INT_COUNTER_REG; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    u8IntCounter = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];

    au8ReadRegister[0] = U8_READ_REGISTER | U8_REGADDR_STATUS_REG; 
    SpiPrimaryDataTransfer(sizeof(au8ReadRegister), au8ReadRegister, Lis2dh_au8SpiRxBuffer);
    u8StatusReg2 = Lis2dh_au8SpiRxBuffer[U8_INDEX_READ_RESULT];
    
    u8StatusReg1 = s8OutXHi + s8OutYHi + s8OutZHi + u8Int1Src + u8Int2Src + u8IntCounter + u8StatusReg1 + u8StatusReg2;
  }
  
} /* end Lis2dhSM_Idle() */
     


/*-------------------------------------------------------------------------------------------------------------------*/
/* Hold the error state */
static void Lis2dhSM_Error(void)          
{
  
} /* end Lis2dhSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

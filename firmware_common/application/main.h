/*!**********************************************************************************************************************
@file main.h
@brief Header file for main.c.
***********************************************************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

/*!**********************************************************************************************************************
Firmware Version - Printed FIRMWARE_MAIN_REV.FIRMWARE_SUB_REV1 FIRMWARE_SUB_REV2

FIRMWARE_MAIN_REV: should be updated only with RELEASES to Github.
FIRMWARE_SUB_REV1: should be updated when a pull request is made to the MASTER branch.
***********************************************************************************************************************/
#define FIRMWARE_VERSION     {'F','i','r','m','w','a','r','e',' ','v','e','r','s','i','o','n',':',' ', \
                             FIRMWARE_MAIN_REV, '.', FIRMWARE_SUB_REV1, FIRMWARE_SUB_REV2,'\n','\n','\r','\0'}

#define FIRMWARE_MAIN_REV    '1'               
#define FIRMWARE_SUB_REV1    '0'
#define FIRMWARE_SUB_REV2    '0'


/***********************************************************************************************************************
* Constant Definitions
***********************************************************************************************************************/

/* G_u32ApplicationFlags */
/* The order of these flags corresponds to the order of applications in SystemStatusReport() (debug.c) */
#define _APPLICATION_FLAGS_LED          0x00000001        /*!< @brief G_u32ApplicationFlags LedStateMachine is initialized */
#define _APPLICATION_FLAGS_BUTTON       0x00000002        /*!< @brief G_u32ApplicationFlags ButtonStateMachine is initialized */
#define _APPLICATION_FLAGS_DEBUG        0x00000004        /*!< @brief G_u32ApplicationFlags DebugStateMachine is initialized */
#define _APPLICATION_FLAGS_TIMER        0x00000008        /*!< @brief G_u32ApplicationFlags TimerStateMachine is initialized */
#define _APPLICATION_FLAGS_LCD          0x00000010        /*!< @brief G_u32ApplicationFlags LcdStateMachine is initialized */
#define _APPLICATION_FLAGS_ADC          0x00000020        /*!< @brief G_u32ApplicationFlags Adc12StateMachine is initialized */
#define _APPLICATION_FLAGS_ANT          0x00000040        /*!< @brief G_u32ApplicationFlags AntStateMachine is initialized */
//#define _APPLICATION_FLAGS_USBCDC       0x00000080        /*!< @brief G_u32ApplicationFlags USBCDC StateMachine */

#ifdef EIE_ASCII
//#define _APPLICATION_FLAGS_SDCARD       0x00000100        /*!< @brief G_u32ApplicationFlags SdCardStateMachine */
/* end G_u32ApplicationFlags */

#define NUMBER_APPLICATIONS             (u8)7             /*!< @brief Total number of system applications */
#endif /* EIE_ASCII */

#ifdef EIE_DOTMATRIX
#define _APPLICATION_FLAGS_CAPTOUCH     0x00000080        /*!< @brief G_u32ApplicationFlags SdCardStateMachine */
/* end G_u32ApplicationFlags */

#define NUMBER_APPLICATIONS             (u8)8             /*!< @brief Total number of system applications */
#endif /* EIE_DOTMATRIX */

#define MAX_TASK_NAME_SIZE              (u8)10            /*!< @brief Maximum string size for task name reported in SystemStatusReport */


/* G_u32SystemFlags */
#define _SYSTEM_CLOCK_NO_STOP_DEBUG     (u32)0x00000001   /*!< @brief G_u32SystemFlags DEBUG module preventing STOP mode */
#define _SYSTEM_CLOCK_NO_STOP_LEDS      (u32)0x00000002   /*!< @brief G_u32SystemFlags LED module preventing STOP mode */
#define _SYSTEM_CLOCK_NO_STOP_USER      (u32)0x00000004   /*!< @brief G_u32SystemFlags User interaction preventing STOP mode */
#define _SYSTEM_CLOCK_OSC_FAIL          (u32)0x00000008   /*!< @brief G_u32SystemFlags set if oscillator start-up fails */
#define _SYSTEM_CLOCK_PLL_NO_LOCK       (u32)0x00000010   /*!< @brief G_u32SystemFlags set if PLL0 does not lock on startup */
#define _SYSTEM_TIME_WARNING            (u32)0x00000020   /*!< @brief G_u32SystemFlags set if a 1ms violation has occurred */

#define _SYSTEM_STARTUP_NO_ANT          (u32)0x01000000   /*!< @brief G_u32SystemFlags set if button hold to disable ANT detected at startup */

#define _SYSTEM_SLEEPING                (u32)0x40000000   /*!< @brief G_u32SystemFlags set into sleep mode to go back to sleep if woken before 1ms period */
#define _SYSTEM_INITIALIZING            (u32)0x80000000   /*!< @brief G_u32SystemFlags set when system is in initialization phase */
/* end G_u32SystemFlags */


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/



#endif /* __MAIN_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/

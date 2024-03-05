/*!**********************************************************************************************************************
@file accelerometer_lis2di.h                                                                
@brief Header file for accelerometer_lis2di.c source.

The default values in this file configure the accelerometer to operate in "Low power" mode (8 bit resolution) at 
a read frequency of 100Hz which should result in operating current consumption of 10uA.

Notes:
Allow 5ms device startup

**********************************************************************************************************************/

#ifndef __LIS2DH
#define __LIS2DH

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
/*! 
@enum AccelResolutionType
@brief Available resolutions for the LIS2DH
*/
typedef enum{ACCEL_RES_2, ACCEL_RES_4, ACCEL_RES_8, ACCEL_RES_16} AccelResolutionType;


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Application error codes */
#define U8_LIS2DH_ERROR_NOI2C      (u8)0x01   

#define U8_SPIM_BUFFER_SIZE        (u8)(32)


/* I²C Addresses */
#define LIS2DH_BASE_ADDRESS        (u8)0x19      /*!< @brief b'0001 1001' 6 bits (no R/W) right justifed (assumes SDO tied high) */
#define LIS2DH_READ                (u8)0x33      /*!< @brief Read address (assumes SDO tied high) */
#define LIS2DH_WRITE               (u8)0x32      /*!< @brief Write address (assumes SDO tied high) */

#define AUTO_INCREMENT             (u8)0x80      /*!< @brief OR this into a sub address to auto increment the sub address after an access */


/* Commands */
#define U8_READ_REGISTER           (u8)0x80      /*!< @brief OR this with a register address to READ that register on the next byte */
#define U8_WRITE_REGISTER          (u8)0x00      /*!< @brief OR this with a register address to WRITE that register on the next byte */

/* Register definitions */
#define U8_INDEX_READ_RESULT       (u8)1         /*!< @brief Index in Lis2dh_au8SpiRxBuffer where a register read result is located */

#define U8_REGADDR_STATUS_REG_AUX  (u8)0x07
#define _STATUS_REG_AUX_TOR        (u8)0x40      /*!< @brief Temperature data overrun */
#define _STATUS_REG_AUX_TDA        (u8)0x04      /*!< @brief New temperature data availabble */

#define U8_REGADDR_OUT_TEMP_L      (u8)0x0C
#define U8_REGADDR_OUT_TEMP_H      (u8)0x0D

#define U8_REGADDR_INT_COUNTER_REG (u8)0x0E

#define U8_REGADDR_WHO_AM_I        (u8)0x0F
#define U8_I_AM                    (u8)0x33
  
#define U8_REGADDR_TEMP_CFG        (u8)0x1F
#define _TEMP_CFG__EN0             (u8)0x40          /* Set both to enable temperature sensor */
#define _TEMP_CFG__EN1             (u8)0x80


#define U8_REGADDR_CTRL_REG1       (u8)0x20
#define _CTRL_REG1_ODR0            (u8)0x10          /* Power down mode */
#define _CTRL_REG1_ODR1            (u8)0x20          /* HR / normal / Low power mode (1 Hz) */
#define _CTRL_REG1_ODR2            (u8)0x40          /* HR / normal / Low power mode (10 Hz) */
#define _CTRL_REG1_ODR3            (u8)0x80          /* HR / normal / Low power mode (25 Hz) */
#define _CTRL_REG1_LPEN            (u8)0x08          /* Low power mode (1 = low power mode) */
#define _CTRL_REG1_ENZ             (u8)0x04          /* Enable z-axis */
#define _CTRL_REG1_ENY             (u8)0x02          /* Enable y-axis */
#define _CTRL_REG1_ENX             (u8)0x01          /* Enable x-axis */

#define U8_REGADDR_CTRL_REG2       (u8)0x21

#define U8_REGADDR_CTRL_REG3       (u8)0x22
#define _CTRL_REG3_INT1_CLICK      (u8)0x80          /* INT1 is CLICK interrupt */
#define _CTRL_REG3_INT1_AOI1       (u8)0x40          /* INT1 is AOI1 interrupt */
#define _CTRL_REG3_INT1_AIO2       (u8)0x20          /* INT1 is AOI2 interrupt */
#define _CTRL_REG3_INT1_DRDY1      (u8)0x10          /* INT1 is data ready 1 interrupt */
#define _CTRL_REG3_INT1_DRDY2      (u8)0x08          /* INT1 is data ready 2 interrupt */
#define _CTRL_REG3_INT1_WTM        (u8)0x04          /* INT1 is watermark interrupt */
#define _CTRL_REG3_INT1_OVERRUN    (u8)0x02          /* INT1 is data overrun interrupt */

#define U8_REGADDR_CTRL_REG4       (u8)0x23

#define U8_REGADDR_CTRL_REG5       (u8)0x24
#define _CTRL_REG5_BOOT            (u8)0x80
#define _CTRL_REG5_FIFO_EN         (u8)0x40
#define _CTRL_REG5_LIR_INT1        (u8)0x08
#define _CTRL_REG5_D4D_INT1        (u8)0x04
#define _CTRL_REG5_LIR_INT2        (u8)0x02
#define _CTRL_REG5_D4D_INT2        (u8)0x01

#define U8_REGADDR_CTRL_REG6       (u8)0x25
#define _CTRL_REG6_I2_CLICKEN      (u8)0x80
#define _CTRL_REG6_I2_INT1         (u8)0x40
#define _CTRL_REG6_I2_INT2         (u8)0x20
#define _CTRL_REG6_I2_BOOT         (u8)0x10
#define _CTRL_REG6_P2_ACT          (u8)0x08
#define _CTRL_REG6_H_LACTIVE       (u8)0x02

#define U8_REGADDR_REFERENCE       (u8)0x26
#define U8_REGADDR_STATUS_REG      (u8)0x27

#define U8_REGADDR_OUT_X_L         (u8)0x28
#define U8_REGADDR_OUT_X_H         (u8)0x29
#define U8_REGADDR_OUT_Y_L         (u8)0x2A
#define U8_REGADDR_OUT_Y_H         (u8)0x2B
#define U8_REGADDR_OUT_Z_L         (u8)0x2C
#define U8_REGADDR_OUT_Z_H         (u8)0x2D

#define U8_REGADDR_FIFO_CTRL_REG   (u8)0x2E
#define U8_REGADDR_FIFO_SRC_REG    (u8)0x2F

#define U8_REGADDR_INT1_CFG        (u8)0x30
#define _INT1_CFG_AOI              (u8)0x80          /* And/Or combination of Interrupt events */
#define _INT1_CFG_6D               (u8)0x40          /* 6 direction detection function enabled */
#define _INT1_CFG_ZH               (u8)0x20          /* Enable interrupt generation on Z high event or on Direction recognition */
#define _INT1_CFG_ZL               (u8)0x10          /* Enable interrupt generation on Z low event or on Direction recognition */
#define _INT1_CFG_YH               (u8)0x08          /* Enable interrupt generation on Y high event or on Direction recognition */
#define _INT1_CFG_YL               (u8)0x04          /* Enable interrupt generation on Y low event or on Direction recognition */
#define _INT1_CFG_XH               (u8)0x02          /* Enable interrupt generation on X high event or on Direction recognition  */
#define _INT1_CFG_XL               (u8)0x01          /* Enable interrupt generation on X low event or on Direction recognition  */

#define U8_REGADDR_INT1_SOURCE     (u8)0x31
#define U8_REGADDR_INT1_THS        (u8)0x32
#define U8_INT1_THRESHOLD          (u8)59            /*!< @brief n * 16mg (MAX n 127) = amount of force (milli-G?) on any axis to trigger interrupt??? */ 
#define U8_REGADDR_INT1_DURATION   (u8)0x33
#define U8_INT1_DURATION           (u8)5             /*!< @brief n/ODR (MAX n 127) = seconds for interrupt duration before it is automatically cleared??? */ 

#define U8_REGADDR_INT2_CFG        (u8)0x34
#define _INT2_CFG_AOI              (u8)0x80          /* And/Or combination of Interrupt events */
#define _INT2_CFG_6D               (u8)0x40          /* 6 direction detection function enabled */
#define _INT2_CFG_ZH               (u8)0x20          /* Enable interrupt generation on Z high event or on Direction recognition */
#define _INT2_CFG_ZL               (u8)0x10          /* Enable interrupt generation on Z low event or on Direction recognition */
#define _INT2_CFG_YH               (u8)0x08          /* Enable interrupt generation on Y high event or on Direction recognition */
#define _INT2_CFG_YL               (u8)0x04          /* Enable interrupt generation on Y low event or on Direction recognition */
#define _INT2_CFG_XH               (u8)0x02          /* Enable interrupt generation on X high event or on Direction recognition  */
#define _INT2_CFG_XL               (u8)0x01          /* Enable interrupt generation on X low event or on Direction recognition  */

#define U8_REGADDR_INT2_SOURCE     (u8)0x35

#define U8_REGADDR_INT2_THS        (u8)0x36
#define U8_INT2_THRESHOLD          (u8)16            /*!< @brief n * 16mg (MAX n 127) = amount of force (milli-G?) on any axis to trigger interrupt??? */ 

#define U8_REGADDR_INT2_DURATION   (u8)0x37
#define U8_INT2_DURATION           (u8)1             /*!< @brief n/ODR (MAX n 127) = seconds for interrupt duration before it is automatically cleared??? */ 

#define U8_REGADDR_CLICK_CFG       (u8)0x38
#define U8_REGADDR_CLICK_SRC       (u8)0x39
#define U8_REGADDR_CLICK_THS       (u8)0x3A

#define U8_REGADDR_TIME_LIMIT      (u8)0x3B
#define U8_REGADDR_TIME_LATENCY    (u8)0x3C
#define U8_REGADDR_TIME_WINDOW     (u8)0x3D

#define U8_REGADDR_ACT_THS         (u8)0x3E
#define U8_REGADDR_ACT_DUR         (u8)0x3F


#define CTRL_REG1_INIT  (u8)0x5F
/*
    07 [0] ODR3 HR / normal / Low power mode (100 Hz)
    06 [1] ODR2 "
    05 [0] ODR1 "
    04 [1] ODR0 "

    03 [1] LPEN Low power (8-bit) mode
    02 [1] ZEN Z-axis enabled
    01 [1] YEN Y-axis enabled
    00 [1] ZEN X-axis enabled
*/
  

#define CTRL_REG2_INIT (u8)0x00
/*
    07 [0] HPM1 High Pass filter Mode Selection DEFAULT
    06 [0] HPM2 "
    05 [0] HPCF2 High Pass filter Cut Off frequency selection
    04 [0] HPCF1 "

    03 [0] FDS Filtered Data Selection BYPASSED
    02 [0] HPCLICK High Pass filter enabled for CLICK function BYPASSED
    01 [0] HPIS2 High Pass filter enabled for AOI function on Interrupt 2 BYPASSED
    00 [0] HPIS1 High Pass filter enabled for AOI function on Interrupt 1 BYPASSED
*/


#define CTRL_REG3_INIT (u8)0x00
/*
    07 [0] I1_CLICK CLICK interrupt on INT1 pin DISABLED
    06 [0] I1_AOI1 AOI1 interrupt on INT1 DISABLED
    05 [0] I1_AOI2 AOI2 interrupt on INT1 pin DISABLED
    04 [0] I1_DRDY1 DRDY1 interrupt on INT1 pin DISABLED

    03 [0] I1_DRDY2 DRDY2 interrupt on INT1 pin DISABLED
    02 [0] I1_WTM FIFO Watermark interrupt on INT1 pin DISABLED
    01 [0] I1_OVERRUN FIFO Overrun interrupt on INT1 pin DISABLED
    00 [0] reserved
*/


#define CTRL_REG4_INIT (u8)0x30
/*
    07 [0] BDU continuous update
    06 [0] BLE Big/Little Endian Lsb at lower
    05 [1] FS1 Full scale selection +/- 16G
    04 [1] FS0 "

    03 [0] HR Normal mode (8 or 10 bit output)
    02 [0] ST1 Self test disabled
    01 [0] ST0 "
    00 [0] SIM N/A
*/


#define CTRL_REG5_INIT (u8)0x00
/*
    07 [0] BOOT Reboot memory content NO
    06 [0] FIFO_EN FIFO DISABLED
    05 [0] Reserved
    04 [0] "

    03 [0] LIR_INT1 Latch interrupt request on INT1_SRC register NOT LATCHED
    02 [0] D4D_INT1 4D enable: 4D detection is enabled on INT1 pin when 6D bit on INT1_CFG set
    01 [0] LIR_INT2 Latch interrupt request on INT2_SRC register NOT LATCHED
    00 [0] D4D_INT2 4D enable: 4D detection is enabled on INT2 pin when 6D bit on INT2_CFG set
*/


#define CTRL_REG6_INIT (u8)0x80
/*
    07 [1] I2_CLICKen Click interrupt on INT2 pin ENABLED
    06 [0] I2_INT1 Interrupt 1 function enabled on INT2 pin DISABLED
    05 [0] I2_INT2 Interrupt 2 function enabled on INT2 pin DISABLED
    04 [0] BOOT_I2 Boot on INT2 pin enable DISABLED

    03 [0] P2_ACT Activity interrupt enable on INT2 pin DISABLED
    02 [0] Reserved
    01 [0] H_LACTIVE interrupt active level HIGH(0)
    00 [0] Reserved
*/




/* Sleep to wake / Return to sleep mode register settings (if used) */
#define ACT_THS_INIT (u8)0x
/*
    07 [0] Threshold accleration value
    06 [0] 
    05 [0] 
    04 [0] 

    03 [0] 
    02 [0] 
    01 [0] 
    00 [0] 
*/

#define ACT_DUR_INIT (u8)0x
/*
    07 [0] Threshold accleration value duration
    06 [0] 
    05 [0] 
    04 [0] 

    03 [0] 
    02 [0] 
    01 [0] 
    00 [0] 
*/

#define TEMPLATE_INIT (u32)0x
/*
    31 [0] 
    30 [0] 
    29 [0] 
    28 [0] 

    27 [0] 
    26 [0] 
    25 [0] 
    24 [0] 

    23 [0] 
    22 [0] 
    21 [0] 
    20 [0] 

    19 [0] 
    18 [0] 
    17 [0] 
    16 [0] 

    15 [0] 
    14 [0] 
    13 [0] 
    12 [0] 

    11 [0] 
    10 [0] 
    09 [0] 
    08 [0] 

    07 [0] 
    06 [0] 
    05 [0] 
    04 [0] 

    03 [0] 
    02 [0] 
    01 [0] 
    00 [0] 
*/

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void Lis2dhInitialize(void);
void Lis2dhRunActiveState(void);
void Lis2dhSetResolution(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void Lis2dhResetTxBuffer(void);
void Lis2dhResetRxBuffer(void);
void Lis2dhSetError(u8 u8ErrorCode_);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void Lis2dhSM_Configure(void);

static void Lis2dhSM_Idle(void);

static void Lis2dhSM_Error(void);




#endif /* __LIS2DH */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

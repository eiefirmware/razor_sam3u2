/*!**********************************************************************************************************************
@file lcd_NHD-C12864LZ.h                                                                
@brief LCD implementation for Newhaven NHD-C12864LZ dot matrix display
*******************************************************************************/

#ifndef __LCD_C12865LZ_H
#define __LCD_C12865LZ_H

#define LCD_STARTUP_ANIMATION

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/

/*! 
@enum LcdFontType
@brief Available fonts to use */
typedef enum {LCD_FONT_SMALL, LCD_FONT_BIG} LcdFontType;

/*! 
@enum LcdShiftType
@brief Available shift directions */
typedef enum {LCD_SHIFT_UP, LCD_SHIFT_DOWN, LCD_SHIFT_RIGHT, LCD_SHIFT_LEFT} LcdShiftType;

/*! 
@enum LcdMessageType
@brief Type of message being sent to the LCD */
typedef enum {LCD_CONTROL_MESSAGE, LCD_DATA_MESSAGE} LcdMessageType;

/*! 
@struct PixelAddressType
@brief Absolute address of one LCD pixel */
typedef struct
{
  u16 u16PixelRowAddress;
  u16 u16PixelColumnAddress;
} PixelAddressType;

/*! 
@struct PixelAddressType
@brief Address structure of a rectangular group of pixels */
typedef struct
{
  u16 u16RowStart;             /*!< @brief  Address of top left pixel row */
  u16 u16ColumnStart;          /*!< @brief  Address of top left pixel column */
  u16 u16RowSize;              /*!< @brief  Number of rows in block */
  u16 u16ColumnSize;           /*!< @brief  Number of columns in block */
} PixelBlockType;

/*! 
@struct PixelAddressType
@brief LCD message struct used to queue LCD data */
typedef struct
{
  LcdMessageType eMessageType; /*!< @brief  Command or data */ 
  u16 u16Length;               /*!< @brief  Number of bytes in the message */
  u8* pu8Data;                 /*!< @brief  Address of first data byte of the message */
} LcdQueueMessageType;


/*******************************************************************************
* Macros 
*******************************************************************************/
/* Public */
#define     LCD_BACKLIGHT_ON()          (LedOn(LCD_BL))   /*!< @brief Turns on LCD backlight */
#define     LCD_BACKLIGHT_OFF()         (LedOff(LCD_BL))  /*!< @brief Turns off LCD backlight */

/* Private */
#define     LCD_RESET_ASSERT()          (AT91C_BASE_PIOB->PIO_CODR = PB_14_LCD_RST)  /*!< @brief Activates LCD reset line */
#define     LCD_RESET_DEASSERT()        (AT91C_BASE_PIOB->PIO_SODR = PB_14_LCD_RST)  /*!< @brief Deactivates LCD reset line */
#define     LCD_COMMAND_MODE()          (AT91C_BASE_PIOB->PIO_CODR = PB_15_LCD_A0)   /*!< @brief Configures IO for LCD command mode */
#define     LCD_DATA_MODE()             (AT91C_BASE_PIOB->PIO_SODR = PB_15_LCD_A0)   /*!< @brief Configures IO for LCD data mode */
#define     LCD_CS_ASSERT()             (AT91C_BASE_PIOB->PIO_CODR = PB_12_LCD_CS)   /*!< @brief Sets the LCD SPI CS line */
#define     LCD_CS_DEASSERT()           (AT91C_BASE_PIOB->PIO_SODR = PB_12_LCD_CS)   /*!< @brief Releases the LCD SPI CS line */



/*******************************************************************************
* Application Values
*******************************************************************************/
/* Lcd_u32Flags */
#define _LCD_FLAGS_COMMAND_IN_QUEUE      (u32)0x00000001      /*!< @brief Command or data in LCD */
#define _LCD_MANUAL_MODE                 (u32)0x10000000      /*!< @brief The task is in manual mode */
/* end Lcd_u32Flags */

/*! @cond DOXYGEN_EXCLUDE */

/* LCD hardware definitions */
#define U8_LCD_PIXEL_BITS                (u8)1
#define U8_LCD_PAGE_SIZE                 (u8)8
#define U8_LCD_PAGES                     (u8)8
#define U16_LCD_ROWS                     (u16)((u16)U8_LCD_PAGES * (u16)U8_LCD_PAGE_SIZE)
#define U16_LCD_COLUMNS                  (u16)128

#define U16_LCD_LEFT_MOST_COLUMN         (u16)0
#define U16_LCD_RIGHT_MOST_COLUMN        (u16)(U16_LCD_COLUMNS - 1)
#define U16_LCD_TOP_MOST_ROW             (u16)0
#define U16_LCD_BOTTOM_MOST_ROW          (u16)(U16_LCD_ROWS - 1)
#define U16_LCD_CENTER_COLUMN            (u16)(U16_LCD_COLUMNS / 2)
#define U16_LCD_IMAGE_ROWS               U16_LCD_ROWS
#define U16_LCD_IMAGE_COLUMNS            (u16)(U16_LCD_COLUMNS * (u16)U8_LCD_PIXEL_BITS / 8)

#define U16_LCD_TX_BUFFER_SIZE           (u16)128   /* Enough for a complete page refresh */
#define U16_LCD_RX_BUFFER_SIZE           (u16)1     /* Enough for a complete page refresh */

#define U32_LCD_STARTUP_DELAY_200        (u32)205
#define U32_LCD_STARTUP_DELAY_10         (u32)11
#define U32_LCD_REFRESH_TIME             (u32)25                

/* Bitmap sizes (x = # of column pixels, y = # of row pixels) */
#define U8_LCD_SMALL_FONT_COLUMNS        (u8)5
#define U8_LCD_SMALL_FONT_COLUMN_BYTES   (u8)1
#define U8_LCD_SMALL_FONT_ROWS           (u8)7
#define U8_LCD_SMALL_FONT_SPACE          (u8)1
#define U8_LCD_MAX_SMALL_CHARS           (u8)(U16_LCD_COLUMNS / (u16)(U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE) )
#define U8_LCD_SMALL_FONT_COLSPACE       (u8)(U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_COLUMN_BYTES)

#define U8_LCD_BIG_FONT_COLUMNS          (u8)10
#define U8_LCD_BIG_FONT_COLUMN_BYTES     (u8)2
#define U8_LCD_BIG_FONT_ROWS             (u8)14
#define U8_LCD_BIG_FONT_SPACE            (u8)1

#define U8_LCD_DOT_XSIZE                 (u8)1
#define U8_LCD_DOT_YSIZE                 (u8)1
#define U8_LCD_IMAGE_COL_SIZE_50PX       (u8)50
#define U8_LCD_IMAGE_COL_BYTES_50PX      (u8)((U8_LCD_IMAGE_COL_SIZE_50PX * U8_LCD_PIXEL_BITS / 8) + 1)
#define U8_LCD_IMAGE_ROW_SIZE_50PX       (u8)50
#define U8_LCD_IMAGE_COL_SIZE_25PX       (u8)25
#define U8_LCD_IMAGE_COL_BYTES_25PX      (u8)((U8_LCD_IMAGE_COL_SIZE_25PX * U8_LCD_PIXEL_BITS / 8) + 1)
#define U8_LCD_IMAGE_ROW_SIZE_25PX       (u8)25

#define U8_LCD_SMALL_FONT_ROW_SPACING    (u8)1
#define U8_LCD_SMALL_FONT_LINE0          (u8)(0)
#define U8_LCD_SMALL_FONT_LINE1          (u8)(1 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE2          (u8)(2 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE3          (u8)(3 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE4          (u8)(4 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE5          (u8)(5 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE6          (u8)(6 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))
#define U8_LCD_SMALL_FONT_LINE7          (u8)(7 * (U8_LCD_SMALL_FONT_ROWS + U8_LCD_SMALL_FONT_ROW_SPACING))

#define U8_LCD_BIG_FONT_ROW_SPACING      (u8)1
#define U8_LCD_BIG_FONT_LINE0            (u8)(0)
#define U8_LCD_BIG_FONT_LINE1            (u8)(1 * (U8_LCD_BIG_FONT_ROWS + U8_LCD_BIG_FONT_ROW_SPACING))
#define U8_LCD_BIG_FONT_LINE2            (u8)(2 * (U8_LCD_BIG_FONT_ROWS + U8_LCD_BIG_FONT_ROW_SPACING))
#define U8_LCD_BIG_FONT_LINE3            (u8)(3 * (U8_LCD_BIG_FONT_ROWS + U8_LCD_BIG_FONT_ROW_SPACING))

#define U8_LCD_FRAMES_MSG_COLUMN         (u8)(10 * U8_LCD_SMALL_FONT_COLSPACE)
#define U8_LCD_IP_START_COLUMN           (u8)(3 * U8_LCD_SMALL_FONT_COLSPACE)

/*! @endcond */


/*******************************************************************************
* ST7565P Controller Values
*******************************************************************************/
/*! @cond DOXYGEN_EXCLUDE */

/* The LCD is a write-only device.  Communication with LCD begins by setting the Register Select 
bit (A0 in the datasheet) to specify instruction (0)or data(1).
Next the chip select line is asserted to indicate SPI communication is starting.
A stream of bytes is sent which will be commands or data depending on the state of A0.

An 'x' at the end of a command indicates bits need to be added into the byte to complete it.
A '_' at the end of a command means it is a two-part command and must be immediately followed by the 2nd byte.
*/

/* Commands with A0 = 0 */
#define U8_LCD_DISPLAY_ON              (u8)0xAF
#define U8_LCD_DISPLAY_OFF             (u8)0xAE
#define U8_LCD_DISPLAY_LINE_SETx       (u8)0x40
#define U8_LCD_SET_PAGE_ADDRESSx       (u8)0xB0
#define U8_LCD_SET_COL_ADDRESS_MSNx    (u8)0x10
#define U8_LCD_SET_COL_ADDRESS_LSNx    (u8)0x00

#define U8_LCD_ADC_SELECT_NORMAL       (u8)0xA0
#define U8_LCD_ADC_SELECT_REVERSE      (u8)0xA1
#define U8_LCD_REVERSE_ON              (u8)0xA7
#define U8_LCD_REVERSE_OFF             (u8)0xA6
#define U8_LCD_PIXEL_TEST_ON           (u8)0xA5
#define U8_LCD_PIXEL_TEST_OFF          (u8)0xA4
#define U8_LCD_BIAS_HIGH               (u8)0xA3
#define U8_LCD_BIAS_LOW                (u8)0xA2
#define U8_LCD_RESET                   (u8)0xE2

#define U8_LCD_COMMON_MODE0            (u8)0xC0
#define U8_LCD_COMMON_MODE1            (u8)0xC8

/* OR the follow options in with this command */
#define U8_LCD_POWER_CONTROLLER_SETx   (u8)0x28
#define U8_BOOST_ON                    (u8)0x04
#define U8_VREG_ON                     (u8)0x02
#define U8_VFOLLOW_ON                  (u8)0x01

/* OR the follow options in with this command */
#define U8_LCD_VOLTAGTE_REG_SETx       (u8)0x20
#define U8_SET_BIT0                    (u8)0x01
#define U8_SET_BIT1                    (u8)0x02
#define U8_SET_BIT2                    (u8)0x04

#define U8_LCD_EVOLUME_UNLOCK_         (u8)0x81
#define U8_LCD_EVOLUME_LEVEL           (u8)0x19
#define U8_LCD_EVOLUDM_STATUSx         (u8)0x1F

#define U8_LCD_SLEEP_MODE_             (u8)0xAC
#define U8_LCD_AWAKE                   (u8)0x01
#define U8_LCD_SLEEPING                (u8)0x00

#define U8_LCD_NOP                     (u8)0xE3

/*! @endcond */


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*-------------------------------------------------------------------------------------------------------------------*/
void LcdSetPixel(PixelAddressType* psPixelAddress_);
void LcdClearPixel(PixelAddressType* psPixelAddress_);
void LcdLoadString(const unsigned char* pu8String_, LcdFontType eFont_, PixelAddressType* psStartPixel_);
void LcdLoadBitmap(u8 const* paau8Bitmap_, PixelBlockType* psBitmapSize_);
void LcdClearPixels(PixelBlockType* psPixelsToClear_);
void LcdClearScreen(void);
void LcdShift(PixelBlockType eShiftArea_, u16 u16PixelsToShift_, LcdShiftType eDirection_);
bool LcdCommand(u8 u8Command_);         


/*-------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*-------------------------------------------------------------------------------------------------------------------*/
void LcdInitialize(void);
void LcdRunActiveState(void);
void LcdManualMode(void);


/*-------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*-------------------------------------------------------------------------------------------------------------------*/
static bool LcdSetStartAddressForDataTransfer(u8 u8Page_);         
static void LcdLoadPageToBuffer(u8 u8LocalRamPage_); 
static void LcdUpdateScreenRefreshArea(PixelBlockType* sPixelsToClear_);


/**********************************************************************************************************************
State Machine Declarations
**********************************************************************************************************************/
static void LcdSM_Idle(void);
static void LcdSM_WaitTransfer(void);
static void BoardTestSM_WaitPixelTestOn(void);          
static void BoardTestSM_WaitPixelTestOff(void);          



#endif /* __LCD_C12865LZ_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
/*!**********************************************************************************************************************
@file lcd_NHD-C12864LZ.c      
@brief LCD implementation for Newhaven NHD-C12864LZ.  

This file contains the task definition for the LCD handler.  The LCD is automatically 
refreshed every LCD_REFRESH_TIME milliseconds.  Only changed pixel data is sent with 
each refresh to minimize processor time.

A copy of the LCD memory is maintained in RAM as 2D array where each bit corresponds 
to one pixel. Any application may write to the LCD using the API, though this only 
impacts the local RAM.  All application addressing of the LCD is managed in pixels.
Mapping pixel addresses in RAM to the LCD is fully managed by this driver.

Task details:
1. The LCD requires access to the SPI resource that is connected to the LCD
2. Local LCD RAM may be written by any task at any time
3. LCD RAM is sent to update the LCD screen every LCD_REFRESH_TIME ms


The physical pixel addresses (row, column) on the LCD are shown here:

|--------------------------------------------------------------------------------------|
|################################### BLACK EPOXY ######################################|
|--------------------------------------------------------------------------------------|
|0,127                                                                              0,0|
|                                                                                      |
|                                                                                      |
|                                                                                      |
|                                                                                      |
|                                                                                      |
|                                                                                      |
|                                                                                      |
|63,127                                                                            63,0|
|--------------------------------------------------------------------------------------|

The LCD RAM maps out logically as shown here:
|--------------------------------------------------------------------------------------|
|###################################     TOP     ######################################|
|--------------------------------------------------------------------------------------|
|[0] [0]   First pixel and Text Line 0 start pixel                             [0][127]|
|[8] [0]   Text Line 1 start pixel                                                     |
|[16][0]   Text Line 2 start pixel                                                     |
|[24][0]   Text Line 3 start pixel                                                     |
|[32][0]   Text Line 4 start pixel                                                     |
|[40][0]   Text Line 5 start pixel                                                     |
|[48][0]   Text Line 6 start pixel                                                     |
|[56][0]   Text Line 7 start pixel                                                     |
|[63][0]                                                                      [63][127]|
|--------------------------------------------------------------------------------------|

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- LcdFontType {LCD_FONT_SMALL, LCD_FONT_BIG}
- LcdShiftType {LCD_SHIFT_UP, LCD_SHIFT_DOWN, LCD_SHIFT_RIGHT, LCD_SHIFT_LEFT}
- PixelAddressType
- PixelBlockType

PUBLIC FUNCTIONS
- void LcdSetPixel(PixelAddressType* psPixelAddress_)
- void LcdClearPixel(PixelAddressType* psPixelAddress_)
- void LcdClearPixels(PixelBlockType* psPixelsToClear_)
- void LcdClearScreen(void)
- void LcdLoadString(const u8* pu8String_, LcdFontType eFont_, PixelAddressType* psStartPixel_);
- void LcdLoadBitmap(u8 const* paau8Bitmap_, PixelBlockType* psBitmapSize_)
- bool LcdCommand(u8 u8Command_)

MACROS
- LCD_BACKLIGHT_ON()
- LCD_BACKLIGHT_OFF()


***********************************************************************************************************************/

#include "configuration.h"
#include "lcd_bitmaps.h"

/***********************************************************************************************************************
* Bookmarks
************************************************************************************************************************
!!!!! LCD Driver Functions
@@@@@ LCD State Machine Functions
***********************************************************************************************************************/


/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Lcd"
***********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*/
/* New variables */
volatile u8 G_aau8LcdRamImage[U16_LCD_IMAGE_ROWS][U16_LCD_IMAGE_COLUMNS];    /*!< @brief A complete copy of the LCD image in RAM */

/* The following are used commonly when working with text on the screen, so are defined */
PixelBlockType G_sLcdClearWholeScreen = 
{
  .u16RowStart = 0,
  .u16ColumnStart = 0,
  .u16RowSize = U16_LCD_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine0 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE0,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine1 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE1,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine2 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE2,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine3 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE3,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine4 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE4,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine5 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE5,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine6 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE6,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};

PixelBlockType G_sLcdClearLine7 = 
{
  .u16RowStart = U8_LCD_SMALL_FONT_LINE7,
  .u16ColumnStart = 0,
  .u16RowSize = U8_LCD_SMALL_FONT_ROWS,
  .u16ColumnSize = U16_LCD_COLUMNS
};


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From board-specific source file */

extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;             /*!< @brief From main.c */

extern const u8 G_aau8SmallFonts[][U8_LCD_SMALL_FONT_ROWS][U8_LCD_SMALL_FONT_COLUMN_BYTES];        /*!< @brief From lcd_bitmaps.c */
extern const u8 G_aau8BigFonts[][U8_LCD_BIG_FONT_ROWS][U8_LCD_BIG_FONT_COLUMN_BYTES];              /*!< @brief From lcd_bitmaps.c */
extern const u8 aau8EngenuicsLogoBlack[U8_LCD_IMAGE_ROW_SIZE_50PX][U8_LCD_IMAGE_COL_BYTES_50PX];   /*!< @brief From lcd_bitmaps.c */
extern const u8 aau8EngenuicsLogoBlackQ1[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX]; /*!< @brief From lcd_bitmaps.c */
extern const u8 aau8EngenuicsLogoBlackQ2[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX]; /*!< @brief From lcd_bitmaps.c */
extern const u8 aau8EngenuicsLogoBlackQ3[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX]; /*!< @brief From lcd_bitmaps.c */
extern const u8 aau8EngenuicsLogoBlackQ4[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX]; /*!< @brief From lcd_bitmaps.c */

extern volatile fnCode_type G_SspStateMachine;         /*!< @brief From sam3u_ssp.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this task.
Variable names shall start with "Lcd_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Lcd_pfnStateMachine;                           /*!< @brief The application state machine */

static u32 Lcd_u32Flags;                                          /*!< @brief LCD flag register */
static u32 Lcd_u32Timer;                                          /*!< @brief Local timer with scope across file */
static u32 Lcd_u32RefreshTimer;                                   /*!< @brief LCD refresh timer */

static fnCode_type Lcd_ReturnState;                               /*!< @brief Saved return state */
static u32 Lcd_u32CurrentMsgToken;                                /*!< @brief Token of message currently being sent to LCD */

static SspConfigurationType Lcd_sSspConfig;                       /*!< @brief Configuration information for SSP peripheral */
static SspPeripheralType* Lcd_Ssp;                                /*!< @brief Pointer to LCD's SSP peripheral object */
static u8 Lcd_u8PagesToUpdate;                                    /*!< @brief Counter for number of pages in current LCD refresh */
static u8 Lcd_u8CurrentPage;                                      /*!< @brief Current page being updated */

static u8 Lcd_au8TxBuffer[U16_LCD_TX_BUFFER_SIZE];                /*!< @brief Buffer for outgoing data to LCD during the current refresh cycle */
static u8 Lcd_au8RxDummyBuffer[U16_LCD_RX_BUFFER_SIZE];           /*!< @brief Dummy location for LCD receive buffer (LCD does not send data) */
static u8* Lcd_pu8RxDummyBuffer;                                  /*!< @brief Dummy buffer pointer */

static PixelBlockType Lcd_sUpdateArea;                            /*!< @brief Area of LCD to update */
static PixelBlockType Lcd_sCurrentUpdateArea;                     /*!< @brief Area of LCD currently being updated */

static u8 Lcd_au8MessageInit[]  = "LCD Ready\r\n";
static u8 Lcd_au8MessageWelcome[] = "SAM3U2 DOT MATRIX";
                                 
static  u8 Lcd_au8SetupArray[] = {U8_LCD_BIAS_LOW, U8_LCD_ADC_SELECT_NORMAL, U8_LCD_COMMON_MODE1, U8_LCD_COMMON_MODE0, U8_LCD_DISPLAY_LINE_SETx,
                                  U8_LCD_VOLTAGTE_REG_SETx | U8_SET_BIT0 | U8_SET_BIT2,
                                  U8_LCD_EVOLUME_UNLOCK_, U8_LCD_EVOLUME_LEVEL,
                                  U8_LCD_POWER_CONTROLLER_SETx | U8_BOOST_ON | U8_VREG_ON | U8_VFOLLOW_ON,
                                  U8_LCD_DISPLAY_ON, U8_LCD_PIXEL_TEST_ON
                                 }; 


/***********************************************************************************************************************
!!!!! LCD Driver Functions                                                   
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdLoadString(const unsigned char* pu8String_, LcdFontType eFont_, PixelAddressType* psStartPixel_) 

@brief Loads a character string into the LCD RAM location specified.

Requires:
@param pu8String_ points to a null-terminated C-string
@param  eFont_ selects the font to use to print the string
@param  psStartPixel_ pointer to the pixel location for the top left pixel of the first character bitmap (assuming a rectangular bitmap area)
    
Promises:
- The string is parsed and the bitmap for each character is loaded into the 
  local LCD RAM G_aau8LcdRamImage.  Any characters that will not fit on the screen are ignored.
   
*/
void LcdLoadString(const unsigned char* pu8String_, LcdFontType eFont_, PixelAddressType* psStartPixel_) 
{
  u8 u8FontWidth;
  PixelBlockType sLetterPosition;
  
  /* Initialize sLetterPosition struct based on font */
  sLetterPosition.u16RowStart = psStartPixel_->u16PixelRowAddress;
  sLetterPosition.u16ColumnStart = psStartPixel_->u16PixelColumnAddress;

  if(eFont_ == LCD_FONT_SMALL)
  {
    u8FontWidth = U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE;
    sLetterPosition.u16RowSize = (u16)(U8_LCD_SMALL_FONT_ROWS) & 0x00FF;
    sLetterPosition.u16ColumnSize = (u16)(U8_LCD_SMALL_FONT_COLUMNS) & 0x00FF;
  }
  else
  {
    u8FontWidth = U8_LCD_BIG_FONT_COLUMNS + U8_LCD_BIG_FONT_SPACE;
    sLetterPosition.u16RowSize = (u16)(U8_LCD_BIG_FONT_ROWS) & 0x00FF;
    sLetterPosition.u16ColumnSize = (u16)(U8_LCD_BIG_FONT_COLUMNS) & 0x00FF;
  }

  /* Process characters until NULL is reached */
  while(*pu8String_ != '\0')
  {
    /* Check to see if the starting address of the next character is outside the LCD area */
    if(sLetterPosition.u16ColumnStart >= U16_LCD_COLUMNS)
    {
      break;
    }

    /* Load each character bitmap */
   if(eFont_ == LCD_FONT_SMALL)
   {
     LcdLoadBitmap(&G_aau8SmallFonts[*pu8String_ - 32][0][0], &sLetterPosition);
   }
   else
   {
     LcdLoadBitmap(&G_aau8BigFonts[*pu8String_ - NUMBER_ASCII_TO_DEC][0][0], &sLetterPosition);
   }
    
    /* Update for the next character */
    sLetterPosition.u16ColumnStart += u8FontWidth;
    pu8String_++;
  }
  
} /* end LcdLoadString */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdSetPixel(PixelAddressType* psPixelAddress_)

@briefTurn on one pixel in the LCD RAM.

Requires:
- G_aau8LcdRamImage is a row x column matrix of 1-bit pixel values
- The origin (0,0 pixel of the LCD RAM image) is at G_aau8LcdRamImage[0][0] bit 0

@param psPixelAddress_ points to the address of the pixel to be set

Promises:
- G_aau8LcdRamImage[psPixelAddress_->u16PixelRowAddress][<bit>psPixelAddress->u16PixelColumnAddress] is set to 1

*/
void LcdSetPixel(PixelAddressType* psPixelAddress_)
{
  u8 u8ColumBitPosition = 0x01;
  u8 u8ColumnGroup = psPixelAddress_->u16PixelColumnAddress / 8; 
  
  u8ColumBitPosition = u8ColumBitPosition << (psPixelAddress_->u16PixelColumnAddress % 8); 

  /* Set the correct bit in RAM */
  G_aau8LcdRamImage[psPixelAddress_->u16PixelRowAddress][u8ColumnGroup] |= u8ColumBitPosition;
  
} /* end LcdSetPixel */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdClearPixel(PixelAddressType* psPixelAddress_)

@brief Turn off one pixel in the LCD RAM.

Requires:
- G_aau8LcdRamImage is a row x column matrix of 1-bit pixel values
- The origin (0,0 pixel of the LCD RAM image) is at G_aau8LcdRamImage[0][0] bit 0

@param psPixelAddress_ points to the address of the pixel to be set

Promises:
- G_aau8LcdRamImage[psPixelAddress_->u16PixelRowAddress][<bit>psPixelAddress_->u16PixelColumnAddress] is cleared

*/
void LcdClearPixel(PixelAddressType* psPixelAddress_)
{
  u8 u8ColumBitPosition = 0x01;
  u8 u8ColumnGroup = psPixelAddress_->u16PixelColumnAddress / 8; 
  
  u8ColumBitPosition = u8ColumBitPosition << (psPixelAddress_->u16PixelColumnAddress % 8); 

  /* Set the correct bit in RAM */
  G_aau8LcdRamImage[psPixelAddress_->u16PixelRowAddress][u8ColumnGroup] &= ~u8ColumBitPosition;
  
} /* end LcdClearPixel */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdLoadBitmap(u8 const* paau8Bitmap_, PixelBlockType* psBitmapSize_)

@brief Loads a bitmap into the LCD RAM location specified.

*** This function can violate 1ms system timing: need to make it better! ***

Requires:
 - G_aau8LcdRamImage is a row x column matrix of 1-bit pixel values
 - The origin (0,0 pixel of the LCD RAM image) is at G_aau8LcdRamImage[0][0] bit 0
 - All LCD bitmaps are 2D arrays of pixel bits, where bit 0 at btimap[0][0] is the top left pixel

@param paau8Bitmap_ points to the first element of a 2D array. Since a 2D array cannot
be passed directly, indexing will be done manually (i.e. to access the jth 
element in the ith row aau8Bitmap_[i * u8ColumnSize_ + j] must be used instead 
of aau8Bitmap_[i][j])

@param psBitmapSize_ points to the size struct for the bitmap being loaded

Promises:
 - The bitmap is parsed and loaded into the local LCD RAM G_aau8LcdRamImage.

*/
void LcdLoadBitmap(u8 const* paau8Bitmap_, PixelBlockType* psBitmapSize_)
{
  u8 u8CurrentBitMaskBitmap;
  u8 u8CurrentBitMaskLcdRAM;
  u16 u16BitGroupBitmap;
  u16 u16BitGroupLcdRAM;
  u8 u8RowIterations;
  u16 u16ColumnIterations;
  u16 u16Temp;
  
  /* Make sure there are at least some pixels inside the LCD screen area */
  if( (psBitmapSize_-> u16RowStart < U16_LCD_ROWS) && (psBitmapSize_-> u16ColumnStart < U16_LCD_COLUMNS) )
  {
    /* Setup the number of row iterations but check to prevent overflow */
    u8RowIterations = psBitmapSize_->u16RowSize;
    if( (psBitmapSize_->u16RowStart + u8RowIterations) > U16_LCD_ROWS)
    {
      u8RowIterations = U16_LCD_ROWS - psBitmapSize_->u16RowStart;
    }
    
    /* Setup the number of column iterations but check to prevent overflow */
    u16ColumnIterations = psBitmapSize_->u16ColumnSize;
    if( (psBitmapSize_->u16ColumnStart + u16ColumnIterations) > U16_LCD_COLUMNS)
    {
      u16ColumnIterations = U16_LCD_COLUMNS - psBitmapSize_->u16ColumnStart;
    }

    /* Index i is the current row in the bitmap image, and the current row in the LCD RAM 
    image relative to psBitmapSize_->u16RowStart */
    for(u16 i = 0; i < u8RowIterations; i++)
    {
      /* Sliding bit masks are used to mask out the current pixel data bit.  The image starts at bit 0. */
      u8CurrentBitMaskBitmap = 0x01;
      u16BitGroupBitmap = 0;

      /* The LCD RAM bit mask must start at whatever bit within the current group corresponds to the 
      starting column index */
      u8CurrentBitMaskLcdRAM =  0x01 << (psBitmapSize_->u16ColumnStart % 8);
      u16BitGroupLcdRAM = (psBitmapSize_->u16ColumnStart / 8);

      /* Index j counts the current column (bitwise) in the image, and the current column in
      the LCD RAM relative to psBitmapSize_->u16ColumnStart.  The bitmasks and bitgroups work out the
      correct translation to the bytes in the images.  The bitmap must be indexed explicitly since we
      only have a pointer to a 1D array; the LCD RAM 2D array can be indexed normally. */
      for(u16 j = 0;  j < u16ColumnIterations; j++)
      {
        /* Get the linear index of the current bitmap byte 2D array - watch for incomplete bytes! */
        if( (psBitmapSize_->u16ColumnSize % 8) == 0 )
        {
          u16Temp = (i * (psBitmapSize_->u16ColumnSize / 8) ) + u16BitGroupBitmap;
        }
        else
        {
          u16Temp = (i * ((psBitmapSize_->u16ColumnSize / 8) + 1)) + u16BitGroupBitmap;
        }
            
        /* Set or clear appropirate bit in LCD RAM */
        if( paau8Bitmap_[u16Temp] & u8CurrentBitMaskBitmap )
        {
          G_aau8LcdRamImage[i + psBitmapSize_->u16RowStart][u16BitGroupLcdRAM] |= u8CurrentBitMaskLcdRAM;
        }
        else
        {
          G_aau8LcdRamImage[i + psBitmapSize_->u16RowStart][u16BitGroupLcdRAM] &= ~u8CurrentBitMaskLcdRAM;
        }

        /* Shift the bitmap mask */
        u8CurrentBitMaskBitmap <<= 1;
        if(u8CurrentBitMaskBitmap == 0x00)
        {
          u8CurrentBitMaskBitmap = 0x01;
          u16BitGroupBitmap++;
        }
        
        /* Shift the LCD RAM mask */
        u8CurrentBitMaskLcdRAM <<= 1;
        if(u8CurrentBitMaskLcdRAM == 0x00)
        {
          u8CurrentBitMaskLcdRAM = 0x01;
          u16BitGroupLcdRAM++;
        }
      } /* end column loop */
    } /* end row loop */

    /* Update the refresh area for the next LCD refresh */
    LcdUpdateScreenRefreshArea(psBitmapSize_);
  }

} /* end LcdLoadBitmap() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdClearPixels(PixelBlockType* psPixelsToClear_)

@brief Sets a rectangular section of LCD RAM to 0 (clear). Verified for full screen clear.

Requires:
- The origin (0,0 pixel) is at the top left of the LCD screen

@param psPixelsToClear_ points to the area to clear

Promises:
- A u16RowSize_ x u16ColumnSize_ block of pixels in LCD RAM starting at u16RowStart, u16ColumnStart
  will be set to 0; if the area defined exceeds the area available, the overflowing addresses will be ignored

*/
void LcdClearPixels(PixelBlockType* psPixelsToClear_)
{
  u8 u8CurrentBitMask;
  u16 u16BitGroup;
  u8 u8RowIterations;
  u16 u16ColumnIterations;
  
  /* Setup the number of row iterations but check to prevent overflow */
  u8RowIterations = psPixelsToClear_->u16RowSize;
  if( (psPixelsToClear_->u16RowStart + u8RowIterations) > U16_LCD_ROWS )
  {
    u8RowIterations = U16_LCD_ROWS - psPixelsToClear_->u16RowStart;
  }
  
  /* Setup the number of column iterations but check to prevent overflow */
  u16ColumnIterations = psPixelsToClear_->u16ColumnSize;
  if( (psPixelsToClear_->u16ColumnStart + u16ColumnIterations) > U16_LCD_COLUMNS)
  {
    u16ColumnIterations = U16_LCD_COLUMNS - psPixelsToClear_->u16ColumnStart;
  }

  /* Index i is the current row in the LCD RAM */
  for(u16 i = psPixelsToClear_->u16RowStart; i < (psPixelsToClear_->u16RowStart + u8RowIterations); i++)
  {
    /* A sliding bit mask is used to mask out the current pixel data bit in the array */
    u16BitGroup = (psPixelsToClear_->u16ColumnStart / 8);
    u8CurrentBitMask = 0x01 << (psPixelsToClear_->u16ColumnStart % 8);

    /* Index j counts the current column (bitwise) in the LCD RAM relative to the starting column */
    for(u16 j = 0;  j < u16ColumnIterations; j++)
    {
      /* Clear the current bit */
      G_aau8LcdRamImage[i][u16BitGroup] &=  ~u8CurrentBitMask;
      
      /* Adjust the bit mask watching for advance to the next bit group */
      u8CurrentBitMask <<= 1;
      if(u8CurrentBitMask == 0x00)
      {
        u8CurrentBitMask = 0x01;
        u16BitGroup++;
      }
    } /* end column loop */
  } /* end row loop */

  LcdUpdateScreenRefreshArea(psPixelsToClear_);
  
} /* end LcdClearPixels() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdClearScreen(void)

@brief Clears all pixel data in G_aau8LcdRamImage

Requires:
- NONE

Promises:
- G_aau8LcdRamImage[i][j] = 0 for all locations 

*/
void LcdClearScreen(void)
{
  /* Zero out all of the image array */
  for(u16 i = 0; i < U16_LCD_IMAGE_ROWS; i++)
  {
    for(u16 j = 0; j < U16_LCD_IMAGE_COLUMNS; j++)
    {
      G_aau8LcdRamImage[i][j] = 0;
    }
  }
      
  /* Queue to refresh whole screen */
  LcdUpdateScreenRefreshArea(&G_sLcdClearWholeScreen);
  
} /* end LcdClearScreen() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn bool LcdCommand(u8 u8Command_)

@brief Simple interface to use to queue a particular command to send to the LCD.  This only
applies to single-byte commands.

Requires:
@param u8Command_ is a valid A0 type command for the LCD (see list in lcd_NHD-C12864LZ.h)

Promises:
- A command message is queued to the TxBuffer

*/
bool LcdCommand(u8 u8Command_)
{
  if( !(Lcd_u32Flags & _LCD_FLAGS_COMMAND_IN_QUEUE) )
  {
    Lcd_u32Flags |= _LCD_FLAGS_COMMAND_IN_QUEUE;
    Lcd_au8TxBuffer[0] = u8Command_;
  
    /* Set hardware for command mode and queue the message */
    LCD_COMMAND_MODE();
    Lcd_u32CurrentMsgToken = SspWriteData(Lcd_Ssp, 1, &Lcd_au8TxBuffer[0]);
    
    /* Zero the timer so the command sends immediately and push the command out if initializing */
    Lcd_u32RefreshTimer = 0;
    LcdManualMode();
    
    return TRUE;
  }

  return FALSE;
  
} /* end LcdCommand() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdShift(PixelBlockType* psShiftArea_, u16 u16PixelsToShift_, LcdShiftType eDirection_)

@brief Moves the pixels in a section of the LCD by a certain number of pixels in some direction.  

This function can be used for simple animations or for scrolling text.  Note that any pixels
that are shifted outside of the defined area are lost.  This ensures that the function will
not try to write pixels not on the screen, and that pixels moving inside a block will not
overwrite into adjacent LCD pixels.

Requires:
@param psShiftArea_ points to the definition for the LCD area in which the pixels will move
@param u16PixelsToShift_ is the number of pixels to shift
@param eDirection_ is the direction in which the pixels will shift

Promises:
- A (psShiftArea_->) u16RowSize_ x u16ColumnSize_ block of pixels in LCD RAM starting at u16RowStart, u16ColumnStart
  will be set to 0; if the area defined exceeds the area available, the overflowing addresses will be ignored.

*/
void LcdShift(PixelBlockType eShiftArea_, u16 u16PixelsToShift_, LcdShiftType eDirection_)
{
  /* To be added some day */
  
} /* end LcdShift() */


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdInitialize(void)

@brief Initialize the local LCD RAM then boot and initialize the LCD. 

Since so much data is transferred to the LCD and speed is essential, LCD transfers are
generally assumed to work.  Unless critical data is being sent, then no checking
is done during data transmission.

Requires:
- LCD SPI peripheral is initialized and ready for communication

Promises:
- G_aau8LcdRamImage[LCD_IMAGE_ROWS][LCD_IMAGE_COLUMNS] = 0;
- LCD is initialized and turned on

*/
void LcdInitialize(void)
{
  u8 u8Size;
  PixelBlockType sEngenuicsImage;
  PixelAddressType sStringLocation;
  
  /* Start with backlight on */
  LCD_BACKLIGHT_ON();
  
  /* Initialize variables */
  Lcd_u32RefreshTimer = G_u32SystemTime1ms;
  Lcd_pfnStateMachine = LcdSM_Idle;
  Lcd_pu8RxDummyBuffer = Lcd_au8RxDummyBuffer;
  
  /* Configure the SSP resource to be used for the application */
  Lcd_sSspConfig.SspPeripheral      = USART1;
  Lcd_sSspConfig.pCsGpioAddress     = AT91C_BASE_PIOB;
  Lcd_sSspConfig.u32CsPin           = PB_12_LCD_CS;
  Lcd_sSspConfig.pu8RxBufferAddress = Lcd_au8RxDummyBuffer;
  Lcd_sSspConfig.ppu8RxNextByte     = &Lcd_pu8RxDummyBuffer;
  Lcd_sSspConfig.u16RxBufferSize    = U16_LCD_RX_BUFFER_SIZE;
  Lcd_sSspConfig.eBitOrder          = SSP_MSB_FIRST;
  Lcd_sSspConfig.eSspMode           = SSP_MASTER_AUTO_CS;

  Lcd_Ssp = SspRequest(&Lcd_sSspConfig);
        
  /* Carry out the prescribed LCD initialization starting with delay after releasing reset */
  LCD_CS_ASSERT();
  LCD_RESET_DEASSERT();
  for(u32 i = 0; i < 10; i++);
  
  LCD_RESET_ASSERT();

  Lcd_u32Timer = G_u32SystemTime1ms;
  while( !IsTimeUp(&Lcd_u32Timer, 2) );

  LCD_RESET_DEASSERT();

  Lcd_u32Timer = G_u32SystemTime1ms;
  while( !IsTimeUp(&Lcd_u32Timer, 2) );
 
  /* Send settings array one command at a time - end with display and pixel test on */
  u8Size = sizeof(Lcd_au8SetupArray);
  for(u8 i = 0; i < u8Size; i++)
  {
    LcdCommand(Lcd_au8SetupArray[i]);
    Lcd_u32Timer = G_u32SystemTime1ms;
    while( !IsTimeUp(&Lcd_u32Timer, 5) );
  }
  
  /* Clear LCD pixel data */
  LcdClearPixels(&G_sLcdClearWholeScreen);
  LcdManualMode();
 
  /* Short pixel test */  
  Lcd_u32Timer = G_u32SystemTime1ms;
  while( !IsTimeUp(&Lcd_u32Timer, 500) );
  
  LcdCommand(U8_LCD_PIXEL_TEST_OFF);
  
#ifdef LCD_STARTUP_ANIMATION
  /* Divide the Engenuics logo up into 4 equal pieces and put them at the corner of the LCD to 
  ensure that the full range of pixels is being addressed correctly */
  u8 u8RowPosition = 0;
  for(u8 i = 0; i < 40; i++)
  {
    LcdClearPixels(&G_sLcdClearWholeScreen);
  
    /* Top left */
    sEngenuicsImage.u16RowStart = 0;
    sEngenuicsImage.u16ColumnStart = i;
    sEngenuicsImage.u16RowSize = 25;
    sEngenuicsImage.u16ColumnSize = 25;
    LcdLoadBitmap(&aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);
  
    /* Top right */
    sEngenuicsImage.u16RowStart = 0;
    sEngenuicsImage.u16ColumnStart = U16_LCD_COLUMNS - 25 - i;
    LcdLoadBitmap(&aau8EngenuicsLogoBlackQ2[0][0], &sEngenuicsImage);
  
    /* Bottom left */
    sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25 - u8RowPosition;
    sEngenuicsImage.u16ColumnStart = i;
    LcdLoadBitmap(&aau8EngenuicsLogoBlackQ3[0][0], &sEngenuicsImage);
    
    /* Bottom right */
    sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25 - u8RowPosition;
    sEngenuicsImage.u16ColumnStart = U16_LCD_COLUMNS - 25 - i;
    LcdLoadBitmap(&aau8EngenuicsLogoBlackQ4[0][0], &sEngenuicsImage);
  
    LcdManualMode();
       
    /* Adjust the row by one every few iterations */
    if( (i % 3) == 0)
    {
      u8RowPosition++;
      /* On the last iteration set, adjust one more row to bring the icon together */
      if(u8RowPosition == 13)
      {
        u8RowPosition = 14;
      }
    }
  } /*   for(u8 i = 0; i < 40; i++) */
  
#else /* LCD_STARTUP_ANIMATION */
  
  /* Show static image in middle of screen */
  LcdClearPixels(&G_sLcdClearWholeScreen);
  
  sEngenuicsImage.u16RowStart = 0;
  sEngenuicsImage.u16ColumnStart = 40;
  sEngenuicsImage.u16RowSize = U8_LCD_IMAGE_ROW_SIZE_50PX;
  sEngenuicsImage.u16ColumnSize = U8_LCD_IMAGE_COL_SIZE_50PX;
  LcdLoadBitmap(&aau8EngenuicsLogoBlack[0][0], &sEngenuicsImage);
#endif /* LCD_STARTUP_ANIMATION */

  /* Write the board string in the middle */
  sStringLocation.u16PixelColumnAddress = 
    U16_LCD_CENTER_COLUMN - ( strlen((char const*)Lcd_au8MessageWelcome) * (U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE) / 2 );
  sStringLocation.u16PixelRowAddress = U8_LCD_SMALL_FONT_LINE7;
  LcdLoadString(Lcd_au8MessageWelcome, LCD_FONT_SMALL, &sStringLocation);

  LcdManualMode();

  /* Announce on the debug port that LCD setup is ready */
  G_u32ApplicationFlags |= _APPLICATION_FLAGS_LCD;
  DebugPrintf(Lcd_au8MessageInit);
  
} /* end LcdInitialize() */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void LcdRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void LcdRunActiveState(void)
{
  Lcd_pfnStateMachine();

} /* end LcdRunActiveState */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LcdManualMode(void)

@brief Runs a cycle of the LCD state machine until there are no messages queued up to transmit.

*** Violates 1ms system timing: use only during initialization ***

Requires:
- SSP state machine ready for manual mode
           
Promises:
- The current command for an LCD will be sent; or an LCD refresh will be carried out.
  
*/
void LcdManualMode(void)
{
  u32 u32ManualModeTimer;
  
  if(G_u32SystemFlags & _SYSTEM_INITIALIZING)
  {
    /* Zero the refresh timer so the LCD refreshes right away in manual mode */
    Lcd_u32RefreshTimer = 0; 
    Lcd_u32Flags |= _LCD_MANUAL_MODE;
    while(Lcd_u32Flags & _LCD_MANUAL_MODE)
    {
      /* Run the two SMs that are needed to send LCD bytes */
      WATCHDOG_BONE();
      Lcd_pfnStateMachine();
      
      /* Provide an equivalent system tick delay */
      u32ManualModeTimer = G_u32SystemTime1ms;
      while( !IsTimeUp(&u32ManualModeTimer, 1) );
    }
  }
  
} /* end LcdManualMode() */


/*----------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*----------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn static bool LcdSetStartAddressForDataTransfer(u8 u8LocalRamPage_)          

@brief Queues a message to set the LCD cursor to the correct position in preparation 
for data that will be sent to update the screen.  

The starting address is mapped appropriately for the actual physical LCD screen.

Requires:
- Lcd_sUpdateArea is up to date for the new LCD data to be written (used for column address).

@param u8LocalRamPage_ is the page address for this update

Promises:
- Command is queued to SSP

*/
static bool LcdSetStartAddressForDataTransfer(u8 u8LocalRamPage_)          
{
  u16 u16ColumnStartLcd = U16_LCD_COLUMNS - (Lcd_sCurrentUpdateArea.u16ColumnStart + Lcd_sCurrentUpdateArea.u16ColumnSize);
  
  if( !(Lcd_u32Flags & _LCD_FLAGS_COMMAND_IN_QUEUE) )
  {
    /* Set the message bytes for the current transfer */
    Lcd_au8TxBuffer[0] = U8_LCD_SET_PAGE_ADDRESSx    | u8LocalRamPage_;
    Lcd_au8TxBuffer[1] = U8_LCD_SET_COL_ADDRESS_MSNx | (u8)( (u16ColumnStartLcd >> 4) & 0x0F);
    Lcd_au8TxBuffer[2] = U8_LCD_SET_COL_ADDRESS_LSNx | (u8)( u16ColumnStartLcd & 0x0F);
      
    LCD_COMMAND_MODE(); 
    Lcd_u32Flags |= _LCD_FLAGS_COMMAND_IN_QUEUE;
    Lcd_u32CurrentMsgToken = SspWriteData(Lcd_Ssp, 3, &Lcd_au8TxBuffer[0]);

    return TRUE;
  }

  return FALSE;

} /* end LcdSetStartAddressForDataTransfer() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn static void LcdLoadPageToBuffer(u8 u8LocalRamPage_) 

@brief Loads Lcd_au8TxBuffer with one page of the current LCD data to refresh the screen.

This function translates the logical addressing of the bits in G_aau8LcdRamImage to the
addressing used by the ST7565 LCD controller.  Column bits must always be loaded
from low address to high address because the LCD controller automatically increments the column
address.

Algorithm notes:
- Pixel 0,0: row 0, column 0 in LCD RAM becomes row 0, column 127 on the LCD
- Must always update all 8 rows in a page, though any number of columns can be updated 

Requires:
- Lcd_sCurrentUpdateArea has the current area for the update
- G_aau8LcdRamImage has the correct updated data to send

@param u8LocalRamPage_ is the LCD page that is to be updated (provides row address for LCD RAM)
           
Promises:
- Data from G_aau8LcdRamImage is parsed out by row & column for the current page that requires
  updating.  A maximum of 128 bytes are posted to Lcd_au8TxBuffer (updates a full page).
   
*/
static void LcdLoadPageToBuffer(u8 u8LocalRamPage_) 
{
  u16 u16LocalRamCurrentRow; 
  u8* pu8TxBufferParser;
  u8 u8LocalRamBitGroup;
  u8 u8CurrentBitInLcdPageMask;
  u8 u8CurrentPixelBitInLocalRamMask;
  u8 u8CurrentColumnByte;

  pu8TxBufferParser = &Lcd_au8TxBuffer[0];
  
  /* Initialize the variables for the first column of pixel data */
  u8LocalRamBitGroup = (Lcd_sCurrentUpdateArea.u16ColumnStart + Lcd_sCurrentUpdateArea.u16ColumnSize - 1) / 8; 
  u8CurrentPixelBitInLocalRamMask = 0x01 << ((Lcd_sCurrentUpdateArea.u16ColumnStart + Lcd_sCurrentUpdateArea.u16ColumnSize - 1) % 8);
  u16LocalRamCurrentRow = u8LocalRamPage_ * U8_LCD_PAGE_SIZE; 
  u8CurrentBitInLcdPageMask = 0x01;
  
  /* Create the message data 1 LCD page column at a time by forming a byte reading the bits down the 8 rows in the current page. 
  Repeat this Lcd_sCurrentUpdateArea.u16ColumnSize times. */
  for(u16 i = 0; i < Lcd_sCurrentUpdateArea.u16ColumnSize; i++)
  {
    u8CurrentColumnByte = 0;
  
    /* Read 8 bits down the rows of the RAM image at the current bit location */
    for(u8 j = 0; j < 8; j++)
    {
      /* OR in the bit to the appropriate location in u8CurrentColumnByte */
      if( G_aau8LcdRamImage[u16LocalRamCurrentRow + j][u8LocalRamBitGroup] & u8CurrentPixelBitInLocalRamMask )
      {
        u8CurrentColumnByte |= ( u8CurrentBitInLcdPageMask << j );
      }
    }
    
    /* The byte has been built: add to Lcd_au8TxBuffer */
    *pu8TxBufferParser = u8CurrentColumnByte;
    pu8TxBufferParser++;
    
    /* Adjust the bitmask and watch for overflow to move to next byte in G_aau8LcdRamImage*/
    u8CurrentPixelBitInLocalRamMask >>= 1;
    if(u8CurrentPixelBitInLocalRamMask == 0x00)
    {
      u8CurrentPixelBitInLocalRamMask = 0x80;
      u8LocalRamBitGroup--;
    }
  }
  
  /* Lcd_au8TxBuffer now has all of the bytes for the current transfer */
  LCD_DATA_MODE();
  Lcd_u32CurrentMsgToken = SspWriteData(Lcd_Ssp, Lcd_sCurrentUpdateArea.u16ColumnSize, &Lcd_au8TxBuffer[0]);
 
} /* end LcdLoadPageToBuffer () */
    

/*!----------------------------------------------------------------------------------------------------------------------
@fn static void LcdUpdateScreenRefreshArea(PixelBlockType* psPixelsToUpdate_)

@brief Updates the current area that should be refrehed on the LCD based on any changes to the 
local LCD RAM.

Requires:
@param psPixelsToUpdate_ points to the data structure that is being adjusted in the LCD RAM
           
Promises:
- Lcd_sUpdateArea is increased in size so that it includes all the pixels of psPixelsToUpdate_;
  If psPixelsToUpdate_ is already fully included in the update area, then no changes are made.
   
*/
static void LcdUpdateScreenRefreshArea(PixelBlockType* psPixelsToUpdate_)
{
  s16 s16Temp;
  
  /* Determine if this is a new update */
  if(Lcd_sUpdateArea.u16RowSize == 0)
  {
    Lcd_sUpdateArea.u16ColumnStart = psPixelsToUpdate_->u16ColumnStart;
    Lcd_sUpdateArea.u16RowStart    = psPixelsToUpdate_->u16RowStart;
    Lcd_sUpdateArea.u16ColumnSize  = psPixelsToUpdate_->u16ColumnSize;
    Lcd_sUpdateArea.u16RowSize     = psPixelsToUpdate_->u16RowSize;
  }
  /* Otherwise update the current area if necessary */
  else
  {
    /* First check and adjust the starting row and columns if they are beyond the current refresh frame */
    if(Lcd_sUpdateArea.u16ColumnStart > psPixelsToUpdate_->u16ColumnStart)
    {
      Lcd_sUpdateArea.u16ColumnStart = psPixelsToUpdate_->u16ColumnStart;
    }
    
    if(Lcd_sUpdateArea.u16RowStart > psPixelsToUpdate_->u16RowStart)
    {
      Lcd_sUpdateArea.u16RowStart = psPixelsToUpdate_->u16RowStart;
    }
  
    /* Now check and adjust the end row and columns if they are beyond the current refresh frame */
    s16Temp = (psPixelsToUpdate_->u16RowStart + psPixelsToUpdate_->u16RowSize) - (Lcd_sUpdateArea.u16RowStart + Lcd_sUpdateArea.u16RowSize);
    if( s16Temp > 0 )
    {
      Lcd_sUpdateArea.u16RowSize += s16Temp;
    }
  
    s16Temp = (psPixelsToUpdate_->u16ColumnStart + psPixelsToUpdate_->u16ColumnSize) - (Lcd_sUpdateArea.u16ColumnStart + Lcd_sUpdateArea.u16ColumnSize);
    if( s16Temp > 0 )
    {
      Lcd_sUpdateArea.u16ColumnSize += s16Temp;
    }
  }    
  
} /* end LcdUpdateScreenRefreshArea() */      


/***********************************************************************************************************************
State Machine Function Definitions
***********************************************************************************************************************/

/*!-------------------------------------------------------------------------------------------------------------------
@fn static void LcdSM_Idle(void)

@brief Monitors the LCD variables to determine if a refresh is required.
If so, it updates the LCD with the current RAM contents.
*/

static void LcdSM_Idle(void)
{
  /* Check if a command is queued: commands are always sent immediately */
  if(Lcd_u32Flags & _LCD_FLAGS_COMMAND_IN_QUEUE)
  {
    Lcd_u32Timer = G_u32SystemTime1ms;
    Lcd_ReturnState = LcdSM_Idle;
    Lcd_pfnStateMachine = LcdSM_WaitTransfer;
  }
  
  /* Monitor the refresh period */
  else if( IsTimeUp(&Lcd_u32RefreshTimer, U32_LCD_REFRESH_TIME) )
  {
    /* Reset the refresh period reference value */
    Lcd_u32RefreshTimer = G_u32SystemTime1ms;
    
    /* Do something only if there is something to do (i.e. at least one row of the LCD needs updating) */
    if(Lcd_sUpdateArea.u16RowSize != 0)
    {
      /* Make a copy of the current Lcd_sUpdateArea then clear it */
      Lcd_sCurrentUpdateArea.u16RowSize     = Lcd_sUpdateArea.u16RowSize;
      Lcd_sCurrentUpdateArea.u16ColumnSize  = Lcd_sUpdateArea.u16ColumnSize;
      Lcd_sCurrentUpdateArea.u16RowStart    = Lcd_sUpdateArea.u16RowStart;
      Lcd_sCurrentUpdateArea.u16ColumnStart = Lcd_sUpdateArea.u16ColumnStart;     

      Lcd_sUpdateArea.u16RowSize     = 0;
      Lcd_sUpdateArea.u16ColumnSize  = 0;
      Lcd_sUpdateArea.u16RowStart    = 0;
      Lcd_sUpdateArea.u16ColumnStart = 0;     

      /* Calculate the number of pages to update -- all rows in a page must be updated to the LCD if any
      pixels are present on the page.  Eg. if 10 rows need updating, then up to 3 pages will have to be updated
      since there could be one pixel row on page n, eight on page n+1 and one on page n+2.  */
      Lcd_u8PagesToUpdate = ( (Lcd_sCurrentUpdateArea.u16RowStart + Lcd_sCurrentUpdateArea.u16RowSize - 1) / U8_LCD_PAGE_SIZE ) - 
                            ( (Lcd_sCurrentUpdateArea.u16RowStart) / U8_LCD_PAGE_SIZE ) + 1;
      
      /* Set the starting page; subsequent pages are incremental */
      Lcd_u8CurrentPage = Lcd_sCurrentUpdateArea.u16RowStart / U8_LCD_PAGE_SIZE;

      /* Start the refresh cycle by loading the command to set the cursor location */
      LcdSetStartAddressForDataTransfer(Lcd_u8CurrentPage);
      Lcd_pfnStateMachine = LcdSM_WaitTransfer;
    }
  }
  else
  {
    /* Nothing to do, so just make sure manual mode is not enabled */
    Lcd_u32Flags &= ~_LCD_MANUAL_MODE;
  }
        
} /* end LcdSM_Idle */


/*!-------------------------------------------------------------------------------------------------------------------
@fn static void LcdSM_WaitTransfer(void)

@brief Sends the current queued LCD command or data to the SPI peripheral through the SSP API.

This waits until the message token is complete or a timeout occurs.  We can determine the next step based
on Lcd_u8PagesToUpdate that will be 0 if the last transfer was a comand or non-zero if we are waiting
on the screen refresh process.
*/
static void LcdSM_WaitTransfer(void)
{
  /* Wait for message to be sent */
  if(QueryMessageStatus(Lcd_u32CurrentMsgToken) == COMPLETE)
  {
    /* The next step depends on what we did last */
    if(Lcd_u8PagesToUpdate != 0)
    {
      /* If the last transmission was a command, that means it's time to load an LCD page */
      if(Lcd_u32Flags & _LCD_FLAGS_COMMAND_IN_QUEUE)
      {
        Lcd_u32Flags &= ~_LCD_FLAGS_COMMAND_IN_QUEUE;
        
        LcdLoadPageToBuffer(Lcd_u8CurrentPage);
        Lcd_u8CurrentPage++;
        Lcd_u8PagesToUpdate--;
      }
      else
      {
        LcdSetStartAddressForDataTransfer(Lcd_u8CurrentPage);
      }
      
      Lcd_ReturnState = LcdSM_WaitTransfer;
    }
    /* Either just sent a command, or just sent that last data page */
    else
    {
      Lcd_u32Flags &= ~(_LCD_MANUAL_MODE | _LCD_FLAGS_COMMAND_IN_QUEUE);
      Lcd_ReturnState = LcdSM_Idle;
    }

    Lcd_pfnStateMachine = Lcd_ReturnState;
  }
  
  /* Check for timeout */
  
} /* end LcdSM_WaitTransfer() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
/*!*********************************************************************************************************************
@file lcd_bitmaps.h                                                                
@brief Bitmap data for 1-bit black & white dot matrix LCD
*******************************************************************************/

#ifndef __LCDBITMAPS_H
#define __LCDBITMAPS_H

#include "lcd_NHD-C12864LZ.h"
#include "configuration.h"


/*******************************************************************************
* Constants / Definitions
*******************************************************************************/

#define U8_LCD_IMAGE_PADDLE_ROW_SIZE         (u8)2
#define U8_LCD_IMAGE_PADDLE_COL_SIZE         (u8)11
#define U8_LCD_IMAGE_PADDLE_COL_BYTES        (u8)((U8_LCD_IMAGE_PADDLE_COL_SIZE * U8_LCD_PIXEL_BITS / 8) + 1)

#define U8_LCD_IMAGE_BALL_ROW_SIZE           (u8)3
#define U8_LCD_IMAGE_BALL_COL_SIZE           (u8)3
#define U8_LCD_IMAGE_BALL_COL_BYTES          (u8)((U8_LCD_IMAGE_BALL_COL_SIZE * U8_LCD_PIXEL_BITS / 8) + 1)

#define U8_LCD_IMAGE_ARROW_ROW_SIZE          (u8)12
#define U8_LCD_IMAGE_ARROW_COL_SIZE          (u8)6
#define U8_LCD_IMAGE_ARROW_COL_BYTES         (u8)((U8_LCD_IMAGE_ARROW_COL_SIZE * U8_LCD_PIXEL_BITS / 8) + 1)



#endif /* __LCDBITMAPS_H */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
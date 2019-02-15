/*!**********************************************************************************************************************
@file sam3u_uart.h                                                                
@brief Header file for sam3u_uart.c
**********************************************************************************************************************/

#ifndef __SAM3U_UART_H
#define __SAM3U_UART_H


/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/

/*! 
@struct UartConfigurationType
@brief Task-provided parameters for a UART 
*/
typedef struct 
{
  PeripheralType UartPeripheral;      /*!< @brief Easy name of peripheral */
  u16 u16RxBufferSize;                /*!< @brief Size of receive buffer in bytes */
  u8* pu8RxBufferAddress;             /*!< @brief Address to circular receive buffer */
  u8** pu8RxNextByte;                 /*!< @brief Pointer to buffer location where next received byte will be placed */
  fnCode_type fnRxCallback;           /*!< @brief Callback function for receiving data */
} UartConfigurationType;

/*! 
@struct UartPeripheralType
@brief Complete configuration parameters for a UART resource 
*/
typedef struct 
{
  AT91PS_USART pBaseAddress;          /*!< @brief Base address of the associated peripheral */
  u32 u32PrivateFlags;                /*!< @brief Flags for peripheral */
  MessageType* psTransmitBuffer;      /*!< @brief Pointer to the transmit message linked list */
  u32 u32CurrentTxBytesRemaining;     /*!< @brief Counter for bytes remaining in current transfer */
  u8* pu8CurrentTxData;               /*!< @brief Pointer to current location in the Tx buffer */
  u8* pu8RxBuffer;                    /*!< @brief Pointer to circular receive buffer in user application */
  u8** pu8RxNextByte;                 /*!< @brief Pointer to buffer location where next received byte will be placed */
  fnCode_type fnRxCallback;           /*!< @brief Callback function for receiving data */
  u16 u16RxBufferSize;                /*!< @brief Size of receive buffer in bytes */
  u8 u8PeripheralId;                  /*!< @brief Simple peripheral ID number */
  u8 u8Pad;
} UartPeripheralType;

/* u32PrivateFlags in UartPeripheralType */
#define   _UART_PERIPHERAL_ASSIGNED     (u32)0x00000001   /*!< @brief Set when the peripheral is in use */
#define   _UART_PERIPHERAL_TX           (u32)0x00200000   /*!< @brief Set when the peripheral is transmitting */
/* end u32PrivateFlags */


/**********************************************************************************************************************
* Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
UartPeripheralType* UartRequest(UartConfigurationType* psUartConfig_);
void UartRelease(UartPeripheralType* psUartPeripheral_);

u32 UartWriteByte(UartPeripheralType* psUartPeripheral_, u8 u8Byte_);
u32 UartWriteData(UartPeripheralType* psUartPeripheral_, u32 u32Size_, u8* pu8Data_);


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void UartInitialize(void);
void UartRunActiveState(void);

static void UartManualMode(void);

void UART_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
static void UartGenericHandler(void);


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UartSM_Idle(void);
static void UartSM_Transmitting(void);
static void UartSM_Error(void);         


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

/* Uart_u32Flags (local UART application flags) */
#define _UART_MANUAL_MODE               (u32)0x00000001   /*!< @brief Set to push a transmit cycle during initialization mode */

#define _UART_NO_ACTIVE_UARTS           (u32)0x02000000   /*!< @brief Set if Uart_u8ActiveUarts is 0 when decremented */
#define _UART_TOO_MANY_UARTS            (u32)0x04000000   /*!< @brief Set if Uart_u8ActiveUarts is 0 when decremented */
/* end of Uart_u32Flags */

#define U8_MAX_NUM_UARTS                (u8)5             /*!< @brief Total number of UARTs possible on SAM3U */



#endif /* __SAM3U_UART_H */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/

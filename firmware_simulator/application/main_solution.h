/***********************************************************************************************************************
File: main.h

Description:
Header file for main.c.
*******************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H


/***********************************************************************************************************************
* Constant Definitions
***********************************************************************************************************************/
#define U8_MAX_DRINKS           (u8)3       /* Maximum number of drinks a server can hold */
#define U8_MAX_SERVERS          (u8)3       /* Maximum number of servers */
#define U32_DRINK_SERVE_TIME    (u32)3000   /* Loop iterations before drink is removed */


/***********************************************************************************************************************
* Type Definitions
***********************************************************************************************************************/
typedef enum {EMPTY, BEER, SHOOTER, WINE, HIBALL} DrinkType;

typedef struct
{
  u8 u8ServerNumber;                       /* Unique token for this item */
  DrinkType asServingTray[U8_MAX_DRINKS];  /* Data payload array */
  void* psNextServer;                      /* Pointer to next ServerType*/
} ServerType;


/***********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
bool InitializeServer(ServerType** psServer_);
bool CreateServer(ServerType** psServerList_);


#endif /* __MAIN_H */
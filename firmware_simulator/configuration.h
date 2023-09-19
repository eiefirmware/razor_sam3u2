/**********************************************************************************************************************
File: configuration.h      

Description:
Main configuration header file for project.  This file bridges many of the generic features of the 
firmware to the specific features of the design. The definitions should be updated
to match the target hardware.  
 
***********************************************************************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

/**********************************************************************************************************************
Runtime switches
***********************************************************************************************************************/
//#define SOLUTION        /* Uncomment to build and use the solution to this module */


/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Common header files */
#include <stdlib.h>
#include <string.h>
#include "AT91SAM3U4.h"
#include "exceptions.h"
#include "interrupts.h"
#include "core_cm3.h"
#include "typedefs.h"

#ifndef SOLUTION
#include "main.h"
#else
#include "main_solution.h"
#endif

#include "utilities.h"


#endif /* __CONFIG_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/


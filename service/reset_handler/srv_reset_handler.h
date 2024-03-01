/*******************************************************************************
  PRIME Reset Handler Service Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    srv_reset_handler.h

  Summary:
    PRIME Reset Handler Service Interface Header File.

  Description:
    The Reset Handler service provides a simple interface to trigger system
    resets and to manage and store reset causes. This file provides the 
    interface definition for this service.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef SRV_RESET_HANDLER_H    // Guards against multiple inclusion
#define SRV_RESET_HANDLER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PRIME Reset Causes

 Summary:
    Reset causes defined for a PRIME system.

 Description:
    The PRIME reset causes include the device reset causes and reset causes 
    specific to the PRIME operation.

 Remarks:
    None.
*/
typedef enum {
    RESET_HANDLER_GENERAL_RESET       = 0,      /* First power reset */
	RESET_HANDLER_BACKUP_RESET        = 1,      /* Wake up from backup mode */
	RESET_HANDLER_WATCHDOG_RESET      = 2,      /* Watchdog fault */
	RESET_HANDLER_SOFTWARE_RESET      = 3,      /* Reset requested by the software */
	RESET_HANDLER_USER_RESET          = 4,      /* Reset requested by the user (NRST pin low) */
	RESET_HANDLER_FU_RESET            = 5,      /* Reset during firmware upgrade */
	RESET_HANDLER_USAGE_FAULT_RESET   = 6,      /* Usage fault */
	RESET_HANDLER_BUS_FAULT_RESET     = 7,      /* Bus fault */
	RESET_HANDLER_MEM_MANAGE_RESET    = 8,      /* Memory management fault */
	RESET_HANDLER_HARD_FAULT_RESET    = 9,      /* Hard fault */
	RESET_HANDLER_VECTOR_FAULT_RESET  = 10,     /* Vector fault */
} SRV_RESET_HANDLER_RESET_CAUSE;

// *****************************************************************************
// *****************************************************************************
// Section: Reset Handler Service Interface Definition
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SRV_RESET_HANDLER_Initialize(void)

  Summary:
    Initializes the Reset Handler service.

  Description:
    This routine initializes the PRIME Reset Handler service.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    int main()
    {
        SRV_RESET_HANDLER_Initialize();
    }
    </code>

  Remarks:
    This routine must be called before any other PRIME Reset Handler service 
    routine. This function is normally not called directly by an application. 
    It is called by the system's initialize routine (SYS_Initialize).
*/

void SRV_RESET_HANDLER_Initialize(void);

// *****************************************************************************
/* Function:
    void SRV_RESET_HANDLER_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType)

  Summary:
    Trigger a system restart.

  Description:
    This routine triggers a system restart. It can be invoked by the PRIME stack
    as well as by the application.

  Precondition:
    The SRV_RESET_HANDLER_Initialize routine must have been called before.
    
  Parameters:
    resetType      - Type of reset

  Returns:
    None.

  Example:
    <code>
    int main(void) 
    {
        SRV_RESET_HANDLER_RestartSystem(RESET_HANDLER_SOFTWARE_RESET);
    </code>

  Remarks:
    None.
*/

void SRV_RESET_HANDLER_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType);


#endif //SRV_RESET_HANDLER_H

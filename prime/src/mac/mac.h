/*******************************************************************************
  PRIME MAC Control Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    mac.h

  Summary:
    PRIME MAC Control Interface Header File

  Description:
    This file contains definitions of the PRIME MAC control functions to be used 
    by the PRIME stack when managing the PRIME MAC layer.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef MAC_H_INCLUDE
#define MAC_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mac_defs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: PRIME MAC Control Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void MAC_Initialize(MAC_VERSION_INFO *macInfo, uint8_t secProfile)

  Summary:
    Initializes the PRIME MAC layer.

  Description:
    This routine initializes the PRIME MAC layer.

  Precondition:
    None.

  Parameters:
    macInfo     - Pointer to the MAC version information
    secProfile  - Security profile (0-2)

  Returns:
    None.

  Example:
    <code>
    MAC_VERSION_INFO macInfo;
    
    macInfo.fwVersion = "HS14.01.01\0\0\0\0\0\0";
    macInfo.fwModel = "PIC32CXXPL460";
    macInfo.fwVendor = "MCHP";
    macInfo.pibVendor = 0;
    macInfo.pibModel = 0x3941;
    
    MAC_Initialize(&macInfo, 0);
    </code>

  Remarks:
    This routine is normally not called directly by an application. It is 
    called by the PRIME stack initalization routine.
*/
void MAC_Initialize(MAC_VERSION_INFO *macInfo, uint8_t secProfile);

// ****************************************************************************
/* Function:
    void MAC_Tasks(void)

  Summary:
    Maintains the MAC layer state machine. 

  Description:
    This function is used to maintain the MAC layer internal state machine and 
    generate callbacks.

  Precondition:
    The MAC_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        MAC_Tasks();
    }
    </code>

  Remarks:
    This function must be periodically called by the user application.
*/
void MAC_Tasks(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* MAC_H_INCLUDE */

/*******************************************************************************
 End of File
*/

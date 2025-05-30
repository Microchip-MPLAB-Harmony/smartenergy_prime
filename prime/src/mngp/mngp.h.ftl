/*******************************************************************************
  PRIME Management Plane Control Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    mngp.h

  Summary:
    PRIME Management Plane Control Interface Header File

  Description:
    This file contains definitions of the PRIME Management Plane control functions 
    to be used by the PRIME stack when managing the PRIME Management Plane.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

#ifndef MNGP_H_INCLUDED
#define MNGP_H_INCLUDED

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "stack/prime/mac/mac_defs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: PRIME Management Plane Control Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void MNGP_Initialize(MAC_VERSION_INFO *macInfo, uint8_t usiPort)

  Summary:
    Initializes the PRIME Management Plane.

  Description:
    This routine initializes the PRIME Management Plane.

  Precondition:
    The MAC_Initialize function should have been called before calling this 
    function.

  Parameters:
    macInfo     - Pointer to the MAC version information
    usiPort     - USI port

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
    
    MNGP_Initialize(&macInfo, 0);
    </code>

  Remarks:
    This routine is normally not called directly by an application. It is 
    called by the PRIME Stack initialization routine.
*/
void MNGP_Initialize(MAC_VERSION_INFO *macInfo, uint8_t usiPort);

<#if (PRIME_MODE == "SN") || (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
// ****************************************************************************
/* Function:
    void MNGP_Tasks(void)

  Summary:
    Maintains the Management Plane state machine. 

  Description:
    This function is used to maintain the Management Plane internal state 
    machine and generate callbacks.

  Precondition:
    The MNGP_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        MNGP_Tasks();
    }
    </code>

  Remarks:
    This function must be periodically called by the user application through
    the PRIME Stack.
*/
void MNGP_Tasks(void);
</#if>

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* MNGP_H_INCLUDED */

/*******************************************************************************
 End of File
*/

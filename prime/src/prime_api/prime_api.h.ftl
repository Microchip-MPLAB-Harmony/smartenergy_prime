/*******************************************************************************
  PRIME API Header 
   
  Company:
    Microchip Technology Inc.

  File Name:
    prime_api.h

  Summary:
    PRIME API Header File

  Description:
    This module converts the PRIME stack library interface into a global 
    interface to be used by the PRIME application.
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

#ifndef PRIME_API_H_INCLUDE
#define PRIME_API_H_INCLUDE

<#if PRIME_MODE == "BN" || (PRIME_MODE == "SN" && PRIME_PROJECT == "application project")>
// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include "prime_api_types.h"

</#if>
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END
<#if PRIME_MODE == "BN" || (PRIME_MODE == "SN" && PRIME_PROJECT != "application project")>
// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

#define PRIME_FW_VENDOR          "${PRIME_FW_VENDOR?string}"
#define PRIME_FW_MODEL           "${PRIME_FW_MODEL?string}"
#define PRIME_FW_VERSION         "${PRIME_FW_VERSION?string}"
#define PRIME_PIB_VENDOR         ${PRIME_PIB_VENDOR}
#define PRIME_PIB_MODEL          0x${PRIME_PIB_MODEL}

</#if>
// *****************************************************************************
// *****************************************************************************
// Section: PRIME API Control Interface Routines
// *****************************************************************************
// *****************************************************************************

<#if PRIME_MODE == "SN" && PRIME_PROJECT == "application project">
void PRIME_API_GetPrime13API(PRIME_API **pPrimeApi);
void PRIME_API_GetPrime14API(PRIME_API **pPrimeApi);
<#else>
// *****************************************************************************
/* Function:
    void PRIME_API_Initialize(void *halApi)

  Summary:
    Initializes the PRIME stack.

  Description:
    This routine initializes the PRIME stack.

  Precondition:
    None.

  Parameters:
    halApi      - Pointer to the HAL API functions

  Returns:
    None.

  Example:
    <code>
    PRIME_API_Initialize((HAL_API*)&halApi);
    </code>

  Remarks:
    This routine is normally not called directly by an application. The 
    PRIME application must use the function located in the header table.
*/
void PRIME_API_Initialize(void *halApi);

// ****************************************************************************
/* Function:
    void PRIME_API_Tasks(void)

  Summary:
    Maintains the PRIME stack state machine. 

  Description:
    This function is used to maintain the PRIME stack internal state machine and 
    generate callbacks.

  Precondition:
    The PRIME_API_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        PRIME_API_Tasks();
    }
    </code>

  Remarks:
    This routine is normally not called directly by an application. The 
    PRIME application must use the function located in the header table 
    and call it periodically.
*/
void PRIME_API_Tasks(void);
</#if>

<#if PRIME_MODE == "BN">
void PRIME_API_GetPrimeAPI(PRIME_API **pPrimeApi);
</#if>

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* PRIME_API_H_INCLUDE */

/*******************************************************************************
 End of File
*/

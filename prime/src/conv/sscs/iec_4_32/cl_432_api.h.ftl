/*******************************************************************************
  PRIME 4.32 Convergence Sublayer API Header

  Company:
    Microchip Technology Inc.

  File Name:
    cl_432_api.h

  Summary:
    PRIME 4-32 Convergence Sublayer API Header File

  Description:
    This file contains definitions of the PRIME 4-32 Convergence Sublayer
    primitives to be used by the PRIME application.
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

#ifndef CL_432_API_H_INCLUDE
#define CL_432_API_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "cl_432_defs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: PRIME 4-32 Convergence Sublayer Interface Primitives
// *****************************************************************************
// *****************************************************************************

// ****************************************************************************
/* Function:
    void CL_432_SetCallbacks(CL_432_CALLBACKS *cl432cbs)

  Summary:
    Sets 4-32 Convergence layer callback functions. 

  Description:
    This routine links callback functions between upper layer and the CL-432 
    Convergence layer.

  Precondition:
    The CL_432_Initialize function should have been called before calling this 
    function.

  Parameters:
    cl432cbs         Callbacks structure

  Returns:
    None

  Example:
    <code>
    static void _dl_data_ind((uint8_t dstLsap, uint8_t srcLsap, uint16_t dstAddress, 
        uint16_t srcAddress, uint8_t *data, uint16_t lsduLen, uint8_t link_Class)
    {
        ...
    }
    
    void main(void) 
    {
        CL_432_CALLBACKS cl432cbs;
    
        CL_432_Initialize();
    
        memset(cl432cbs, NULL, sizeof(cl432cbs));
        cl432cbs.cl_432_dl_data_ind = _dl_data_ind;

        CL_432_SetCallbacks(&cl432cbs);
    }
    </code>

  Remarks:
    None
*/
void CL_432_SetCallbacks(CL_432_CALLBACKS *cl432cbs);

// *****************************************************************************
/* Function:
    void CL_432_ReleaseRequest
    (
        uint16_t dstAddress
    )

  Summary:
    Request to release a CL-432 connection.

  Description:
    This routine is used to request a CL-432 connection release.
    
  Precondition:
    The CL_432_Initialize routine must have been called before.

  Parameters:
    dstAddress  - Address to disconnect

  Returns:
    None.

  Example:
    <code>
	CL_432_ReleaseRequest(300);
    </code>

  Remarks:
    The result of the request is returned in the confirm callback.
*/
void CL_432_ReleaseRequest(uint16_t dstAddress);

// *****************************************************************************
/* Function:
    void CL_432_DlDataRequest
    (
        uint8_t dstLsap, 
        uint8_t srcLsap, 
        uint16_t dstAddress, 
        DL_432_BUFFER *buff, 
        uint16_t lsduLen, 
        uint8_t linkClass
    )

  Summary:
    Request the transmission of data over a CL-432 connection.

  Description:
    This routine is used to request the transmission of data over a CL-432 connection.
    
  Precondition:
    The CL_432_Initialize routine must have been called before.

  Parameters:
    dstLsap     - Destination LSAP
    srcLsap     - Source LSAP
    dstAddress  - Destination 4-32 address
    buff        - Pointer to the data buffer
    lsduLen     - Length of the data
    linkClass   - Link class (not used)

  Returns:
    None.

  Example:
    <code>    
    uint16_t msgLen = 20;
    DL_432_BUFFER msg[msgLen] = {0};
    
    CL_432_DlDataRequest(2, 0, 300, msg, msgLen, 0);
    </code>

  Remarks:
    The result of the request is returned in the confirm callback.
*/
void CL_432_DlDataRequest(uint8_t dstLsap, uint8_t srcLsap, uint16_t dstAddress, 
    DL_432_BUFFER *buff, uint16_t lsduLen, uint8_t linkClass);

<#if (PRIME_MODE == "SN") || (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
// *****************************************************************************
/* Function:
    void CL_432_EstablishRequest
    (
        uint8_t *deviceId, 
        uint8_t deviceIdLen, 
        uint8_t ae
    )

  Summary:
    Request a CL-432 connection establishment.

  Description:
    This routine is used to request a CL-432 connection establishment.
    
  Precondition:
    The CL_432_Initialize routine must have been called before.

  Parameters:
    deviceId        - Pointer to the device identifier data
    deviceIdLen     - Length of the device identifier
    ae              -  Flag to indicate that authentication and encryption is 
                    requested (v1.4)

  Returns:
    None.

  Example:
    <code>    
    uint8_t eui48[6];
    memset(eui48, 0x12, 6);
    
    CL_432_EstablishRequest(eui48, 6, 0);
    </code>

  Remarks:
    The result of the request is returned in the confirm callback.
*/
void CL_432_EstablishRequest(uint8_t *deviceId, uint8_t deviceIdLen, uint8_t ae);
</#if>

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* CL_432_API_H_INCLUDE */

/*******************************************************************************
 End of File
*/

/*******************************************************************************
  PRIME Non-Volatile Storage Service Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    srv_storage.h

  Summary:
    PRIME Non-Volatile Storage Service Interface Header File.

  Description:
    The non-volatile Storage service provides a simple interface to read and
    write non-volatile data used by the PRIME stack. This file provides the
    interface definition for the Storage service.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#ifndef SRV_STORAGE_H    // Guards against multiple inclusion
#define SRV_STORAGE_H

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
/* Non-Volatile Storage Data Types

  Summary:
    List of data types used in PRIME non-volatile storage service.

  Description:
    This data type defines the list of the different information stored in
    non-volatile memory used by the PRIME stack.

  Remarks:
    None
*/

typedef enum
{
    SRV_STORAGE_TYPE_MAC_INFO = 0,
    SRV_STORAGE_TYPE_PHY_INFO = 1,
    SRV_STORAGE_TYPE_BN_INFO = 2,
    SRV_STORAGE_TYPE_MODE_PRIME = 3,
    SRV_STORAGE_TYPE_SECURITY = 4,
    SRV_STORAGE_TYPE_BOOT_INFO = 5,
    SRV_STORAGE_TYPE_END_LIST

} SRV_STORAGE_TYPE;

// *****************************************************************************
// *****************************************************************************
// Section: Storage Service Interface Definition
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SRV_STORAGE_Initialize(void);

  Summary:
    Initializes the PRIME non-volatile Storage service.

  Description:
    This routine initializes the PRIME non-volatile Storage service.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SRV_STORAGE_Initialize();
    </code>

  Remarks:
    This routine must be called before any other PRIME non-volatile Storage
    service routine. This function is normally not called directly by an
    application. It is called by the system's initialize routine
    (SYS_Initialize).
*/

void SRV_STORAGE_Initialize(void);

// *****************************************************************************
/* Function:
    bool SRV_STORAGE_GetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, void* pData);

  Summary:
    Reads configuration information from non-volatile memory.

  Description:
    This routine reads configuration information from non-volatile memory.

  Precondition:
    The SRV_STORAGE_Initialize routine must have been called before.

  Parameters:
    infoType - Configuration information type to read.
    size     - Size in bytes of the configuration information to read.
    pData    - Pointer to store the read configuration information.

  Returns:
    The result of the read operation (true if success, false if error).

  Example:
    <code>
    uint8_t macInfo[8];

    if (SRV_STORAGE_GetConfigInfo(SRV_STORAGE_TYPE_MAC_INFO, 8, macInfo))
    {

    }
    </code>

  Remarks:
    None.
*/

bool SRV_STORAGE_GetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, void *pData);

// *****************************************************************************
/* Function:
    bool SRV_STORAGE_SetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, void* pData);

  Summary:
    Writes configuration information in non-volatile memory.

  Description:
    This routine writes configuration information in non-volatile memory.

  Precondition:
    The SRV_STORAGE_Initialize routine must have been called before.

  Parameters:
    infoType - Configuration information type to write.
    size     - Size in bytes of the configuration information to write.
    pData    - Pointer to configuration information data to write.

  Returns:
    The result of the write operation (true if success, false if error).

  Example:
    <code>
    uint8_t macInfo[8] = {0x55, 0xAA, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

    SRV_STORAGE_SetConfigInfo(SRV_STORAGE_TYPE_MAC_INFO, 8, macInfo);
    </code>

  Remarks:
    None.
*/

bool SRV_STORAGE_SetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, void *pData);

#endif //SRV_STORAGE_H

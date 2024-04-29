/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    pal_serial_local.h

  Summary:
    Platform Abstraction Layer (PAL) PLC Interface Local header file.

  Description:
    Platform Abstraction Layer (PAL) PLC Interface Local header file.
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

#ifndef PAL_SERIAL_LOCAL_H
#define PAL_SERIAL_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "pal_types.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
#define PAL_SERIAL_ID_INFO_VERSION  0x0400  // TBD
#define PAL_SERIAL_ID_INFO_DEVICE   0x0400  // TBD

// *****************************************************************************
/* SERIAL PAL Module Status

  Summary:
    Identifies the current status/state of the SERIAL PAL module.

  Description:
    This enumeration identifies the current status/state of the SERIAL PAL module.

  Remarks:
    This enumeration is the return type for the PAL_SERIAL_Status routine. The
    upper layer must ensure that PAL_SERIAL_Status returns PAL_SERIAL_STATUS_READY
    before performing SERIAL PAL operations.
*/
typedef enum
{
    PAL_SERIAL_STATUS_UNINITIALIZED = SYS_STATUS_UNINITIALIZED,
    PAL_SERIAL_STATUS_BUSY = SYS_STATUS_BUSY,
    PAL_SERIAL_STATUS_READY = SYS_STATUS_READY,
    PAL_SERIAL_STATUS_ERROR = SYS_STATUS_ERROR,
    PAL_SERIAL_STATUS_INVALID_OBJECT = SYS_STATUS_ERROR_EXTENDED - 1,
} PAL_SERIAL_STATUS;

// *****************************************************************************
/* PAL PLC Data

  Summary:
    Holds PAL PLC internal data.

  Description:
    This data type defines the all data required to handle the PAL PLC module.

  Remarks:
    None.
*/
typedef struct
{
    PAL_CALLBACKS serialCallbacks;

    PAL_SERIAL_STATUS status;

    SRV_USI_HANDLE usiHandler;

    uint8_t serialData[PAL_SERIAL_MAX_DATA_SIZE];

    DRV_SERIAL_PHY_PIB_OBJ plcPIB;

    DRV_PLC_PHY_TRANSMISSION_OBJ phyTxObj;

    DRV_PLC_PHY_RECEPTION_OBJ phyRxObj;

    DRV_PLC_PHY_TRANSMISSION_CFM_OBJ phyCfmObj;

} PAL_SERIAL_DATA;

#endif // #ifndef PAL_SERIAL_LOCAL_H
/*******************************************************************************
 End of File
*/
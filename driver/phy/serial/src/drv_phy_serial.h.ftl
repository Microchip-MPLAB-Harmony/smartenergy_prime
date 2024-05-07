/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    drv_phy_serial.h

  Summary:
    Physical layer for the PRIME serial interface header file.

  Description:
    This module handles the serial transmission and reception of PRIME messages 
    at the physical.
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

#ifndef PHY_SERIAL_H
#define PHY_SERIAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
//DOM-IGNORE-END
#include "configuration.h" 
#include "service/usi/srv_usi.h"
    
// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
/* PHY Serial Instance Configuration Options */
#define DRV_PHY_SERIAL_INDEX            0U

#define DRV_PHY_SERIAL_USI_INSTANCE       SRV_USI_INDEX_${DRV_PHY_SERIAL_USI_INSTANCE?string}

/* Maximum physical pdu size */
#define DRV_PHY_SERIAL_MAX_PPDU_SIZE    512U

/* Serial channel */
#define DRV_PHY_SERIAL_CHANNEL          0x400

// *****************************************************************************
/* PHY Serial TX result macros
 Summary:
    PHY Serial transmission results.

 Description:
    PHY Serial transmission result macros are used as the return types for data 
    transmission.

 Remarks:
    None.
*/
/* Transmission result: already in process */
#define DRV_PHY_SERIAL_TX_RESULT_PROCESS                   0
/* Transmission result: end successfully */
#define DRV_PHY_SERIAL_TX_RESULT_SUCCESS                   1
/* Transmission result: busy transmission error */
#define DRV_PHY_SERIAL_TX_RESULT_BUSY_TX                   4

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PHY Serial mode type enumeration

 Summary:
    PHY Serial PRIME mode of transmission.

 Description:
    This mode type will be used to indicate the transfer of data.

 Remarks:
    None.
*/
typedef enum 
{
    PHY_SERIAL_MODE_TYPE_A = 0,
    PHY_SERIAL_MODE_TYPE_B = 2,
    PHY_SERIAL_MODE_TYPE_BC = 3,
} DRV_PHY_SERIAL_MODE_TYPES;

// *****************************************************************************
/* PHY Serial transmission data structure
 Summary:
    Data structure used for serial transmission

 Description:
    This structure contains the MAC Protocol Data Unit (MPDU) to be transmitted
    and its associated transmission parameters. For serial transmissions from 
    the PRIME stack, the Embedded USI provides packs and sends each message via
    the USI service packs and sends.

 Remarks:
    None.
*/
typedef struct 
{
    /* Pointer to data buffer */
    uint8_t *dataBuf;
    /* Delay for transmission in us */
    uint32_t timeDelay;
    /* Length of the data buffer */
    uint16_t dataLen;
    /* Time mode: 0: Absolute mode, 1: Differential mode, 2: Cancel TX */
    /* NOT USED: keep for backwards compatibility */
    uint8_t timeMode;
    /* Attenuation level with which the message must be transmitted */
    /* NOT USED: keep for backwards compatibility */
    uint8_t attLevel; 
    /* CSMA algorithm parameters */
    /* NOT USED: keep for backwards compatibility */
    uint8_t csma;
    /* Buffer identifier */
    uint8_t buffId;
    /* Modulation scheme of last transmitted message */
    /* NOT USED: keep for backwards compatibility */
    uint8_t scheme; 
    /* Type A, Type B, Type BC, Type Radio */
    /* NOT USED: keep for backwards compatibility */
    uint8_t mode;
    /* Physical channel to transmit the message */
    uint16_t pch;
} DRV_PHY_SERIAL_MSG_REQUEST_DATA;

// *****************************************************************************
/* PHY Serial confirm data structure
 Summary:
    Data structure used to confirm the result of transmission request

 Description:
    This structure is used to indicate to the MAC layer the result of the 
    transmission request of the MAC Protocol Data Unit (MPDU).

 Remarks:
    None.
*/
typedef struct 
{
    /* Transmission time in 10us */
    uint32_t txTime;
    /* RMS value emitted (valid only when txQRMode is enable) */
    uint32_t rmsCalc;
    /* Type mode: Type A, Type B or Type BC  */
    uint8_t mode;
    /* Result */
    uint8_t result;
    /* Number of the buffer used to tx */
    uint8_t buffId;
} DRV_PHY_SERIAL_MSG_CONFIRM_DATA;

// *****************************************************************************
/* PHY Serial received data structure
 Summary:
    PHY Serial receiver parameters.

 Description:
    This structure contains the received MAC Protocol Data Unit (MPDU) and its
    associated reception parameters provided by the USI service. The PHY Serial 
    forwards this information to the MAC layer.

 Remarks:
    None.
*/
typedef struct
{
    /* Pointer to local data buffer */
    uint8_t *dataBuf;
    /* Reception time in us */
    uint32_t rxTime;
    /* Accumulated Error Vector Magnitude for header */
    uint32_t evmHeaderAcum;
    /* Accumulated Error Vector Magnitude for payload */
    uint32_t evmPayloadAcum;
    /* Error Vector Magnitude for header */
    uint16_t evmHeader;
    /* Error Vector Magnitude for payload */
    uint16_t evmPayload;
    /* Length of the data buffer. */
    uint16_t dataLen;
    /* Modulation scheme of the last received message */
    uint8_t scheme;
    /* Type A, Type B or Type BC  */
    uint8_t mode;
    /* Header Type of the last received message */
    uint8_t headerType;
    /* Average RSSI (Received Signal Strength Indication) */
    uint8_t rssiAvg;
    /* Average CNIR (Carrier to Interference + Noise ratio) */
    uint8_t cinrAvg;
    /* Minimum CNIR (Carrier to Interference + Noise ratio) */
    uint8_t cinrMin;
    /* Viterbi soft Bit Error Rate value */
    uint8_t bersoft;
    /* Viterbi soft Bit Error Rate Maximum value */
    uint8_t bersoftMax;
    /* QT value */
    uint8_t qt;
    /* Extended signal noise ratio value */
    uint8_t snrEx;
    /* Noise result in case of noise capture mode */
    uint8_t noiseResult;
    /* Buffer identifier */
    uint8_t buffId;
} DRV_PHY_SERIAL_MSG_RX_DATA;

// *****************************************************************************
/* PHY Serial confirm data transmission function pointer

  Summary:
    Callback used for confirm data transmission.
    
  Description:
    The confirm attribute is used  to tells the MAC layer if MAC Protocol Data
	Unit (MPDU) request has been successfully transmitted.

  Remarks:
    None.
*/
typedef void (*DRV_PHY_SERIAL_CONFIRM_DATA_CB)(DRV_PHY_SERIAL_MSG_CONFIRM_DATA *dataConfirm);

// *****************************************************************************
/* PHY Serial RX data reception function pointer

  Summary:
    Callback used for data reception.
    
  Description:
    This callback is used to indicate the reception of a message to the MAC layer. 

  Remarks:
    None.
*/
typedef void (*DRV_PHY_SERIAL_RX_MSG_CB)(DRV_PHY_SERIAL_MSG_RX_DATA *rxMsg);

// *****************************************************************************
/* PHY Serial callback handlers data

  Summary:
    Defines the handlers required to manage the PHY Serial module.

  Description:
    This data type defines the handlers required to manage the PHY Serial
    module.

  Remarks:
    None.
*/
typedef struct 
{
    DRV_PHY_SERIAL_CONFIRM_DATA_CB dataConfirm;
    DRV_PHY_SERIAL_RX_MSG_CB dataReception;
} DRV_PHY_SERIAL_CALLBACKS;

// *****************************************************************************
/* PHY Serial Initialization Data

  Summary:
    Defines the data required to initialize the PHY serial module.

  Description:
    This data type defines the data required to initialize the PHY serial module,
    using DRV_PHY_SERIAL_Initialize.

  Remarks:
    None.
*/
typedef struct
{
    DRV_PHY_SERIAL_CALLBACKS serialPhyHandlers;
} DRV_PHY_SERIAL_INIT;

// ****************************************************************************
// ****************************************************************************
// Section: Interface Routines
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_PHY_SERIAL_Initialize(const SYS_MODULE_INDEX index,
 *                                      const SYS_MODULE_INIT * const init)

  Summary:
    Initializes PHY Serial module. 

  Description:
    This routine initializes PHY Serial module. 

  Precondition:
    The low-level processor and board initialization must be completed before 
    the system can call the initialization functions for any modules.
    It is a single instance module, so this function should be called only once.

  Parameters:
    index - Identifier for the instance to be initialized. Only one instance
            (index 0) supported.
    init -  Module initialization
 
  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    int main()
    {
        SYS_MODULE_OBJ sysObjDrvPhySerial;
        const SYS_MODULE_INIT * const init;
        sysObjDrvPhySerial = DRV_PHY_SERIAL_Initialize(DRV_PHY_SERIAL_INDEX_0, init);
    }
    </code>

  Remarks:
    This routine must be called before any other PHY Serial Driver routine and should
    only be called once during system initialization.
*/
SYS_MODULE_OBJ DRV_PHY_SERIAL_Initialize(const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init);

// ****************************************************************************
/* Function:
    void DRV_PHY_SERIAL_Deinitialize(SYS_MODULE_OBJ object)

  Summary:
    De-initializes the specified instance of the PHY Serial module. 

  Description:
    This function de-initializes the specified instance of the PHY Serial module,
    disabling its operation and invalidates all of the internal data.

  Precondition:
    Function DRV_PHY_SERIAL_Initialize must have been called before calling this
    routine and a valid SYS_MODULE_OBJ must have been returned.

  Parameters:
    object  - PHY Serial module object handle, returned by DRV_PHY_SERIAL_Initialize.

  Returns:
    None

  Example:
    <code>
    SYS_MODULE_OBJ phySerialobj;

    DRV_PHY_SERIAL_Deinitialize(phySerialobj);
    </code>

  Remarks:
    Once DRV_PHY_SERIAL_Initialize has been called, DRV_PHY_SERIAL_Deinitialize must be
    called before DRV_PHY_SERIAL_Initialize can be called again.
*/
void DRV_PHY_SERIAL_Deinitialize(SYS_MODULE_OBJ object);

// ****************************************************************************
/* Function:
    void DRV_PHY_SERIAL_Tasks(void)

  Summary:
    Maintains the PHY Serial state machine. 

  Description:
    This function is used to maintain the PHY Serial internal state machine and 
    generate callbacks.

  Precondition:
    The DRV_PHY_SERIAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        DRV_PHY_SERIAL_Tasks();
    }
    </code>

  Remarks:
    This function must be periodically called by the user application.
*/
void DRV_PHY_SERIAL_Tasks(void);

// ****************************************************************************
/* Function:
    void DRV_PHY_SERIAL_SetCallbacks(PHY_SERIAL_CALLBACKS *phy_serial_cbs)

  Summary:
    Configures the PHY Serial callbacks. 

  Description:
    This function is used to set PHY serial callback functions.

  Precondition:
    The PHY_SERIAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    phy_serial_cbs         Callbacks structure

  Returns:
    None

  Example:
    <code>
    void PAL_Initialize(void)
    {
        DRV_PHY_SERIAL_CALLBACKS phy_serial_cbs;

        memset(phy_serial_cbs, NULL, sizeof(phy_serial_cbs));

        DRV_PHY_SERIAL_SetCallbacks(&phy_serial_cbs);
    }
    </code>

  Remarks:
    None.
*/
void DRV_PHY_SERIAL_SetCallbacks(DRV_PHY_SERIAL_CALLBACKS *phy_serial_cbs);

// ****************************************************************************
/* Function:
    uint8_t DRV_PHY_SERIAL_DataRequestTransmission(DRV_PHY_SERIAL_MSG_REQUEST_DATA *requestMsg)

  Summary:
    Request to transmit a message

  Description:
    This functions is used to initiate the transmission process of a PPDU 
    (PHY Protocol Data Unit) over the PHY Serial medium.

  Precondition:
    The PHY_SERIAL_Initialize function should have been called before calling 
    this function.

  Parameters:
    requestMsg         MPDU PHY serial data transmission structure

  Returns:
    PHY Serial Transmission result.

  Example:
    <code>

    uint8_t PAL_DataRequestTransmission(PAL_MSG_REQUEST_DATA *requestMsg)
    {
        return DRV_PHY_SERIAL_DataRequestTransmission((DRV_PHY_SERIAL_MSG_REQUEST_DATA *)requestMsg);
    }
    
    </code>

  Remarks:
    None
*/
uint8_t DRV_PHY_SERIAL_DataRequestTransmission(DRV_PHY_SERIAL_MSG_REQUEST_DATA *requestMsg);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* PHY_SERIAL_H */

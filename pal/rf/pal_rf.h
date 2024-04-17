/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    pal.h

  Summary:
    Physical Abstraction Layer (PAL) header file.

  Description:
    This module provides the interface between the PRIME MAC layer and the 
    different physical layers.
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

#ifndef PAL_RF_H_INCLUDED
#define PAL_RF_H_INCLUDED

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
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
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/* Robust Management (RM) value used when there is no information available 
about the modulation scheme to be used */
#define PAL_OUTDATED_INF    0x0F

// *****************************************************************************
/* PAL PHY Timer Mode Macros
 Summary:
    PAL PHY different timer modes.

 Description:
    These timer modes macros are used during PHY communication with MAC layer.

 Remarks:
    None.
*/
/* Absolute TX scheduling mode (absolute TX time specified) */
#define PAL_TX_MODE_ABSOLUTE         0
/* Relative or Differential TX scheduling mode (delay for TX time specified) */
#define PAL_TX_MODE_RELATIVE         1
/* Cancel TX scheduling mode (cancel TX) */
#define PAL_TX_MODE_CANCEL           2

// *****************************************************************************
/* PAL configuration success result macros
 Summary:
    PAL configuration success results.

 Description:
    PAL configuration output results

 Remarks:
    None.
*/
#define PAL_CFG_SUCCESS              0
#define PAL_CFG_INVALID_INPUT        1

// *****************************************************************************
/* PAL TX result macros
 Summary:
    PAL transmission results.

 Description:
    PAL transmission result macros are used as the return types for data 
    transmission.

 Remarks:
    None.
*/

/* Transmission result: already in process */
#define PAL_TX_RESULT_PROCESS        0
/* Transmission result: end successfully */
#define PAL_TX_RESULT_SUCCESS        1
/* Transmission result: invalid length error */
#define PAL_TX_RESULT_INV_LENGTH     2
/* Transmission result: busy channel error */
#define PAL_TX_RESULT_BUSY_CH        3
/* Transmission result: busy transmission error */
#define PAL_TX_RESULT_BUSY_TX        4
/* Transmission result: busy reception error */
#define PAL_TX_RESULT_BUSY_RX        5
/* Transmission result: invalid scheme error */
#define PAL_TX_RESULT_INV_SCHEME     6
/* Transmission result: timeout error */
#define PAL_TX_RESULT_TIMEOUT        7
/* Transmission result: invalid buffer identifier error */
#define PAL_TX_RESULT_INV_BUFFER     8
/* Transmission result: invalid PRIME Mode error */
#define PAL_TX_RESULT_INV_PRIME_MODE 9
/* Transmission result: invalid transmission mode error */
#define PAL_TX_RESULT_INV_TX_MODE    10
/* Transmission result: transmission cancelled */
#define PAL_TX_RESULT_INV_CANCELLED  11
/* Transmission result: high temperature 
(>120 Degree Centigrade) error (only with PL460) */
#define PAL_TX_RESULT_HIGH_TEMP_120  12
/* Transmission result: high temperature 
(>110 Degree Centigrade) error (only with PL460) */
#define PAL_TX_RESULT_HIGH_TEMP_110  13
/* Transmission Result (only RF): invalid parameter */
#define PAL_TX_RESULT_INV_PARAM      20
/* Transmission result: error in tx */
#define PAL_TX_RESULT_PHY_ERROR      0xFE

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*PAL RF modulation scheme enumeration

 Summary:
    RF physical scheme of modulation

 Description:
    RF modulation scheme to be used for communicating with the selected node.

 Remarks:
    None.
*/
typedef enum  
{
	PAL_RF_FSK_FEC_OFF = 0,
	PAL_RF_FSK_FEC_ON  = 1,
} PAL_RF_MODULATION_SCHEME;

/* PAL transmission data structure
 Summary:
    Data structure used for transmission

 Description:
    This structure contains the MAC Protocol Data Unit (MPDU) to be transmitted
    and its associated transmission parameters. The PHY Abstraction Layer 
    forwards this information to the medium selected by the MAC layer.

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
    /* Physical channel to transmit the message */
    uint16_t pch;
    /* Buffer identifier */
    uint8_t buffIdentifier;
    /* Attenuation level with which the message must be transmitted */
    uint8_t attLevel;
    /* Modulation scheme of last transmitted message */
    uint8_t scheme;
    /* TX Forced */
    uint8_t disableRx;
    /* Type A, Type B, Type BC, Type Radio */
    uint8_t mode;
    /* Time mode: 0: Absolute mode, 1: Differential mode, 2: Cancel TX */
    uint8_t timeMode;
    /* Number of channel senses */
    uint8_t numSenses;
    /* Delay between channel senses in ms */
    uint8_t senseDelayMs;
} PAL_MSG_REQUEST_DATA;

/* PAL confirm data structure
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
    /* Transmission time in us */
    uint32_t txTime;
    /* RMS value */
    uint16_t rmsCalc;
    /* Physical channel where the message has been transmitted */
    uint16_t pch;
    /* Type mode: Type A, Type B, Type BC, Type Radio  */
    uint8_t mode;
    /* Buffer identifier */
    uint8_t buffId;
    /* Result */
    uint8_t result;
} PAL_MSG_CONFIRM_DATA;

/* PAL indication data structure
 Summary:
    Data structure used for PHY data indication

 Description:
    This structure contains the received MAC Protocol Data Unit (MPDU) and its
    associated reception parameters. The PHY Abstraction Layer forwards this 
    information to the MAC layer.

 Remarks:
    None.
*/
typedef struct 
{
    /* Pointer to data buffer */
    uint8_t *dataBuf;
    /* Reception time in us */
    uint32_t rxTime;
    /* Length of the data buffer */
    uint16_t dataLen;
    /* Physical channel where the message has been received */
    uint16_t pch;
    /* Bitrate estimation in Kbs */
    uint16_t estimatedBitrate;
    /* RSSI (Received Signal Strength Indicator) coded as specified */
    int16_t rssi;
    /* Buffer identifier */
    uint8_t buffId;
    /* Modulation scheme of the last received message */
    uint8_t scheme;
    /* Type A, Type B, Type BC, Type Radio */
    uint8_t mode;
    /* Header type of the last received message */
    uint8_t headerType;
    /* Less robust modulation */
    uint8_t lessRobustMod;
    /* SNR (Signal Noise Ratio) /LQI (Link Quality Indicator) */
    uint8_t lqi;
} PAL_MSG_INDICATION_DATA;

/* PAL PLC PHY receiver data structure
 Summary:
    PLC PHY receiver parameters.

 Description:
    This structure contains information about the last received message in the 
    PLC PHY layer. This information is needed in the MAC layer for management 
    purposes.

 Remarks:
    None.
*/
typedef struct
{
    /* Accumulated Error Vector Magnitude for header */
    uint32_t evmHeaderAcum;
    /* Accumulated Error Vector Magnitude for payload */
    uint32_t evmPayloadAcum;
    /* Reception time (start of message) in 1us */
    uint32_t rXTime;
    /* Error Vector Magnitude for header */
    uint16_t evmHeader;
    /* Error Vector Magnitude for payload */
    uint16_t evmPayload;
    /* Length of the data buffer in bytes */
    uint16_t dataLen;
    /* Modulation scheme of the received message */
    uint8_t scheme;
    /* Type A, Type B or Type BC */
    uint8_t modType;
    /* Header Type of the received message */
    uint8_t headerType;
    /* Average RSSI (Received Signal Strength Indication) */
    uint8_t rssiAvg;
    /* Average CNIR (Carrier to Interference + Noise ratio) */
    uint8_t cinrAvg;
    /* Minimum CNIR (Carrier to Interference + Noise ratio) */
    uint8_t cinrMin;
    /* Average Soft BER (Bit Error Rate) */
    uint8_t berSoft;
    /* Maximum Soft BER (Bit Error Rate) */
    uint8_t berSoftMax;
    /* Percentage of carriers affected by narrow band noise */
    uint8_t narBandPercent;
    /* Percentage of symbols affected by impulsive noise */
    uint8_t impPercent;
} PAL_PLC_RX_PHY_PARAMS;

/* PHY Abstraction Layer confirm data transmission function pointer

  Summary:
    Callback used for confirm data transmission.
    
  Description:
    The confirm attribute is used  to tells the MAC layer if MAC Protocol Data
	Unit (MPDU) request has been successfully transmitted.

  Remarks:
    None.
*/
typedef void (*PAL_DATA_CONFIRM_CB)(PAL_MSG_CONFIRM_DATA *dataConfirm);

/* PHY Abstraction Layer Indication Data Transmission Function Pointer

  Summary:
    Callback used for indication data transmission.

  Description:
    This callback is used to indicate the reception of a message to the MAC layer. 

  Remarks:
    None.
*/
typedef void (*PAL_DATA_INDICATION_CB)(PAL_MSG_INDICATION_DATA *dataIndication);

/* PHY Abstraction Layer switch RF channel function pointer

  Summary:
    Callback designed to inform the MAC layer the execution of a programmed 
    change of the RF channel. 

  Description:
    This callback is used to inform the MAC layer upon the execution of a 
    programmed change of the RF (Radio Frequency) channel.

  Remarks:
    None.
*/
typedef void (*PAL_SWITCH_RF_CH_CB)(void);

// ****************************************************************************
/* PRIME PAL handlers data

  Summary:
    Defines the handlers required to manage the PRIME PAL module.

  Description:
    This data type defines the handlers required to manage the PRIME PAL module.

  Remarks:
    None.
*/
typedef struct 
{
    PAL_DATA_CONFIRM_CB data_confirm;
    PAL_DATA_INDICATION_CB data_indication;
    PAL_SWITCH_RF_CH_CB switch_rf_ch;
} PAL_CALLBACKS;

// ****************************************************************************
// ****************************************************************************
// Section: Interface Routines
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    void PAL_Initialize(void)

  Summary:
    Initializes PRIME PAL layer. 

  Description:
    This routine allows the user to configure PAL layer initialization. Users 
    can adapt the PAL layer to every hardware platform.

  Precondition:
    The low-level processor and board initialization must be completed before 
    the system can call the initialization functions for any modules.
    It is a single instance module, so this function should be called only once.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    int main()
    {
        PAL_Initialize();
    }
    </code>

  Remarks:
    None
*/
void PAL_Initialize(void);

// ****************************************************************************
/* Function:
    void PAL_Tasks(void)

  Summary:
    Maintains the PAL state machine. 

  Description:
    This function is used to maintain the PAL internal state machine and 
    generate callbacks.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        PAL_Tasks();
    }
    </code>

  Remarks:
    This function must be periodically called by the user application.
*/
void PAL_Tasks(void);

// ****************************************************************************
/* Function:
    void PAL_SetCallbacks(PAL_CALLBACKS *palCBs)

  Summary:
    Sets PAL layer callback functions 

  Description:
    This routine links callback functions between upper layer and phy layer.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pal_cbs         Callbacks structure

  Returns:
    None

  Example:
    <code>
    static void _data_confirm_handler(PAL_MSG_CONFIRM_DATA *dataConfirm)
    {
        ...
    }
    
    void main(void) 
    {
        PAL_CALLBACKS pal_cbs;
    
        PAL_Initialize();
    
        memset(pal_cbs, NULL, sizeof(pal_cbs));
        pal_cbs.data_confirm = _data_confirm_handler;

        PAL_SetCallbacks(&palCBs);
    }
    </code>

  Remarks:
    None
*/
void PAL_SetCallbacks(PAL_CALLBACKS *palCBs);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetSNR(uint8_t *snr, uint8_t qt, uint16_t pch)

  Summary:
    Convert QT value to Signal Noise Ratio (SNR). 

  Description:
    This function is used to get the value of the Signal to Noise Ratio, 
    defined as the ratio of measured received signal level to noise level of 
    last received PPDU (PHY Protocol Data Unit).

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    snr         Signal to noise ratio output parameter
    qt          QT input parameter to get SNR level
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t snr=0;
    uint8_t qt=5;
    uint16_t pch=16;

    result = PAL_GetSNR(&snr, qt, pch);
    </code>

  Remarks:
    Not available in PHY Serial medium
*/
uint8_t PAL_GetSNR(uint8_t *snr, uint8_t qt, uint16_t pch); 

// ****************************************************************************
/* Function:
    uint8_t PAL_GetZCT(uint8_t *zct, uint16_t pch)

  Summary:
    Get zero-cross time (ZCT). 

  Description:
    This function is used to get the value of the zero-cross time of the mains
    and the time between the last transmission or reception and the zero cross
    of the mains.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    zct         Zero time output parameter
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t zct=0;
    uint16_t pch=16;

    result = PAL_GetZCT(&zct, pch);
    </code>

  Remarks:
    Not available for both PHY Serial and RF medium.
*/
uint8_t PAL_GetZCT(uint32_t *zct, uint16_t pch);  

// ****************************************************************************
/* Function:
    uint8_t PAL_GetTimer(uint32_t *timer, uint16_t pch)

  Summary:
    Get the current PHY time in us.

  Description:
    This routine is used to get current PHY time.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    timer       Current output time of PHY 
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint32_t timer=0;
    uint16_t pch=16;

    result = PAL_GetTimer(&timer, pch);
    </code>

  Remarks:
    None
*/
uint8_t PAL_GetTimer(uint32_t *timer, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetTimerExtended(uint64_t *timer)

  Summary:
    Get the extended PHY time in us. 

  Description:
    This routine is used to get the extended PHY time.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    timer       Extended output time  of PHY 

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t  result=PAL_CFG_SUCCESS;
    uint64_t timer=0;

    result = PAL_GetTimerExtended(&timer);
    </code>

  Remarks:
    None
*/
uint8_t PAL_GetTimerExtended(uint64_t *timer);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCD(
        uint8_t *cd, 
        uint8_t *rssi, 
        uint32_t *time, 
        uint8_t *header, 
        uint16_t pch)

  Summary:
    Get the carrier detect signal. 

  Description:
    This routine is used to get the value of carrier detect signal.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    cd          Carrier detect signal output parameter
    rssi        Received signal strength indicator
    time        Current time in us
    header      Header type
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t cd=0;
    uint8_t rssi=0;
    uint32_t time=0;
    uint8_t header=0;
    uint16_t pch=16;

    result = PAL_GetCD(&cd, &rssi, &time, &header, pch);
    </code>

  Remarks:
    Not available for both PHY Serial and PHY RF.
*/
uint8_t PAL_GetCD(uint8_t *cd, uint8_t *rssi, uint32_t *time, uint8_t *header, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetNL(uint8_t *noise, uint16_t pch)

  Summary:
    Get the noise floor level value. 

  Description:
    This routine is used to know the noise level present in the powerline.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    noise       Noise floor level output parameter
    pch         Medium used by the PHY to receive the PSDU

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t nl=0;
    uint16_t pch=16;

    result = PAL_GetNL(&nl, pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_GetNL(uint8_t *noise, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetAGC(uint8_t *mode, uint8_t *gain, uint16_t pch)

  Summary:
    Get the automatic gain mode of the PHY PLC layer. 

  Description:
    This routine is used to get Automatic Gain Mode (AGC) of the PHY PLC layer.
   
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    mode        Auto/Manual mode
    gain        Initial receiving gain in auto mode
    pch         Medium used by the PHY to receive the PSDU

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t mode=0;
    uint8_t gain=0;
    uint16_t pch=16;

    result = PAL_GetAGC(&mode, &gain, pch);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_GetAGC(uint8_t *mode, uint8_t *gain, uint16_t pch);  

// ****************************************************************************
/* Function:
    uint8_t PAL_SetAGC(uint8_t mode, uint8_t gain, uint16_t pch)

  Summary:
    Set the automatic gain mode of the PHY PLC layer. 

  Description:
    This routine is used to set Automatic Gain Mode (AGC) of the PHY PLC layer.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    mode        Auto/Manual mode (auto mode(0), manual mode(1))
    gain        Initial receiving gain in auto mode
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t mode=0;
    uint8_t gain=0;
    uint16_t pch=16;

    result = PAL_SetAGC(mode, gain, pch);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_SetAGC(uint8_t mode, uint8_t gain, uint16_t pch);    

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCCA(uint8_t *channelState)

  Summary:
    Get clear channel assessment mode value. 

  Description:
    This routine is used to get the clear channel assesment mode.
    The channel state helps to know whether or not the RF physical medium is 
    free.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    channelState       Channel state (0: busy, 1: free) of RF module

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t channelState=0;

    result = PAL_GetCCA(&channelState);
    </code>

  Remarks:
    Only available for PHY RF.
*/
uint8_t PAL_GetCCA(uint8_t *channelState);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCH(uint16_t *pch)

  Summary:
    Get the band (PLC) or the channel (RF). 

  Description:
    This routine is used to get the channel or band used for the communication.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t pch=0;

    result = PAL_GetCH(&pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_GetCH(uint16_t *pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_SetCH(uint16_t pch)

  Summary:
    Set the band (PLC) or the channel (RF). 

  Description:
    This routine is used to set the channel or band used for the communication.
    
  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t pch=16;

    result = PAL_SetCH(pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_SetCH(uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_DataRequestTransmission(PAL_MSG_REQUEST_DATA *requestMsg)

  Summary:
    Request to transmit a message 

  Description:
    This functions is used to initiate the transmission process of a PPDU 
    (PHY Protocol Data Unit) to the medium indicated in the transmission 
    information structure.  

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    requestMsg         MPDU data transmission structure

  Returns:
    PAL Transmission results.

  Example:
    <code>
    uint8_t result=PAL_TX_RESULT_SUCCESS;
    PAL_MSG_REQUEST_DATA requestMsg;
    uint8_t msg[30];
   
    requestMsg.dataBuf = &msg;
    requestMsg.timeDelay = 10000;
    requestMsg.dataLen = sizeof(msg);
    requestMsg.pch = 16;
    requestMsg.buffIdentifier = 2;
    requestMsg.attLevel = 0;
    requestMsg.scheme = PAL_PLC_DBPSK_R;
    requestMsg.disableRx = 0;
    requestMsg.mode = PAL_MODE_TYPE_B;
    requestMsg.timeMode = PAL_TX_MODE_ABSOLUTE;
    requestMsg.numSenses = 3;
    requestMsg.senseDelayMs = 3;

    result = PAL_DataRequestTransmission(&requestMsg);
    </code>

  Remarks:
    None
*/
uint8_t PAL_DataRequestTransmission(PAL_MSG_REQUEST_DATA *requestMsg);

// ****************************************************************************
/* Function:
    void PAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode)

  Summary:
    Program a channel switch in the given time. 

  Description:
    This routine is used to program a channel switch in the given time.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    timeSync          Initial channel switch time in us
    pch               Frequency channel number to be updated
    timeMode          Channel switch time mode

  Returns:
    None

  Example:
    <code>
    uint32_t timeSync = 10000
    uint16_t pch = 600;
    uint8_t timeMode = PAL_TX_MODE_ABSOLUTE;
    
    PAL_ProgramChannelSwitch(timeSync, pch, timeMode);
    </code>

  Remarks:
    Only available for PHY RF.
*/
void PAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetConfiguration(
        uint16_t id, 
        void *val, 
        uint16_t len,
        uint16_t pch)

  Summary:
    Get a PHY attribute. 

  Description:
    This function is used to get a PHY attribute from the selected medium.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    id              Identifiers requested from the MAC layer
    val             Output parameter value
    len             Length of the parameter
    pch             Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t id=PAL_ID_CFG_ATTENUATION;
    void val=0;
    uint16_t len=1;
    uint16_t pch = 16;

    result = PAL_GetConfiguration(id, &val, len, pch);
    </code>

  Remarks:
    None
*/
uint8_t PAL_GetConfiguration(uint16_t id, void *val, uint16_t len, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_SetConfiguration(
        uint16_t id, 
        void *val, 
        uint16_t len,
        uint16_t pch)

  Summary:
    Set PHY attribute.

  Description:
    This function is used to set a PHY attribute in the selected medium.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    id              PHY attribute identifier
    val             Input parameter value
    len             Length of the parameter
    pch             Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t id=PAL_ID_CFG_ATTENUATION;
    void val=2;
    uint16_t len=1;
    uint16_t pch = 16;

    result = PAL_SetConfiguration(id, &val, len, pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_SetConfiguration(uint16_t id, void *val, uint16_t len, uint16_t pch);

// ****************************************************************************
/* Function:
    uint16_t PAL_GetSignalCapture(
        uint8_t *noiseCapture,
        uint8_t mode,
        uint32_t timeStart,
        uint32_t duration)

  Summary:
    Get Capture Noise Data 

  Description:
    This routine is used to read noise data for PLC medium communication.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    noiseCapture    Pointer to destination buffer to store data
    mode            Capture mode
    timeStart       Start time in us based on PL360 timer reference
    duration        Duration time in us

  Returns:
    Size in bytes of data capture. 

  Example:
    <code>
    uint32_t timeStart = 10000;
    uint32_t duration = 5000;
    uint8_t noiseCapture[300];
    uint16_t noiseSize;
    uint8_t mode = PAL_MODE_TYPE_B;

    noiseSize = PAL_GetSignalCapture(&noiseCapture, mode, timeStart, duration);
    </code>

  Remarks:
    Only available for PHY PLC.
*/
uint16_t PAL_GetSignalCapture(uint8_t *noiseCapture, uint8_t mode, uint32_t timeStart, uint32_t duration);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetMsgDuration(
        uint16_t pch, 
        uint16_t msgLen, 
        uint8_t scheme, 
        uint8_t mode, 
        uint32_t *duration)

  Summary:
    Get message duration 

  Description:
    This function is used to calculate the message duration.    

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pch             Physical channel used
    msgLen          Message length
    scheme          Modulation scheme of message
    mode            Indicates if the message to transmit is type A, type B or type BC
    duration        Pointer to message duration in us (output)

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 

  Example:
    <code>
    uint32_t duration = 0;
    uint16_t pch = 16;
    uint16_t msgLen = 30;
    uint8_t scheme = PAL_PLC_DBPSK_R;
    uint8_t mode = PAL_MODE_TYPE_B;
    uint8_t result=PAL_CFG_SUCCESS;

    result = PAL_GetMsgDuration(pch, msgLen, scheme, mode, &duration);
    </code>

  Remarks:
    Not available for PHY serial.
*/
uint8_t PAL_GetMsgDuration(uint16_t pch, uint16_t msgLen, uint8_t scheme, uint8_t mode, uint32_t *duration); 

// ****************************************************************************
/* Function:
   bool PAL_HasMinimumQuality(
    uint16_t pch, 
    uint8_t scheme, 
    uint8_t lessRobustMode)

  Summary:
    Check minimum quality for modulation scheme

  Description:
    This routine is used to check if the modulation is good enough for a low FER
    (Frame Error rate) for the given scheme. 

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pch                 Physical channel used
    scheme              Modulation scheme of message
    lessRobustMode      Less robust modulation

  Returns:
    true         - If successful
    false        - If unsuccessful 

  Example:
    <code>
    uint16_t pch = 16;
    uint8_t scheme = PAL_PLC_DBPSK_R;
    uint8_t lessRobustMode = PAL_PLC_DQPSK;
    bool result=true;
    
    result = PAL_HasMinimumQuality(pch, scheme, lessRobustMode);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
bool PAL_HasMinimumQuality(uint16_t pch, uint8_t scheme, uint8_t lessRobustMode);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetLessRobustModulation(uint16_t pch, uint8_t mod1, uint8_t mod2)

  Summary:
    Get less robust modulation scheme.

  Description:
    This routine is used to get less robust modulation scheme for a selected 
    channel.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    pch             Physical channel used
    mod1            Modulation 1 to compare
    mod2            Modulation 2 to compare

  Returns:
   mod1 or mod2 scheme

  Example:
    <code>
    uint16_t pch=16;
    uint8_t mod;
    uint8_t mod1=PAL_PLC_DBPSK_R;
    uint8_t mod2=PAL_PLC_DQPSK;

    mod = PAL_GetLessRobustModulation(pch, mod1, mod2);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_GetLessRobustModulation(uint16_t pch, uint8_t mod1, uint8_t mod2);

#ifdef __cplusplus
}
#endif

#endif /* PAL_RF_H_INCLUDED */

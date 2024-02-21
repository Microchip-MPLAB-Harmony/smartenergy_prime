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

#ifndef PAL_H_INCLUDED
#define PAL_H_INCLUDED

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
    PAL PHY different time modes.

 Description:
    These time modes macros are used during PHY communication with MAC layer.

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
/* PAL config success result macros
 Summary:
    PAL config success results.

 Description:
    PAL config output results

 Remarks:
    None.
*/
#define PAL_CFG_SUCCESS              0
#define PAL_CFG_INVALID_INPUT        1

// *****************************************************************************
/* PAL TX Result Macros
 Summary:
    PAL Transmission results.

 Description:
    These transmission result macros are used as the return types for data 
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
/* Transmission Result (only RF): invalid Parameter */
#define PAL_TX_RESULT_INV_PARAM      20
/* Transmission result: error in tx */
#define PAL_TX_RESULT_PHY_ERROR      0xFE
/* Transmission result: not use physical layer and no transmission ongoing*/
#define PAL_TX_RESULT_PHY_UNUSED     0xFF

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PRIME mode type enumeration
 Summary:
    Physical PRIME Mode of transmission

 Description:
    This mode type will be used to indicate the transfer of data.

 Remarks:
    None.
*/
typedef enum 
{
    PAL_MODE_TYPE_A  = 0,
    PAL_MODE_TYPE_B  = 2,
    PAL_MODE_TYPE_BC = 3,
    PAL_MODE_TYPE_RF = 5,
    PAL_MODE_NOISE   = 0xFE,  /* Not in PL360 */
    PAL_MODE_TEST    = 0xFF,  /* Not in PL360 */
} PAL_PRIME_MODE_TYPE;

/* PAL medium enumeration
 Summary:
    Physical Layer Medium Identifier

 Description:
    The medium identifier is used to indicate in which physical layer the data
    is transmitted or received. 
    Upon transmission, the PHY Abstraction Layer forwards the PHY frame to the
    physical layer selected by the MAC layer.
    Upon reception, the PHY Abstraction Layer indicates to the MAC layer the 
    physical layer in which the PHY frame was received.

 Remarks:
    None.
*/
typedef enum 
{
    PAL_PHY_PLC     = 0,
    PAL_PHY_SERIAL  = 1,
    PAL_PHY_RF      = 2,
    PAL_PHY_UNKNOWN = 0xFF,
} PAL_PHY_LAYER_IDENTIFIER;

/* PAL PLC modulation scheme enumeration
 Summary:
    PLC Physical Scheme of modulation

 Description:
    Modulation scheme to be used for communicating with the selected node.

 Remarks:
    None.
*/
typedef enum  
{
	/* Differential Binary Phase Shift Keying Scheme Modulation */
	PAL_PLC_DBPSK        = 0x00,
	/* Differential Quaternary Phase Shift Keying Scheme Modulation */
	PAL_PLC_DQPSK        = 0x01,
	/* Differential 8-Phase Shift Keying Scheme Modulation */
	PAL_PLC_D8PSK        = 0x02,
	/* Differential Binary Phase Shift Keying Scheme Modulation with Convolutional Encoder */
	PAL_PLC_DBPSK_CC     = 0x04,
	/* Differential Quaternary Phase Shift Keying Scheme Modulation with Convolutional Encoder */
	PAL_PLC_DQPSK_CC     = 0x05,
	/* Differential 8-Phase Shift Keying Scheme Modulation with Convolutional Encoder*/
	PAL_PLC_D8PSK_CC     = 0x06,
	/* Robust Differential Binary Phase Shift Keying Scheme Modulation */
	PAL_PLC_DBPSK_R      = 0x0C,
	/* Robust Differential Quaternary Phase Shift Keying Scheme Modulation */
	PAL_PLC_DQPSK_R      = 0x0D,
} PAL_PLC_MODULATION_SCHEME;

/*PAL RF modulation scheme enumeration

 Summary:
    RF Physical Scheme of modulation

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

/* PAL PLC Information Base enumeration
 Summary:
    PHY parameter identifiers requested from the MAC layer

 Description:
    This list contains the PHY parameter identifiers to be used by the MAC 
    layer to access PHY parameters.

 Remarks:
    None.
*/
typedef enum 
{
    /* Enable/Disable continuous transmission mode in PLC */
    PAL_ID_CONTINUOUS_TX_EN,
    /* Zero Crossing Period in PLC */
    PAL_ID_ZC_PERIOD,
    /* Host Controller version identifier in PLC */
    PAL_ID_HOST_VERSION,
    /* Maximum Transmission/Reception Number of Channels in PLC */
    PAL_ID_CFG_MAX_TXRX_NUM_CHANNELS,
    /* Attenuation to be applied to every message */
    PAL_ID_CFG_ATTENUATION,
    /* List of available transmission/reception channels, Double combination in PLC */
    PAL_ID_CFG_TXRX_DOUBLE_CHANNEL_LIST,
    /* Product identifier */
    PAL_ID_INFO_VERSION,
    /* Enable/disable PHY Sniffer */
    PAL_ID_PHY_SNIFFER_EN,
    /* Flag to enable branch auto detection in PLC */
    PAL_ID_CFG_AUTODETECT_BRANCH,
    /* When branch auto detection disabled, indicate impedance to use in PLC */
    PAL_ID_CFG_IMPEDANCE,
    /* Transmission/Reception Channel */
    PAL_ID_CFG_TXRX_CHANNEL,
    /* List of available transmission/reception channels 
    (depends on coupling and band plan) in PLC */
    PAL_ID_CFG_TXRX_CHANNEL_LIST,
    /* RX Payload length in OFDM symbols */
    PAL_ID_RX_PAYLOAD_LEN_SYM,
    /* Duration of channel senses in CSMA RF */
    PAL_ID_CSMA_RF_SENSE_TIME,
    /* Duration of a unit backoff period in CSMA RF */
    PAL_ID_UNIT_BACKOFF_PERIOD,
    /* Network detection in PLC*/
    PAL_ID_NETWORK_DETECTION,
    /* Device information in PLC*/
    PAL_ID_INFO_DEVICE,
    /* Remaining duration of present frame in PLC */
    PAL_ID_REMAINING_FRAME_DURATION,
    /* Default scheme for RF */
    PAL_ID_RF_DEFAULT_SCHEME,
    /* Physical parameters for a received PLC message */
    PAL_ID_PLC_RX_PHY_PARAMS,
    /* RF channels used to generate the main hopping sequence */
    PAL_ID_RF_BITS_HOPPING_SEQUENCE,
    /* RF channels used to generate the beacon hopping sequence */
    PAL_ID_RF_BITS_BCN_HOPPING_SEQUENCE,
    /* Number of channels in the hopping sequence. */
    PAL_ID_RF_MAC_HOPPING_SEQUENCE_LENGTH,
    /* Number of channels in the Beacon hopping sequence */
    PAL_ID_RF_MAC_HOPPING_BCN_SEQUENCE_LENGTH,
    /* Number of supported RF channels */
    PAL_ID_RF_NUM_CHANNELS,
    /* The Maximum PSDU (PHY Service Data Unit) in Octets */
    PAL_ID_MAX_PHY_PACKET_SIZE,
    /* Turn arround time for the SUN (Smart Utility Network) FSK PHY layer */
    PAL_ID_TURNAROUND_TIME,
    /* The transmit power of the device in dBm */
    PAL_ID_PHY_TX_POWER,
    /* Status of FEC(Forward Error Correction) */
    PAL_ID_PHY_FSK_FEC_ENABLED,
    /* Status of the RSC (Recursive and Systematic Code) interleaving */
    PAL_ID_PHY_FSK_FEC_INTERLEAVING_RSC,
    /* FEC scheme */
    PAL_ID_PHY_FSK_FEC_SCHEME,
    /* Length of the preamble pattern in FSK */
    PAL_ID_PHY_FSK_PREAMBLE_LENGTH,
    /* Used group of SFDs (Start of Frame Delimiter) */
    PAL_ID_PHY_SUN_FSK_SFD,
    /* Status of PSDU (PHY Service Data Unit) data whitening */
    PAL_ID_PHY_FSK_SCRAMBLE_PSDU,
    /* The duration for CCA (Clear Channel Assessment) */
    PAL_ID_PHY_CCA_DURATION,
    /* Number of dB above the specified receiver sensitivity for the RF PHY layer */
    PAL_ID_PHY_CCA_THRESHOLD,
    /* RF band and operating mode */
    PAL_ID_RF_PHY_BAND_OPERATING_MODE,
} PAL_ATTRIBUTE_ID;

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

/* PAL Confirm data structure
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

/* PAL Indication data structure
 Summary:
    Data structure used for PHY data Indication

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

/* PAL  PLC PHY receiver data structure
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

/* PHY Attribute Layer Confirm data Transmission Function Pointer

  Summary:
    Callback used for confirm data transmission.
    
  Description:
    The confirm attribute is used  to tells the MAC layer if MAC Protocol Data
	Unit(MPDU) request has been successfully transmitted.

  Remarks:
    None.
*/
typedef void (*PAL_DATA_CONFIRM_CB)(PAL_MSG_CONFIRM_DATA *dataConfirm);

/* PHY Attribute Layer Indication Data Transmission Function Pointer

  Summary:
    Callback used for indication data transmission.

  Description:
    This callback is used to indicate the reception of a message to the MAC layer. 

  Remarks:
    None.
*/
typedef void (*PAL_DATA_INDICATION_CB)(PAL_MSG_INDICATION_DATA *dataIndication);

/* PHY Attribute Layer Switch RF Channel Function Pointer

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
/* PRIME PAL Handlers Data

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
    void

  Returns:
    void

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
    void

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
    uint8_t phySnr=0;
    uint8_t phyQt=5;
    uint16_t pch=16;

    result = PAL_GetSNR(&phySnr, phyQt, pch);
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
    uint8_t phyZct=0;
    uint16_t pch=16;

    result = PAL_GetZCT(&phyZct, pch);
    </code>

  Remarks:
    Not available for both PHY Serial and RF medium.
*/
uint8_t PAL_GetZCT(uint32_t *zct, uint16_t pch);  

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCurrentTime(uint8_t *timer, uint16_t pch)

  Summary:
    Get the current PHY time in us.

  Description:
    This routine is used to get current PHY time.

  Precondition:
    The PAL_Initialize function should have been called before calling this 
    function.

  Parameters:
    timer       Current output time  of PHY 
    pch         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful 
    
  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t phyTimer=0;
    uint16_t pch=16;

    result = PAL_GetCurrentTime(&phyTimer, pch);
    </code>

  Remarks:
    None
*/
uint8_t PAL_GetCurrentTime(uint32_t *timer, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCurrentTimeExtended(uint64_t *timer)

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
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t phyTimer=0;

    result = PAL_Extended_GetCurrentTime(&phyTimer);
    </code>

  Remarks:
    None
*/
uint8_t PAL_Extended_GetCurrentTime(uint64_t *pull_timer);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetCarrierDetect(
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
    uint8_t phyCd=0;
    uint8_t rssi=0;
    uint32_t time=0;
    uint8_t header=0;
    uint16_t pch=16;

    result = PAL_GetCarrierDetect(&phyCd, &rssi, &time, &header, pch);
    </code>

  Remarks:
    Not available for both PHY Serial and PHY RF.
*/
uint8_t PAL_GetCarrierDetect(uint8_t *cd, uint8_t *rssi, uint32_t *time, uint8_t *header, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetNoiseLevel(uint8_t *noise, uint16_t pch)

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
    uint8_t phyNl=0;
    uint16_t pch=16;

    result = PAL_GetNoiseLevel(&phyNl, pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_GetNoiseLevel(uint8_t *noise, uint16_t pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetAutoGainMode(uint8_t *mode, uint8_t *gain, uint16_t pch)

  Summary:
    Get the automatic gain mode of the PHY PLC layer. 

  Description:
    This routine is used to get Automatic Gain Mode (AGC) of the PHY PLC layer.
    The Medium pch (Physical channel characteristics) used by the PHY to 
    receive the PSDU. 
    PLC channels range from 1..511 (0x01..0x1FF)

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
    uint8_t phymode=0;
    uint8_t gain=0;
    uint16_t pch=16;

    result = PAL_GetAutoGainMode(&phymode, &gain, pch);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_GetAutoGainMode(uint8_t *mode, uint8_t *gain, uint16_t pch);  

// ****************************************************************************
/* Function:
    uint8_t PAL_SetAutoGainMode(uint8_t mode, uint8_t gain, uint16_t pch)

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
    uint8_t phymode=0;
    uint8_t gain=0;
    uint16_t pch=16;

    result = PAL_SetAutoGainMode(&phymode, &gain, pch);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_SetAutoGainMode(uint8_t mode, uint8_t gain, uint16_t pch);    

// ****************************************************************************
/* Function:
    uint8_t PAL_GetClearChannelState(uint8_t *channelState)

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

    result = PAL_GetNoiseLevel(&channelState);
    </code>

  Remarks:
    Only available for PHY RF.
*/
uint8_t PAL_GetClearChannelState(uint8_t *channelState);

// ****************************************************************************
/* Function:
    uint8_t PAL_GetChannel(uint16_t *pch)

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

    result = PAL_GetChannel(&pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_GetChannel(uint16_t *pch);

// ****************************************************************************
/* Function:
    uint8_t PAL_SetChannel(uint16_t pch)

  Summary:
    Get the band (PLC) or the channel (RF). 

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

    result = PAL_SetChannel(pch);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_SetChannel(uint16_t pch);

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
    requestMsg.pch = 5;
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
    void PAL_SetChannelRequest(uint32_t timeSync, uint16_t pch, uint8_t timeMode)

  Summary:
    Set frequency channel switch for PHY RF. 

  Description:
    This routine is used to set the frequency channel for the transmission 
    process of data over a RF medium.

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
    
    PAL_SetChannelRequest(timeSync, pch, timeMode);
    </code>

  Remarks:
    Only available for PHY RF.
*/
void PAL_SetChannelRequest(uint32_t timeSync, uint16_t pch, uint8_t timeMode);

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
    id              PHy attribute identifier
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
        uint8_t *dst,
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
    dst             Pointer to destination buffer to store data
    mode            Capture Mode
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

    noiseSize = PAL_GetSignalCapture(&dst, mode, timeStart, duration);
    </code>

  Remarks:
    Only available for PHY PLC.
*/
uint16_t PAL_GetSignalCapture(uint8_t *dst, uint8_t mode, uint32_t timeStart, uint32_t duration);

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
   bool PAL_RobustModulationQuality(
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
bool PAL_RobustModulationQuality(uint16_t pch, uint8_t scheme, uint8_t lessRobustMode);

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

#endif /* PAL_H_INCLUDED */

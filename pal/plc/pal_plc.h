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

#ifndef  PAL_PLC_H_INCLUDED
#define  PAL_PLC_H_INCLUDED

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "pal_types.h"
#include "pal_plc_local.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// // ****************************************************************************
// ****************************************************************************
// Section: Interface Routines
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    SYS_MODULE_OBJ PAL_PLC_Initialize(
        const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes PRIME PAL module.

  Description:
    This routine initializes the PAL module, making it ready for clients to
    use it. The initialization data is specified by the init parameter. It is a
    single instance module, so this function should be called only once.

  Precondition:
    None.

  Parameters:
    index - Identifier for the instance to be initialized. Only one instance
            (index 0) supported.
    init  - Pointer to the initialization data structure containing the data
            necessary to initialize the module.

  Returns:
    If successful, returns a valid handle to a module instance object.
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    int main()
    {
        PAL_PLC_Initialize();
    }
    </code>

  Remarks:
    None
*/
SYS_MODULE_OBJ PAL_PLC_Initialize(void);

// *************************************************************************
/* Function:
    SYS_STATUS PAL_PLC_Status(void)

  Summary:
    Gets the current status of the PAL PLC module

  Description:
    This routine provides the current status of the PAL PLC module.

  Preconditions:
    Function PAL_PLC_Initialize should have been called before calling
    this function.

  Parameters:
    None.

  Returns:
    see PAL_PLC_STATUS

  Example:
    <code>
    SYS_STATUS status;

    status = (PAL_PLC_STATUS)PAL_PLC_Status();
    </code>

  Remarks:
    None.
*/

SYS_STATUS PAL_PLC_Status(void);

// ****************************************************************************
/* Function:
    void PAL_PLC_Tasks(void)

  Summary:
    Maintains the PAL state machine.

  Description:
    This function is used to maintain the PAL internal state machine and
    generate callbacks.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    while (true)
    {
        PAL_PLC_Tasks();
    }
    </code>

  Remarks:
    None.
*/
void PAL_PLC_Tasks(void);

// ****************************************************************************
/* Function:
    void PAL_PLC_DataConfirmCallbackRegister(PAL_DATA_CONFIRM_CB callback)

  Summary:
    Sets PAL PLC callback function for PLC Data Confirm

  Description:
    This routine links callback functions between upper layer and phy layer.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    callback         Callback function to be called when PLC Data Confirm is rised

  Returns:
    None

  Example:
    <code>
    static void lPAL_PlcDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
    {
        ...
    }

    void main(void)
    {

        PAL_PLC_Initialize();

        PAL_PLC_DataConfirmCallbackRegister(lPAL_PlcDataConfirmCallback);
    }
    </code>

  Remarks:
    None
*/
void PAL_PLC_DataConfirmCallbackRegister(PAL_DATA_CONFIRM_CB callback);

// ****************************************************************************
/* Function:
    void PAL_PLC_DataIndicationCallbackRegister(PAL_DATA_INDICATION_CB callback)

  Summary:
    Sets PAL PLC callback function for PLC Data Confirm

  Description:
    This routine links callback functions between upper layer and phy layer.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    callback         Callback function to be called when PLC Data Confirm is rised

  Returns:
    None

  Example:
    <code>
    static void lPAL_PlcDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
    {
        ...
    }

    void main(void)
    {

        PAL_PLC_Initialize();

        PAL_PLC_DataIndicationCallbackRegister(lPAL_PlcDataIndicationCallback);
    }
    </code>

  Remarks:
    None
*/
void PAL_PLC_DataIndicationCallbackRegister(PAL_DATA_INDICATION_CB callback);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_DataRequest(PAL_MSG_REQUEST_DATA *pMessageData)

  Summary:
    Request to transmit a message

  Description:
    This functions is used to initiate the transmission process of a PPDU
    (PHY Protocol Data Unit) to the medium indicated in the transmission
    information structure.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    pMessageData         pointer to the data transmission structure

  Returns:
    PAL Transmission results.

  Example:
    <code>
    uint8_t result=PAL_TX_RESULT_SUCCESS;
    PAL_PLC_MSG_REQUEST_DATA requestMsg;
    uint8_t msg[30];

    requestMsg.dataBuf = &msg;
    requestMsg.timeDelay = 10000;
    requestMsg.dataLen = sizeof(msg);
    requestMsg.channelMask = 16;
    requestMsg.buffIdentifier = 2;
    requestMsg.attLevel = 0;
    requestMsg.scheme = PAL_PLC_DBPSK_R;
    requestMsg.disableRx = 0;
    requestMsg.mode = PAL_PLC_MODE_TYPE_B;
    requestMsg.timeMode = PAL_PLC_TX_MODE_ABSOLUTE;
    requestMsg.numSenses = 3;
    requestMsg.senseDelayMs = 3;

    result = PAL_PLC_DataRequest(&requestMsg);
    </code>

  Remarks:
    None
*/
uint8_t PAL_PLC_DataRequest(PAL_MSG_REQUEST_DATA *requestMsg);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetSNR(uint8_t *snr, uint8_t qt, uint16_t channelMask)

  Summary:
    Convert QT value to Signal Noise Ratio (SNR).

  Description:
    This function is used to get the value of the Signal to Noise Ratio,
    defined as the ratio of measured received signal level to noise level of
    last received PPDU (PHY Protocol Data Unit).

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    snr         Signal to noise ratio output parameter
    qt          QT input parameter to get SNR level
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t snr=0;
    uint8_t qt=5;
    uint16_t channelMask=16;

    result = PAL_PLC_GetSNR(&snr, qt, channelMask);
    </code>

  Remarks:
    Not available in PHY Serial medium
*/
uint8_t PAL_PLC_GetSNR(uint8_t *snr, uint8_t qt, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetZCT(uint8_t *zct, uint16_t channelMask)

  Summary:
    Get zero-cross time (ZCT).

  Description:
    This function is used to get the value of the zero-cross time of the mains
    and the time between the last transmission or reception and the zero cross
    of the mains.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    zct         Zero time output parameter
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t zct=0;
    uint16_t channelMask=16;

    result = PAL_PLC_GetZCT(&zct, channelMask);
    </code>

  Remarks:
    Not available for both PHY Serial and RF medium.
*/
uint8_t PAL_PLC_GetZCT(uint32_t *zct, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetTimer(uint32_t *timer, uint16_t channelMask)

  Summary:
    Get the current PHY time in us.

  Description:
    This routine is used to get current PHY time.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    timer       Current output time of PHY
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint32_t timer=0;
    uint16_t channelMask=16;

    result = PAL_PLC_GetTimer(&timer, channelMask);
    </code>

  Remarks:
    None
*/
uint8_t PAL_PLC_GetTimer(uint32_t *timer, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetTimerExtended(uint64_t *timer)

  Summary:
    Get the extended PHY time in us.

  Description:
    This routine is used to get the extended PHY time.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
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

    result = PAL_PLC_GetTimerExtended(&timer);
    </code>

  Remarks:
    None
*/
uint8_t PAL_PLC_GetTimerExtended(uint64_t *timer);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetCD(
        uint8_t *pCD,
        uint8_t *pRSSI,
        uint32_t *pTime,
        uint8_t *pHeader,
        uint16_t channelMask)

  Summary:
    Get the carrier detect signal.

  Description:
    This routine is used to get the value of carrier detect signal.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    pCD              Carrier detect signal output parameter
    pRSSI            Received signal strength indicator
    pTime            Current time in us
    pHeader          Header type
    channelMask      Physical channel mask

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
    uint16_t channelMask=16;

    result = PAL_PLC_GetCD(&cd, &rssi, &time, &header, channelMask);
    </code>

  Remarks:
    Not available for both PHY Serial and PHY RF.
*/
uint8_t PAL_PLC_GetCD(uint8_t *pCD, uint8_t *pRSSI, uint32_t *pTime, uint8_t *pHeader, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetNL(uint8_t *noise, uint16_t channelMask)

  Summary:
    Get the noise floor level value.

  Description:
    This routine is used to know the noise level present in the powerline.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    noise       Noise floor level output parameter
    channelMask         Medium used by the PHY to receive the PSDU

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t nl=0;
    uint16_t channelMask=16;

    result = PAL_PLC_GetNL(&nl, channelMask);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_PLC_GetNL(uint8_t *noise, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetAGC(uint8_t *mode, uint8_t *gain, uint16_t channelMask)

  Summary:
    Get the automatic gain mode of the PHY PLC layer.

  Description:
    This routine is used to get Automatic Gain Mode (AGC) of the PHY PLC layer.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    mode        Auto/Manual mode
    gain        Initial receiving gain in auto mode
    channelMask         Medium used by the PHY to receive the PSDU

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t mode=0;
    uint8_t gain=0;
    uint16_t channelMask=16;

    result = PAL_PLC_GetAGC(&mode, &gain, channelMask);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_PLC_GetAGC(uint8_t *mode, uint8_t *gain, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_SetAGC(uint8_t mode, uint8_t gain, uint16_t channelMask)

  Summary:
    Set the automatic gain mode of the PHY PLC layer.

  Description:
    This routine is used to set Automatic Gain Mode (AGC) of the PHY PLC layer.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    mode        Auto/Manual mode (auto mode(0), manual mode(1))
    gain        Initial receiving gain in auto mode
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint8_t mode=0;
    uint8_t gain=0;
    uint16_t channelMask=16;

    result = PAL_PLC_SetAGC(mode, gain, channelMask);
    </code>

  Remarks:
    Not available for PHY Serial and PHY RF.
*/
uint8_t PAL_PLC_SetAGC(uint8_t mode, uint8_t gain, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetCCA(uint8_t *channelState)

  Summary:
    Get clear channel assessment mode value.

  Description:
    This routine is used to get the clear channel assesment mode.
    The channel state helps to know whether or not the RF physical medium is
    free.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
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

    result = PAL_PLC_GetCCA(&channelState);
    </code>

  Remarks:
    Only available for PHY RF.
*/
uint8_t PAL_PLC_GetCCA(uint8_t *channelState);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetChannel(uint16_t *channelMask)

  Summary:
    Get the band (PLC) or the channel (RF).

  Description:
    This routine is used to get the channel or band used for the communication.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t channelMask=0;

    result = PAL_PLC_GetChannel(&channelMask);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_PLC_GetChannel(uint16_t *channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_SetChannel(uint16_t channelMask)

  Summary:
    Set the band (PLC) or the channel (RF).

  Description:
    This routine is used to set the channel or band used for the communication.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    channelMask         Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t channelMask=16;

    result = PAL_PLC_SetChannel(channelMask);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_PLC_SetChannel(uint16_t channelMask);

// ****************************************************************************
/* Function:
    void PAL_PLC_ProgramChannelSwitch(uint32_t timeSync, uint16_t channelMask, uint8_t timeMode)

  Summary:
    Program a channel switch in the given time.

  Description:
    This routine is used to program a channel switch in the given time.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    timeSync          Initial channel switch time in us
    channelMask               Frequency channel number to be updated
    timeMode          Channel switch time mode

  Returns:
    None

  Example:
    <code>
    uint32_t timeSync = 10000
    uint16_t channelMask = 600;
    uint8_t timeMode = PAL_PLC_TX_MODE_ABSOLUTE;

    PAL_PLC_ProgramChannelSwitch(timeSync, channelMask, timeMode);
    </code>

  Remarks:
    Only available for PHY RF.
*/
void PAL_PLC_ProgramChannelSwitch(uint32_t timeSync, uint16_t channelMask, uint8_t timeMode);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetConfiguration(
        uint16_t id,
        void *val,
        uint16_t len,
        uint16_t channelMask)

  Summary:
    Get a PHY attribute.

  Description:
    This function is used to get a PHY attribute from the selected medium.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    id              Identifiers requested from the MAC layer
    val             Output parameter value
    len             Length of the parameter
    channelMask             Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t id=PAL_ID_CFG_ATTENUATION;
    void val=0;
    uint16_t len=1;
    uint16_t channelMask = 16;

    result = PAL_PLC_GetConfiguration(id, &val, len, channelMask);
    </code>

  Remarks:
    None
*/
uint8_t PAL_PLC_GetConfiguration(uint16_t id, void *val, uint16_t len, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_SetConfiguration(
        uint16_t id,
        void *val,
        uint16_t len,
        uint16_t channelMask)

  Summary:
    Set PHY attribute.

  Description:
    This function is used to set a PHY attribute in the selected medium.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    id              PHY attribute identifier
    val             Input parameter value
    len             Length of the parameter
    channelMask             Physical channel

  Returns:
    PAL_CFG_SUCCESS         - If successful
    PAL_CFG_INVALID_INPUT   - If unsuccessful

  Example:
    <code>
    uint8_t result=PAL_CFG_SUCCESS;
    uint16_t id=PAL_ID_CFG_ATTENUATION;
    void val=2;
    uint16_t len=1;
    uint16_t channelMask = 16;

    result = PAL_PLC_SetConfiguration(id, &val, len, channelMask);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_PLC_SetConfiguration(uint16_t id, void *val, uint16_t len, uint16_t channelMask);

// ****************************************************************************
/* Function:
    uint16_t PAL_PLC_GetSignalCapture(
        uint8_t *noiseCapture,
        uint8_t mode,
        uint32_t timeStart,
        uint32_t duration)

  Summary:
    Get Capture Noise Data

  Description:
    This routine is used to read noise data for PLC medium communication.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
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
    uint8_t mode = PAL_PLC_MODE_TYPE_B;

    noiseSize = PAL_PLC_GetSignalCapture(&noiseCapture, mode, timeStart, duration);
    </code>

  Remarks:
    Only available for PHY PLC.
*/
uint16_t PAL_PLC_GetSignalCapture(uint8_t *noiseCapture, uint8_t mode, uint32_t timeStart, uint32_t duration);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetMsgDuration(
        uint16_t channelMask,
        uint16_t msgLen,
        uint8_t scheme,
        uint8_t mode,
        uint32_t *duration)

  Summary:
    Get message duration

  Description:
    This function is used to calculate the message duration.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    channelMask             Physical channel used
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
    uint16_t channelMask = 16;
    uint16_t msgLen = 30;
    uint8_t scheme = PAL_PLC_DBPSK_R;
    uint8_t mode = PAL_PLC_MODE_TYPE_B;
    uint8_t result=PAL_CFG_SUCCESS;

    result = PAL_PLC_GetMsgDuration(channelMask, msgLen, scheme, mode, &duration);
    </code>

  Remarks:
    Not available for PHY serial.
*/
uint8_t PAL_PLC_GetMsgDuration(uint16_t channelMask, uint16_t msgLen, uint8_t scheme, uint8_t mode, uint32_t *duration);

// ****************************************************************************
/* Function:
   bool PAL_PLC_HasMinimumQuality(
    uint16_t channelMask,
    uint8_t scheme,
    uint8_t lessRobustMode)

  Summary:
    Check minimum quality for modulation scheme

  Description:
    This routine is used to check if the modulation is good enough for a low FER
    (Frame Error rate) for the given scheme.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    channelMask                 Physical channel used
    scheme              Modulation scheme of message
    lessRobustMode      Less robust modulation

  Returns:
    true         - If successful
    false        - If unsuccessful

  Example:
    <code>
    uint16_t channelMask = 16;
    uint8_t scheme = PAL_PLC_DBPSK_R;
    uint8_t lessRobustMode = PAL_PLC_DQPSK;
    bool result=true;

    result = PAL_PLC_HasMinimumQuality(channelMask, scheme, lessRobustMode);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
bool PAL_PLC_HasMinimumQuality(uint16_t channelMask, uint8_t scheme, uint8_t lessRobustMode);

// ****************************************************************************
/* Function:
    uint8_t PAL_PLC_GetLessRobustModulation(uint16_t channelMask, uint8_t mod1, uint8_t mod2)

  Summary:
    Get less robust modulation scheme.

  Description:
    This routine is used to get less robust modulation scheme for a selected
    channel.

  Precondition:
    The PAL_PLC_Initialize function should have been called before calling this
    function.

  Parameters:
    channelMask             Physical channel used
    mod1            Modulation 1 to compare
    mod2            Modulation 2 to compare

  Returns:
   mod1 or mod2 scheme

  Example:
    <code>
    uint16_t channelMask=16;
    uint8_t mod;
    uint8_t mod1=PAL_PLC_DBPSK_R;
    uint8_t mod2=PAL_PLC_DQPSK;

    mod = PAL_PLC_GetLessRobustModulation(channelMask, mod1, mod2);
    </code>

  Remarks:
    Not available for PHY Serial.
*/
uint8_t PAL_PLC_GetLessRobustModulation(uint16_t channelMask, uint8_t mod1, uint8_t mod2);

#ifdef __cplusplus
}
#endif

#endif /*  PAL_PLC_H_INCLUDED */

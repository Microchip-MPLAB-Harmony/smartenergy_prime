/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    pal.c

  Summary:
    Platform Abstraction Layer (PAL) Interface source file.

  Description:
    This module provides the interface between the PRIME MAC layer and the
    PLC physical layer.
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

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "configuration.h"
#include "pal.h"
#include "pal_types.h"
#include "pal_local.h"
<#if PRIME_PAL_PLC_EN == true>
#include "pal_plc.h"
</#if>
<#if PRIME_PAL_RF_EN == true>
#include "pal_rh.h"
</#if>
<#if PRIME_PAL_SERIAL_EN == true>
#include "pal_serial.h"
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Global data
// *****************************************************************************
// *****************************************************************************

static PAL_DATA palData;

<#if PRIME_PAL_PLC_EN == true>
extern const PAL_INTERFACE PAL_PLC_Interface;
</#if>
<#if PRIME_PAL_RF_EN == true>
extern const PAL_INTERFACE PAL_RF_Interface;
</#if>
<#if PRIME_PAL_SERIAL_EN == true>
extern const PAL_INTERFACE PAL_SERIAL_Interface;
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
<#if PRIME_PAL_PLC_EN == true>
static void lPAL_PlcDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if (palData.dataConfirmCallback) {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_PlcDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if (palData.dataIndicationCallback) {
        palData.dataIndicationCallback(pData);
    }
}

</#if>
<#if PRIME_PAL_RF_EN == true>
static void lPAL_RfDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if (palData.dataConfirmCallback) {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_RfDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if (palData.dataIndicationCallback) {
        palData.dataIndicationCallback(pData);
    }
}

</#if>
<#if PRIME_PAL_SERIAL_EN == true>
static void lPAL_SerialDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if (palData.dataConfirmCallback) {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_SerialDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if (palData.dataIndicationCallback) {
        palData.dataIndicationCallback(pData);
    }
}

</#if>
static PAL_INTERFACE * lPAL_GetInterface(uint16_t channelMask)
{
<#if PRIME_PAL_PLC_EN == true && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == false>
    (void)channelMask;
    return (PAL_INTERFACE *)&PAL_PLC_Interface;
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == true && PRIME_PAL_SERIAL_EN == false>
    (void)channelMask;
    return (PAL_INTERFACE *)&PAL_RF_Interface;
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == true>
    (void)channelMask;
    return (PAL_INTERFACE *)&PAL_SERIAL_Interface;
<#else>
    if (channelMask < 512)
    {
    return (PAL_INTERFACE *)&PAL_PLC_Interface;
    }
    else if (channelMask < 1024)
    {
    return (PAL_INTERFACE *)&PAL_RF_Interface;
    }
    else
    {
    return (PAL_INTERFACE *)&PAL_SERIAL_Interface;
    }
</#if>
}

// *****************************************************************************
// *****************************************************************************
// Section: PAL Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ PAL_Initialize(const SYS_MODULE_INDEX index)
{
    if (index != PRIME_PAL_INDEX)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

<#if PRIME_PAL_PLC_EN == true>
    if (PAL_PLC_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    PAL_PLC_DataConfirmCallbackRegister(lPAL_PlcDataConfirmCallback);
    PAL_PLC_DataIndicationCallbackRegister(lPAL_PlcDataIndicationCallback);

</#if>
<#if PRIME_PAL_RF_EN == true>
    if (PAL_RF_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

</#if>
<#if PRIME_PAL_SERIAL_EN == true>
    if (PAL_SERIAL_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

</#if>
    return (SYS_MODULE_OBJ)PRIME_PAL_INDEX;
}

void PAL_Tasks(SYS_MODULE_OBJ object)
{
    if (object != PRIME_PAL_INDEX)
    {
        return;
    }

<#if PRIME_PAL_PLC_EN == true>
    PAL_PLC_Tasks();

</#if>
<#if PRIME_PAL_RF_EN == true>
    PAL_RF_Tasks();

</#if>
<#if PRIME_PAL_SERIAL_EN == true>
    PAL_SERIAL_Tasks();

</#if>
}

void PAL_CallbackRegister(PAL_CALLBACKS *pCallbacks)
{
    palData.dataConfirmCallback = pCallbacks->dataConfirm;
    palData.dataIndicationCallback = pCallbacks->dataIndication;
}

uint8_t PAL_DataRequest(PAL_MSG_REQUEST_DATA *pData)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pData->channelMask);
    return palIface->PAL_DataRequest(pData);
}

uint8_t PAL_GetSNR(uint16_t channelMask, uint8_t *snr, uint8_t qt)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetSNR(snr, qt, channelMask);
}

uint8_t PAL_GetZCT(uint16_t channelMask, uint32_t *zct)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetZCT(zct, channelMask);
}

uint8_t PAL_GetTimer(uint16_t channelMask, uint32_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetTimer(timer, channelMask);
}

uint8_t PAL_GetTimerExtended(uint16_t channelMask, uint64_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetTimerExtended(timer);
}

uint8_t PAL_GetCD(uint16_t channelMask, uint8_t *cd, uint8_t *rssi, uint32_t *time, uint8_t *header)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetCD(cd, rssi, time, header, channelMask);
}

uint8_t PAL_GetNL(uint16_t channelMask, uint8_t *noise)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetNL(noise, channelMask);
}

uint8_t PAL_GetAGC(uint16_t channelMask, uint8_t *mode, uint8_t *gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetAGC(mode, gain, channelMask);
}

uint8_t PAL_SetAGC(uint16_t channelMask, uint8_t mode, uint8_t gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_SetAGC(mode, gain, channelMask);
}

uint8_t PAL_GetCCA(uint16_t channelMask, uint8_t *channelMaskState)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetCCA(channelMaskState);
}

uint8_t PAL_GetChannel(uint16_t *channelMask, uint16_t channelReference)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelReference);
    return palIface->PAL_GetChannel(channelMask);
}

uint8_t PAL_SetChannel(uint16_t channelMask)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_SetChannel(channelMask);
}

void PAL_ProgramChannelSwitch(uint16_t channelMask, uint32_t timeSync, uint8_t timeMode)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    palIface->PAL_ProgramChannelSwitch(timeSync, channelMask, timeMode);
}

uint8_t PAL_GetConfiguration(uint16_t channelMask, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetConfiguration(id, val, length, channelMask);
}

uint8_t PAL_SetConfiguration(uint16_t channelMask, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_SetConfiguration(id, val, length, channelMask);
}

uint16_t PAL_GetSignalCapture(uint16_t channelMask, uint8_t *noiseCapture, uint8_t mode, 
                              uint32_t timeStart, uint32_t duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetSignalCapture(noiseCapture, mode, timeStart, duration);
}

uint8_t PAL_GetMsgDuration(uint16_t channelMask, uint16_t length, uint8_t scheme, uint8_t mode, uint32_t *duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetMsgDuration(channelMask, length, scheme, mode, duration);
}

bool PAL_CheckMinimumQuality(uint16_t channelMask, uint8_t reference, uint8_t modulation)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_CheckMinimumQuality(reference, modulation);
}

uint8_t PAL_GetLessRobustModulation(uint16_t channelMask, uint8_t mod1, uint8_t mod2)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelMask);
    return palIface->PAL_GetLessRobustModulation(mod1, mod2);
}

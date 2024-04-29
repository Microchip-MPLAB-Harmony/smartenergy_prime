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
#include "pal_rf.h"
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

<#if PRIME_PAL_RF_FREQ_HOPPING == true>
static void lPAL_RfCHannelSwitchCallback(PAL_PCH pch)
{
    if (palData.channelSwitchCallback) {
        palData.channelSwitchCallback(pch);
    }
}

</#if>
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
<#if PRIME_PAL_PHY_SNIFFER == true>
static void lPAL_PhySnifferCallback(uint8_t *pData, uint16_t length)
{
    SRV_USI_Send_Message(palData.usiHandler, SRV_USI_PROT_ID_SNIF_PRIME, 
            pData, length);
}

</#if>
static PAL_INTERFACE * lPAL_GetInterface(PAL_PCH pch)
{
<#if PRIME_PAL_PLC_EN == true && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == false>
    (void)pch;
    return (PAL_INTERFACE *)&PAL_PLC_Interface;
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == true && PRIME_PAL_SERIAL_EN == false>
    (void)pch;
    return (PAL_INTERFACE *)&PAL_RF_Interface;
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == true>
    (void)pch;
    return (PAL_INTERFACE *)&PAL_SERIAL_Interface;
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == false>
    #warning PAL interface is not defined. Please, review PRIME PAL interface configuration in MCC.
<#else>
  <#if PRIME_PAL_PLC_EN == true>
    if (pch < PAL_RF_CHN)
    {
        return (PAL_INTERFACE *)&PAL_PLC_Interface;
    }

  </#if>
  <#if PRIME_PAL_RF_EN == true>
    if (pch < PAL_SERIAL_CHN)
    {
        return (PAL_INTERFACE *)&PAL_RF_Interface;
    }

  </#if>
  <#if PRIME_PAL_SERIAL_EN == true>
    if (pch == PAL_SERIAL_CHN)
    {
        return (PAL_INTERFACE *)&PAL_SERIAL_Interface;
    }

  </#if>
    return NULL;
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

<#if PRIME_PAL_PHY_SNIFFER == true>
    /* Open USI */
    palData.usiHandler = SRV_USI_Open(PRIME_PAL_USI_INSTANCE);
    if (palData.usiHandler == DRV_HANDLE_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Register USI callback : TBD - Tiene que recibir los comandos de Prime Sniffer?? */
    // SRV_USI_CallbackRegister(palData.usiHandler,
    //         SRV_USI_PROT_ID_SNIF_PRIME, _APP_UsiSnifferEventCb);


</#if>
<#if PRIME_PAL_PLC_EN == true>
    if (PAL_PLC_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    PAL_PLC_DataConfirmCallbackRegister(lPAL_PlcDataConfirmCallback);
    PAL_PLC_DataIndicationCallbackRegister(lPAL_PlcDataIndicationCallback);

  <#if PRIME_PAL_PHY_SNIFFER == true>
    /* Register PLC PHY Sniffer callback */
    PAL_PLC_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>

</#if>
<#if PRIME_PAL_RF_EN == true>
    if (PAL_RF_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    PAL_RF_DataConfirmCallbackRegister(lPAL_RfDataConfirmCallback);
    PAL_RF_DataIndicationCallbackRegister(lPAL_RfDataIndicationCallback);
  <#if PRIME_PAL_RF_FREQ_HOPPING == true>
    PAL_RF_ChannelSwitchCallbackRegister(lPAL_RfCHannelSwitchCallback);
  </#if>

  <#if PRIME_PAL_PHY_SNIFFER == true>
    /* Register RF PHY Sniffer callback */
    PAL_RF_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>

</#if>
<#if PRIME_PAL_SERIAL_EN == true>
    if (PAL_SERIAL_Initialize() == SYS_MODULE_OBJ_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    PAL_SERIAL_DataConfirmCallbackRegister(lPAL_SerialDataConfirmCallback);
    PAL_SERIAL_DataIndicationCallbackRegister(lPAL_SerialDataIndicationCallback);

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
    PAL_INTERFACE *palIface = lPAL_GetInterface(pData->pch);
    return palIface->PAL_DataRequest(pData);
}

uint8_t PAL_GetSNR(PAL_PCH pch, uint8_t *snr, uint8_t qt)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetSNR(snr, qt);
}

uint8_t PAL_GetZCT(PAL_PCH pch, uint32_t *zct)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetZCT(zct);
}

uint8_t PAL_GetTimer(PAL_PCH pch, uint32_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetTimer(timer);
}

uint8_t PAL_GetTimerExtended(PAL_PCH pch, uint64_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetTimerExtended(timer);
}

uint8_t PAL_GetCD(PAL_PCH pch, uint8_t *cd, uint8_t *rssi, uint32_t *time, uint8_t *header)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetCD(cd, rssi, time, header);
}

uint8_t PAL_GetNL(PAL_PCH pch, uint8_t *noise)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetNL(noise);
}

uint8_t PAL_GetAGC(PAL_PCH pch, uint8_t *frameType, uint8_t *gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetAGC(frameType, gain);
}

uint8_t PAL_SetAGC(PAL_PCH pch, PAL_FRAME frameType, uint8_t gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_SetAGC(frameType, gain);
}

uint8_t PAL_GetCCA(PAL_PCH pch, uint8_t *pState)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetCCA(pState);
}

uint8_t PAL_GetChannel(PAL_PCH *pPch, PAL_PCH channelReference)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelReference);
    return palIface->PAL_GetChannel(pPch);
}

uint8_t PAL_SetChannel(PAL_PCH pch)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_SetChannel(pch);
}

void PAL_ProgramChannelSwitch(PAL_PCH pch, uint32_t timeSync, uint8_t timeMode)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    palIface->PAL_ProgramChannelSwitch(timeSync, pch, timeMode);
}

uint8_t PAL_GetConfiguration(PAL_PCH pch, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetConfiguration(id, val, length);
}

uint8_t PAL_SetConfiguration(PAL_PCH pch, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_SetConfiguration(id, val, length);
}

uint16_t PAL_GetSignalCapture(PAL_PCH pch, uint8_t *noiseCapture, PAL_FRAME frameType, 
                              uint32_t timeStart, uint32_t duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetSignalCapture(noiseCapture, frameType, timeStart, duration);
}

uint8_t PAL_GetMsgDuration(PAL_PCH pch, uint16_t length, PAL_SCHEME scheme, PAL_FRAME frameType, uint32_t *duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetMsgDuration(length, scheme, frameType, duration);
}

bool PAL_CheckMinimumQuality(PAL_PCH pch, uint8_t reference, uint8_t modulation)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_CheckMinimumQuality(reference, modulation);
}

uint8_t PAL_GetLessRobustModulation(PAL_PCH pch, uint8_t mod1, uint8_t mod2)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return palIface->PAL_GetLessRobustModulation(mod1, mod2);
}

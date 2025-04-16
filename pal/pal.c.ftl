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
<#if PRIME_PAL_PHY_SNIFFER == true>
  <#if (srv_psniffer.SRV_PSNF_PLC_PROFILE)??>
#include "service/psniffer/srv_psniffer.h"
  </#if>
  <#if (srv_rsniffer.SRV_RSNF_PROTOCOL)??>
#include "service/rsniffer/srv_rsniffer.h"
  </#if>
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Global data
// *****************************************************************************
// *****************************************************************************

static PAL_DATA palData;

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
<#if PRIME_PAL_PLC_EN == true>
static void lPAL_PlcDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if((palData.dataConfirmCallback) != NULL)
    {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_PlcDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if((palData.dataIndicationCallback) != NULL)
    {
        palData.dataIndicationCallback(pData);
    }
}

</#if>
<#if PRIME_PAL_RF_EN == true>
static void lPAL_RfDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if((palData.dataConfirmCallback) != NULL)
    {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_RfDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if((palData.dataIndicationCallback) != NULL)
    {
        palData.dataIndicationCallback(pData);
    }
}

<#if PRIME_PAL_RF_FREQ_HOPPING == true>
static void lPAL_RfCHannelSwitchCallback(uint16_t pch)
{
    if ((palData.channelSwitchCallback) != NULL)
    {
        palData.channelSwitchCallback(pch);
    }
}

</#if>
</#if>
<#if PRIME_PAL_SERIAL_EN == true>
static void lPAL_SerialDataConfirmCallback(PAL_MSG_CONFIRM_DATA *pData)
{
    if((palData.dataConfirmCallback) != NULL)
    {
        palData.dataConfirmCallback(pData);
    }
}

static void lPAL_SerialDataIndicationCallback(PAL_MSG_INDICATION_DATA *pData)
{
    if((palData.dataIndicationCallback) != NULL)
    {
        palData.dataIndicationCallback(pData);
    }
}

</#if>
<#if PRIME_PAL_PHY_SNIFFER == true>
static void lPAL_PhySnifferCallback(uint8_t *pData, uint16_t length)
{
    if(palData.snifferEnabled > 0U)
    {
        (void)SRV_USI_Send_Message(palData.usiHandler, SRV_USI_PROT_ID_SNIF_PRIME,
                pData, length);
    }
}

static void lPAL_UsiSnifferEventCb(uint8_t *pData, size_t length)
{
    uint8_t command;

    /* Protection for invalid length */
    if(length == 0U)
    {
        return;
    }

    /* Process received command */
    command = *pData;

    switch(command)
    {
<#if (srv_psniffer.SRV_PSNF_PLC_PROFILE)??>
        case (uint8_t)SRV_PSNIFFER_CMD_SET_PLC_CHANNEL:
        {
            uint8_t channel;

            channel = *(pData + 1);
            (void)PAL_SetConfiguration(1, (uint16_t)PLC_ID_CHANNEL_CFG, &channel, 1);
            break;
        }
</#if>

<#if (srv_rsniffer.SRV_RSNF_PROTOCOL)??>
        case (uint8_t)SRV_RSNIFFER_CMD_SET_RF_BAND_OPM_CHANNEL:
        {
            uint16_t rfBandOpMode, rfChannel;
            uint16_t rfPhyChannel=0U;

            /* Parse Band, Operating Mode and Channel parameters */
            SRV_RSNIFFER_ParseConfigCommand(pData, &rfBandOpMode, &rfChannel);

            /* Set configuration in RF PHY */
            (void)PAL_SetConfiguration(PRIME_PAL_RF_CHN_MASK, (uint16_t)PAL_ID_RF_PHY_BAND_OPERATING_MODE, &rfBandOpMode, 2U);

            rfPhyChannel = (uint16_t)(PRIME_PAL_RF_CHN_MASK | rfChannel);

            (void)PAL_SetChannel(rfPhyChannel);

            break;
        }
</#if>

        default:
            /* Do Nothing */
            break;
    }
}

</#if>

static __inline__ PAL_INTERFACE* __attribute__((always_inline)) lPAL_PointerToPalInterface(void const * pParam)
{
    union
    {
        const void* pPtr;
        PAL_INTERFACE * pPalIntf;
    }PAL_INTF_UNION;

    PAL_INTF_UNION.pPtr = pParam;
    return PAL_INTF_UNION.pPalIntf;
}

static PAL_INTERFACE * lPAL_GetInterface(uint16_t pch)
{
<#if PRIME_PAL_PLC_EN == true && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == false>
    (void)pch;
    return lPAL_PointerToPalInterface(&PAL_PLC_Interface);
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == true && PRIME_PAL_SERIAL_EN == false>
    (void)pch;
    return lPAL_PointerToPalInterface(&PAL_RF_Interface);
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == true>
    (void)pch;
    return lPAL_PointerToPalInterface(&PAL_SERIAL_Interface);
<#elseif PRIME_PAL_PLC_EN == false && PRIME_PAL_RF_EN == false && PRIME_PAL_SERIAL_EN == false>
    #warning PAL interface is not defined. Please, review PRIME PAL interface configuration in MCC.
<#else>
  <#if PRIME_PAL_PLC_EN == true>
    if (pch < PRIME_PAL_RF_CHN_MASK)
    {
        return lPAL_PointerToPalInterface(&PAL_PLC_Interface);
    }

  </#if>
  <#if PRIME_PAL_RF_EN == true>
    if (pch < PRIME_PAL_SERIAL_CHN_MASK)
    {
        return lPAL_PointerToPalInterface(&PAL_RF_Interface);
    }

  </#if>
  <#if PRIME_PAL_SERIAL_EN == true>
    if (pch == PRIME_PAL_SERIAL_CHN_MASK)
    {
        return lPAL_PointerToPalInterface(&PAL_SERIAL_Interface);
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

    palData.snifferEnabled = 0;

<#if PRIME_PAL_PHY_SNIFFER == true>
    /* Open USI */
    palData.usiHandler = SRV_USI_Open(PRIME_PAL_USI_INSTANCE);
    if (palData.usiHandler == DRV_HANDLE_INVALID)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Register USI callback */
    SRV_USI_CallbackRegister(palData.usiHandler,
            SRV_USI_PROT_ID_SNIF_PRIME, lPAL_UsiSnifferEventCb);


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

  <#if PRIME_PAL_PHY_SNIFFER == true>
    /* Register RF PHY Sniffer callback */
    PAL_SERIAL_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>

</#if>
    return (SYS_MODULE_OBJ)PRIME_PAL_INDEX;
}

void PAL_Enable(SYS_MODULE_OBJ object, uint8_t enablePAL)
{
    if (object != PRIME_PAL_INDEX)
    {
        return;
    }

<#if PRIME_PAL_PLC_EN == true>
    if ((enablePAL & PAL_PLC_EN) == 0U)
    {
        /* Remove callbacks */
        PAL_PLC_DataConfirmCallbackRegister(NULL);
        PAL_PLC_DataIndicationCallbackRegister(NULL);

  <#if PRIME_PAL_PHY_SNIFFER == true>
        PAL_PLC_USISnifferCallbackRegister(palData.usiHandler, NULL);
  </#if>
    }
    else
    {
        /* Register callbacks */
        PAL_PLC_DataConfirmCallbackRegister(lPAL_PlcDataConfirmCallback);
        PAL_PLC_DataIndicationCallbackRegister(lPAL_PlcDataIndicationCallback);

  <#if PRIME_PAL_PHY_SNIFFER == true>
        /* Register PLC PHY Sniffer callback */
        PAL_PLC_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>
    }
</#if>

<#if PRIME_PAL_RF_EN == true>
    if ((enablePAL & PAL_RF_EN) == 0U)
    {
        /* Remove callbacks */
        PAL_RF_DataConfirmCallbackRegister(NULL);
        PAL_RF_DataIndicationCallbackRegister(NULL);
  <#if PRIME_PAL_RF_FREQ_HOPPING == true>
        PAL_RF_ChannelSwitchCallbackRegister(NULL);
  </#if>

  <#if PRIME_PAL_PHY_SNIFFER == true>
        PAL_RF_USISnifferCallbackRegister(palData.usiHandler, NULL);
  </#if>
    }
    else
    {
        /* Register callbacks */
        PAL_RF_DataConfirmCallbackRegister(lPAL_RfDataConfirmCallback);
        PAL_RF_DataIndicationCallbackRegister(lPAL_RfDataIndicationCallback);
  <#if PRIME_PAL_RF_FREQ_HOPPING == true>
        PAL_RF_ChannelSwitchCallbackRegister(lPAL_RfCHannelSwitchCallback);
  </#if>

  <#if PRIME_PAL_PHY_SNIFFER == true>
        /* Register RF PHY Sniffer callback */
        PAL_RF_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>
    }
</#if>

<#if PRIME_PAL_SERIAL_EN == true>
    if ((enablePAL & PAL_SERIAL_EN) == 0U)
    {
        /* Remove callbacks */
        PAL_SERIAL_DataConfirmCallbackRegister(NULL);
        PAL_SERIAL_DataIndicationCallbackRegister(NULL);

  <#if PRIME_PAL_PHY_SNIFFER == true>
        PAL_SERIAL_USISnifferCallbackRegister(palData.usiHandler, NULL);
  </#if>
    }
    else
    {
        /* Register callbacks */
        PAL_SERIAL_DataConfirmCallbackRegister(lPAL_SerialDataConfirmCallback);
        PAL_SERIAL_DataIndicationCallbackRegister(lPAL_SerialDataIndicationCallback);

  <#if PRIME_PAL_PHY_SNIFFER == true>
        /* Register RF PHY Sniffer callback */
        PAL_SERIAL_USISnifferCallbackRegister(palData.usiHandler, lPAL_PhySnifferCallback);
  </#if>
    }
</#if>
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

SYS_STATUS PAL_Status(SYS_MODULE_OBJ object)
{
    if (object != PRIME_PAL_INDEX)
    {
        return SYS_STATUS_ERROR;
    }

<#if PRIME_PAL_PLC_EN == true>
    SYS_STATUS plcStatus = PAL_PLC_Status();
</#if>
<#if PRIME_PAL_RF_EN == true>
    SYS_STATUS rfStatus = PAL_RF_Status();
</#if>
<#if PRIME_PAL_SERIAL_EN == true>
    SYS_STATUS serialStatus = PAL_SERIAL_Status();
</#if>

<#if (PRIME_PAL_PLC_EN == true) && (PRIME_PAL_RF_EN == false) && (PRIME_PAL_SERIAL_EN == false)>
    if ((plcStatus != SYS_STATUS_READY) && (plcStatus != SYS_STATUS_ERROR))
    {
        return SYS_STATUS_BUSY;
    }

    return plcStatus;
</#if>
<#if (PRIME_PAL_PLC_EN == false) && (PRIME_PAL_RF_EN == true) && (PRIME_PAL_SERIAL_EN == false)>
    if ((rfStatus != SYS_STATUS_READY) && (rfStatus != SYS_STATUS_ERROR))
    {
        return SYS_STATUS_BUSY;
    }

    return rfStatus;
</#if>
<#if (PRIME_PAL_PLC_EN == false) &&(PRIME_PAL_RF_EN == false) && (PRIME_PAL_SERIAL_EN == true)>
    if ((serialStatus != SYS_STATUS_READY) && (serialStatus != SYS_STATUS_ERROR))
    {
        return SYS_STATUS_BUSY;
    }

    return serialStatus;
</#if>
<#if (PRIME_PAL_PLC_EN == true) && (PRIME_PAL_RF_EN == true) && (PRIME_PAL_SERIAL_EN == false)>
    if (((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_READY)) ||
        ((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_ERROR)) ||
        ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_READY)))
    {
        return SYS_STATUS_READY;
    }

    if ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_ERROR))
    {
        return SYS_STATUS_ERROR;
    }

    return SYS_STATUS_BUSY;
</#if>
<#if (PRIME_PAL_PLC_EN == true) && (PRIME_PAL_RF_EN == false) && (PRIME_PAL_SERIAL_EN == true)>
    if (((plcStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_READY)) ||
        ((plcStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_ERROR)) ||
        ((plcStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_READY)))
    {
        return SYS_STATUS_READY;
    }

    if ((plcStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_ERROR))
    {
        return SYS_STATUS_ERROR;
    }

    return SYS_STATUS_BUSY;
</#if>
<#if (PRIME_PAL_PLC_EN == false) && (PRIME_PAL_RF_EN == true) && (PRIME_PAL_SERIAL_EN == true)>
    if (((rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_READY)) ||
        ((rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_ERROR)) ||
        ((rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_READY)))
    {
        return SYS_STATUS_READY;
    }

    if ((rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_ERROR))
    {
        return SYS_STATUS_ERROR;
    }

    return SYS_STATUS_BUSY;
</#if>
<#if (PRIME_PAL_PLC_EN == true) && (PRIME_PAL_RF_EN == true) && (PRIME_PAL_SERIAL_EN == true)>
    if (((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_READY)) ||
        ((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_READY)) ||
        ((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_ERROR)) ||
        ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_READY)))
        ((plcStatus == SYS_STATUS_READY) && (rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_ERROR)) ||
        ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_READY) && (serialStatus == SYS_STATUS_ERROR)) ||
        ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_READY)))
    {
        return SYS_STATUS_READY;
    }

    if ((plcStatus == SYS_STATUS_ERROR) && (rfStatus == SYS_STATUS_ERROR) && (serialStatus == SYS_STATUS_ERROR))
    {
        return SYS_STATUS_ERROR;
    }

    return SYS_STATUS_BUSY;
</#if>
}

void PAL_CallbackRegister(PAL_CALLBACKS *pCallbacks)
{
    palData.dataConfirmCallback = pCallbacks->dataConfirm;
    palData.dataIndicationCallback = pCallbacks->dataIndication;
    palData.channelSwitchCallback = pCallbacks->switchRfChannel;
}

uint8_t PAL_DataRequest(PAL_MSG_REQUEST_DATA *pData)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pData->pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_DataRequest(pData));
    }

    return((uint8_t)PAL_TX_RESULT_PHY_ERROR);
}

uint8_t PAL_GetSNR(uint16_t pch, uint8_t *snr, uint8_t qt)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetSNR(snr, qt));
    }

    *snr = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetZCT(uint16_t pch, uint32_t *zct)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetZCT(zct));
    }

    *zct = 0UL;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetTimer(uint16_t pch, uint32_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetTimer(timer));
    }

    *timer = 0UL;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetTimerExtended(uint16_t pch, uint64_t *timer)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetTimerExtended(timer));
    }

    *timer = 0UL;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetCD(uint16_t pch, uint8_t *cd, uint8_t *rssi, uint32_t *timeVal, uint8_t *header)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetCD(cd, rssi, timeVal, header));
    }

    *cd = 0U;
    *rssi = 0U;
    *timeVal = 0UL;
    *header = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetNL(uint16_t pch, uint8_t *noise)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);
    return(palIface->MPAL_GetNL(noise));
}

uint8_t PAL_GetAGC(uint16_t pch, uint8_t *mode, uint8_t *gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetAGC(mode, gain));
    }

    *mode = 0U;
    *gain = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_SetAGC(uint16_t pch, uint8_t mode, uint8_t gain)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_SetAGC(mode, gain));
    }

    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetCCA(uint16_t pch, uint8_t *pState)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetCCA(pState));
    }

    *pState = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetChannel(uint16_t *pPch, uint16_t channelReference)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(channelReference);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetChannel(pPch));
    }

    *pPch = 0xFFFFU;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_SetChannel(uint16_t pch)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_SetChannel(pch));
    }

    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

void PAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        palIface->MPAL_ProgramChannelSwitch(timeSync, pch, timeMode);
    }
}

uint8_t PAL_GetConfiguration(uint16_t pch, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface == NULL)
    {
        *(uint8_t *)val = 0U;
        return((uint8_t)PAL_CFG_INVALID_INPUT);
    }

    if(id == (uint16_t)PAL_ID_PHY_SNIFFER_EN)
    {
<#if PRIME_PAL_PHY_SNIFFER == true>
        *(uint8_t *)val = palData.snifferEnabled;
<#else>
        *(uint8_t *)val = 0U;
</#if>
        return (uint8_t)PAL_CFG_SUCCESS;
    }

    return(palIface->MPAL_GetConfiguration(id, val, length));
}

uint8_t PAL_SetConfiguration(uint16_t pch, uint16_t id, void *val, uint16_t length)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface == NULL)
    {
        return((uint8_t)PAL_CFG_INVALID_INPUT);
    }

    if (id == (uint16_t)PAL_ID_PHY_SNIFFER_EN)
    {
<#if PRIME_PAL_PHY_SNIFFER == true>
        palData.snifferEnabled = *(uint8_t *)val;
<#else>
        palData.snifferEnabled = 0U;
</#if>
        return (uint8_t)PAL_CFG_SUCCESS;
    }

    return(palIface->MPAL_SetConfiguration(id, val, length));
}

uint16_t PAL_GetSignalCapture(uint16_t pch, uint8_t *noiseCapture, PAL_FRAME frameType,
                              uint32_t timeStart, uint32_t duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetSignalCapture(noiseCapture, frameType, timeStart, duration));
    }

    *noiseCapture = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

uint8_t PAL_GetMsgDuration(uint16_t pch, uint16_t length, PAL_SCHEME scheme, PAL_FRAME frameType, uint32_t *duration)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetMsgDuration(length, scheme, frameType, duration));
    }

    *duration = 0U;
    return((uint8_t)PAL_CFG_INVALID_INPUT);
}

bool PAL_CheckMinimumQuality(uint16_t pch, uint8_t reference, uint8_t modulation)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_CheckMinimumQuality(reference, modulation));
    }

    return false;
}

uint8_t PAL_GetLessRobustModulation(uint16_t pch, uint8_t mod1, uint8_t mod2)
{
    PAL_INTERFACE *palIface = lPAL_GetInterface(pch);

    if (palIface != NULL)
    {
        return(palIface->MPAL_GetLessRobustModulation(mod1, mod2));
    }

    return((uint8_t)PAL_OUTDATED_INF);
}

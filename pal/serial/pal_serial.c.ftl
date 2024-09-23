/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    pal_serial.c

  Summary:
    SERIAL Platform Abstraction Layer (PAL) Interface source file.

  Description:
    This module provides the interface between the PRIME MAC layer and the
    SERIAL physical layer.
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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
#include "driver/driver.h"
#include "driver/phy_serial/drv_phy_serial.h"
#include "service/time_management/srv_time_management.h"
#include "service/usi/srv_usi.h"
#include "pal_types.h"
#include "pal_local.h"
#include "pal_serial.h"
#include "pal_serial_local.h"
<#if PRIME_PAL_PHY_SNIFFER == true>
#include "service/psniffer/srv_psniffer.h"
</#if>

static uint8_t lPAL_SERIAL_RM_GetLessRobustModulation(PAL_SCHEME mod1, PAL_SCHEME mod2);
static bool lPAL_SERIAL_RM_CheckMinimumQuality(PAL_SCHEME reference, PAL_SCHEME modulation);

/******************************************************************************
 * PRIME PAL SERIAL interface implementation
 ******************************************************************************/
const PAL_INTERFACE PAL_SERIAL_Interface =
{
    .PAL_GetSNR = PAL_SERIAL_GetSNR,
    .PAL_GetZCT = PAL_SERIAL_GetZCT,
    .PAL_GetTimer = PAL_SERIAL_GetTimer,
    .PAL_GetTimerExtended = PAL_SERIAL_GetTimerExtended,
    .PAL_GetCD = PAL_SERIAL_GetCD,
    .PAL_GetNL = PAL_SERIAL_GetNL,
    .PAL_GetAGC = PAL_SERIAL_GetAGC,
    .PAL_SetAGC = PAL_SERIAL_SetAGC,
    .PAL_GetCCA = PAL_SERIAL_GetCCA,
    .PAL_GetChannel = PAL_SERIAL_GetChannel,
    .PAL_SetChannel = PAL_SERIAL_SetChannel,
    .PAL_DataRequest = PAL_SERIAL_DataRequest,
    .PAL_ProgramChannelSwitch = PAL_SERIAL_ProgramChannelSwitch,
    .PAL_GetConfiguration = PAL_SERIAL_GetConfiguration,
    .PAL_SetConfiguration = PAL_SERIAL_SetConfiguration,
    .PAL_GetSignalCapture = PAL_SERIAL_GetSignalCapture,
    .PAL_GetMsgDuration = PAL_SERIAL_GetMsgDuration,
    .PAL_CheckMinimumQuality = lPAL_SERIAL_RM_CheckMinimumQuality,
    .PAL_GetLessRobustModulation = lPAL_SERIAL_RM_GetLessRobustModulation,
};

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************
static PAL_SERIAL_DATA palSerialData = {0};

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************
static uint8_t lPAL_SERIAL_RM_GetLessRobustModulation(PAL_SCHEME mod1, PAL_SCHEME mod2)
{
    (void)mod1;
    (void)mod2;

    return PAL_CFG_INVALID_INPUT;
}

static bool lPAL_SERIAL_RM_CheckMinimumQuality(PAL_SCHEME reference, PAL_SCHEME modulation)
{
    (void)reference;
    (void)modulation;

    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************
static void lPAL_SERIAL_SERIAL_DataCfmCb(DRV_PHY_SERIAL_MSG_CONFIRM_DATA *pCfmData)
{
    PAL_MSG_CONFIRM_DATA dataCfm;

    dataCfm.bufId = pCfmData->buffId;
    dataCfm.frameType = (PAL_FRAME)pCfmData->mode;
    dataCfm.pch = (uint16_t)DRV_PHY_SERIAL_CHANNEL;
    dataCfm.result = (PAL_TX_RESULT)pCfmData->result;
    dataCfm.rmsCalc = pCfmData->rmsCalc;
    dataCfm.txTime = pCfmData->txTime;

    if (palSerialData.serialCallbacks.dataConfirm != NULL)
    {
        palSerialData.serialCallbacks.dataConfirm(&dataCfm);
    }

<#if PRIME_PAL_PHY_SNIFFER == true>
    if (palSerialData.snifferCallback)
    {
        size_t dataLength;

        SRV_PSNIFFER_SetTxPayloadSymbols(0);

        dataLength = SRV_PSNIFFER_SerialCfmMessage(palSerialData.snifferData, (void *)pCfmData);

        palSerialData.snifferCallback(palSerialData.snifferData, dataLength);
    }

</#if>
}

static void lPAL_SERIAL_SERIAL_DataIndCb(DRV_PHY_SERIAL_MSG_RX_DATA *pRxData)
{
    PAL_MSG_INDICATION_DATA dataInd;

    dataInd.bufId = pRxData->buffId;
    dataInd.dataLength = pRxData->dataLen;
    dataInd.estimatedBitrate = 20;
    dataInd.frameType = pRxData->mode;
    dataInd.headerType = pRxData->headerType;
    dataInd.lessRobustMod = pRxData->scheme;
    dataInd.lqi = ((pRxData->cinrAvg + 12) / 4);
    dataInd.pData = pRxData->dataBuf;
    dataInd.pch = (uint16_t)DRV_PHY_SERIAL_CHANNEL;
    dataInd.rssi = pRxData->rssiAvg;
    dataInd.rxTime = pRxData->rxTime;
    dataInd.scheme = pRxData->scheme;

    if (palSerialData.serialCallbacks.dataIndication != NULL)
    {
        palSerialData.serialCallbacks.dataIndication(&dataInd);
    }

<#if PRIME_PAL_PHY_SNIFFER == true>
    if (palSerialData.snifferCallback)
    {
        size_t length;

        SRV_PSNIFFER_SetRxPayloadSymbols(0);

        length = SRV_PSNIFFER_SerialRxMessage(palSerialData.snifferData, (void *)pRxData);

        palSerialData.snifferCallback(palSerialData.snifferData, length);
    }

</#if>
}

// *****************************************************************************
// *****************************************************************************
// Section: PAL SERIAL Interface Implementation
// *****************************************************************************
// *****************************************************************************
SYS_MODULE_OBJ PAL_SERIAL_Initialize(void)
{
    DRV_PHY_SERIAL_INIT drvPhySerialInitData;

    /* Check previously initialized */
    if (palSerialData.status != PAL_SERIAL_STATUS_UNINITIALIZED)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Initialize PHY Serial */
    drvPhySerialInitData.serialPhyHandlers.dataConfirm = lPAL_SERIAL_SERIAL_DataCfmCb;
    drvPhySerialInitData.serialPhyHandlers.dataReception = lPAL_SERIAL_SERIAL_DataIndCb;

    if (DRV_PHY_SERIAL_Initialize(DRV_PHY_SERIAL_INDEX, 
            (const SYS_MODULE_INIT *)&drvPhySerialInitData) != SYS_MODULE_OBJ_INVALID)
    {
        palSerialData.status = PAL_SERIAL_STATUS_ERROR;
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
        palSerialData.status = PAL_SERIAL_STATUS_READY;
        return SYS_MODULE_OBJ_STATIC;
    }
}

SYS_STATUS PAL_SERIAL_Status(void)
{
    /* Return the PAL SERIAL status */
    return ((SYS_STATUS)palSerialData.status);
}

void PAL_SERIAL_Tasks(void)
{
    DRV_PHY_SERIAL_Tasks();
}

void PAL_SERIAL_DataConfirmCallbackRegister(PAL_DATA_CONFIRM_CB callback)
{
    palSerialData.serialCallbacks.dataConfirm = callback;
}

void PAL_SERIAL_DataIndicationCallbackRegister(PAL_DATA_INDICATION_CB callback)
{
    palSerialData.serialCallbacks.dataIndication = callback;
}

uint8_t PAL_SERIAL_DataRequest(PAL_MSG_REQUEST_DATA *pMessageData)
{
    DRV_PHY_SERIAL_MSG_REQUEST_DATA phyTxData;

    if (palSerialData.status != PAL_SERIAL_STATUS_READY)
    {
        return ((uint8_t)PAL_TX_RESULT_PHY_ERROR);
    }

    phyTxData.dataBuf = pMessageData->pData;
    phyTxData.timeDelay = pMessageData->timeDelay;
    phyTxData.dataLen = pMessageData->dataLength;
    phyTxData.pch = pMessageData->pch;
    phyTxData.buffId = pMessageData->buffId;

    if (DRV_PHY_SERIAL_DataRequestTransmission(&phyTxData) != DRV_PHY_SERIAL_TX_RESULT_PROCESS)
    {
        return ((uint8_t)PAL_TX_RESULT_PHY_ERROR);
    }

<#if PRIME_PAL_PHY_SNIFFER == true>
    SRV_PSNIFFER_SetTxMessage((void *)&phyTxData);

</#if>

    return ((uint8_t)PAL_TX_RESULT_PROCESS);
}

void PAL_SERIAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode)
{
    (void)timeSync;
    (void)pch;
    (void)timeMode;
}

uint8_t PAL_SERIAL_GetTimer(uint32_t *pTimer)
{
    *pTimer = SRV_TIME_MANAGEMENT_GetTimeUS();

    return PAL_CFG_SUCCESS;
}

uint8_t PAL_SERIAL_GetTimerExtended(uint64_t *pTimeExtended)
{
    *pTimeExtended = SRV_TIME_MANAGEMENT_GetTimeUS64();

    return PAL_CFG_SUCCESS;
}

uint8_t PAL_SERIAL_GetCD(uint8_t *pCD, uint8_t *pRSSI, uint32_t *pTime, uint8_t *pHeader)
{
    *pCD = 0;
    *pRSSI = 0;
    *pTime = 0;
    *pHeader = 0;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetZCT(uint32_t *pZcTime)
{
    *pZcTime = 0;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetAGC(uint8_t *pMode, uint8_t *pGain)
{
    *pMode = 0;
    *pGain = 0;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_SetAGC(uint8_t mode, uint8_t gain)
{
    (void)(mode);
    (void)(gain);

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetCCA(uint8_t *channelState)
{
    *channelState = 0;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetNL(uint8_t *pNoise)
{
    *pNoise = 0;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetChannel(uint16_t *pPch)
{
    *pPch = 0xFFFF;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_SetChannel(uint16_t pch)
{
    (void)pch;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetConfiguration(uint16_t id, void *pValue, uint16_t length)
{
    uint16_t serialID = 0;

    (void)length;

    /* Check identifier */
    switch (id) 
    {
        case PAL_ID_INFO_VERSION:
            serialID = PAL_SERIAL_ID_INFO_VERSION;
            break;

        case PAL_ID_INFO_DEVICE:
            serialID = PAL_SERIAL_ID_INFO_DEVICE;
            break;

        default:
            return PAL_CFG_INVALID_INPUT;
    }

    *(uint16_t *)pValue = serialID;

    return PAL_CFG_SUCCESS;
}

uint8_t PAL_SERIAL_SetConfiguration(uint16_t id, void *pValue, uint16_t length)
{
    (void)id;
    (void)pValue;
    (void)length;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetSNR(uint8_t *pSnr, uint8_t qt)
{
    *pSnr = 0;
    (void)qt;

    return PAL_CFG_INVALID_INPUT;
}

uint16_t PAL_SERIAL_GetSignalCapture(uint8_t *pData, uint8_t frameType, uint32_t timeStart, uint32_t duration)
{
    *pData = 0;
    (void)frameType;
    (void)timeStart;
    (void)duration;

    return PAL_CFG_INVALID_INPUT;
}

uint8_t PAL_SERIAL_GetMsgDuration(uint16_t length, PAL_SCHEME scheme, uint8_t frameType, uint32_t *pDuration)
{
    *pDuration = 0;
    (void)length;
    (void)scheme;
    (void)frameType;

    return PAL_CFG_INVALID_INPUT;
}

<#if PRIME_PAL_PHY_SNIFFER == true>
void PAL_Serial_USISnifferCallbackRegister(SRV_USI_HANDLE usiHandler, PAL_USI_SNIFFER_CB callback)
{
    palSerialData.usiHandler = usiHandler;
    palSerialData.snifferCallback = callback;
}

</#if>
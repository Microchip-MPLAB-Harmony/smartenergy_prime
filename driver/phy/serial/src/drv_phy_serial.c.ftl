/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    drv_phy_serial.c

  Summary:
    Physical layer for the PRIME serial interface.

  Description:
    This module handles the serial transmission and reception of PRIME messages 
    at the physical
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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "configuration.h"
#include "system/system.h"
#include "stack/prime/driver/phy/serial/drv_phy_serial.h"
#include "service/time_management/srv_time_management.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_PHY_SERIAL_MSG_RCV_MAX_NUM    (4)

/* Modulation scheme of the payload: Differential 8PSK */
#define DRV_PHY_SERIAL_PROTOCOL_DBPSK_CC      0x04

#define DRV_PHY_SERIAL_GET_HEADER_TYPE(val)         ((val >> 4) & 0x03)

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// ***************************************************************************** 
typedef struct 
{
    uint16_t len;
    uint8_t dataBuf[DRV_PHY_SERIAL_MAX_PPDU_SIZE];
} DRV_PHY_SERIAL_MSG_RCV;

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************
/* Internal Data structure to manage RX */
static DRV_PHY_SERIAL_MSG_RX_DATA sPhySerialRxMsg;
/* Internal Tx buf to store last TX message */
static uint8_t sPhyTxBuffer[DRV_PHY_SERIAL_MAX_PPDU_SIZE];
/* Internal Tx Confirm to store last confirm */
static DRV_PHY_SERIAL_MSG_CONFIRM_DATA sPhySerialTXConfirmData;


/* Message received with a size of PHY_MAX_PPDU_SIZE over PHY Serial medium  */
static DRV_PHY_SERIAL_MSG_RCV sPhySerialMsgRecv[DRV_PHY_SERIAL_MSG_RCV_MAX_NUM];

static uint8_t sOutputMsgRcvIndex;

static uint8_t sInputMsgRcvIndex;

/* PHY Serial Init data */
static DRV_PHY_SERIAL_INIT phySerialInitData = {0};

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************
bool lDRV_PHY_SERIAL_RxFrame(uint8_t *rxMsg, size_t len) 
{
    if(!sPhySerialMsgRecv[sInputMsgRcvIndex].len) 
    {
        memcpy(sPhySerialMsgRecv[sInputMsgRcvIndex].dataBuf, rxMsg, len);
        sPhySerialMsgRecv[sInputMsgRcvIndex].len = len;

        if(++sInputMsgRcvIndex == DRV_PHY_SERIAL_MSG_RCV_MAX_NUM) 
        {
            sInputMsgRcvIndex = 0;
        }
    } 
    else 
    {
        /* ERROR ,RX queue full */
        return false;
    }

    return true;
}

// *****************************************************************************
// *****************************************************************************
// Section: PHY Serial Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************
SYS_MODULE_OBJ DRV_PHY_SERIAL_Initialize(const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init)
{
    uint8_t loopIndex;

    const DRV_PHY_SERIAL_INIT * const phySerialInit = (const DRV_PHY_SERIAL_INIT * const)init;

     /* Check single instance */
    if(index != DRV_PHY_SERIAL_INDEX)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    if(phySerialInit != NULL)
    {
        phySerialInitData.serialPhyHandlers.dataReception = phySerialInit->serialPhyHandlers.dataReception;
        phySerialInitData.serialPhyHandlers.dataConfirm = phySerialInit->serialPhyHandlers.dataConfirm;
    }
    else
    {
        phySerialInitData.serialPhyHandlers.dataReception = NULL;
        phySerialInitData.serialPhyHandlers.dataConfirm = NULL;
    }


    sInputMsgRcvIndex = 0;
    sOutputMsgRcvIndex = 0;
    for(loopIndex = 0; loopIndex < DRV_PHY_SERIAL_MSG_RCV_MAX_NUM; loopIndex++) 
    {
        sPhySerialMsgRecv[loopIndex].len = 0;
    }

    /* Get USI handler for PHY Serial protocol */
    phySerialInitData.srvUsiHandler = SRV_USI_Open(DRV_PHY_SERIAL_USI_INSTANCE);

    if(phySerialInitData.srvUsiHandler != SRV_USI_HANDLE_INVALID)
    {
        /* Register Handler */
        SRV_USI_CallbackRegister(phySerialInitData.srvUsiHandler, SRV_USI_PROT_ID_PHY_SERIAL_PRIME, lDRV_PHY_SERIAL_RxFrame);
    }
    
    return (SYS_MODULE_OBJ)DRV_PHY_SERIAL_INDEX;
}

void DRV_PHY_SERIAL_Deinitialize(SYS_MODULE_OBJ object)
{
    if (object != (SYS_MODULE_OBJ)DRV_PHY_SERIAL_INDEX)
    {
        return;
    }

    phySerialInitData.srvUsiHandler = SRV_USI_HANDLE_INVALID;
}

void DRV_PHY_SERIAL_SetCallbacks(DRV_PHY_SERIAL_CALLBACKS *phySerCBs) 
{
    phySerialInitData.serialPhyHandlers.dataReception = phySerCBs->dataReception;
    phySerialInitData.serialPhyHandlers.dataConfirm = phySerCBs->dataConfirm;
}

uint8_t DRV_PHY_SERIAL_TxFrame(DRV_PHY_SERIAL_MSG_REQUEST_DATA *txMsg) 
{
    size_t txDataCnt=0;

    memcpy(sPhyTxBuffer, txMsg->dataBuf, txMsg->dataLen);

    /* Send through USI */
    if(phySerialInitData.srvUsiHandler != SRV_USI_HANDLE_INVALID)
    {       
        txDataCnt = SRV_USI_Send_Message(phySerialInitData.srvUsiHandler, SRV_USI_PROT_ID_PHY_SERIAL_PRIME, sPhyTxBuffer, txMsg->dataLen);       
    }
    else
    {
        /* return from here */
        return 0xFE;
    }


    /* Generate Phy Data Confirm Callback */
    if(phySerialInitData.serialPhyHandlers.dataConfirm != NULL) 
    {
        uint32_t currentTime;

        currentTime = SRV_TIME_MANAGEMENT_GetTimeUS();

        if(txDataCnt > 0) 
        {
            sPhySerialTXConfirmData.result = DRV_PHY_SERIAL_TX_RESULT_SUCCESS;
        }
        else
        {
             sPhySerialTXConfirmData.result = DRV_PHY_SERIAL_TX_RESULT_BUSY_TX;
        }

        sPhySerialTXConfirmData.txTime = currentTime;
        sPhySerialTXConfirmData.buffId = txMsg->buffId;
        sPhySerialTXConfirmData.mode = PHY_SERIAL_MODE_TYPE_A;
        sPhySerialTXConfirmData.rmsCalc = 140;


        phySerialInitData.serialPhyHandlers.dataConfirm(&sPhySerialTXConfirmData);
    }

    return DRV_PHY_SERIAL_TX_RESULT_PROCESS;
}


void DRV_PHY_SERIAL_Tasks(void)
{
    while(sPhySerialMsgRecv[sOutputMsgRcvIndex].len) 
    {
            /* Generate Phy Data Indication Callback */
        if( phySerialInitData.serialPhyHandlers.dataReception != NULL) 
        {
            uint32_t currentTime;
            currentTime = SRV_TIME_MANAGEMENT_GetTimeUS();

            /* Copy data payload */
            sPhySerialRxMsg.dataBuf = sPhySerialMsgRecv[sOutputMsgRcvIndex].dataBuf ;
            sPhySerialRxMsg.dataLen = sPhySerialMsgRecv[sOutputMsgRcvIndex].len;
            /* Get header type */
            sPhySerialRxMsg.headerType = DRV_PHY_SERIAL_GET_HEADER_TYPE(sPhySerialMsgRecv[sOutputMsgRcvIndex].dataBuf[0]);
            /* Fill other data with values that assure minimum quality */
            sPhySerialRxMsg.rxTime = currentTime;
            sPhySerialRxMsg.evmHeaderAcum = 0;
            sPhySerialRxMsg.evmPayloadAcum = 0;
            sPhySerialRxMsg.evmHeader = 0;
            sPhySerialRxMsg.evmPayload = 0;
            sPhySerialRxMsg.buffId = 0;
            sPhySerialRxMsg.scheme = DRV_PHY_SERIAL_PROTOCOL_DBPSK_CC;
            sPhySerialRxMsg.mode = PHY_SERIAL_MODE_TYPE_A;
            sPhySerialRxMsg.noiseResult = 0;
            sPhySerialRxMsg.rssiAvg = 0;
            sPhySerialRxMsg.cinrAvg = 100;
            sPhySerialRxMsg.cinrMin = 100;
            sPhySerialRxMsg.bersoft = 0;
            sPhySerialRxMsg.bersoftMax = 0;
            sPhySerialRxMsg.qt = 255;
            sPhySerialRxMsg.snrEx = 0;

    
            phySerialInitData.serialPhyHandlers.dataReception(&sPhySerialRxMsg);
        }

        sPhySerialMsgRecv[sOutputMsgRcvIndex].len = 0;
        if(++sOutputMsgRcvIndex == DRV_PHY_SERIAL_MSG_RCV_MAX_NUM) 
        {
            sOutputMsgRcvIndex = 0;
        }
    }
}


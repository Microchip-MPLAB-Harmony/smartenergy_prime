/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    pal_serial.h

  Summary:
    Physical Abstraction Layer (PAL) SERIAL header file.

  Description:
    This module provides the interface between the PRIME MAC layer and the
    SERIAL physical layer.
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

#ifndef  PAL_SERIAL_H
#define  PAL_SERIAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "pal_types.h"
#include "pal_serial_local.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END
extern const PAL_INTERFACE PAL_SERIAL_Interface;

SYS_MODULE_OBJ PAL_SERIAL_Initialize(void);
SYS_STATUS PAL_SERIAL_Status(void);
void PAL_SERIAL_Tasks(void);
void PAL_SERIAL_DataConfirmCallbackRegister(PAL_DATA_CONFIRM_CB callback);
void PAL_SERIAL_DataIndicationCallbackRegister(PAL_DATA_INDICATION_CB callback);
uint8_t PAL_SERIAL_DataRequest(PAL_MSG_REQUEST_DATA *requestMsg);
void PAL_SERIAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode);
uint8_t PAL_SERIAL_GetSNR(uint8_t *snr, uint8_t qt);
uint8_t PAL_SERIAL_GetZCT(uint32_t *zct);
uint8_t PAL_SERIAL_GetTimer(uint32_t *timer);
uint8_t PAL_SERIAL_GetTimerExtended(uint64_t *timer);
uint8_t PAL_SERIAL_GetCD(uint8_t *pCD, uint8_t *pRSSI, uint32_t *pTime, uint8_t *pHeader);
uint8_t PAL_SERIAL_GetNL(uint8_t *noise);
uint8_t PAL_SERIAL_GetAGC(uint8_t *mode, uint8_t *gain);
uint8_t PAL_SERIAL_SetAGC(uint8_t mode, uint8_t gain);
uint8_t PAL_SERIAL_GetCCA(uint8_t *channelState);
uint8_t PAL_SERIAL_GetChannel(uint16_t *pPch);
uint8_t PAL_SERIAL_SetChannel(uint16_t pch);
void PAL_SERIAL_ProgramChannelSwitch(uint32_t timeSync, uint16_t pch, uint8_t timeMode);
uint8_t PAL_SERIAL_GetConfiguration(uint16_t id, void *val, uint16_t len);
uint8_t PAL_SERIAL_SetConfiguration(uint16_t id, void *val, uint16_t len);
uint16_t PAL_SERIAL_GetSignalCapture(uint8_t *noiseCapture, PAL_FRAME frameType, uint32_t timeStart, uint32_t duration);
uint8_t PAL_SERIAL_GetMsgDuration(uint16_t msgLen, PAL_SCHEME scheme, PAL_FRAME frameType, uint32_t *duration);
<#if PRIME_PAL_PHY_SNIFFER == true>
void PAL_SERIAL_USISnifferCallbackRegister(SRV_USI_HANDLE usiHandler, PAL_USI_SNIFFER_CB callback);
</#if>

#ifdef __cplusplus
}
#endif

#endif /*  PAL_SERIAL_H */

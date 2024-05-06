/*******************************************************************************
  PRIME User PIBs Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_user_pib.c

  Summary:
    Source code for the PRIME User PIBs service implementation.

  Description:
    The User PIBs service provides a simple interface to handle a parameter 
    interface base defined by the user from the PRIME stack. This file contains 
    the source code for the implementation of this service.
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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "srv_user_pib.h"
#include "device.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

/* PIB values */
static uint32_t srvUserPibValues[11];

/* Callback function pointers */
static SRV_USER_PIB_GET_REQUEST_CALLBACK SRV_USER_PIB_GetRequestCallback;
static SRV_USER_PIB_SET_REQUEST_CALLBACK SRV_USER_PIB_SetRequestCallback;

// *****************************************************************************
// *****************************************************************************
// Section: User PIBs Service Interface Implementation
// *****************************************************************************
// *****************************************************************************

void SRV_USER_PIB_GetRequest(uint16_t pibAttrib)
{
    uint32_t pibValue;
    uint8_t getResult;

    /* Check PIB value */
    if ((pibAttrib >= PIB_USER_RESET_INFO) && (pibAttrib <= PIB_USER_R12)) 
    {
        getResult = true;
        pibValue = srvUserPibValues[pibAttrib & 0x000F];
    } 
    else 
    {
        getResult = false;
        pibValue = 0;
    }

    /* Return result */
    if (SRV_USER_PIB_GetRequestCallback != NULL) 
    {
        SRV_USER_PIB_GetRequestCallback(getResult, pibAttrib, &pibValue, 4);
    }
}

void SRV_USER_PIB_SetRequest(uint16_t pibAttrib, void *pibValue, uint8_t pibSize)
{
    (void)pibAttrib;
    (void)pibValue;
    (void)pibSize;

    /* Return result */
    if (SRV_USER_PIB_SetRequestCallback != NULL) 
    {
        SRV_USER_PIB_SetRequestCallback(false);
    }
}

void SRV_USER_PIB_GetRequestCallbackRegister(SRV_USER_PIB_GET_REQUEST_CALLBACK callback)
{
    SRV_USER_PIB_GetRequestCallback = callback;
}

void SRV_USER_PIB_SetRequestCallbackRegister(SRV_USER_PIB_SET_REQUEST_CALLBACK callback)
{
    SRV_USER_PIB_SetRequestCallback = callback;
}

void SRV_USER_PIB_Initialize(void)
{
    SRV_USER_PIB_GetRequestCallback = NULL;
    SRV_USER_PIB_SetRequestCallback = NULL;

<#if DEFAULT_PIB_HANDLING_EN == true>
    /* Store PIB values */
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
    srvUserPibValues[PIB_USER_RESET_INFO & 0x000F] = SUPC_GPBRRead(GPBR_REGS_5);
    srvUserPibValues[PIB_USER_PC & 0x000F] = SUPC_GPBRRead(GPBR_REGS_6);
    srvUserPibValues[PIB_USER_LR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_7);
    srvUserPibValues[PIB_USER_PSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_8);
    srvUserPibValues[PIB_USER_HFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_9);
    srvUserPibValues[PIB_USER_CFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_10);
    srvUserPibValues[PIB_USER_R0 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_11);
    srvUserPibValues[PIB_USER_R1 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_12);
    srvUserPibValues[PIB_USER_R2 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_13);
    srvUserPibValues[PIB_USER_R3 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_14);
    srvUserPibValues[PIB_USER_R12 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_15);
<#else>
    srvUserPibValues[PIB_USER_RESET_INFO & 0x000F] = SUPC_GPBRRead(GPBR_REGS_0);
    srvUserPibValues[PIB_USER_PC & 0x000F] = SUPC_GPBRRead(GPBR_REGS_1);
    srvUserPibValues[PIB_USER_LR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_2);
    srvUserPibValues[PIB_USER_PSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_3);
    srvUserPibValues[PIB_USER_HFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_4);
    srvUserPibValues[PIB_USER_CFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_5);
    srvUserPibValues[PIB_USER_R0 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_6);
    srvUserPibValues[PIB_USER_R1 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_7);
    srvUserPibValues[PIB_USER_R2 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_8);
    srvUserPibValues[PIB_USER_R3 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_9);
    srvUserPibValues[PIB_USER_R12 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_10);
</#if>
</#if>
</#if>
<#if __PROCESSOR?matches("ATSAME70*")>
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
    srvUserPibValues[PIB_USER_RESET_INFO & 0x000F] = SUPC_GPBRRead(GPBR_REGS_5);
    srvUserPibValues[PIB_USER_PC & 0x000F] = SUPC_GPBRRead(GPBR_REGS_6);
    srvUserPibValues[PIB_USER_LR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_7);
    srvUserPibValues[PIB_USER_PSR & 0x000F] = 0;
    srvUserPibValues[PIB_USER_HFSR & 0x000F] = 0;
    srvUserPibValues[PIB_USER_CFSR & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R0 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R1 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R2 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R3 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R12 & 0x000F] = 0;
<#else>
    srvUserPibValues[PIB_USER_RESET_INFO & 0x000F] = SUPC_GPBRRead(GPBR_REGS_0);
    srvUserPibValues[PIB_USER_PC & 0x000F] = SUPC_GPBRRead(GPBR_REGS_1);
    srvUserPibValues[PIB_USER_LR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_2);
    srvUserPibValues[PIB_USER_PSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_3);
    srvUserPibValues[PIB_USER_HFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_4);
    srvUserPibValues[PIB_USER_CFSR & 0x000F] = SUPC_GPBRRead(GPBR_REGS_5);
    srvUserPibValues[PIB_USER_R0 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_6);
    srvUserPibValues[PIB_USER_R1 & 0x000F] = SUPC_GPBRRead(GPBR_REGS_7);
    srvUserPibValues[PIB_USER_R2 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R3 & 0x000F] = 0;
    srvUserPibValues[PIB_USER_R12 & 0x000F] = 0;
</#if>
</#if>
</#if>

    /* Clear registers (except reset information) */
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
    SUPC_GPBRWrite(GPBR_REGS_6, 0);
    SUPC_GPBRWrite(GPBR_REGS_7, 0);
    SUPC_GPBRWrite(GPBR_REGS_8, 0);
    SUPC_GPBRWrite(GPBR_REGS_9, 0);
    SUPC_GPBRWrite(GPBR_REGS_10, 0);
    SUPC_GPBRWrite(GPBR_REGS_11, 0);
    SUPC_GPBRWrite(GPBR_REGS_12, 0);
    SUPC_GPBRWrite(GPBR_REGS_13, 0);
    SUPC_GPBRWrite(GPBR_REGS_14, 0);
    SUPC_GPBRWrite(GPBR_REGS_15, 0);
<#else>
    SUPC_GPBRWrite(GPBR_REGS_1, 0);
    SUPC_GPBRWrite(GPBR_REGS_2, 0);
    SUPC_GPBRWrite(GPBR_REGS_3, 0);
    SUPC_GPBRWrite(GPBR_REGS_4, 0);
    SUPC_GPBRWrite(GPBR_REGS_5, 0);
    SUPC_GPBRWrite(GPBR_REGS_6, 0);
    SUPC_GPBRWrite(GPBR_REGS_7, 0);
    SUPC_GPBRWrite(GPBR_REGS_8, 0);
    SUPC_GPBRWrite(GPBR_REGS_9, 0);
    SUPC_GPBRWrite(GPBR_REGS_10, 0);
</#if>
</#if>
</#if>
<#if __PROCESSOR?matches("ATSAME70*")>
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
    SUPC_GPBRWrite(GPBR_REGS_6, 0);
    SUPC_GPBRWrite(GPBR_REGS_7, 0);
<#else>
    SUPC_GPBRWrite(GPBR_REGS_1, 0);
    SUPC_GPBRWrite(GPBR_REGS_2, 0);
    SUPC_GPBRWrite(GPBR_REGS_3, 0);
    SUPC_GPBRWrite(GPBR_REGS_4, 0);
    SUPC_GPBRWrite(GPBR_REGS_5, 0);
    SUPC_GPBRWrite(GPBR_REGS_6, 0);
    SUPC_GPBRWrite(GPBR_REGS_7, 0);
</#if>
</#if>
</#if>
</#if> 
}

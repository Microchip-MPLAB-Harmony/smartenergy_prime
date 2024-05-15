/*******************************************************************************
  PRIME API Source 
   
  Company:
    Microchip Technology Inc.

  File Name:
    prime_api.c

  Summary:
    PRIME API Source File

  Description:
    This module manages the the PRIME stack from the PRIME application.
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
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include "cmsis_compiler.h" 
#include "prime_api.h"
#include "prime_api_types.h"
<#if PRIME_MODE == "SN" && PRIME_PROJECT == "application project">
#include "configuration.h"

void PRIME_API_GetPrime13API(PRIME_API **pPrimeApi)
{
    *pPrimeApi = (PRIME_API *)PRIME_SN_FWSTACK13_ADDRESS;
}

void PRIME_API_GetPrime14API(PRIME_API **pPrimeApi)
{
    *pPrimeApi = (PRIME_API *)PRIME_SN_FWSTACK14_ADDRESS;
}

<#else>
  <#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
#include <libpic32c.h>
  </#if>
#include "stack/prime/prime_api/prime_hal_wrapper.h"
#include "stack/prime/mac/mac_defs.h"
//#include "stack/prime/mac/mac.h"
//#include "stack/prime/mngp/mngp.h"
//#include "stack/prime/conv/sscs/null/cl_null.h"
//#include "stack/prime/conv/sscs/iec_4_32/cl_432.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
/* Configuration of the CORTEX-M4 Processor and Core Peripherals */
/* PIC32CXMT */
#define __NVIC_PRIO_BITS         4
  <#if (PRIME_MODE == "BN" && BN_SLAVE_EN == false)>
#define NUM_MAX_NODES            ${NUM_MAX_NODES}
  </#if>
#define MAC_SECURITY_PROFILE     ${MAC_SECURITY_PROFILE}
#define MNGP_SPROF_USI_PORT      ${MNGP_SPROF_USI_PORT}
  <#if (MAC_SNIFFER_EN?? && MAC_SNIFFER_EN == true)>
#define MAC_SNIFFER_USI_PORT     ${MAC_SNIFFER_USI_PORT}
  </#if>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
  <#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
void __attribute__((optimize("-O1"), section(".text.Reset_Handler"), long_call)) Reset_Handler(void)
{
    // Avoid warning in compilation. Reset Handler is not needed.
}

  </#if>
  <#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
__attribute__ ((section(".vectors"), used))
  </#if>
const PRIME_API  PRIME_API_Interface =
{
    .vendor = PRIME_PIB_VENDOR,
    .model = PRIME_PIB_MODEL,
    .version = PRIME_FW_VERSION,
    .Initialize = PRIME_API_Initialize,
    .Tasks = PRIME_API_Tasks,
    // .MacSetCallbacks = ,     
    // .MacEstablishRequest = , 
    // .MacEstablishResponse = ,
    // .MacReleaseRequest = ,
    // .MacReleaseResponse = ,
    // .MacJoinRequest = ,
    // .MacJoinResponse = ,
    // .MacLeaveRequest = ,
    // .MacLeaveResponse = ,
    // .PlmeResetRequest = ,
    // .PlmeSleepRequest = ,
    // .PlmeResumeRequest = ,
    // .PlmeTestModeRequest = ,
    // .PlmeGetRequest = ,
    // .PlmeSetRequest = ,
    // .MlmeRegisterRequest = ,
    // .MlmeUnregisterRequest
    // .MlmePromoteRequest = ,
    // .MlmeDemoteRequest = ,
    // .MlmeResetRequest = ,
    // .MlmeGetRequest = ,
    // .MlmeListGetRequest = ,
    // .MlmeSetRequest = ,
    // .Cl432SetCallbacks = ,
    // .Cl432EstablishRequest
    // .Cl432ReleaseRequest = ,
    // .Cl432DlDataRequest = ,
    // .MlmeMpPromoteRequest = ,
    // .MlmeMpDemoteRequest = ,
};

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
  <#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
static void lPRIME_API_PrimeDataStartup(void)
{
    __pic32c_data_initialization();
}

  </#if>
static void lPRIME_API_SetPrimeVersion(MAC_VERSION_INFO *macInfo)
{
    uint8_t sizeConfig, sizeInfo;
    uint8_t copyLen;

    memset(macInfo, 0, sizeof(MAC_VERSION_INFO));

    /* Update MODEL */
    sizeConfig = sizeof(PRIME_FW_MODEL);
    sizeInfo = sizeof(macInfo->fwModel);
    if (sizeConfig < sizeInfo) 
    {
        copyLen = sizeConfig;
    } 
    else 
    {
        copyLen = sizeInfo;
    }

    memcpy(macInfo->fwModel, PRIME_FW_MODEL, copyLen);
    macInfo->pibModel = PRIME_PIB_MODEL;

    /* Update VENDOR */
    sizeConfig = sizeof(PRIME_FW_VENDOR);
    sizeInfo = sizeof(macInfo->fwVendor);
    if (sizeConfig < sizeInfo) {
        copyLen = sizeConfig;
    } else {
        copyLen = sizeInfo;
    }

    memcpy(macInfo->fwVendor, PRIME_FW_VENDOR, copyLen);
    macInfo->pibVendor = PRIME_PIB_VENDOR;

    /* Update VERSION */
    sizeConfig = sizeof(PRIME_FW_VERSION);
    sizeInfo = sizeof(macInfo->fwVersion);
    if (sizeConfig < sizeInfo) {
        copyLen = sizeConfig;
    } else {
        copyLen = sizeInfo;
    }

    memcpy(macInfo->fwVersion, PRIME_FW_VERSION, copyLen);
}

// *****************************************************************************
// *****************************************************************************
// Section: CRC Service Interface Implementation
// *****************************************************************************
// *****************************************************************************
void PRIME_API_Initialize(void *halApi)
{
    MAC_VERSION_INFO macInfo;

    /* Set critical region */
    __set_BASEPRI( 2 << (8 - __NVIC_PRIO_BITS));

  <#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
    lPRIME_API_PrimeDataStartup();
  </#if>

    /* Set PRIME HAL wrapper */
    PRIME_HAL_WRP_Configure(halApi);

    /* Set PRIME version from configuration */
    lPRIME_API_SetPrimeVersion(&macInfo);

//	/* Initialize MAC layer */
//	MAC_Initialize(&macInfo, (uint8_t)MAC_SECURITY_PROFILE);
//
//	/* Initialize Convergence layers */
//	CL_NULL_Initialize();
//	CL_432_Initialize();
//
//	/* Initialize Management Plane */
//	MNGP_Initialize(&macInfo, (uint8_t)MNGP_SPROF_USI_PORT);

    /* Set critical region */
    __set_BASEPRI(0);
}

void PRIME_API_Tasks(void)
{
    /* Set critical region */
    __set_BASEPRI( 3 << (8 - __NVIC_PRIO_BITS));

//	/* Process MAC layer */
//	MAC_Tasks();

  <#if (PRIME_MODE == "SN") || (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
//	/* Process Management Plane */
//	MNGP_Tasks();
  </#if>

    /* Set critical region */
    __set_BASEPRI(0);
}

  <#if PRIME_MODE == "BN">
void PRIME_API_GetPrimeAPI(PRIME_API **pPrimeApi)
{
    *pPrimeApi = (PRIME_API *)&PRIME_API_Interface;
}

  </#if>
</#if>
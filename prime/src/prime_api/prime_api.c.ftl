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

#include <stdio.h>
#include "prime_hal_wrapper.h"
#include "prime_api.h"
#include "stack/prime/mac/mac.h"
#include "stack/prime/mngp/mngp.h"
#include "stack/prime/conv/sscs/null/cl_null.h"
#include "stack/prime/conv/sscs/iec_4_32/cl_432.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

<#if (PRIME_MODE == "BN" && BN_SLAVE_EN == false)>
#define NUM_MAX_NODES            ${NUM_MAX_NODES}
</#if>
#define MAC_SECURITY_PROFILE     ${MAC_SECURITY_PROFILE}
#define MNGP_SPROF_USI_PORT      ${MNGP_SPROF_USI_PORT}
<#if MAC_SNIFFER_EN == true>
#define MAC_SNIFFER_USI_PORT     ${MAC_SNIFFER_USI_PORT}
</#if>


<#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* Initialize segments */
extern uint32_t _szero;
extern uint32_t _ezero;
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

<#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
static void lPRIME_API_PrimeDataStartup(void)
{
	uint32_t *pDest;

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) 
    {
		*pDest++ = 0;
	}
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
	PRIME_HAL_WRP_SetConfig(halApi);

	/* Set PRIME version from configuration */
	lPRIME_API_SetPrimeVersion(&macInfo);

	/* Initialize MAC layer */
	MAC_Initialize(&macInfo, (uint8_t)MAC_SECURITY_PROFILE);

	/* Initialize Convergence layers */
	CL_NULL_Initialize();
	CL_432_Initialize();

	/* Initialize Management Plane */
	MNGP_Initialize(&macInfo, (uint8_t)MNGP_SPROF_USI_PORT);

	/* Set critical region */
	__set_BASEPRI(0);
}

void PRIME_API_Tasks(void)
{
	/* Set critical region */
	__set_BASEPRI( 3 << (8 - __NVIC_PRIO_BITS));

	/* Process MAC layer */
	MAC_Tasks();

<#if (PRIME_MODE == "SN") || (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
	/* Process Management Plane */
	MNGP_Tasks();
</#if>

	/* Set critical region */
	__set_BASEPRI(0);
}

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

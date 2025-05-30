/*******************************************************************************
  PRIME Stack Source File

  Company:
    Microchip Technology Inc.

  File Name:
    prime_stack.c

  Summary:
    PRIME Stack Source File

  Description:
    This file provides the source code for the implementation of the management
    of the PRIME stack from the application.
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

#include "prime_stack.h"
#include "prime_stack_local.h"
#include "prime_api/prime_api.h"
#include "prime_api/prime_api_defs.h"
#include "prime_api/prime_api_types.h"
<#if PRIME_MODE == "SN" && PRIME_PROJECT == "application project">
#include "service/storage/srv_storage.h"
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Extern Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static PRIME_OBJ primeObj;
static PRIME_API_INIT primeApiInit;

// *****************************************************************************
// *****************************************************************************
// Section: PRIME Stack Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ PRIME_Initialize(const SYS_MODULE_INDEX index,
    const SYS_MODULE_INIT * init)
{
/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 11.3 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.3" "H3_MISRAC_2012_R_11_3_DR_1"
</#if>
    const PRIME_STACK_INIT* primeInit = (const PRIME_STACK_INIT *)init;
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
/* MISRA C-2012 deviation block end */

    /* Validate the request */
    if (index >= PRIME_INSTANCES_NUMBER)
    {
        primeObj.status = PRIME_STATUS_ERROR;
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Fill in initialization data */
    primeApiInit.palIndex = primeInit->palIndex;
    primeApiInit.mngPlaneUsiPort = primeInit->mngPlaneUsiPort;
    primeApiInit.halApi = &primeHalAPI;
    
    /* Get the PRIME version */
    SRV_STORAGE_PRIME_MODE_INFO_CONFIG boardInfo;

    (void) memset(&boardInfo, 0, sizeof(boardInfo));

    (void) SRV_STORAGE_GetConfigInfo(SRV_STORAGE_TYPE_MODE_PRIME, (uint8_t)sizeof(boardInfo),
                              (void *)&boardInfo);

<#if PRIME_MODE == "BN" || (PRIME_MODE == "SN" && PRIME_PROJECT == "monolithic project")>
    /* Get PRIME API pointer */
    PRIME_API_GetPrimeAPI(&primeObj.primeApi);
</#if>
<#if PRIME_MODE == "SN" && PRIME_PROJECT == "application project">
    /* Get PRIME API pointer */
    switch (boardInfo.primeVersion)
    {
        case PRIME_VERSION_1_3:
            PRIME_API_GetPrime13API(&primeObj.primeApi);
            break;

        case PRIME_VERSION_1_4:
        default:
            PRIME_API_GetPrime14API(&primeObj.primeApi);
            break;
    }
 </#if>

    primeObj.primeVersion = boardInfo.primeVersion;

    /* Update status */
    primeObj.status = PRIME_STATUS_POINTER_READY;

    return (SYS_MODULE_OBJ)0;
}


void PRIME_Tasks(SYS_MODULE_OBJ object)
{
    if (object != (SYS_MODULE_OBJ)0)
    {
        /* Invalid object */
        primeObj.status = PRIME_STATUS_ERROR;
        return;
    }

    switch (primeObj.status)
    {
        case PRIME_STATUS_POINTER_READY:
            primeObj.primeApi->Initialize((PRIME_API_INIT*)&primeApiInit, false, primeObj.primeVersion);
            primeObj.status = PRIME_STATUS_INITIALIZING;
            break;
            
        case PRIME_STATUS_RESTART:
            primeObj.primeApi->Initialize((PRIME_API_INIT*)&primeApiInit, true, primeObj.primeVersion);
            primeObj.status = PRIME_STATUS_INITIALIZING;
            break;


        case PRIME_STATUS_INITIALIZING:
            /* Complete initialization of PRIME stack takes several cycles */
            /* due to the initialization of PAL and drivers */
            primeObj.primeApi->Tasks();

            /* Do not allow the application to call PRIME until ready */
            if (primeObj.primeApi->Status() == SYS_STATUS_READY)
            {
                primeObj.status = PRIME_STATUS_RUNNING;
            }

            break;

        case PRIME_STATUS_RUNNING:
            primeObj.primeApi->Tasks();
            break;

        default:
           primeObj.status = PRIME_STATUS_ERROR;
           break;
    }
}

void PRIME_Restart(uint32_t *primePtr, uint8_t version)
{
    /* Set PRIME API pointer */
/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 11.3 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.3" "H3_MISRAC_2012_R_11_3_DR_1"
</#if>
    primeObj.primeApi = (const PRIME_API *)primePtr;
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
/* MISRA C-2012 deviation block end */

    /* Set new PRIME version */
    primeObj.primeVersion = version;

    if (primeObj.status == PRIME_STATUS_RUNNING)
    {
        /* Update status */
        primeObj.status = PRIME_STATUS_RESTART;
    }
}

SYS_STATUS PRIME_Status(void)
{
    /* Return the PRIME status */
    return ((SYS_STATUS)primeObj.status);
}

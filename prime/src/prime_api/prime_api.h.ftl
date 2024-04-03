/*******************************************************************************
  PRIME API Header 
   
  Company:
    Microchip Technology Inc.

  File Name:
    prime_api.h

  Summary:
    PRIME API Header File

  Description:
    This module converts the PRIME stack library interface into a global 
    interface to be used by the PRIME application.
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

#ifndef PRIME_API_H_INCLUDE
#define PRIME_API_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "prime_api_defs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

#define PRIME_FW_VENDOR          "${PRIME_FW_VENDOR?string}"
#define PRIME_FW_MODEL           "${PRIME_FW_MODEL?string}"
#define PRIME_FW_VERSION         "${PRIME_FW_VERSION?string}"
#define PRIME_PIB_VENDOR         ${PRIME_PIB_VENDOR}
#define PRIME_PIB_MODEL          0x${PRIME_PIB_MODEL}

// *****************************************************************************
// *****************************************************************************
// Section: PRIME API Control Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PRIME_API_Initialize(void *halApi)

  Summary:
    Initializes the PRIME stack.

  Description:
    This routine initializes the PRIME stack.

  Precondition:
    None.

  Parameters:
    halApi      - Pointer to the HAL API functions

  Returns:
    None.

  Example:
    <code>
    PRIME_API_Initialize((HAL_API*)&halApi);
    </code>

  Remarks:
    This routine is normally not called directly by an application. The 
    PRIME application must use the function located in the header table.
*/
void PRIME_API_Initialize(void *halApi);

// ****************************************************************************
/* Function:
    void PRIME_API_Tasks(void)

  Summary:
    Maintains the PRIME stack state machine. 

  Description:
    This function is used to maintain the PRIME stack internal state machine and 
    generate callbacks.

  Precondition:
    The PRIME_API_Initialize function should have been called before calling this 
    function.

  Parameters:
    None

  Returns:
    None

  Example:
    <code>
    while (true)
    {
        PRIME_API_Tasks();
    }
    </code>

  Remarks:
    This routine is normally not called directly by an application. The 
    PRIME application must use the function located in the header table 
    and call it periodically.
*/
void PRIME_API_Tasks(void);


<#if (PRIME_MODE == "SN") || (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
// *****************************************************************************
// *****************************************************************************
// Section: PRIME API Interface Routines in separated application binaries
// *****************************************************************************
// *****************************************************************************

/* Pointer to the PRIME stack */
extern uint32_t primeApi;

#define PRIME_API_FUNCS_OFFSET                    0x18

/* PRIME constants located in header table (offset, type) duplets */

#define PRIME_VENDOR                              (*((uint16_t *)((primeApi) + (0x00))))
#define PRIME_MODEL                               (*((uint16_t *)((primeApi) + (0x02))))
#define PRIME_VERSION                             (*((char *)((primeApi) + (0x04))))

/* PRIME control functions interface - (index, type) duplets */
#define PRIME_Initialize                                 ((PRIME_API_INITIALIZE)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[0]))
#define PRIME_Tasks                                      ((PRIME_API_TASKS)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[1]))
#define PRIME_CL_NULL_SetCallbacks                       ((MAC_SET_CALLBACKS)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[2]))
#define PRIME_CL_NULL_EstablishRequest                   ((MAC_ESTABLISH_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[3]))
#define PRIME_CL_NULL_EstablishResponse                  ((MAC_ESTABLISH_RESPONSE)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[4]))
#define PRIME_CL_NULL_ReleaseRequest                     ((MAC_RELEASE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[5]))
#define PRIME_CL_NULL_ReleaseResponse                    ((MAC_RELEASE_RESPONSE)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[6]))
#define PRIME_CL_NULL_JoinRequest                        ((MAC_JOIN_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[7]))
#define PRIME_CL_NULL_JoinResponse                       ((MAC_JOIN_RESPONSE)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[8]))
#define PRIME_CL_NULL_LeaveRequest                       ((MAC_LEAVE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[9]))
#define PRIME_CL_NULL_DataRequest                        ((MAC_DATA_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[10]))
#define PRIME_CL_NULL_PlmeResetRequest                   ((PLME_RESET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[11]))
#define PRIME_CL_NULL_PlmeSleepRequest                   ((PLME_SLEEP_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[12]))
#define PRIME_CL_NULL_PlmeResumeRequest                  ((PLME_RESUME_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[13]))
#define PRIME_CL_NULL_PlmeTestModeRequest                ((PLME_TESTMODE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[14]))
#define PRIME_CL_NULL_PlmeGetRequest                     ((PLME_GET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[15]))
#define PRIME_CL_NULL_PlmeSetRequest                     ((PLME_SET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[16]))
#define PRIME_CL_NULL_MlmeRegisterRequest                ((MLME_REGISTER_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[17]))
#define PRIME_CL_NULL_MlmeUnregisterRequest              ((MLME_UNREGISTER_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[18]))
#define PRIME_CL_NULL_MlmePromoteRequest                 ((MLME_PROMOTE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[19]))
#define PRIME_CL_NULL_MlmeDemoteRequest                  ((MLME_DEMOTE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[20]))
#define PRIME_CL_NULL_MlmeResetRequest                   ((MLME_RESET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[21]))
#define PRIME_CL_NULL_MlmeGetRequest                     ((MLME_GET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[22]))
#define PRIME_CL_NULL_MlmeListGetRequest                 ((MLME_LIST_GET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[23]))
#define PRIME_CL_NULL_MlmeSetRequest                     ((MLME_SET_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[24]))
#define PRIME_CL_432_SetCallbacks                        ((CL_432_SET_CALLBACKS)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[25]))
#define PRIME_CL_432_EstablishRequest                    ((CL_432_ESTABLISH_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[26]))
#define PRIME_CL_432_ReleaseRequest                      ((CL_432_RELEASE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[27]))
#define PRIME_CL_432_DlDataRequest                       ((CL_432_DL_DATA_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[28]))
#define PRIME_CL_NULL_MlmeMpPromoteRequest               ((MLME_MP_PROMOTE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[29]))
#define PRIME_CL_NULL_MlmeMpDemoteRequest                ((MLME_MP_DEMOTE_REQUEST)(primeApi + ((uint32_t *)(primeApi + PRIME_API_FUNCS_OFFSET))[30]))

<#else>

// *****************************************************************************
// *****************************************************************************
// Section: PRIME API Interface Routines in monolithic application
// *****************************************************************************
// *****************************************************************************

#include "stack/prime/conv/sscs/null/cl_null_api.h"
#include "stack/prime/conv/sscs/iec_4_32/cl_432_api.h"

<#if PRIME_MODE == "BN" && BN_SLAVE_EN == false>
#include "stack/prime/mngp/bmng_api.h"
</#if>

/* PRIME control functions interface - direct access to PRIME library */
#define PRIME_Initialize                                 PRIME_API_Initialize
#define PRIME_Tasks                                      PRIME_API_Tasks
#define PRIME_CL_NULL_SetCallbacks                       CL_NULL_SetCallbacks
#define PRIME_CL_NULL_EstablishRequest                   CL_NULL_EstablishRequest
#define PRIME_CL_NULL_EstablishResponse                  CL_NULL_EstablishResponse
#define PRIME_CL_NULL_ReleaseRequest                     CL_NULL_ReleaseRequest
#define PRIME_CL_NULL_ReleaseResponse                    CL_NULL_ReleaseResponse
<#if PRIME_MODE == "BN" && BN_SLAVE_EN == false>
#define PRIME_CL_NULL_RedirectResponse                   CL_NULL_RedirectResponse
</#if>
#define PRIME_CL_NULL_JoinRequest                        CL_NULL_JoinRequest
#define PRIME_CL_NULL_JoinResponse                       CL_NULL_JoinResponse
#define PRIME_CL_NULL_LeaveRequest                       CL_NULL_LeaveRequest
#define PRIME_CL_NULL_DataRequest                        CL_NULL_DataRequest
#define PRIME_CL_NULL_PlmeResetRequest                   CL_NULL_PlmeResetRequest
#define PRIME_CL_NULL_PlmeSleepRequest                   CL_NULL_PlmeSleepRequest
#define PRIME_CL_NULL_PlmeResumeRequest                  CL_NULL_PlmeResumeRequest
#define PRIME_CL_NULL_PlmeTestModeRequest                CL_NULL_PlmeTestModeRequest
#define PRIME_CL_NULL_PlmeGetRequest                     CL_NULL_PlmeGetRequest
#define PRIME_CL_NULL_PlmeSetRequest                     CL_NULL_PlmeSetRequest
<#if (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
#define PRIME_CL_NULL_MlmeRegisterRequest                CL_NULL_MlmeRegisterRequest
#define PRIME_CL_NULL_MlmeUnregisterRequest              CL_NULL_MlmeUnregisterRequest
</#if>
#define PRIME_CL_NULL_MlmePromoteRequest                 CL_NULL_MlmePromoteRequest
#define PRIME_CL_NULL_MlmeMpPromoteRequest               CL_NULL_MlmeMpPromoteRequest
<#if (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
#define PRIME_CL_NULL_MlmeDemoteRequest                  CL_NULL_MlmeDemoteRequest
#define PRIME_CL_NULL_MlmeMpDemoteRequest                CL_NULL_MlmeMpDemoteRequest
</#if>
#define PRIME_CL_NULL_MlmeResetRequest                   CL_NULL_MlmeResetRequest
#define PRIME_CL_NULL_MlmeGetRequest                     CL_NULL_MlmeGetRequest
#define PRIME_CL_NULL_MlmeListGetRequest                 CL_NULL_MlmeListGetRequest
#define PRIME_CL_NULL_MlmeSetRequest                     CL_NULL_MlmeSetRequest
#define PRIME_CL_432_SetCallbacks                        CL_432_SetCallbacks
<#if (PRIME_MODE == "BN" && BN_SLAVE_EN == true)>
#define PRIME_CL_432_EstablishRequest                    CL_432_EstablishRequest
</#if>
#define PRIME_CL_432_ReleaseRequest                      CL_432_ReleaseRequest
#define PRIME_CL_432_DlDataRequest                       CL_432_DlDataRequest
<#if PRIME_MODE == "BN" && BN_SLAVE_EN == false>
#define PRIME_BMNG_SetCallbacks                          BMNG_SetCallbacks
#define PRIME_BMNG_FUP_ClearTargetListRequest            BMNG_FUP_ClearTargetListRequest
#define PRIME_BMNG_FUP_AddTargetRequest                  BMNG_FUP_AddTargetRequest
#define PRIME_BMNG_FUP_SetFwDataRequest                  BMNG_FUP_SetFwDataRequest
#define PRIME_BMNG_FUP_SetUpgradeOptionsRequest          BMNG_FUP_SetUpgradeOptionsRequest
#define PRIME_BMNG_FUP_InitFileTxRequest                 BMNG_FUP_InitFileTxRequest
#define PRIME_BMNG_FUP_DataFrameRequest                  BMNG_FUP_DataFrameRequest
#define PRIME_BMNG_FUP_CheckCrcRequest                   BMNG_FUP_CheckCrcRequest
#define PRIME_BMNG_FUP_AbortFuRequest                    BMNG_FUP_AbortFuRequest
#define PRIME_BMNG_FUP_StartFuRequest                    BMNG_FUP_StartFuRequest
#define PRIME_BMNG_FUP_SetMatchRuleRequest               BMNG_FUP_SetMatchRuleRequest
#define PRIME_BMNG_FUP_GetVersionRequest                 BMNG_FUP_GetVersionRequest
#define PRIME_BMNG_FUP_GetStateRequest                   BMNG_FUP_GetStateRequest
#define PRIME_BMNG_FUP_SetSignatureDataRequest           BMNG_FUP_SetSignatureDataRequest
#define PRIME_BMNG_PPROF_GetRequest                      BMNG_PPROF_GetRequest
#define PRIME_BMNG_PPROF_SetRequest                      BMNG_PPROF_SetRequest
#define PRIME_BMNG_PPROF_ResetRequest                    BMNG_PPROF_ResetRequest
#define PRIME_BMNG_PPROF_RebootRequest                   BMNG_PPROF_RebootRequest
#define PRIME_BMNG_PPROF_GetEnhancedRequest              BMNG_PPROF_GetEnhancedRequest
#define PRIME_BMNG_PPROF_GetZcDiffRequest                BMNG_PPROF_GetZcDiffRequest
#define PRIME_BMNG_WHITELIST_AddRequest                  BMNG_WHITELIST_AddRequest
#define PRIME_BMNG_WHITELIST_RemoveRequest               BMNG_WHITELIST_RemoveRequest
</#if>

</#if>

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* PRIME_API_H_INCLUDE */

/*******************************************************************************
 End of File
*/

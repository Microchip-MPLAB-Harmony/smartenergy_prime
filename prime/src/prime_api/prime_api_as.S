  /* Prime Firmware Version Identifiers */
  #include "prime_api.h"

  .section .prmwrp:.text
  .thumb

  .global prime_header
  .global prime_vendor
  .global prime_model
  .global prime_version
  .global prime_functions
    
  /* PRIME WRAPPER control functions interface */
  .extern PRIME_API_Initialize
  .extern PRIME_API_Tasks

  /* PRIME CL_NULL functions interface */
  .extern CL_NULL_SetCallbacks
  .extern CL_NULL_EstablishRequest
  .extern CL_NULL_EstablishResponse
  .extern CL_NULL_ReleaseRequest
  .extern CL_NULL_ReleaseResponse
  .extern CL_NULL_JoinRequest
  .extern CL_NULL_JoinResponse
  .extern CL_NULL_LeaveRequest
  .extern CL_NULL_DataRequest
  .extern CL_NULL_PlmeResetRequest
  .extern CL_NULL_PlmeSleepRequest
  .extern CL_NULL_PlmeResumeRequest
  .extern CL_NULL_PlmeTestModeRequest
  .extern CL_NULL_PlmeGetRequest
  .extern CL_NULL_PlmeSetRequest
  .extern CL_NULL_MlmeRegisterRequest
  .extern CL_NULL_MlmeUnregisterRequest
  .extern CL_NULL_MlmePromoteRequest
  .extern CL_NULL_MlmeDemoteRequest
  .extern CL_NULL_MlmeResetRequest
  .extern CL_NULL_MlmeGetRequest
  .extern CL_NULL_MlmeListGetRequest
  .extern CL_NULL_MlmeSetRequest

  /* PRIME SSCS_432 functions interface */
  .extern CL_432_SetCallbacks
  .extern CL_432_EstablishRequest
  .extern CL_432_ReleaseRequest
  .extern CL_432_DlDataRequest
  
  .extern CL_NULL_MlmeMpPromoteRequest
  .extern CL_NULL_MlmeMpDemoteRequest

  .section .prmwrp
prime_header:
  /* Prime Firmware Version Identifiers */
prime_vendor:
  .hword PRIME_PIB_VENDOR
prime_model:
  .hword PRIME_PIB_MODEL
prime_version:
  .ascii PRIME_FW_VERSION

  /* PRIME WRAPPER control functions interface */
  .word 0
prime_functions:
  .word PRIME_API_Initialize - prime_header
  .word PRIME_API_Tasks - prime_header

  /* PRIME CL_NULL functions interface */
  .word CL_NULL_SetCallbacks - prime_header
  .word CL_NULL_EstablishRequest - prime_header
  .word CL_NULL_EstablishResponse - prime_header
  .word CL_NULL_ReleaseRequest - prime_header
  .word CL_NULL_ReleaseResponse - prime_header
  .word CL_NULL_JoinRequest - prime_header
  .word CL_NULL_JoinResponse - prime_header
  .word CL_NULL_LeaveRequest - prime_header
  .word CL_NULL_DataRequest - prime_header
  .word CL_NULL_PlmeResetRequest - prime_header
  .word CL_NULL_PlmeSleepRequest - prime_header
  .word CL_NULL_PlmeResumeRequest - prime_header
  .word CL_NULL_PlmeTestModeRequest - prime_header
  .word CL_NULL_PlmeGetRequest - prime_header
  .word CL_NULL_PlmeSetRequest - prime_header
  .word CL_NULL_MlmeRegisterRequest - prime_header
  .word CL_NULL_MlmeUnregisterRequest - prime_header
  .word CL_NULL_MlmePromoteRequest - prime_header
  .word CL_NULL_MlmeDemoteRequest - prime_header
  .word CL_NULL_MlmeResetRequest - prime_header
  .word CL_NULL_MlmeGetRequest - prime_header
  .word CL_NULL_MlmeListGetRequest - prime_header
  .word CL_NULL_MlmeSetRequest - prime_header

  /* PRIME SSCS_432 functions interface */
  .word CL_432_SetCallbacks - prime_header
  .word CL_432_EstablishRequest - prime_header
  .word CL_432_ReleaseRequest - prime_header
  .word CL_432_DlDataRequest - prime_header
  
  .word CL_NULL_MlmeMpPromoteRequest - prime_header
  .word CL_NULL_MlmeMpDemoteRequest - prime_header

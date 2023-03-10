  /* Prime Firmware Version Identifiers */
  #include "conf_prime_stack.h"

#if defined (__GNUC__)   /* Atmel Studio */
  .section .prmwrp:.text
  .thumb

  .global prime_header
  .global prime_vendor
  .global prime_model
  .global prime_version
  .global prime_functions
    
  /* PRIME WRAPPER control functions interface */
  .extern prime_stack_init
  .extern prime_stack_process

  /* PRIME CL_NULL functions interface */
  .extern cl_null_set_callbacks
  .extern cl_null_establish_request
  .extern cl_null_establish_response
  .extern cl_null_release_request
  .extern cl_null_release_response
  .extern cl_null_join_request
  .extern cl_null_join_response
  .extern cl_null_leave_request
  .extern cl_null_data_request
  .extern cl_null_plme_reset_request
  .extern cl_null_plme_sleep_request
  .extern cl_null_plme_resume_request
  .extern cl_null_plme_testmode_request
  .extern cl_null_plme_get_request
  .extern cl_null_plme_set_request
  .extern cl_null_mlme_register_request
  .extern cl_null_mlme_unregister_request
  .extern cl_null_mlme_promote_request
  .extern cl_null_mlme_demote_request
  .extern cl_null_mlme_reset_request
  .extern cl_null_mlme_get_request
  .extern cl_null_mlme_list_get_request
  .extern cl_null_mlme_set_request

#if USED_SSCS == SSCS_432
  /* PRIME SSCS_432 functions interface */
  .extern cl_432_set_callbacks
  .extern cl_432_establish_request
  .extern cl_432_release_request
  .extern cl_432_dl_data_request
#endif

#if USED_SSCS == SSCS_IPV6
  /* PRIME SSCS_IPv6 functions interface */
  .extern cl_ipv6_set_callbacks
  .extern cl_ipv6_establish_request
  .extern cl_ipv6_release_request
  .extern cl_ipv6_register_request
  .extern cl_ipv6_unregister_request
  .extern cl_ipv6_mul_join_request
  .extern cl_ipv6_mul_leave_request
  .extern cl_ipv6_data_request
#endif

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
  .word prime_stack_init - prime_header
  .word prime_stack_process - prime_header

  /* PRIME CL_NULL functions interface */
  .word cl_null_set_callbacks - prime_header
  .word cl_null_establish_request - prime_header
  .word cl_null_establish_response - prime_header
  .word cl_null_release_request - prime_header
  .word cl_null_release_response - prime_header
  .word cl_null_join_request - prime_header
  .word cl_null_join_response - prime_header
  .word cl_null_leave_request - prime_header
  .word cl_null_data_request - prime_header
  .word cl_null_plme_reset_request - prime_header
  .word cl_null_plme_sleep_request - prime_header
  .word cl_null_plme_resume_request - prime_header
  .word cl_null_plme_testmode_request - prime_header
  .word cl_null_plme_get_request - prime_header
  .word cl_null_plme_set_request - prime_header
  .word cl_null_mlme_register_request - prime_header
  .word cl_null_mlme_unregister_request - prime_header
  .word cl_null_mlme_promote_request - prime_header
  .word cl_null_mlme_demote_request - prime_header
  .word cl_null_mlme_reset_request - prime_header
  .word cl_null_mlme_get_request - prime_header
  .word cl_null_mlme_list_get_request - prime_header
  .word cl_null_mlme_set_request - prime_header

#if USED_SSCS == SSCS_432
  /* PRIME SSCS_432 functions interface */
  .word cl_432_set_callbacks - prime_header
  .word cl_432_establish_request - prime_header
  .word cl_432_release_request - prime_header
  .word cl_432_dl_data_request - prime_header
#endif

#if USED_SSCS == SSCS_IPV6
  /* PRIME SSCS_IPv6 functions interface */
  .word cl_ipv6_set_callbacks - prime_header
  .word cl_ipv6_establish_request - prime_header
  .word cl_ipv6_release_request - prime_header
  .word cl_ipv6_register_request - prime_header
  .word cl_ipv6_unregister_request - prime_header
  .word cl_ipv6_mul_join_request - prime_header
  .word cl_ipv6_mul_leave_request - prime_header
  .word cl_ipv6_data_request - prime_header
#endif

#elif defined(__CC_ARM)   /* Keil */

#else /* IAR */
  SECTION .prmwrp:CODE

  AAPCS INTERWORK, ROPI, VFP_COMPATIBLE
  PRESERVE8

  PUBLIC prime_header
  PUBLIC prime_fw_vendor
  PUBLIC prime_fw_model
  PUBLIC prime_fw_version
  PUBLIC prime_functions

  /* PRIME WRAPPER control functions interface */
  EXTERN prime_stack_init
  EXTERN prime_stack_process

  /* PRIME CL_NULL functions interface */
  EXTERN cl_null_set_callbacks
  EXTERN cl_null_establish_request
  EXTERN cl_null_establish_response
  EXTERN cl_null_release_request
  EXTERN cl_null_release_response
  EXTERN cl_null_join_request
  EXTERN cl_null_join_response
  EXTERN cl_null_leave_request
  EXTERN cl_null_data_request
  EXTERN cl_null_plme_reset_request
  EXTERN cl_null_plme_sleep_request
  EXTERN cl_null_plme_resume_request
  EXTERN cl_null_plme_testmode_request
  EXTERN cl_null_plme_get_request
  EXTERN cl_null_plme_set_request
  EXTERN cl_null_mlme_register_request
  EXTERN cl_null_mlme_unregister_request
  EXTERN cl_null_mlme_promote_request
  EXTERN cl_null_mlme_demote_request
  EXTERN cl_null_mlme_reset_request
  EXTERN cl_null_mlme_get_request
  EXTERN cl_null_mlme_list_get_request
  EXTERN cl_null_mlme_set_request
#if USED_SSCS == SSCS_432
  /* PRIME SSCS_432 functions interface */
  EXTERN cl_432_set_callbacks
  EXTERN cl_432_establish_request
  EXTERN cl_432_release_request
  EXTERN cl_432_dl_data_request
#endif
#if USED_SSCS == SSCS_IPV6
  /* PRIME SSCS_IPv6 functions interface */
  EXTERN cl_ipv6_set_callbacks
  EXTERN cl_ipv6_establish_request
  EXTERN cl_ipv6_release_request
  EXTERN cl_ipv6_register_request
  EXTERN cl_ipv6_unregister_request
  EXTERN cl_ipv6_mul_join_request
  EXTERN cl_ipv6_mul_leave_request
  EXTERN cl_ipv6_data_request
#endif

  DATA
prime_header:
  /* Prime Firmware Version Identifiers */
prime_fw_vendor:
  DC16 PRIME_PIB_VENDOR
prime_fw_model:
  DC16 PRIME_PIB_MODEL
prime_fw_version:
  DC8  PRIME_FW_VERSION

  /* PRIME WRAPPER control functions interface */
prime_functions:
  DC32 prime_stack_init - prime_header
  DC32 prime_stack_process - prime_header

  /* PRIME CL_NULL functions interface */
  DC32 cl_null_set_callbacks - prime_header
  DC32 cl_null_establish_request - prime_header
  DC32 cl_null_establish_response - prime_header
  DC32 cl_null_release_request - prime_header
  DC32 cl_null_release_response - prime_header
  DC32 cl_null_join_request - prime_header
  DC32 cl_null_join_response - prime_header
  DC32 cl_null_leave_request - prime_header
  DC32 cl_null_data_request - prime_header
  DC32 cl_null_plme_reset_request - prime_header
  DC32 cl_null_plme_sleep_request - prime_header
  DC32 cl_null_plme_resume_request - prime_header
  DC32 cl_null_plme_testmode_request - prime_header
  DC32 cl_null_plme_get_request - prime_header
  DC32 cl_null_plme_set_request - prime_header
  DC32 cl_null_mlme_register_request - prime_header
  DC32 cl_null_mlme_unregister_request - prime_header
  DC32 cl_null_mlme_promote_request - prime_header
  DC32 cl_null_mlme_demote_request - prime_header
  DC32 cl_null_mlme_reset_request - prime_header
  DC32 cl_null_mlme_get_request - prime_header
  DC32 cl_null_mlme_list_get_request - prime_header
  DC32 cl_null_mlme_set_request - prime_header

#if USED_SSCS == SSCS_432
  /* PRIME SSCS_432 functions interface */
  DC32 cl_432_set_callbacks - prime_header
  DC32 cl_432_establish_request - prime_header
  DC32 cl_432_release_request - prime_header
  DC32 cl_432_dl_data_request - prime_header
#endif

#if USED_SSCS == SSCS_IPV6
  /* PRIME SSCS_IPv6 functions interface */
  DC32 cl_ipv6_set_callbacks - prime_header
  DC32 cl_ipv6_establish_request - prime_header
  DC32 cl_ipv6_release_request - prime_header
  DC32 cl_ipv6_register_request - prime_header
  DC32 cl_ipv6_unregister_request - prime_header
  DC32 cl_ipv6_mul_join_request - prime_header
  DC32 cl_ipv6_mul_leave_request - prime_header
  DC32 cl_ipv6_data_request - prime_header
#endif

  END
#endif
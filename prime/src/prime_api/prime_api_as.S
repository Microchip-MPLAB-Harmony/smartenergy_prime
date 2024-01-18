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

  /* PRIME SSCS_432 functions interface */
  .extern cl_432_set_callbacks
  .extern cl_432_establish_request
  .extern cl_432_release_request
  .extern cl_432_dl_data_request
  .extern cl_null_mlme_mp_promote_request
  .extern cl_null_mlme_mp_demote_request

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

  /* PRIME SSCS_432 functions interface */
  .word cl_432_set_callbacks - prime_header
  .word cl_432_establish_request - prime_header
  .word cl_432_release_request - prime_header
  .word cl_432_dl_data_request - prime_header
  .word cl_null_mlme_mp_promote_request - prime_header
  .word cl_null_mlme_mp_demote_request - prime_header

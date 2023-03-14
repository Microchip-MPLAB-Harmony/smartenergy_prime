/**
 * \file
 *
 * \brief PRIME_API : PRIME WRAPPER API joins a library's existing interface into a global interface
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef PRIME_API_H_INCLUDE
#define PRIME_API_H_INCLUDE

#include "prime_api_defs.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \defgroup prime_ng_group PRIME NG
 *
 * This module provides configuration and utils for PRIME.
 */

/**
 * \ingroup prime_ng_group
 * \defgroup prime_api_group PRIME Application Interface
 *
 * This module provides configuration and utils for
 * the PRIME stack in a Service Node.
 * @{
 */

/** \brief PRIME control interface */
/* @{ */
void prime_stack_init(void *px_hal_api);
void prime_stack_process(void);

/* @} */

#ifdef PRIME_API_SEPARATED_APPS
/** Pointer to the PRIME stack */
extern uint32_t prime_api;
#define PRIME_API_FUNCS_OFFSET                    0x18
/** \brief PRIME constants located in header table (offset, type) duplets */
/* @{ */
/* define PRIME versions */
#define PRIME_1_3                                 1
#define PRIME_1_4                                 2


/* define PRIME MODE */
#define PRIME_BN                                  1
#define PRIME_SN                                  2
#define PRIME_GN                                  3

#define PRIME_MODE                                PRIME_SN

#define PRIME_VENDOR                              (*((uint16_t *)((prime_api) + (0x00))))
#define PRIME_MODEL                               (*((uint16_t *)((prime_api) + (0x02))))
#define PRIME_VERSION                             (*((char *)((prime_api) + (0x04))))
/* @} */
/** \brief PRIME control functions interface (index, type) duplets */
/* @{ */
#define prime_init                                        ((prime_init_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[0]))
#define prime_process                                     ((prime_process_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[1]))
#define prime_cl_null_set_callbacks                       ((mac_set_callbacks_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[2]))
#define prime_cl_null_establish_request                   ((mac_establish_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[3]))
#define prime_cl_null_establish_response                  ((mac_establish_response_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[4]))
#define prime_cl_null_release_request                     ((mac_release_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[5]))
#define prime_cl_null_release_response                    ((mac_release_response_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[6]))
#define prime_cl_null_join_request                        ((mac_join_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[7]))
#define prime_cl_null_join_response                       ((mac_join_response_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[8]))
#define prime_cl_null_leave_request                       ((mac_leave_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[9]))
#define prime_cl_null_data_request                        ((mac_data_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[10]))
#define prime_cl_null_plme_reset_request                  ((plme_reset_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[11]))
#define prime_cl_null_plme_sleep_request                  ((plme_sleep_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[12]))
#define prime_cl_null_plme_resume_request                 ((plme_resume_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[13]))
#define prime_cl_null_plme_testmode_request               ((plme_testmode_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[14]))
#define prime_cl_null_plme_get_request                    ((plme_get_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[15]))
#define prime_cl_null_plme_set_request                    ((plme_set_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[16]))
#define prime_cl_null_mlme_register_request               ((mlme_register_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[17]))
#define prime_cl_null_mlme_unregister_request             ((mlme_unregister_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[18]))
#define prime_cl_null_mlme_promote_request                ((mlme_promote_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[19]))
#define prime_cl_null_mlme_demote_request                 ((mlme_demote_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[20]))
#define prime_cl_null_mlme_reset_request                  ((mlme_reset_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[21]))
#define prime_cl_null_mlme_get_request                    ((mlme_get_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[22]))
#define prime_cl_null_mlme_list_get_request               ((mlme_list_get_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[23]))
#define prime_cl_null_mlme_set_request                    ((mlme_set_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[24]))
#if USED_SSCS == SSCS_432
#define prime_cl_432_set_callbacks                        ((cl_432_set_callbacks_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[25]))
#define prime_cl_432_establish_request                    ((cl_432_establish_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[26]))
#define prime_cl_432_release_request                      ((cl_432_release_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[27]))
#define prime_cl_432_dl_data_request                      ((cl_432_dl_data_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[28]))
#elif USED_SSCS == SSCS_IPV6
#define prime_cl_ipv6_set_callbacks                       ((cl_ipv6_set_callbacks_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[25]))
#define prime_cl_ipv6_establish_request                   ((cl_ipv6_establish_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[26]))
#define prime_cl_ipv6_release_request                     ((cl_ipv6_release_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[27]))
#define prime_cl_ipv6_register_request                    ((cl_ipv6_register_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[28]))
#define prime_cl_ipv6_unregister_request                  ((cl_ipv6_unregister_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[29]))
#define prime_cl_ipv6_mul_join_request                    ((cl_ipv6_mul_join_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[30]))
#define prime_cl_ipv6_mul_leave_request                   ((cl_ipv6_mul_leave_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[31]))
#define prime_cl_ipv6_data_request                        ((cl_ipv6_data_request_t)(prime_api + ((uint32_t *)(prime_api + PRIME_API_FUNCS_OFFSET))[32]))
#else
#error No SSCS defined
#endif
/* @} */
#else
#include "cl_null_api.h"
#if USED_SSCS == SSCS_432
#include "cl_432_api.h"
#elif USED_SSCS == SSCS_IPV6
#include "cl_ipv6_api.h"
#else
#error No SSCS defined
#endif
/** \brief PRIME control functions interface. Direct Access to PRIME lib */
/* @{ */
#define prime_init                                        prime_stack_init
#define prime_process                                     prime_stack_process
#define prime_cl_null_set_callbacks                       cl_null_set_callbacks
#define prime_cl_null_establish_request                   cl_null_establish_request
#define prime_cl_null_establish_response                  cl_null_establish_response
#define prime_cl_null_release_request                     cl_null_release_request
#define prime_cl_null_release_response                    cl_null_release_response
#ifdef PRIME_API_BN
#define prime_cl_null_redirect_response                   cl_null_redirect_response
#endif
#define prime_cl_null_join_request                        cl_null_join_request
#define prime_cl_null_join_response                       cl_null_join_response
#define prime_cl_null_leave_request                       cl_null_leave_request
#define prime_cl_null_data_request                        cl_null_data_request
#define prime_cl_null_plme_reset_request                  cl_null_plme_reset_request
#define prime_cl_null_plme_sleep_request                  cl_null_plme_sleep_request
#define prime_cl_null_plme_resume_request                 cl_null_plme_resume_request
#define prime_cl_null_plme_testmode_request               cl_null_plme_testmode_request
#define prime_cl_null_plme_get_request                    cl_null_plme_get_request
#define prime_cl_null_plme_set_request                    cl_null_plme_set_request
#ifndef PRIME_API_BN
#define prime_cl_null_mlme_register_request               cl_null_mlme_register_request
#define prime_cl_null_mlme_unregister_request             cl_null_mlme_unregister_request
#endif
#define prime_cl_null_mlme_promote_request                cl_null_mlme_promote_request
#ifndef PRIME_API_BN
#define prime_cl_null_mlme_demote_request                 cl_null_mlme_demote_request
#endif
#define prime_cl_null_mlme_reset_request                  cl_null_mlme_reset_request
#define prime_cl_null_mlme_get_request                    cl_null_mlme_get_request
#define prime_cl_null_mlme_list_get_request               cl_null_mlme_list_get_request
#define prime_cl_null_mlme_set_request                    cl_null_mlme_set_request
#define prime_cl_432_set_callbacks                        cl_432_set_callbacks
#ifndef PRIME_API_BN
#define prime_cl_432_establish_request                    cl_432_establish_request
#endif
#define prime_cl_432_release_request                      cl_432_release_request
#define prime_cl_432_dl_data_request                      cl_432_dl_data_request
#define prime_cl_ipv6_set_callbacks                       cl_ipv6_set_callbacks
#ifndef PRIME_API_BN
#define prime_cl_ipv6_establish_request                   cl_ipv6_establish_request
#define prime_cl_ipv6_register_request                    cl_ipv6_register_request
#define prime_cl_ipv6_unregister_request                  cl_ipv6_unregister_request
#endif
#define prime_cl_ipv6_release_request                     cl_ipv6_release_request
#define prime_cl_ipv6_mul_join_request                    cl_ipv6_mul_join_request
#define prime_cl_ipv6_mul_leave_request                   cl_ipv6_mul_leave_request
#define prime_cl_ipv6_data_request                        cl_ipv6_data_request
#ifdef PRIME_API_BN
#include "bmng_api.h"
#define prime_bmng_set_callbacks                          bmng_set_callbacks
#define prime_bmng_fup_clear_target_list_request          bmng_fup_clear_target_list_request
#define prime_bmng_fup_add_target_request                 bmng_fup_add_target_request
#define prime_bmng_fup_set_fw_data_request                bmng_fup_set_fw_data_request
#define prime_bmng_fup_set_upg_options_request            bmng_fup_set_upg_options_request
#define prime_bmng_fup_init_file_tx_request               bmng_fup_init_file_tx_request
#define prime_bmng_fup_data_frame_request                 bmng_fup_data_frame_request
#define prime_bmng_fup_check_crc_request                  bmng_fup_check_crc_request
#define prime_bmng_fup_abort_fu_request                   bmng_fup_abort_fu_request
#define prime_bmng_fup_start_fu_request                   bmng_fup_start_fu_request
#define prime_bmng_fup_set_match_rule_request             bmng_fup_set_match_rule_request
#define prime_bmng_fup_get_version_request                bmng_fup_get_version_request
#define prime_bmng_fup_get_state_request                  bmng_fup_get_state_request
#define prime_bmng_fup_set_signature_data_request         bmng_fup_set_signature_data_request
#define prime_bmng_pprof_get_request                      bmng_pprof_get_request
#define prime_bmng_pprof_set_request                      bmng_pprof_set_request
#define prime_bmng_pprof_reset_request                    bmng_pprof_reset_request
#define prime_bmng_pprof_reboot_request                   bmng_pprof_reboot_request
#define prime_bmng_pprof_get_enhanced_request             bmng_pprof_get_enhanced_request
#define prime_bmng_pprof_zc_diff_request                  bmng_pprof_zc_diff_request
#define prime_bmng_whitelist_add_request                  bmng_whitelist_add_request
#define prime_bmng_whitelist_remove_request               bmng_whitelist_remove_request
#endif
/* @} */
#endif

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* PRIME_API_H_INCLUDE */

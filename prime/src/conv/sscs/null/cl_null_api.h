/**
 * \file
 *
 * \brief CL_NULL_API: PRIME NULL API Convergence Sublayer
 *
 * Copyright (c) 2021 Atmel Corporation. All rights reserved.
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

#ifndef CONV_CL_NULL_API_H
#define CONV_CL_NULL_API_H

/* System includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* MAC includes */
#include "mac_defs.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \weakgroup prime_sscs_null_group
 * @{
 */

/** \brief CL NULL functions interface */
/* @{ */
void cl_null_set_callbacks(mac_callbacks_t *px_prime_cbs);

/** \brief CL NULL_ESTABLISH_PRIMITIVES */
/* @{ */
void cl_null_establish_request(uint8_t *puc_eui48, uint8_t uc_type, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_arq, uint8_t uc_cfbytes, uint8_t uc_ae);
void cl_null_establish_response(uint16_t us_con_handle, mac_establish_response_answer_t uc_answer, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_ae);

/* @} */

/** \brief CL NULL_RELEASE_PRIMITIVES */
/* @{ */
void cl_null_release_request(uint16_t us_con_handle);
void cl_null_release_response(uint16_t us_con_handle, mac_release_response_answer_t uc_answer);

/* @} */

/** \brief CL NULL JOIN PRIMITIVES */
/* @{ */
void cl_null_join_request(mac_join_mode_t us_broadcast, uint16_t us_con_handle, uint8_t *puc_eui48, uint8_t uc_con_type, uint8_t *puc_data,
		uint16_t us_data_len, uint8_t uc_ae);
void cl_null_join_response(uint16_t us_con_handle, uint8_t *puc_eui48, mac_join_response_answer_t uc_answer, uint8_t uc_ae);

/* @} */

/** \brief CL NULL LEAVE PRIMITIVES */
/* @{ */
void cl_null_leave_request(uint16_t us_con_handle, uint8_t *puc_eui48);

/* @} */

#ifdef PRIME_API_BN
/** \brief CL NULL SIGNALING PRIMITIVES */
/* @{ */
void cl_null_redirect_response(uint16_t us_con_handle, uint8_t *puc_eui48, uint8_t *puc_data, uint16_t us_data_len);
#endif

/* @} */

/** \brief CL NULL DATA PRIMITIVES */
/* @{ */
void cl_null_data_request(uint16_t us_con_handle, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_prio, uint32_t ul_time_ref);

/* @} */

/** \brief CL NULL PLME PRIMITIVES */
/* @{ */
void cl_null_plme_reset_request(uint16_t us_pch);
void cl_null_plme_sleep_request(uint16_t us_pch);
void cl_null_plme_resume_request(uint16_t us_pch);
void cl_null_plme_testmode_request(uint8_t uc_enable, uint8_t uc_mode, uint8_t uc_modulation, uint8_t uc_pwr_level, uint16_t us_pch);
void cl_null_plme_get_request(uint16_t us_pib_attrib, uint16_t us_pch);
void cl_null_plme_set_request(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size, uint16_t us_pch);

/* @} */

/** \brief CL NULL MLME PRIMITIVES */
/* @{ */
#if (PRIME_MODE == PRIME_SN)
void cl_null_mlme_register_request(uint8_t *puc_sna, uint8_t uc_sid);
void cl_null_mlme_unregister_request(void);
void cl_null_mlme_demote_request(void);
#endif

void cl_null_mlme_promote_request(uint8_t *puc_eui48, uint8_t uc_bcn_mode);
void cl_null_mlme_reset_request(void);
void cl_null_mlme_get_request(uint16_t us_pib_attrib);
void cl_null_mlme_list_get_request(uint16_t us_pib_attrib);
void cl_null_mlme_set_request(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size);

/* @} */

/* @} */

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* CONV_CL_NULL_API_H */

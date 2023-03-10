/**
 * \file
 *
 * \brief CL_432_DEFS: CONV 432 layer
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
#ifndef CL_432_DEFS_H_INCLUDE
#define CL_432_DEFS_H_INCLUDE

#include <stdint.h>
#include "conf_prime_stack.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \ingroup prime_ng_group
 * \defgroup prime_sscs_432_group PRIME SSCS 4-32
 *
 * This module provides configuration and utils for the
 * SSCS 4-32 in PRIME.
 *
 * @{
 */

/** Length of the LPDU */
#define LPDU_HEADER                    3

/** \brief LSDU Data length */
/** \note It must be smaller than PRIME_MACSAP_DATA_SIZE */
/* @{ */
#define MAX_LENGTH_432_DATA            (1024 - LPDU_HEADER)
/* @} */

/** RESULTS values for convergence layer primitives */
typedef enum {
	DL_432_RESULT_SUCCESS = 0,
	DL_432_RESULT_REJECT = 1,
	DL_432_RESULT_TIMEOUT = 2,
	DL_432_RESULT_NOT_REGISTERED = 6
} dl_432_result_t;

/** \brief Transmission errors defined in 432 layer */
/* @{ */
typedef enum {
	/* Standard errors */
	CL_432_TX_STATUS_SUCCESS                           = 0,
	CL_432_TX_STATUS_TIMEOUT                           = 2,
	/* Errors from the MAC layer */
	CL_432_TX_STATUS_ERROR_MAC_SENDING                 = 0x80,
	CL_432_TX_STATUS_ERROR_MAC_BUSY                    = 0x81,
	CL_432_TX_STATUS_ERROR_MAC_NO_FREE_BUFFERS         = 0x82,
	CL_432_TX_STATUS_ERROR_MAC_CON_CLOSED              = 0x83,
	CL_432_TX_STATUS_ERROR_MAC_RECEIVING_DATA          = 0x84,
	/* Errors from the CL 4-32 */
	CL_432_TX_STATUS_ERROR_BAD_ADDRESS                 = 0xC0,
	CL_432_TX_STATUS_ERROR_BAD_HANLDER                 = 0xC1,
	CL_432_TX_STATUS_ERROR_BUSY                        = 0xC2,
	CL_432_TX_STATUS_ERROR_BAD_DST_ADDRESS             = 0xC3
} dl_432_tx_status_t;
/* @} */

/** LSDU part of DL message */
#define lsdu                           dl.buff

/** Buffer defined for reception/transmission */
typedef union {
	uint8_t lpdu[MAX_LENGTH_432_DATA + LPDU_HEADER];

	struct {
		uint8_t control;
		uint8_t dsap;
		uint8_t lsap;
		uint8_t buff[MAX_LENGTH_432_DATA];
	} dl;
} dl_432_buffer_t;

/** \brief Addresses defined in 432 layer */
/* @{ */
#define CL_432_INVALID_ADDRESS                 (0xFFFF)
#define CL_432_BROADCAST_ADDRESS               (0x0FFF)
/* @} */

/** \brief Connection status */
/* @{ */
#define CL_432_CON_CLOSE              (0)
#define CL_432_CON_CONNECTING         (1)
#define CL_432_CON_DISCONNECTING      (2)
#define CL_432_CON_OPEN               (3)
/* @} */

/** PRIME CL 432 declarations */

/**
 * CL 432 Establish request
 *
 * - puc_device_id:         Pointer to the device identifier data
 * - uc_device_id_len:      Length of the device identfier
 * - uc_ae (v1.4):          Flag to indicate that authentication and encryption is requested
 */
typedef void (*cl_432_establish_request_t)(uint8_t *puc_device_id, uint8_t uc_device_id_len, uint8_t uc_ae);

/**
 * CL 432 Establish confirm
 *
 * - puc_device_id:          Pointer to the device identifier data
 * - uc_device_id_len:       Length of the device identfier
 * - us_dst_address:         Destination 432 Address
 * - us_base_address:        Base 432 Address
 * - uc_ae (v1.4):           Flag to indicate that authentication and encryption is requested
 */
typedef void (*cl_432_establish_cfm_cb_t)(uint8_t *puc_device_id, uint8_t uc_device_id_len, uint16_t us_dst_address, uint16_t us_base_address, uint8_t uc_ae);

/**
 * CL 432 Release request
 *
 * - us_dst_address:   Address to disconnect
 */
typedef void (*cl_432_release_request_t)(uint16_t us_dst_address);

/**
 * CL 432 Data request
 *
 * - uc_dst_lsap:      Destination LSAP
 * - uc_src_lsap:      Source LSAP
 * - us_dst_address:   Destination 432 Address
 * - px_buff:          Pointer to the data buffer
 * - us_lsdu_len:      Length of the data
 * - uc_link_class:    Link class (non used)
 */
typedef void (*cl_432_dl_data_request_t)(uint8_t uc_dst_lsap, uint8_t uc_src_lsap, uint16_t us_dst_address, dl_432_buffer_t *px_buff, uint16_t us_lsdu_len,
		uint8_t uc_link_class);

/**
 * CL 432 Data indication
 *
 * - uc_dst_lsap:      Destination LSAP
 * - uc_src_lsap:      Source LSAP
 * - us_dst_address:   Destination 432 Address
 * - src_address:      Source Address
 * - puc_data:         Pointer to received data
 * - us_lsdu_len:      Length of the data
 * - uc_link_class:    Link class (non used)
 */
typedef void (*cl_432_dl_data_ind_cb_t)(uint8_t uc_dst_lsap, uint8_t uc_src_lsap, uint16_t us_dst_address, uint16_t us_src_address,
		uint8_t *puc_data, uint16_t us_lsdu_len, uint8_t uc_link_class);

/**
 * CL 432 Data confirm
 *
 * - uc_dst_lsap:      Destination LSAP
 * - uc_src_lsap:      Source LSAP
 * - us_dst_address:   Destination 432 Address
 * - uc_tx_status:     Tx status (see mac_defs.h)
 */
typedef void (*cl_432_dl_data_cfm_cb_t)(uint8_t uc_dst_lsap, uint8_t uc_src_lsap, uint16_t us_dst_address, dl_432_tx_status_t uc_tx_status);

#ifdef PRIME_API_BN

/**
 * CL 432 Join Indication
 *
 * - puc_device_id:          Pointer to the device identifier data
 * - uc_device_id_len:       Length of the device identfier
 * - us_dst_address:         Destination 432 Address
 * - puc_mac:                Pointer to mac address
 * - uc_ae (v1.4):           Flag to indicate that authentication and encryption is requested
 */
typedef void (*cl_432_join_ind_cb_t)(uint8_t *puc_device_id, uint8_t uc_device_id_len, uint16_t us_dst_address, uint8_t *puc_mac, uint8_t uc_ae);

/**
 * CL 432 Leave Indication
 *
 * - us_dst_address:   Destination 432 Address
 */
typedef void (*cl_432_leave_ind_cb_t)(uint16_t us_dst_address);
#else

/**
 * CL 432 Release confirm
 *
 * - us_dst_address:   Destination 432 Address
 * - uc_result:        Confirmation result
 */
typedef void (*cl_432_release_cfm_cb_t)(uint16_t us_dst_address, dl_432_result_t uc_result);
#endif

/** CL432 callbacks configuration */
typedef struct {
	cl_432_dl_data_ind_cb_t cl_432_dl_data_ind_cb;
	cl_432_dl_data_cfm_cb_t cl_432_dl_data_cfm_cb;
#ifdef PRIME_API_BN
	cl_432_join_ind_cb_t cl_432_join_ind_cb;
	cl_432_leave_ind_cb_t cl_432_leave_ind_cb;
#else
	cl_432_establish_cfm_cb_t cl_432_establish_cfm_cb;
	cl_432_release_cfm_cb_t cl_432_release_cfm_cb;
#endif
} cl_432_callbacks_t;

/**
 * CL432 Set callback functions
 * - px_prime_cbs:  Pointer to 432 callbacks structure
 */
typedef void (*cl_432_set_callbacks_t)(cl_432_callbacks_t *px_cl_432_cbs);

/* @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
#endif

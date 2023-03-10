/**
 * \file
 *
 * \brief BMNG_DEFS: Base Management Definitions header file
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

#ifndef BMNG_DEFS_H_
#define BMNG_DEFS_H_

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \ingroup prime_ng_group
 * \defgroup prime_mngp_group PRIME Base Management
 *
 * This module provides configuration and utils for the Base
 * Management in PRIME.
 *
 * @{
 */

/** Page size for FU */
typedef enum {
	PAGE_SIZE_AUTO = 0,
	PAGE_SIZE_32,
	PAGE_SIZE_64,
	PAGE_SIZE_128,
	PAGE_SIZE_192
} fup_page_size_t;

/** FUP ACK codes  */
typedef enum {
	FUP_ACK_OK = 0,
	FUP_ACK_ERROR,
	FUP_ACK_ERROR_MAC,
	FUP_ACK_ERROR_MODEL,
	FUP_ACK_ERROR_CRC,
	FUP_ACK_ERROR_DATA,
	FUP_ACK_ERROR_CRC_FILE,
	FUP_ACK_CRC_ONGOING,
	FUP_ACK_FU_ONGOING
} fup_ack_code_t;

/** FUP error codes  */
typedef enum {
	FUP_ERROR_NODE_NO_ERROR = 0,
	FUP_ERROR_NODE_WRONGSTATE_EXEC,
	FUP_ERROR_NODE_WRONGSTATE_UPG,
	FUP_ERROR_NODE_WRONGSTATE,
	FUP_ERROR_NODE_WRONGSTATE_RCV,
	FUP_ERROR_NODE_WRONGSTATE_CNTDWN,
	FUP_ERROR_NODE_FW_NOTMATCH,
	FUP_ERROR_NODE_REVERT_ERRORINIT,
	FUP_ERROR_NODE_REVERT_ERR7,
	FUP_ERROR_NODE_RETRY_KILL,
	FUP_ERROR_NODE_UNICAST_TIMEOUT,
	FUP_ERROR_NODE_CONFIRM_IND,
	FUP_ERROR_NODE_REVERT_CRCNOK,
	FUP_ERROR_NODE_RESTART,
	FUP_ERROR_NODE_WRONG_MISS_BITMAP,
	FUP_ERROR_NODE_WRONG_MISS_LIST,
	FUP_ERROR_NODE_VENDOR_INVALID,
	FUP_ERROR_NODE_MODEL_NOTMATCH,
	FUP_WARNING_NODE_ALREADY_UPDATED,
	FUP_WARNING_NODE_ALREADY_EXECUTED,
	FUP_WARNING_NODE_LINK_QUALITY
} fup_error_code_t;

#define FUP_STATE_ENDED_NOTIFICATION 0x80

/** Valid node upgrade state */
typedef enum {
	FUP_NODE_STATE_IDLE        = 0,
	FUP_NODE_STATE_RECEIVING   = 1,
	FUP_NODE_STATE_COMPLETE    = 2,
	FUP_NODE_STATE_COUNTDOWN   = 3,
	FUP_NODE_STATE_UPGRADE     = 4,
	FUP_NODE_STATE_EXCEPTION   = 5, /* Only 1.4 */
	FUP_NODE_STATE_UNKNOWN     = 0x7F
} fup_node_state_t;

/** Network events */
typedef enum {
	BMNG_NET_EVENT_REGISTER,
	BMNG_NET_EVENT_UNREGISTER,
	BMNG_NET_EVENT_PROMOTE,
	BMNG_NET_EVENT_DEMOTE,
	BMNG_NET_EVENT_ALIVE,
	BMNG_NET_EVENT_REBOOT,
	BMNG_NET_EVENT_NO_DUK, /* Only 1.4 */
	BMNG_NET_EVENT_UNKNOWN_NODE
} bmng_event_t;

/** Network event information */
typedef struct {
	bmng_event_t net_event;
	uint8_t puc_eui48[6];
	uint8_t sid;
	uint16_t lnid;
	uint8_t lsid;
	uint8_t alv_rx_cnt;
	uint8_t alv_tx_cnt;
	uint8_t alv_time;
} bmng_net_event_t;

/** PRIME Profile ACK codes  */
typedef enum {
	PPROF_ACK_OK = 0,
	PPROF_ACK_ERROR
} pprof_ack_code_t;

/** PRIME Whitelist ACK codes  */
typedef enum {
	WHITELIST_ACK_OK = 0,
	WHITELIST_ACK_ERROR
} whitelist_ack_code_t;

/** PRIME BMNG declarations */

/**
 * Base Management FUP Acknowledgement
 * - uc_cmd:             Command being acknowledged
 * - x_ack_code:         ACK returned code
 * - us_extra_info:      Extra information
 */
typedef void (*bmng_fup_ack_cb_t)(uint8_t uc_cmd, fup_ack_code_t x_ack_code, uint16_t us_extra_info);

/**
 * Base Management FUP Status Indication
 * - x_fup_node_state:   Node upgrade state
 * - ul_pages:           Pages received by the SN according to the BN
 * - puc_eui48:          MAC address of the node
 */
typedef void (*bmng_fup_status_ind_cb_t)(fup_node_state_t x_state, uint16_t ul_pages, uint8_t *puc_eui48);

/**
 * Base Management FUP Error Indication
 * - x_error_code:       Error code
 * - puc_eui48:          MAC address of the node
 */
typedef void (*bmng_fup_error_ind_cb_t)(fup_error_code_t x_error_code, uint8_t *puc_eui48);

/**
 * Base Management FUP Version Indication
 * - puc_eui48:          MAC address of the node
 * - uc_vendor_len:      Vendor ID length
 * - pch_vendor:         Vendor ID
 * - uc_model_len:       Model length
 * - pch_model:          Model
 * - uc_version_len:     Version length
 * - pch_version:        Version
 */
typedef void (*bmng_fup_version_ind_cb_t)(uint8_t *puc_eui48, uint8_t uc_vendor_len, char *pch_vendor, uint8_t uc_model_len,
		char *pch_model, uint8_t uc_version_len, char *pch_version);

/**
 * Base Management FUP Kill Indication
 * - puc_eui48:          MAC address of the node
 */
typedef void (*bmng_fup_kill_ind_cb_t)(uint8_t *puc_eui48);

/**
 * Base Management Network Event Indication
 * - px_net_event:          Network event information
 */
typedef void (*bmng_network_event_ind_cb_t)(bmng_net_event_t *px_net_event);

/**
 * Base Management PRIME Profile Acknowledgement
 * - uc_cmd:             Command being acknowledged
 * - x_ack_code:         ACK returned code
 */
typedef void (*bmng_pprof_ack_cb_t)(uint8_t uc_cmd, pprof_ack_code_t x_ack_code);

/**
 * Base Management PRIME Profile Get Response
 * - puc_eui48:            MAC address of the node
 * - us_data_length:       Data length
 * - puc_data:             Data
 */
typedef void (*bmng_pprof_get_response_cb_t)(uint8_t *puc_eui48, uint16_t us_data_len, uint8_t *puc_data);

/**
 * Base Management PRIME Profile Get Enhanced Response
 * - puc_eui48:            MAC address of the node
 * - us_data_length:       Data length
 * - puc_data:             Data
 */
typedef void (*bmng_pprof_get_enhanced_response_cb_t)(uint8_t *puc_eui48, uint16_t us_data_len, uint8_t *puc_data);

/**
 * Base Management PRIME Profile Get Zero Crossing Response
 * - puc_eui48:            MAC address of the node
 * - uc_zc_status:         Zero cross status
 * - ul_zc_time:           Zero crossing time (in microsec)
 */
typedef void (*bmng_pprof_get_zc_response_cb_t)(uint8_t *puc_eui48, uint8_t uc_zc_status, uint32_t ul_zc_time);

/**
 * Base Management PRIME Profile Zero Crossing Difference between BN and SN Response
 * - puc_eui48:            MAC address of the node
 * - ul_time_freq:         Frequency
 * - ul_time_diff:         Zero crossing difference (in 10s of microsec)
 */
typedef void (*bmng_pprof_zc_diff_response_cb_t)(uint8_t *puc_eui48, uint32_t ul_time_freq, uint32_t ul_time_diff);

/**
 * Base Management Whitelist Acknowledgement
 * - uc_cmd:             Command being acknowledged
 * - x_ack_code:         ACK returned code
 */
typedef void (*bmng_whitelist_ack_cb_t)(uint8_t uc_cmd, whitelist_ack_code_t x_ack_code);

/** Base Management callbacks configuration */
typedef struct {
	bmng_fup_ack_cb_t fup_ack_cb;
	bmng_fup_status_ind_cb_t fup_status_ind_cb;
	bmng_fup_error_ind_cb_t fup_error_ind_cb;
	bmng_fup_version_ind_cb_t fup_version_ind_cb;
	bmng_fup_kill_ind_cb_t fup_kill_ind_cb;
	bmng_network_event_ind_cb_t network_event_ind_cb;
	bmng_pprof_ack_cb_t pprof_ack_cb;
	bmng_pprof_get_response_cb_t pprof_get_response_cb;
	bmng_pprof_get_enhanced_response_cb_t pprof_get_enhanced_response_cb;
	bmng_pprof_get_zc_response_cb_t pprof_get_zc_response_cb;
	bmng_pprof_zc_diff_response_cb_t pprof_zc_diff_response_cb;
	bmng_whitelist_ack_cb_t whitelist_ack_cb;
} bmng_callbacks_t;

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* BMNG_DEFS_H_ */

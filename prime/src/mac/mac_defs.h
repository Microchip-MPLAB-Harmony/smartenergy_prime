/**
 * \file
 *
 * \brief MAC_DEFS: PRIME MAC definitions
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

#ifndef MAC_DEFS_H_INCLUDE
#define MAC_DEFS_H_INCLUDE

/* System includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
 * \defgroup prime_mac_group PRIME MAC Layer
 *
 * This module provides configuration and utils for
 * the MAC layer of PRIME.
 * @{
 */

/** \brief MAC invalid values */
/* @{ */
#define MAC_INVALID_SID                         0xFF
#define MAC_INVALID_LNID                        0x3FFF
#define MAC_BROADCAST_LNID                      0x3FFF
#define MAC_MULTICAST_LNID                      0x3FFE
#define MAC_MUL_SW_LEAVE_LNID                   0x0
#define MAC_INVALID_HANDLER                     0xFFFF
#define MAC_INVALID_LCID                        0x7FFF
/* @} */

/** \brief MAC Reserved LCID*/
/* @{ */
#define LCI_CL_IPV4_BROADCAST                   1
#define LCI_CL_432_BROADCAST                    2
/* @} */


/** \brief MAC Reserved handlers*/
/* @{ */
#define BN_DIRECT_CON_HANDLER                   3
#define BN_DIRECT_CON_ERROR_HANDLER             4

#define SN_DIRECT_CON_HANDLER_MSK               0x0080
/* @} */

/** PRIME version customer values */
typedef struct {
	char fw_version[16];
	char fw_model[16];
	char fw_vendor[16];
	uint16_t pib_vendor;
	uint16_t pib_model;
} mac_version_info_t;

/** PLME result values */
typedef enum {
	PLME_RESULT_SUCCESS     = 0,
	PLME_RESULT_FAILED      = 1,
	PLME_RESULT_REJECTED    = 2,
	PLME_RESULT_BADATTR     = 2,
} plme_result_t;

/** MLME result values */
typedef enum {
	MLME_RESULT_DONE          = 0,
	MLME_RESULT_FAILED        = 1,
	MLME_RESULT_REJECTED      = 1,
	MLME_RESULT_TIMEOUT       = 2,
	MLME_RESULT_NOSUCHDEVICE  = 4,
	MLME_RESULT_NOSNA         = 8,
	MLME_RESULT_NOSWITCH      = 9,
	MLME_RESULT_REDUNDANT     = 10,
	MLME_RESULT_BADATTR       = 11,
	MLME_RESULT_OUTOFRANGE    = 12,
	MLME_RESULT_READONLY      = 13
} mlme_result_t;

/** Connection types */
typedef enum {
	MAC_CONNECTION_INVALID_TYPE         = 0,
	MAC_CONNECTION_IPV4_AR_TYPE         = 1,
	MAC_CONNECTION_IPV4_UNICAST_TYPE    = 2,
	MAC_CONNECTION_CL_432_TYPE          = 3,
	MAC_CONNECTION_MNGT_TYPE            = 4,
	MAC_CONNECTION_IPV6_AR_TYPE         = 5,
	MAC_CONNECTION_IPV6_UNICAST_TYPE    = 6,
} connection_type_t;

/** MAC SAP result values for MAC_ESTABLISH.confirm primitive */
typedef enum {
	MAC_ESTABLISH_CONFIRM_RESULT_SUCCESS            = 0,
	MAC_ESTABLISH_CONFIRM_RESULT_REJECT             = 1,
	MAC_ESTABLISH_CONFIRM_RESULT_TIMEOUT            = 2,
	MAC_ESTABLISH_CONFIRM_RESULT_NO_BANDWIDTH       = 3,
	MAC_ESTABLISH_CONFIRM_RESULT_NO_SUCH_DEVICE     = 4,
	MAC_ESTABLISH_CONFIRM_RESULT_REDIRECT_FAILED    = 5,
	MAC_ESTABLISH_CONFIRM_RESULT_NOT_REGISTERED     = 6,
	MAC_ESTABLISH_CONFIRM_RESULT_NO_MORE_LCIDS      = 7,
	MAC_ESTABLISH_CONFIRM_RESULT_DC_NO_SUPPORTED    = 8,
	MAC_ESTABLISH_CONFIRM_RESULT_UNSUPPORTED_SP     = 14,
	MAC_ESTABLISH_CONFIRM_RESULT_PROCCESS_ACTIVE    = 0x80
} mac_establish_confirm_result_t;

/** Values for the answer parameter in MAC_ESTABLISH.response primitive */
typedef enum {
	MAC_ESTABLISH_RESPONSE_ANSWER_ACCEPT    = 0,
	MAC_ESTABLISH_RESPONSE_ANSWER_REJECT    = 1,
} mac_establish_response_answer_t;

/** Values for the reason parameter in MAC_RELEASE.indication primitive */
typedef enum {
	MAC_RELEASE_INDICATION_REASON_SUCCESS   = 0,
	MAC_RELEASE_INDICATION_REASON_ERROR     = 1,
} mac_release_indication_reason_t;

/** Values for the answer parameter in MAC_RELEASE.response primitive */
typedef enum {
	MAC_RELEASE_RESPONSE_ACCEPT = 0,
} mac_release_response_answer_t;

/** Values for the result parameter in MAC_RELEASE.confirm primitive */
typedef enum {
	MAC_RELEASE_CONFIRM_RESULT_SUCCESS          = 0,
	MAC_RELEASE_CONFIRM_RESULT_TIMEOUT          = 2,
	MAC_RELEASE_CONFIRM_RESULT_NOT_REGISTERED   = 6,
	MAC_RELEASE_CONFIRM_RESULT_PROCCESS_ACTIVE  = 0x80,
	MAC_RELEASE_CONFIRM_RESULT_BAD_HANDLER      = 0x81,
	MAC_RELEASE_CONFIRM_RESULT_NOT_OPEN_CONN    = 0x82,
	MAC_RELEASE_CONFIRM_RESULT_ERROR_SENDING    = 0x83,
	MAC_RELEASE_CONFIRM_RESULT_BAD_FLOW_MODE    = 0x84,
} mac_release_confirm_result_t;

/** Values of the Result parameter in MAC_DATA.confirm primitive */
typedef enum {
	MAC_DATA_SUCCESS                            = 0,
	MAC_DATA_TIMEOUT                            = 2,
	MAC_DATA_ERROR_SENDING                      = 0x80,
	MAC_DATA_ERROR_PROCESSING_PREVIOUS_REQUEST  = 0x81,
	MAC_DATA_ERROR_NO_FREE_BUFFERS              = 0x82,
	MAC_DATA_ERROR_CON_CLOSED                   = 0x83,
	MAC_DATA_ERROR_RECEIVING_DATA               = 0x84
} mac_data_result_t;

/** Type of join request */
typedef enum {
	REQUEST_MULTICAST       = 0,
	REQUEST_BROADCAST       = 1,
} mac_join_mode_t;

/** Reserved handler multicast connection */
#define MAC_RESERVED_HANDLER      0

/** Values for the result parameter in MAC_JOIN.confirm primitive */
typedef enum {
	JOIN_CONFIRM_SUCCESS          = 0,
	JOIN_CONFIRM_FAILURE          = 1,
	JOIN_CONFIRM_TIMEOUT          = 2,
	NOT_REGISTERED                = 6,
	JOIN_CONFIRM_UNSUPPORTED_SP   = 14
} mac_join_confirm_result_t;

/** Values for the result parameter in MAC_JOIN.response primitive */
typedef enum {
	JOIN_RESPONSE_ACCEPT    = 0,
	JOIN_RESPONSE_REJECT    = 1,
} mac_join_response_answer_t;

/** Values for the result parameter in MAC_LEAVE.confirm primitive */
typedef enum {
	LEAVE_CONFIRM_ACCEPT    = 0,
	LEAVE_CONFIRM_TIMEOUT   = 1,
	LEAVE_CONFIRM_RESULT_PROCCESS_ACTIVE  = 0x80,
	LEAVE_CONFIRM_RESULT_BAD_HANDLER      = 0x81,
	LEAVE_CONFIRM_RESULT_NOT_OPEN_CONN    = 0x82,
	LEAVE_CONFIRM_RESULT_ERROR_SENDING    = 0x83,
	LEAVE_CONFIRM_RESULT_BAD_FLOW_MODE    = 0x84,
	LEAVE_CONFIRM_RESULT_NOT_REGISTERED   = 0x85,
} mac_leave_confirm_result_t;

/** Certification test modes prime 1.3 and prime 1.4 */
typedef enum {
	PHY_TYPE_A_FRAME = 0,
	PHY_TYPE_B_FRAME = 2,
	PHY_TYPE_BC_FRAME = 3
} cert_prime_frame_type_t;

/** Certification test parameters */
typedef struct {
	uint16_t us_cert_messg_count;
	uint8_t uc_cert_modulation;
	uint8_t us_cert_signal_att;
	uint8_t us_cert_duty_cycle;
	uint8_t us_cert_prime_frame_type;
} mlme_certification_parameter_t;

/** Certification modes */
typedef enum {
	NO_CERTIFICATION_MODE      = 0,
	PHY_CERTIFICATION_MODE     = 1,
	MAC_CERTIFICATION_MODE     = 2,
	PHY_CERTIFICATION_MODE_1_4 = 3,
} cert_mode_t;

/** Rejection actions */
typedef enum {
	REJECT_PRO_REQ_S      = 0,
	REJECT_PRM            = 1,
	REJECT_CON_REQ_S      = 2,
	REJECT_REG_REQ        = 3,
} cert_reject_action_t;

/** PRIME MAC declarations */

/**
 * MAC Establish Request
 * - puc_eui48:     Address of the node to which this connection will be addressed
 * - uc_type:       Convergence Layer type of the connection
 * - puc_data:      Data associated with the connection establishment procedure
 * - us_data_len:   Length of the data in bytes
 * - uc_arq:        Flag to indicate whether or not the ARQ mechanism should be used for this connection
 * - uc_cfbytes:    Flag to indicate whether or not the connection should use the contention or contention-free channel access scheme
 * - uc_ae (v1.4):  Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_establish_request_t)(uint8_t *puc_eui48, uint8_t uc_type, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_arq, uint8_t uc_cfbytes, uint8_t uc_ae);

/**
 * MAC Establish Indication
 * - us_con_handle: Unique identifier of the connection
 * - puc_eui48:     Address of the node which initiates the connection establish procedure
 * - uc_type:       Convergence Layer type of the connection
 * - puc_data:      Data associated with the connection establishment procedure
 * - us_data_len:   Length of the data in bytes
 * - uc_cfbytes:    Flag to indicate whether or not the connection should use the contention or contention-free channel access scheme
 * - uc_ae (v1.4):  Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_establish_ind_cb_t)(uint16_t us_con_handle, uint8_t *puc_eui48, uint8_t uc_type, uint8_t *puc_data,
		uint16_t us_data_len, uint8_t uc_cfbytes, uint8_t uc_ae);

/**
 * MAC Establish Confirm
 * - us_con_handle: Unique identifier of the connection
 * - uc_result:     Result of the connection establishment process
 * - puc_eui48:     Address of the node to which this connection is being established
 * - uc_type:       Convergence Layer type of the connection
 * - puc_data:      Data associated with the connection establishment procedure
 * - us_data_len:   Length of the data in bytes
 * - uc_ae (v1.4):  Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_establish_cfm_cb_t)(uint16_t us_con_handle, mac_establish_confirm_result_t uc_result,
		uint8_t *puc_eui48, uint8_t uc_type, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_ae);

/**
 * MAC Establish Response
 * - us_con_handle: Unique identifier of the connection
 * - uc_answer:     Action to be taken for this connection establishment
 * - puc_data:      Data associated with the connection establishment procedure
 * - us_data_len:   Length of the data in bytes
 * - uc_ae (v1.4):  Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_establish_response_t)(uint16_t us_con_handle, mac_establish_response_answer_t uc_answer, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_ae);

/**
 * MAC Release Response
 * - us_con_handle: Unique identifier of the connection
 * - uc_answer:     Action to be taken for this connection release procedure
 */
typedef void (*mac_release_response_t)(uint16_t us_con_handle, mac_release_response_answer_t uc_answer);

/**
 * MAC Release Request
 * - us_con_handle: Unique identifier of the connection
 */
typedef void (*mac_release_request_t)(uint16_t us_con_handle);

/**
 * MAC Release Indication
 * - us_con_handle: Unique identifier of the connection
 * - uc_reason:     Cause of the connection release
 */
typedef void (*mac_release_ind_cb_t)(uint16_t us_con_handle, mac_release_indication_reason_t uc_reason);

/**
 * MAC Release Confirm
 * - us_con_handle: Unique identifier of the connection
 * - uc_result:     Result of the connection release process
 */
typedef void (*mac_release_cfm_cb_t)(uint16_t us_con_handle, mac_release_confirm_result_t uc_result);

/**
 * MAC Redirect Response
 * - us_con_handle: Unique identifier of the connection
 * - puc_eui48_dst: Address of the node to which this connection will be addressed
 * - puc_data:      Data associated with the connection establishment procedure
 * - us_data_len:   Length of the data in bytes
 */
typedef void (*mac_redirect_response_t)(uint16_t us_con_handle, uint8_t *puc_eui48_dst, uint8_t *puc_data, uint16_t us_data_len);

/**
 * MAC Join Request
 * - us_broadcast:      Join type (broadcast or multicast connection)
 * - us_con_handle:     Unique identifier of the connection (only used for base node)
 * - puc_eui48:         Address of the node to which this join is being requested (only used for base node)
 * - uc_con_type:       Connection type
 * - puc_data:          Data associated with the join request procedure
 * - us_data_len:       Length of the data in bytesn
 * - uc_ae (v1.4):      Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_join_request_t)(mac_join_mode_t us_broadcast, uint16_t us_con_handle, uint8_t *puc_eui48, connection_type_t uc_con_type, uint8_t *puc_data,
		uint16_t us_data_len, uint8_t uc_ae);

/**
 * MAC Join Indication
 * - us_con_handle:        Unique identifier of the connection
 * - puc_eui48:            Address of the node which wishes to join the multicast group (only valid for base node)
 * - uc_con_type:          Connection type
 * - puc_data:             Data associated with the join request procedure
 * - us_data_len:          Length of the data in bytes
 * - uc_ae (v1.4):         Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_join_ind_cb_t)(uint16_t us_con_handle, uint8_t *puc_eui48, uint8_t uc_con_type, uint8_t *puc_data,
		uint16_t us_data_len, uint8_t uc_ae);

/**
 * MAC Join Response
 * - us_con_handle:        Unique identifier of the connection
 * - puc_eui48:            Address of the node which requested the multicast group join (only used for base node)
 * - uc_answer:            Action to be taken for this join request
 * - uc_ae (v1.4):         Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_join_response_t)(uint16_t us_con_handle, uint8_t *puc_eui48, mac_join_response_answer_t uc_answer, uint8_t uc_ae);

/**
 * MAC Join Confirm
 * - us_con_handle:        Unique identifier of the connection
 * - uc_result:            Result of the join request process
 * - uc_ae (v1.4):         Flag to indicate that authentication and encryption is requested
 */
typedef void (*mac_join_cfm_cb_t)(uint16_t us_con_handle, mac_join_confirm_result_t uc_result, uint8_t uc_ae);

/**
 * MAC Leave Request
 * - us_con_handle:        Unique identifier of the connection
 * - puc_eui48:            Address of the node to be removed from the multicast group (only used for base node)
 */
typedef void (*mac_leave_request_t)(uint16_t us_con_handle, uint8_t *puc_eui48);

/**
 * MAC Leave Confirm
 * - us_con_handle:        Unique identifier of the connection
 * - uc_result:            Result of the leave request process
 */
typedef void (*mac_leave_cfm_cb_t)(uint16_t us_con_handle, mac_leave_confirm_result_t uc_result);

/**
 * MAC Leave Indication
 * - us_con_handle:        Unique identifier of the connection
 * - puc_eui48:            Address of the node to remove from the multicast group (only valid for base node)
 */
typedef void (*mac_leave_ind_cb_t)(uint16_t us_con_handle, uint8_t *puc_eui48);

/**
 * MAC Data Request
 * - us_con_handle:          Unique identifier of the connection
 * - puc_data:               Pointer to data to be transmitted through this connection
 * - us_data_len:            Length of the data in bytes
 * - uc_prio:                Priority of the data to be sent when using the CSMA access scheme
 * - ul_time_ref (v1.4):     Time reference (in 10s of microsec)
 */
typedef void (*mac_data_request_t)(uint16_t us_con_handle, uint8_t *puc_data, uint16_t us_data_len, uint8_t uc_prio, uint32_t ul_time_ref);

/**
 * MAC Data Confirm
 * - us_con_handle:    Unique identifier of the connection
 * - puc_data:         Pointer to data to be transmitted through this connection
 * - drt_result:       Result of the transmission (MAC_DATA_SUCCESS, MAC_DATA_TIMEOUT, MAC_DATA_ERROR_SENDING, ...)
 */
typedef void (*mac_data_cfm_cb_t)(uint16_t us_con_handle, uint8_t *puc_data, mac_data_result_t drt_result);

/**
 * MAC Data Indication
 * - us_con_handle:          Unique identifier of the connection
 * - puc_data:               Pointer to data to be received through this connection
 * - us_data_len:            Length of the data in bytes
 * - ul_time_ref (v1.4):     Time reference (in 10s of microsec)
 */
typedef void (*mac_data_ind_cb_t)(uint16_t us_con_handle, uint8_t *puc_data, uint16_t us_data_len, uint32_t ul_time_ref);

/**
 * PLME Reset Request
 * - us_pch:             Physical channel
 */
typedef void (*plme_reset_request_t)(uint16_t us_pch);

/**
 * PLME Reset Confirm
 * - x_result:           Result
 * - us_pch:             Physical channel
 */
typedef void (*plme_reset_cfm_cb_t)(plme_result_t x_result, uint16_t us_pch);

/**
 * PLME Sleep Request
 * - us_pch:             Physical channel
 */
typedef void (*plme_sleep_request_t)(uint16_t us_pch);

/**
 * PLME Sleep Confirm
 * - x_result:           Result
 * - us_pch:             Physical channel
 */
typedef void (*plme_sleep_cfm_cb_t)(plme_result_t x_result, uint16_t us_pch);

/**
 * PLME Resume Request
 * - us_pch:             Physical channel
 */
typedef void (*plme_resume_request_t)(uint16_t us_pch);

/**
 * PLME Resume Confirm
 * - x_result:           Result
 * - us_pch:             Physical channel
 */
typedef void (*plme_resume_cfm_cb_t)(plme_result_t x_result, uint16_t us_pch);

/**
 * PLME Testmode Request
 * - uc_enable:         Start/Stop test mode
 * - uc_mode:           Transmission mode
 * - uc_modulation:     Transmission modulation
 * - uc_pwr_level:      Transmission power level
 * - us_pch:            Physical channel
 */
typedef void (*plme_testmode_request_t)(uint8_t uc_enable, uint8_t uc_mode, uint8_t uc_modulation, uint8_t uc_pwr_level, uint16_t us_pch);

/**
 * PLME Testmode Confirm
 * - x_result:           Result
 * - us_pch:             Physical channel
 */
typedef void (*plme_testmode_cfm_cb_t)(plme_result_t x_result, uint16_t us_pch);

/**
 * PLME Get Request
 * - us_pib_attrib:      PIB attribute
 * - us_pch:             Physical channel
 */
typedef void (*plme_get_request_t)(uint16_t us_pib_attrib, uint16_t us_pch);

/**
 * PLME Get Confirm
 * - x_status:           Status
 * - us_pib_attrib:      PIB attribute
 * - pv_pib_value:       PIB attribute value
 * - uc_pib_size:        PIB attribute value size
 * - us_pch:             Physical channel
 */
typedef void (*plme_get_cfm_cb_t)(plme_result_t x_status, uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size, uint16_t us_pch);

/**
 * PLME Set Request
 * - us_pib_attrib:      PIB attribute
 * - pv_pib_value:       PIB attribute value
 * - uc_pib_size:        PIB attribute value size
 * - us_pch:             Physical channel
 */
typedef void (*plme_set_request_t)(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size, uint16_t us_pch);

/**
 * PLME Set Confirm
 * - x_result:           Result
 * - us_pch:             Physical channel
 */
typedef void (*plme_set_cfm_cb_t)(plme_result_t x_result, uint16_t us_pch);

/**
 * MLME Register Request
 * - puc_sna:      Pointer to SNA
 * - uc_sid:       Switch Identifier
 */
typedef void (*mlme_register_request_t)(uint8_t *puc_sna, uint8_t uc_sid);

/**
 * MLME Register Confirm
 * - x_result:     Result
 * - puc_sna:      Pointer to SNA
 * - uc_sid:       Switch Identifier
 */
typedef void (*mlme_register_cfm_cb_t)(mlme_result_t x_result, uint8_t *puc_sna, uint8_t uc_sid);

/**
 * MLME Register Indication
 * - puc_sna:      Pointer to SNA
 * - uc_sid:       Switch Identifier
 */
typedef void (*mlme_register_ind_cb_t)(uint8_t *puc_sna, uint8_t uc_sid);

/**
 * MLME Unregister Request
 */
typedef void (*mlme_unregister_request_t)(void);

/**
 * MLME Unregister Confirm
 * - x_result:      Result
 */
typedef void (*mlme_unregister_cfm_cb_t)(mlme_result_t x_result);

/**
 * MLME Unregister Indication
 */
typedef void (*mlme_unregister_ind_cb_t)(void);

/**
 * MLME Promote Request
 * - puc_eui48:            Address of the node to be promoted (NULL in Service Node)
 * - uc_bcn_mode (v1.4):   Beacon mode
 */
typedef void (*mlme_promote_request_t)(uint8_t *puc_eui48, uint8_t uc_bcn_mode);

/**
 * MLME Promote Confirm
 * - x_result:      Result
 */
typedef void (*mlme_promote_cfm_cb_t)(mlme_result_t x_result);

/**
 * MLME Promote Indication
 */
typedef void (*mlme_promote_ind_cb_t)(void);

/**
 * MLME Demote Request
 */
typedef void (*mlme_demote_request_t)(void);

/**
 * MLME Demote Confirm
 * - x_result:      Result
 */
typedef void (*mlme_demote_cfm_cb_t)(mlme_result_t x_result);

/**
 * MLME Demote Indication
 */
typedef void (*mlme_demote_ind_cb_t)(void);

/**
 * MLME Reset Request
 */
typedef void (*mlme_reset_request_t)(void);

/**
 * MLME Reset Confirm
 * - x_result:           Result
 */
typedef void (*mlme_reset_cfm_cb_t)(mlme_result_t x_result);

/**
 * MLME Get Request
 * - us_pib_attrib:      PIB attribute
 */
typedef void (*mlme_get_request_t)(uint16_t us_pib_attrib);

/**
 * MLME Get Confirm
 * - x_status:           Status
 * - us_pib_attrib:      PIB attribute
 * - pv_pib_value:       PIB attribute value
 * - uc_pib_size:        PIB attribute value size
 */
typedef void (*mlme_get_cfm_cb_t)(mlme_result_t x_status, uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size);

/**
 * MLME List Get Request
 * - us_pib_attrib:      PIB attribute
 */
typedef void (*mlme_list_get_request_t)(uint16_t us_pib_attrib);

/**
 * MLME List Get Confirm
 * - x_status:           Status
 * - us_pib_attrib:      PIB attribute
 * - puc_pib_buff:       Buffer with PIB attribute values
 * - us_pib_len:         Buffer length
 * NOTE: The buffer contains an array of records according to the PRIME specification. Values are stored starting with the MSB.
 */
typedef void (*mlme_list_get_cfm_cb_t)(mlme_result_t x_status, uint16_t us_pib_attrib, uint8_t *puc_pib_buff, uint16_t us_pib_len);

/**
 * MLME Set Request
 * - us_pib_attrib:      PIB attribute
 * - pv_pib_value:       PIB attribute value
 * - uc_pib_size:        PIB attribute value size
 */
typedef void (*mlme_set_request_t)(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size);

/**
 * MLME Set Confirm
 * - x_result:           Result
 */
typedef void (*mlme_set_cfm_cb_t)(mlme_result_t x_result);

/* @} */

/** MAC callbacks configuration */
typedef struct {
	mac_establish_ind_cb_t mac_establish_ind_cb;
	mac_establish_cfm_cb_t mac_establish_cfm_cb;
	mac_release_ind_cb_t mac_release_ind_cb;
	mac_release_cfm_cb_t mac_release_cfm_cb;
	mac_join_ind_cb_t mac_join_ind_cb;
	mac_join_cfm_cb_t mac_join_cfm_cb;
	mac_leave_ind_cb_t mac_leave_ind_cb;
	mac_leave_cfm_cb_t mac_leave_cfm_cb;
	mac_data_ind_cb_t mac_data_ind_cb;
	mac_data_cfm_cb_t mac_data_cfm_cb;
	plme_reset_cfm_cb_t plme_reset_cfm_cb;
	plme_sleep_cfm_cb_t plme_sleep_cfm_cb;
	plme_resume_cfm_cb_t plme_resume_cfm_cb;
	plme_testmode_cfm_cb_t plme_testmode_cfm_cb;
	plme_get_cfm_cb_t plme_get_cfm_cb;
	plme_set_cfm_cb_t plme_set_cfm_cb;
	mlme_register_ind_cb_t mlme_register_ind_cb;
	mlme_register_cfm_cb_t mlme_register_cfm_cb;
	mlme_unregister_ind_cb_t mlme_unregister_ind_cb;
	mlme_unregister_cfm_cb_t mlme_unregister_cfm_cb;
	mlme_promote_ind_cb_t mlme_promote_ind_cb;
	mlme_promote_cfm_cb_t mlme_promote_cfm_cb;
	mlme_demote_ind_cb_t mlme_demote_ind_cb;
	mlme_demote_cfm_cb_t mlme_demote_cfm_cb;
	mlme_reset_cfm_cb_t mlme_reset_cfm_cb;
	mlme_get_cfm_cb_t mlme_get_cfm_cb;
	mlme_list_get_cfm_cb_t mlme_list_get_cfm_cb;
	mlme_set_cfm_cb_t mlme_set_cfm_cb;
} mac_callbacks_t;

/**
 * MAC Set callback functions
 * - px_prime_cbs:  Pointer to mac callbacks struct
 */
typedef void (*mac_set_callbacks_t)(mac_callbacks_t *px_prime_cbs);

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* MAC_DEFS_H_INCLUDE */

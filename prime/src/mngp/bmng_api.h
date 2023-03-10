/**
 * \file
 *
 * \brief BMNG_API: Base Management API header file
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

#ifndef BMNG_API_H_
#define BMNG_API_H_

#include <stdint.h>
#include "bmng_defs.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \weakgroup prime_mngp_group
 * @{
 */

/** \brief Base Management functions interface */
/* @{ */

/**
 * \brief Function to establish Base Management callback functions
 *
 * \param px_base_mng_cbs      Pointer to the callback struct
 */
void bmng_set_callbacks(bmng_callbacks_t *px_base_mng_cbs);

/**
 * \brief Base Management FUP Clear Target List Request
 *
 * \param uc_cmd             Command to acknowledge
 */
void bmng_fup_clear_target_list_request(uint8_t uc_cmd);

/**
 * \brief Base Management FUP Add Target Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node to be added
 */
void bmng_fup_add_target_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management FUP Set Firmware Data Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param uc_vendor_len      Vendor ID length
 * \param pch_vendor         Vendor ID
 * \param uc_model_len       Model length
 * \param pch_model          Model
 * \param uc_version_len     Version length
 * \param pch_version        Version
 */
void bmng_fup_set_fw_data_request(uint8_t uc_cmd, uint8_t uc_vendor_len, char *pch_vendor, uint8_t uc_model_len, char *pch_model, uint8_t uc_version_len,
		char *pch_version);

/**
 * \brief Base Management FUP Set Upgrade Options Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param uc_arq_en          Enable/Disable ARQ protocol (1/0)
 * \param x_page_size        Page size
 * \param uc_multicast_en    Enable/Disable multicast (1/0)
 * \param ul_delay_restart   Delay restart time in seconds
 * \param ul_safety_timer    Safety timer in seconds
 */
void bmng_fup_set_upg_options_request(uint8_t uc_cmd, uint8_t uc_arq_en, fup_page_size_t x_page_size, uint8_t uc_multicast_en, uint32_t ul_delay_restart,
		uint32_t ul_safety_timer);

/**
 * \brief Base Management FUP Init File Transmission Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param us_frame_number    Frame number (0x0000)
 * \param ul_file_size       File size
 * \param us_frame_size      Frame size
 * \param ul_crc             File CRC-32
 */
void bmng_fup_init_file_tx_request(uint8_t uc_cmd, uint16_t us_frame_number, uint32_t ul_file_size, uint16_t us_frame_size, uint32_t ul_crc);

/**
 * \brief Base Management FUP Data Frame Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param us_frame_number    Frame number
 * \param us_data_len        Date length
 * \param puc_data           Data frame
 */
void bmng_fup_data_frame_request(uint8_t uc_cmd, uint16_t us_frame_number, uint16_t us_data_len, uint8_t *puc_data);

/**
 * \brief Base Management CRC Request
 *
 * \param uc_cmd             Command to acknowledge
 */
void bmng_fup_check_crc_request(uint8_t uc_cmd);

/**
 * \brief Base Management FUP Abort FU Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node to be added
 * \note If the MAC address is FF:FF:FF:FF:FF:FF, the FU is aborted for all nodes.
 */
void bmng_fup_abort_fu_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management FUP Start FU Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param uc_enable          Enable/Disable FU (1/0)
 */
void bmng_fup_start_fu_request(uint8_t uc_cmd, uint8_t uc_enable);

/**
 * \brief Base Management FUP Set Match Rule Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param uc_rules           Match rules: 0000 0MV0
 * \note If M and/or V are set, only the nodes matching model and/or vendor will be upgraded.
 */
void bmng_fup_set_match_rule_request(uint8_t uc_cmd, uint8_t uc_rules);

/**
 * \brief Base Management FUP Version Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node to be added
 * \note If the MAC address is FF:FF:FF:FF:FF:FF, version information is asked to all registered nodes.
 */
void bmng_fup_get_version_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management FUP State Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node
 * \note If the MAC address is FF:FF:FF:FF:FF:FF, state information will be asked to all registered nodes.
 */
void bmng_fup_get_state_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management FUP Set Signature Data Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param uc_algorithm         Used algorithm to check signature
 * \param us_length            Signature length
 */
void bmng_fup_set_signature_data_request(uint8_t uc_cmd, uint8_t uc_algorithm, uint16_t us_length);

/**
 * \brief Base Management PRIME Profile Get Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 * \param us_data_len          Data length
 * \param puc_data             Data
 */
void bmng_pprof_get_request(uint8_t uc_cmd, uint8_t *puc_eui48, uint16_t us_data_len, uint8_t *puc_data);

/**
 * \brief Base Management PRIME Profile Set Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 * \param us_data_len          Data length
 * \param puc_data             Data
 */
void bmng_pprof_set_request(uint8_t uc_cmd, uint8_t *puc_eui48, uint16_t us_data_len, uint8_t *puc_data);

/**
 * \brief Base Management PRIME Profile Reset Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 */
void bmng_pprof_reset_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management PRIME Profile Reboot Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 */
void bmng_pprof_reboot_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * \brief Base Management PRIME Profile Get Enhanced Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 * \param us_data_len          Data length
 * \param puc_data             Data
 */
void bmng_pprof_get_enhanced_request(uint8_t uc_cmd, uint8_t *puc_eui48, uint16_t us_data_len, uint8_t *puc_data);

/**
 * \brief Base Management PRIME Profile Zero Crossing Difference between BN and SN Request
 *
 * \param uc_cmd               Command to acknowledge
 * \param puc_eui48            MAC address of the node
 */
void bmng_pprof_zc_diff_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * Base Management Whitelist Add Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node to be added
 */
void bmng_whitelist_add_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/**
 * Base Management Whitelist Remove Request
 *
 * \param uc_cmd             Command to acknowledge
 * \param puc_eui48          MAC address of the node to be removed
 */
void bmng_whitelist_remove_request(uint8_t uc_cmd, uint8_t *puc_eui48);

/* @} */

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* BMNG_API_H_ */

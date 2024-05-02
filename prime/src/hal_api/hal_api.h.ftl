/**
 * \file
 *
 * \brief HAL: PRIME Hardware Abstraction Layer Header
 *
 * Copyright (c) 2024 Atmel Corporation. All rights reserved.
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

#ifndef HAL_H_INCLUDE
#define HAL_H_INCLUDE

/* System includes */
#include <stdint.h>
#include <stdbool.h>
#include "service/storage/srv_storage.h"
#include "service/user_pib/srv_user_pib.h"
#include "service/reset_handler/srv_reset_handler.h"
#include "service/pcrc/srv_pcrc.h"
#include "service/random/srv_random.h"
#include "service/log_report/srv_log_report.h"
#include "service/usi/srv_usi.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \ingroup prime_ng_group
 * \defgroup prime_hal_group PRIME Hardware Abstraction Layer
 *
 * This module provides configuration and utils for HAL layer.
 *
 * @{
 */

/** Signature algorithms */
typedef enum {
	HAL_FU_NO_SIGNATURE = 0,
	HAL_FU_RSA3072_SHA256,
	HAL_FU_ECDSA256_SHA256,
} hal_fu_signature_algorithm_t;

/** FU information */
typedef struct {
	uint32_t image_size;
	uint8_t page_size;
	hal_fu_signature_algorithm_t sign_algorithm;
	uint16_t sign_length;
} hal_fu_info_t;

/** FU result */
typedef enum {
	HAL_FU_SUCCESS,          /* Request to restart with new image */
	HAL_FU_CANCEL,           /* The FU has been killed */
	HAL_FU_CRC_ERROR,        /* CRC error */
	HAL_FU_FW_ERROR,         /* (Deprecated) */
	HAL_FU_FW_REVERT,        /* Request to restart with old image */
	HAL_FU_FW_CONFIRM,       /* The FU has been confirmed */
	HAL_FU_ERROR,            /* Error during FU */
	HAL_FU_SIGNATURE_ERROR,  /* Signature error (only PRIME 1.4) */
	HAL_FU_IMAGE_ERROR       /* Image verification (model/vendor) failed (only PRIME 1.4) */
} hal_fu_result_t;

/** FU verification result */
typedef enum {
	HAL_FU_VERIF_SUCCESS,
	HAL_FU_SIGNATURE_FAIL,
	HAL_FU_IMAGE_FAIL
} hal_fu_verif_result_t;

/** \brief HAL functions interface */
/* @{ */
typedef void (*hal_wrp_iface_t)(void);

/** HAL functions wrapper interface */
/* @{ */
typedef void (*hal_restart_system_t)(SRV_RESET_HANDLER_RESET_CAUSE resetType);
typedef uint32_t (*hal_pcrc_calc_t)(uint8_t *puc_buf, size_t ul_len, PCRC_HEADER_TYPE uc_header_type, PCRC_CRC_TYPE uc_crc_type, uint32_t initValue, bool b_v14_mode);
typedef void (*hal_pcrc_config_sna_t)(uint8_t *puc_sna);
typedef void (*hal_fu_data_read_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef uint8_t (*hal_fu_data_write_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef void (*hal_fu_data_cfg_read_t)(void *pv_dst, uint16_t us_size);
typedef uint8_t (*hal_fu_data_cfg_write_t)(void *pv_src, uint16_t us_size);
typedef void (*hal_fu_start_t)(hal_fu_info_t *x_fu_info);
typedef void (*hal_fu_end_t)(hal_fu_result_t uc_hal_res);
typedef void (*hal_fu_revert_t)(void);
typedef void (*hal_fu_crc_calculate_t)(void);
typedef void (*hal_fu_crc_set_callback_t)(void (*p_handler)(uint32_t ul_crc));
typedef void (*hal_fu_signature_image_check_t)(void);
typedef void (*hal_fu_signature_image_check_set_callback_t)(void (*p_handler)(hal_fu_verif_result_t uc_result));
typedef uint16_t (*hal_fu_get_bitmap_t)(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages);
typedef void (*hal_plc_init_t)(void);
typedef void (*hal_plc_reset_t)(void);
typedef void (*hal_plc_set_handler_t)(void (*p_handler)(void));
typedef void (*hal_plc_tx_signal_t)(void);
typedef void (*hal_plc_rx_signal_t)(void);
typedef bool (*hal_get_config_info_t)(SRV_STORAGE_TYPE cfg_type, uint8_t us_size, void *pv_data);
typedef bool (*hal_set_config_info_t)(SRV_STORAGE_TYPE cfg_type, uint8_t us_size, void *pv_data);
typedef SRV_USI_HANDLE (*hal_usi_open_t)(const SYS_MODULE_INDEX index);
typedef void (*hal_usi_set_callback_t)(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback);
typedef void (*hal_usi_send_cmd_t)(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, uint8_t *data, size_t length);
typedef uint32_t (*hal_trng_read_t)(void);
typedef void (*hal_debug_report_t)(SRV_LOG_REPORT_LEVEL logLevel, SRV_LOG_REPORT_CODE code,const char *info, ...);
typedef uint32_t (*hal_net_get_freq_t)(void);
typedef void (*hal_pib_get_request_t)(uint16_t us_pib_attrib);
typedef void (*hal_pib_get_request_set_callback_t)(void (*p_handler)(uint8_t uc_result, uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size));
typedef void (*hal_pib_set_request_t)(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size);
typedef void (*hal_pib_set_request_set_callback_t)(void (*p_handler)(uint8_t uc_result));
typedef void (*hal_aes_init_t)(void);
typedef void (*hal_aes_set_callback_t)(void (*p_handler)(void));
typedef void (*hal_aes_key_t)(uint8_t *puc_key, uint8_t uc_key_len);
typedef void (*hal_aes_crypt_t)(bool b_crypt_mode, uint8_t *puc_in_text, uint8_t *puc_out_text);

/* HAL USI Null Device Interface */
typedef uint16_t (*hal_null_dev_read_callback_t)(uint8_t chn, void *buffer, uint16_t len);
typedef uint16_t (*hal_null_dev_write_callback_t)(uint8_t chn, const void *buffer, uint16_t len);
bool hal_null_dev_set_read_callback(hal_null_dev_read_callback_t ptr_func);
bool hal_null_dev_set_write_callback(hal_null_dev_write_callback_t ptr_func);

<#if primePal.PRIME_PAL_PLC_EN == true>
typedef bool (*hal_plc_send_boot_cmd_t)(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read);
typedef bool (*hal_plc_send_wrrd_cmd_t)(uint8_t uc_cmd, void *px_spi_data, void *px_spi_status_info);
typedef void (*hal_plc_enable_interrupt_t)(bool enable);
typedef void (*hal_plc_delay_t)(uint8_t uc_tref, uint32_t ul_delay);
typedef bool (*hal_plc_set_stby_mode_t)(bool sleep);
typedef bool (*hal_plc_get_thermal_warning_t)(void);
</#if>

#ifdef HAL_NWK_RECOVERY_INTERFACE
typedef uint32_t (*hal_nwk_recovery_init_t)(void);
typedef void (*hal_nwk_recovery_read_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef uint8_t (*hal_nwk_recovery_write_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
#endif

typedef void (*hal_swap_stack_t)(uint8_t uc_traffic);

/* Timer of 1us service interface */
typedef uint32_t (*hal_timer_1us_get_t)(void);
typedef bool (*hal_timer_1us_set_int_t)(uint32_t ul_time_us, bool b_relative, void (*p_handler)(uint32_t), uint32_t *pul_int_id);
typedef bool (*hal_timer_1us_cancel_int_t)(uint32_t ul_int_id);
typedef void (*hal_timer_1us_enable_interrupt_t)(bool b_enable);
/* @} */

<#if primePal.PRIME_PAL_RF_EN == true>
typedef uint8_t (*hal_prf_if_init_t)(void);
typedef void (*hal_prf_if_reset_t)(void);
typedef void (*hal_prf_if_enable_interrupt_t)(bool b_enable);
typedef void (*hal_prf_if_set_handler_t)(void (*p_handler)(void));
typedef bool (*hal_prf_if_send_spi_cmd_t)(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode);
typedef bool (*hal_prf_if_is_spi_busy_t)(void);
typedef void (*hal_prf_if_led_t)(uint8_t uc_led_id, bool b_led_on);
</#if>

/** Structure of HAL functions interface */
typedef struct {
	hal_restart_system_t restart_system;

	hal_pcrc_calc_t pcrc_calc;
	hal_pcrc_config_sna_t pcrc_config_sna;

	hal_fu_data_read_t fu_data_read;
	hal_fu_data_write_t fu_data_write;
	hal_fu_data_cfg_read_t fu_data_cfg_read;
	hal_fu_data_cfg_write_t fu_data_cfg_write;
	hal_fu_start_t fu_start;
	hal_fu_end_t fu_end;
	hal_fu_revert_t fu_revert;
	hal_fu_crc_calculate_t fu_crc_calculate;
	hal_fu_crc_set_callback_t fu_crc_set_callback;
	hal_fu_signature_image_check_t fu_signature_image_check;
	hal_fu_signature_image_check_set_callback_t fu_signature_image_check_set_callback;
	hal_fu_get_bitmap_t fu_get_bitmap;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_init_t plc_init;
	hal_plc_reset_t plc_reset;
	hal_plc_set_handler_t plc_set_handler;
	hal_plc_tx_signal_t plc_tx_signal;
	hal_plc_rx_signal_t plc_rx_signal;
</#if>

	hal_get_config_info_t get_config_info;
	hal_set_config_info_t set_config_info;

    hal_usi_open_t usi_open;
	hal_usi_set_callback_t usi_set_callback;
	hal_usi_send_cmd_t usi_send_cmd;

	hal_trng_read_t trng_read;

	hal_debug_report_t debug_report;

	hal_net_get_freq_t net_get_freq;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_send_boot_cmd_t plc_send_boot_cmd;
	hal_plc_send_wrrd_cmd_t plc_send_wrrd_cmd;
	hal_plc_enable_interrupt_t plc_enable_int;
	hal_plc_delay_t plc_delay;
</#if>

#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init_t nwk_recovery_init;
	hal_nwk_recovery_read_t nwk_recovery_read;
	hal_nwk_recovery_write_t nwk_recovery_write;
#endif

	hal_pib_get_request_t pib_get_request;
	hal_pib_get_request_set_callback_t pib_get_request_set_callback;
	hal_pib_set_request_t pib_set_request;
	hal_pib_set_request_set_callback_t pib_set_request_set_callback;

	hal_aes_init_t aes_init;
	hal_aes_set_callback_t aes_set_callback;
	hal_aes_key_t aes_key;
	hal_aes_crypt_t aes_crypt;
	hal_swap_stack_t swap_stack;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_set_stby_mode_t plc_set_stby_mode;
	hal_plc_get_thermal_warning_t plc_get_thermal_warning;
</#if>

	/* New functions must be added at the end */


	hal_timer_1us_get_t timer_1us_get;
	hal_timer_1us_set_int_t timer_1us_set_int;
	hal_timer_1us_cancel_int_t timer_1us_cancel_int;
	hal_timer_1us_enable_interrupt_t timer_1us_enable_interrupt;

<#if primePal.PRIME_PAL_RF_EN == true>
	hal_prf_if_init_t prf_if_init;
	hal_prf_if_reset_t prf_if_reset;
	hal_prf_if_enable_interrupt_t prf_if_enable_interrupt;
	hal_prf_if_set_handler_t prf_if_set_handler;
	hal_prf_if_send_spi_cmd_t prf_if_send_spi_cmd;
	hal_prf_if_is_spi_busy_t prf_if_is_spi_busy;
	hal_prf_if_led_t prf_if_led;
</#if>

} hal_api_t;




/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* HAL_H_INCLUDE */

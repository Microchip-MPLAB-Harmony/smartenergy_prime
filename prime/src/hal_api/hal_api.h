/**
 * \file
 *
 * \brief HAL: PRIME Hardware Abstraction Layer Header
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

#ifndef HAL_H_INCLUDE
#define HAL_H_INCLUDE

/* System includes */
#include <stdint.h>
#include <stdbool.h>


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

/** \brief General Purpose Timers identificators */
typedef enum {
	HAL_GP_PLC_TIMER = 0,
	HAL_GP_TIMER_NUM
} hal_gp_timer_t;

/** \brief Timer mode */
typedef enum {
	HAL_TIMER_MODE_16_BITS = 0,
	HAL_TIMER_MODE_32_BITS,
	HAL_TIMER_MODE_NUM
} hal_timer_mode_t;

/** \brief Timer CLK source */
typedef enum {
	HAL_TIMER_CLK_SRC_CLK1 = (0x0u << 0),  /* TC_CMR_TCCLKS_TIMER_CLOCK1 */
	HAL_TIMER_CLK_SRC_CLK2 = (0x1u << 0),  /* TC_CMR_TCCLKS_TIMER_CLOCK2 */
	HAL_TIMER_CLK_SRC_CLK3 = (0x2u << 0),  /* TC_CMR_TCCLKS_TIMER_CLOCK3 */
	HAL_TIMER_CLK_SRC_CLK4 = (0x3u << 0),  /* TC_CMR_TCCLKS_TIMER_CLOCK4 */
	HAL_TIMER_CLK_SRC_CLK5 = (0x4u << 0),  /* TC_CMR_TCCLKS_TIMER_CLOCK5 */
} hal_timer_clk_src_t;

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

/* Type of information relative to configuration parameters */
typedef enum {
	CONFIG_TYPE_MAC_INFO = 0,
	CONFIG_TYPE_PHY_INFO = 1,
	CONFIG_TYPE_BN_INFO  = 2,
	CONFIG_TYPE_MODE_PRIME = 3,
	CONFIG_TYPE_SECURITY = 4,
	CONFIG_TYPE_BOOT_INFO = 5,
	CONFIG_TYPE_END_LIST
} config_info_type_t;

/** \brief PLC Universal Serial Interface */
/* @{ */
/** Management Plane Protocol Spec and ATMEL serialized protocols */
typedef enum {
	PROTOCOL_MNGP_PRIME           = 0x00,
	PROTOCOL_MNGP_PRIME_GETQRY    = 0x00,
	PROTOCOL_MNGP_PRIME_GETRSP    = 0x01,
	PROTOCOL_MNGP_PRIME_SET       = 0x02,
	PROTOCOL_MNGP_PRIME_RESET     = 0x03,
	PROTOCOL_MNGP_PRIME_REBOOT    = 0x04,
	PROTOCOL_MNGP_PRIME_FU        = 0x05,
	PROTOCOL_MNGP_PRIME_GETQRY_EN = 0x06,
	PROTOCOL_MNGP_PRIME_GETRSP_EN = 0x07,
	PROTOCOL_SNIF_PRIME           = 0x13,
	PROTOCOL_PHY_SERIAL           = 0x1F,
	PROTOCOL_PHY_TESTER           = 0x22,
	PROTOCOL_PRIME_API            = 0x30,
	PROTOCOL_INTERNAL             = 0x3F,
	PROTOCOL_USER_DEFINED         = 0xFE,
	PROTOCOL_INVALID              = 0xFF
} usi_protocol_t;


/** \brief HAL functions interface */
/* @{ */
typedef void (*hal_wrp_iface_t)(void);

/** HAL functions wrapper interface */
/* @{ */
typedef void (*hal_restart_system_t)(void);
typedef void (*hal_set_gp_timer_handler_t)(hal_gp_timer_t gpt, void (*p_handler)(void));
typedef void (*hal_clear_gp_timer_handler_t)(hal_gp_timer_t gpt);
typedef uint8_t (*hal_timer_init_t)(hal_gp_timer_t gpt, hal_timer_mode_t mode, hal_timer_clk_src_t clk_src);
typedef uint32_t (*hal_timer_count_get_t)(hal_gp_timer_t gpt);
typedef void (*hal_timer_stop_t)(hal_gp_timer_t gpt, hal_timer_mode_t mode);
typedef void (*hal_timer_plc_init_t)(uint32_t uc_time_us);
typedef void (*hal_timer_plc_stop_t)(void);
typedef void (*hal_set_plc_timer_handler_t)(void (*p_handler)(void));
typedef uint32_t (*hal_pcrc_calc_t)(uint8_t *puc_buf, uint32_t ul_len, uint8_t uc_header_type, uint8_t uc_crc_type, bool b_v14_mode);
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
typedef int8_t (*hal_plc_cmd_op_t)(uint8_t uc_cmd, uint16_t us_addr, uint16_t us_len, uint8_t *ptr_buf, uint8_t uc_bytes_rep);
typedef void (*hal_plc_set_handler_t)(void (*p_handler)(void));
typedef void (*hal_plc_tx_signal_t)(void);
typedef void (*hal_plc_rx_signal_t)(void);
typedef bool (*hal_get_config_info_t)(config_info_type_t cfg_type, uint16_t us_size, void *pv_data);
typedef bool (*hal_set_config_info_t)(config_info_type_t cfg_type, uint16_t us_size, void *pv_data);
typedef usi_status_t (*hal_usi_set_callback_t)(usi_protocol_t protocol_id, bool (*p_handler)(uint8_t *puc_rx_msg, uint16_t us_len), uint8_t usi_port);
typedef usi_status_t (*hal_usi_send_cmd_t)(void *msg);
typedef void (*hal_trng_init_t)(void);
typedef uint32_t (*hal_trng_read_t)(void);
typedef void (*hal_debug_report_t)(uint32_t ul_err_type);
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

#ifdef HAL_ATPL360_INTERFACE
typedef bool (*hal_plc_send_boot_cmd_t)(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read);
typedef bool (*hal_plc_send_wrrd_cmd_t)(uint8_t uc_cmd, void *px_spi_data, void *px_spi_status_info);
typedef void (*hal_plc_enable_interrupt_t)(bool enable);
typedef void (*hal_plc_delay_t)(uint8_t uc_tref, uint32_t ul_delay);
typedef bool (*hal_plc_get_cd_t)(void);
typedef bool (*hal_plc_set_stby_mode_t)(bool sleep);
typedef bool (*hal_plc_get_thermal_warning_t)(void);
#endif

#ifdef HAL_NWK_RECOVERY_INTERFACE
typedef uint32_t (*hal_nwk_recovery_init_t)(void);
typedef void (*hal_nwk_recovery_read_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef uint8_t (*hal_nwk_recovery_write_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
#endif

#ifdef HAL_ENABLE_DUAL_MODE
typedef void (*hal_swap_stack_t)(uint8_t uc_traffic);
#endif

/* Timer of 1us service interface */
typedef uint32_t (*hal_timer_1us_get_t)(void);
typedef bool (*hal_timer_1us_set_int_t)(uint32_t ul_time_us, bool b_relative, void (*p_handler)(uint32_t), uint32_t *pul_int_id);
typedef bool (*hal_timer_1us_cancel_int_t)(uint32_t ul_int_id);
typedef void (*hal_timer_1us_enable_interrupt_t)(bool b_enable);
/* @} */

#ifdef HAL_ENABLE_PHY_RF
typedef uint8_t (*hal_prf_if_init_t)(void);
typedef void (*hal_prf_if_reset_t)(void);
typedef void (*hal_prf_if_enable_interrupt_t)(bool b_enable);
typedef void (*hal_prf_if_set_handler_t)(void (*p_handler)(void));
typedef bool (*hal_prf_if_send_spi_cmd_t)(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode);
typedef bool (*hal_prf_if_is_spi_busy_t)(void);
typedef void (*hal_prf_if_led_t)(uint8_t uc_led_id, bool b_led_on);
#endif


/** Structure of HAL functions interface */
typedef struct {
	hal_restart_system_t restart_system;

	hal_set_gp_timer_handler_t set_gp_timer_handler;
	hal_clear_gp_timer_handler_t clear_gp_timer_handler;
	hal_timer_init_t timer_init;
	hal_timer_count_get_t timer_count_get;
	hal_timer_stop_t timer_stop;
	hal_timer_plc_init_t timer_plc_init;
	hal_timer_plc_stop_t timer_plc_stop;
	hal_set_plc_timer_handler_t set_plc_timer_handler;

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

	hal_plc_init_t plc_init;
	hal_plc_reset_t plc_reset;
	hal_plc_cmd_op_t plc_cmd_op;
	hal_plc_set_handler_t plc_set_handler;
	hal_plc_tx_signal_t plc_tx_signal;
	hal_plc_rx_signal_t plc_rx_signal;

	hal_get_config_info_t get_config_info;
	hal_set_config_info_t set_config_info;

	hal_usi_set_callback_t usi_set_callback;
	hal_usi_send_cmd_t usi_send_cmd;

	hal_trng_init_t trng_init;
	hal_trng_read_t trng_read;

	hal_debug_report_t debug_report;

	hal_net_get_freq_t net_get_freq;

#if !(defined(HAL_ENABLE_DUAL_MODE)) && PRIME_HAL_VERSION == HAL_PRIME_1_4
	hal_aes_init_t aes_init;
	hal_aes_set_callback_t aes_set_callback;
	hal_aes_key_t aes_key;
	hal_aes_crypt_t aes_crypt;
#endif

#ifdef HAL_ATPL360_INTERFACE
	hal_plc_send_boot_cmd_t plc_send_boot_cmd;
	hal_plc_send_wrrd_cmd_t plc_send_wrrd_cmd;
	hal_plc_enable_interrupt_t plc_enable_int;
	hal_plc_delay_t plc_delay;
	hal_plc_get_cd_t plc_get_cd;
#endif

#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init_t nwk_recovery_init;
	hal_nwk_recovery_read_t nwk_recovery_read;
	hal_nwk_recovery_write_t nwk_recovery_write;
#endif

	hal_pib_get_request_t pib_get_request;
	hal_pib_get_request_set_callback_t pib_get_request_set_callback;
	hal_pib_set_request_t pib_set_request;
	hal_pib_set_request_set_callback_t pib_set_request_set_callback;

#ifdef HAL_ENABLE_DUAL_MODE
	hal_aes_init_t aes_init;
	hal_aes_set_callback_t aes_set_callback;
	hal_aes_key_t aes_key;
	hal_aes_crypt_t aes_crypt;
	hal_swap_stack_t swap_stack;
#endif

#ifdef HAL_ATPL360_INTERFACE
	hal_plc_set_stby_mode_t plc_set_stby_mode;
	hal_plc_get_thermal_warning_t plc_get_thermal_warning;
#endif

	/* New functions must be added at the end */


	hal_timer_1us_get_t timer_1us_get;
	hal_timer_1us_set_int_t timer_1us_set_int;
	hal_timer_1us_cancel_int_t timer_1us_cancel_int;
	hal_timer_1us_enable_interrupt_t timer_1us_enable_interrupt;

#ifdef HAL_ENABLE_PHY_RF
	hal_prf_if_init_t prf_if_init;
	hal_prf_if_reset_t prf_if_reset;
	hal_prf_if_enable_interrupt_t prf_if_enable_interrupt;
	hal_prf_if_set_handler_t prf_if_set_handler;
	hal_prf_if_send_spi_cmd_t prf_if_send_spi_cmd;
	hal_prf_if_is_spi_busy_t prf_if_is_spi_busy;
	hal_prf_if_led_t prf_if_led;
#endif

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

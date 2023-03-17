/**
 * \file
 *
 * \brief PRIME_HAL_WRAPPER : API wrapper to manage HAL functions
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

#include <stdio.h>
#include "prime_hal_wrapper.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \weakgroup prime_hal_wrapper_group
 * @{
 */

/** Define HAL API interface */
static hal_api_t *p_hal_api;

/** HAL wrapper configuration */
void prime_hal_config(hal_api_t *hal_api_ptr)
{
	p_hal_api = hal_api_ptr;
}

/** System interface */
void prime_hal_restart_system(void)
{
	p_hal_api->restart_system();
}

/** \brief Timer Counters interface */
/* @{ */
void prime_hal_set_gp_timer_handler(hal_gp_timer_t gpt, void (*p_handler)(void))
{
	p_hal_api->set_gp_timer_handler(gpt, p_handler);
}

uint8_t prime_hal_timer_init(hal_gp_timer_t gpt, hal_timer_mode_t mode, hal_timer_clk_src_t clk_src)
{
	return p_hal_api->timer_init(gpt, mode, clk_src);
}

uint32_t prime_hal_timer_count_get(hal_gp_timer_t gpt)
{
	return p_hal_api->timer_count_get(gpt);
}

void prime_hal_timer_stop(hal_gp_timer_t gpt, hal_timer_mode_t mode)
{
	p_hal_api->timer_stop(gpt, mode);
}

void prime_hal_timer_plc_init(uint32_t uc_time_us)
{
	p_hal_api->timer_plc_init(uc_time_us);
}

void prime_hal_timer_plc_stop(void)
{
	p_hal_api->timer_plc_stop();
}

void prime_hal_set_plc_timer_handler(void (*p_handler)(void))
{
	p_hal_api->set_plc_timer_handler(p_handler);
}

/* @} */

/** \brief PCRC interface */
/* @{ */
uint32_t prime_hal_pcrc_calc(uint8_t *puc_buf, uint32_t ul_len, uint8_t uc_header_type, uint8_t uc_crc_type, bool b_v14_mode)
{
	return p_hal_api->pcrc_calc(puc_buf, ul_len, uc_header_type, uc_crc_type, b_v14_mode);
}

void prime_hal_pcrc_config_sna(uint8_t *puc_sna)
{
	p_hal_api->pcrc_config_sna(puc_sna);
}

/* @} */

/** \brief PLC interface */
/* @{ */
void prime_hal_plc_init(void)
{
	p_hal_api->plc_init();
}

void prime_hal_plc_reset(void)
{
	p_hal_api->plc_reset();
}

int8_t prime_hal_plc_cmd_op(uint8_t uc_cmd, uint16_t us_addr, uint16_t us_len, uint8_t *ptr_buf, uint8_t uc_bytes_rep)
{
	return p_hal_api->plc_cmd_op(uc_cmd, us_addr, us_len, ptr_buf, uc_bytes_rep);
}

void prime_hal_plc_set_handler(void (*p_handler)(void))
{
	p_hal_api->plc_set_handler(p_handler);
}

void prime_hal_plc_tx_signal(void)
{
	p_hal_api->plc_tx_signal();
}

void prime_hal_plc_rx_signal(void)
{
	p_hal_api->plc_rx_signal();
}

<#if PRIME_OPERATION_MODE == "Hybrid" || PRIME_OPERATION_MODE == "PLC">
bool prime_hal_plc_send_boot_cmd(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read)
{
	return p_hal_api->plc_send_boot_cmd(us_cmd, ul_addr, ul_data_len, puc_data_buf, puc_data_read);
}

bool prime_hal_plc_send_wrrd_cmd(uint8_t uc_cmd, void *px_spi_data, void *px_spi_status_info)
{
	return p_hal_api->plc_send_wrrd_cmd(uc_cmd, px_spi_data, px_spi_status_info);
}

void prime_hal_plc_enable_interrupt(bool enable)
{
	p_hal_api->plc_enable_int(enable);
}

void prime_hal_plc_delay(uint8_t uc_tref, uint32_t ul_delay)
{
	p_hal_api->plc_delay(uc_tref, ul_delay);
}

bool prime_hal_plc_get_cd(void)
{
	return p_hal_api->plc_get_cd();
}

bool prime_hal_plc_set_stby_mode(bool sleep)
{
	return p_hal_api->plc_set_stby_mode(sleep);
}

bool prime_hal_plc_get_thermal_warning(void)
{
	return p_hal_api->plc_get_thermal_warning();
}
</#if>

/* @} */

/** \brief Permanent store of configuration parameters */
/* @{ */
bool prime_hal_get_config_info(config_info_type_t cfg_type, uint16_t us_size, void *pv_data)
{
	return p_hal_api->get_config_info(cfg_type, us_size, pv_data);
}

bool prime_hal_set_config_info(config_info_type_t cfg_type, uint16_t us_size, void *pv_data)
{
	return p_hal_api->set_config_info(cfg_type, us_size, pv_data);
}

uint32_t prime_hal_nwk_recovery_init(void)
{
#ifdef HAL_NWK_RECOVERY_INTERFACE
	return p_hal_api->nwk_recovery_init();
#else
	return 0;
#endif
}

void prime_hal_nwk_recovery_read(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
#ifdef HAL_NWK_RECOVERY_INTERFACE
	p_hal_api->nwk_recovery_read(addr, puc_buf, us_size);
#else
	(void)addr;
	(void)puc_buf;
	(void)us_size;
#endif
}

uint8_t prime_hal_nwk_recovery_write(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
#ifdef HAL_NWK_RECOVERY_INTERFACE
	return p_hal_api->nwk_recovery_write(addr, puc_buf, us_size);
#else
	(void)addr;
	(void)puc_buf;
	(void)us_size;
	return 0;
#endif
}

/* @} */

/** \brief Firmware Upgrade interface */
/* @{ */
void prime_hal_fu_data_read(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
	p_hal_api->fu_data_read(addr, puc_buf, us_size);
}

uint8_t prime_hal_fu_data_write(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
	return p_hal_api->fu_data_write(addr, puc_buf, us_size);
}

void prime_hal_fu_data_cfg_read(void *pv_dst, uint16_t us_size)
{
	p_hal_api->fu_data_cfg_read(pv_dst, us_size);
}

uint8_t prime_hal_fu_data_cfg_write(void *pv_src, uint16_t us_size)
{
	return p_hal_api->fu_data_cfg_write(pv_src, us_size);
}

void prime_hal_fu_start(hal_fu_info_t *x_fu_info)
{
	p_hal_api->fu_start(x_fu_info);
}

void prime_hal_fu_end(hal_fu_result_t uc_hal_res)
{
	p_hal_api->fu_end(uc_hal_res);
}

void prime_hal_fu_revert(void)
{
	p_hal_api->fu_revert();
}

void prime_hal_fu_crc_calculate(void)
{
	p_hal_api->fu_crc_calculate();
}

void prime_hal_fu_crc_set_callback(void (*p_handler)(uint32_t ul_crc))
{
	p_hal_api->fu_crc_set_callback(p_handler);
}

uint16_t prime_hal_fu_get_bitmap(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages)
{
	return p_hal_api->fu_get_bitmap(puc_bitmap, pus_num_rcv_pages);
}

void prime_hal_fu_signature_image_check(void)
{
	p_hal_api->fu_signature_image_check();
}

void prime_hal_fu_signature_image_check_set_callback(void (*p_handler)(hal_fu_verif_result_t uc_result))
{
	p_hal_api->fu_signature_image_check_set_callback(p_handler);
}

/* @} */

/** \brief Universal Serial Interface */
/* @{ */
usi_status_t prime_hal_usi_set_callback(usi_protocol_t protocol_id, bool (*p_handler)(uint8_t *puc_rx_msg, uint16_t us_len), uint8_t usi_port)
{
	return p_hal_api->usi_set_callback(protocol_id, p_handler, usi_port);
}

usi_status_t prime_hal_usi_send_cmd(void *msg)
{
	return p_hal_api->usi_send_cmd(msg);
}

/* @} */

/** \brief True Random Number Generator */
/* @{ */
void prime_hal_trng_init(void)
{
	p_hal_api->trng_init();
}

uint32_t prime_hal_trng_read(void)
{
	return p_hal_api->trng_read();
}

/* @} */

/** \brief Debug Interface */
/* @{ */
void prime_hal_debug_report(uint32_t ul_err_type)
{
	p_hal_api->debug_report(ul_err_type);
}

/* @} */


/** \brief AES Interface */
/* @{ */
void prime_hal_aes_init(void)
{
	p_hal_api->aes_init();
}

void prime_hal_aes_set_callback(void (*p_handler)(void))
{
	p_hal_api->aes_set_callback(p_handler);
}

void prime_hal_aes_key(uint8_t *puc_key, uint8_t uc_key_len)
{
	p_hal_api->aes_key(puc_key, uc_key_len);
}

void prime_hal_aes_crypt(bool b_crypt_mode, uint8_t *puc_in_text, uint8_t *puc_out_text)
{
	p_hal_api->aes_crypt(b_crypt_mode, puc_in_text, puc_out_text);
}

/* @} */

/** \brief Parameter Information Base Interface */
/* @{ */
void prime_hal_pib_get_request(uint16_t us_pib_attrib)
{
	p_hal_api->pib_get_request(us_pib_attrib);
}

void prime_hal_pib_get_request_set_callback(void (*p_handler)(uint8_t uc_result, uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size))
{
	p_hal_api->pib_get_request_set_callback(p_handler);
}

void prime_hal_pib_set_request(uint16_t us_pib_attrib, void *pv_pib_value, uint8_t uc_pib_size)
{
	p_hal_api->pib_set_request(us_pib_attrib, pv_pib_value, uc_pib_size);
}

void prime_hal_pib_set_request_set_callback(void (*p_handler)(uint8_t uc_result))
{
	p_hal_api->pib_set_request_set_callback(p_handler);
}
/* @} */

void prime_hal_swap_stack(uint8_t uc_traffic)
{
	p_hal_api->swap_stack(uc_traffic);
}

/** \brief Parameter Information Base Interface */
/* @{ */
uint32_t prime_hal_timer_1us_get(void)
{
	return p_hal_api->timer_1us_get();
}

void prime_hal_timer_1us_enable_interrupt(bool b_enable)
{
	p_hal_api->timer_1us_enable_interrupt(b_enable);
}

bool prime_hal_timer_1us_set_int(uint32_t ul_time_us, bool b_relative, void (*p_handler)(uint32_t), uint32_t *pul_int_id)
{
	return p_hal_api->timer_1us_set_int(ul_time_us, b_relative, p_handler, pul_int_id);
}

bool prime_hal_timer_1us_cancel_int(uint32_t ul_int_id)
{
	return p_hal_api->timer_1us_cancel_int(ul_int_id);
}
/* @} */


/** \brief RF Interface */
/* @{ */
<#if PRIME_OPERATION_MODE == "Hybrid" || PRIME_OPERATION_MODE == "RF">
uint8_t prime_hal_prf_if_init(void)
{
	return p_hal_api->prf_if_init();
}

void prime_hal_prf_if_reset(void)
{
	p_hal_api->prf_if_reset();
}

void prime_hal_prf_if_enable_interrupt(bool b_enable)
{
	p_hal_api->prf_if_enable_interrupt(b_enable);
}

void prime_hal_prf_if_set_handler(void (*p_handler)(void))
{
	p_hal_api->prf_if_set_handler(p_handler);
}

bool prime_hal_prf_if_send_spi_cmd(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode)
{
	return p_hal_api->prf_if_send_spi_cmd(puc_data_buf, us_addr, us_len, uc_mode);
}

bool prime_hal_prf_if_is_spi_busy(void)
{
	return p_hal_api->prf_if_is_spi_busy();
}

void prime_hal_prf_if_led(uint8_t uc_led_id, bool b_led_on)
{
	p_hal_api->prf_if_led(uc_led_id, b_led_on);
}
</#if>

	/* @} */


/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */

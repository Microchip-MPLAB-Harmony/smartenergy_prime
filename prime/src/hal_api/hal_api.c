/**
 * \file
 *
 * \brief HAL: PRIME Hardware Abstraction Layer.
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
#include <stdio.h>

#include "hal_api.h"

#ifdef PRIME_DEBUG_REPORT
#include "modem.h"
#endif

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \weakgroup prime_hal_group
 * @{
 */
const hal_api_t hal_api = {
	hal_restart_system,
	hal_set_gp_timer_handler,
	hal_clear_gp_timer_handler,
	hal_timer_init,
	hal_timer_count_get,
	hal_timer_stop,
	hal_timer_plc_init,
	hal_timer_plc_stop,
	hal_set_plc_timer_handler,
	hal_pcrc_calc,
	hal_pcrc_config_sna,
	hal_fu_data_read,
	hal_fu_data_write,
	hal_fu_data_cfg_read,
	hal_fu_data_cfg_write,
	hal_fu_start,
	hal_fu_end,
	hal_fu_revert,
	hal_fu_crc_calculate,
	hal_fu_crc_set_callback,
	hal_fu_signature_image_check,
	hal_fu_signature_image_check_set_callback,
	hal_fu_get_bitmap,
	hal_plc_init,
	hal_plc_reset,
	hal_plc_cmd_op,
	hal_plc_set_handler,
	hal_plc_tx_signal,
	hal_plc_rx_signal,
	hal_get_config_info,
	hal_set_config_info,
	hal_usi_set_callback,
	hal_usi_send_cmd,
	hal_trng_init,
	hal_trng_read,
	hal_debug_report,
	hal_net_get_freq,
#if !(defined(HAL_ENABLE_DUAL_MODE)) && PRIME_HAL_VERSION == HAL_PRIME_1_4
	hal_aes_init,
	hal_aes_set_callback,
	hal_aes_key,
	hal_aes_crypt,
#endif
#ifdef HAL_ATPL360_INTERFACE
	hal_plc_send_boot_cmd,
	hal_plc_send_wrrd_cmd,
	hal_plc_enable_interrupt,
	hal_plc_delay,
	hal_plc_get_cd,
#endif
#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init,
	hal_nwk_recovery_read,
	hal_nwk_recovery_write,
#endif

	hal_pib_get_request,
	hal_pib_get_request_set_callback,
	hal_pib_set_request,
	hal_pib_set_request_set_callback,

#ifdef HAL_ENABLE_DUAL_MODE
	hal_aes_init,
	hal_aes_set_callback,
	hal_aes_key,
	hal_aes_crypt,
	hal_swap_stack,
#endif

#ifdef HAL_ATPL360_INTERFACE
	hal_plc_set_stby_mode,
	hal_plc_get_thermal_warning,
#endif


	timer_1us_get,
	timer_1us_set_int,
	timer_1us_cancel_int,
	timer_1us_enable_interrupt,

#ifdef HAL_ENABLE_PHY_RF
	prf_if_init,
	prf_if_reset,
	prf_if_enable_interrupt,
	prf_if_set_handler,
	prf_if_send_spi_cmd,
	prf_if_is_spi_busy,
	prf_if_led,
#endif

};


/**
 * \brief Restart program
 *
 */
void hal_restart_system(void)
{
#if (!PIC32CX)
	NVIC_SystemReset();
#else
	hal_reset_trigger(USER_RESET);
#endif
}



#ifdef HAL_ENABLE_DUAL_MODE
/** Callback function pointer for stack swap request */
static void (*_swap_stack_cb_function)(uint8_t uc_traffic);

/**
 * \brief Request to swap stack
 *
 * \param uc_traffic   Detected traffic (1 = PRIME_1_3, 2 = PRIME_1_4)
 */
void hal_swap_stack(uint8_t uc_traffic)
{
	/* check callback is initialized */
	if (_swap_stack_cb_function) {
		_swap_stack_cb_function(uc_traffic);
	}
}

/**
 * \brief Setup the callback for request of stack swap
 */
void hal_swap_stack_set_callback(void (*p_handler)(uint8_t uc_traffic))
{
	_swap_stack_cb_function = p_handler;
}
#endif

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */

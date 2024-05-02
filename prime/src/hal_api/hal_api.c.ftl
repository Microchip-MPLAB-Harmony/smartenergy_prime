/**
 * \file
 *
 * \brief HAL: PRIME Hardware Abstraction Layer.
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
#include <stdio.h>

#include "hal_api.h"

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
	SRV_RESET_HANDLER_RestartSystem,
	SRV_PCRC_GetValue,
	SRV_PCRC_ConfigureSNA,
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
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_init,
	hal_plc_reset,
	hal_plc_set_handler,
	hal_plc_tx_signal,
	hal_plc_rx_signal,
</#if>
	SRV_STORAGE_GetConfigInfo,
	SRV_STORAGE_SetConfigInfo,
    SRV_USI_Open,
	SRV_USI_CallbackRegister,
	SRV_USI_Send_Message,
	SRV_RANDOM_Get32bits,
	SRV_LOG_REPORT_Message_With_Code,
	hal_net_get_freq,
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_send_boot_cmd,
	hal_plc_send_wrrd_cmd,
	hal_plc_enable_interrupt,
	hal_plc_delay,
</#if>
#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init,
	hal_nwk_recovery_read,
	hal_nwk_recovery_write,
#endif
	SRV_USER_PIB_GetRequest,
	SRV_USER_PIB_GetRequestCallbackRegister,
	SRV_USER_PIB_SetRequest,
	SRV_USER_PIB_SetRequestCallbackRegister,
	hal_aes_init,
	hal_aes_set_callback,
	hal_aes_key,
	hal_aes_crypt,
	hal_swap_stack,
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_set_stby_mode,
	hal_plc_get_thermal_warning,
</#if>
	timer_1us_get,
	timer_1us_set_int,
	timer_1us_cancel_int,
	timer_1us_enable_interrupt,
<#if primePal.PRIME_PAL_RF_EN == true>
	prf_if_init,
	prf_if_reset,
	prf_if_enable_interrupt,
	prf_if_set_handler,
	prf_if_send_spi_cmd,
	prf_if_is_spi_busy,
	prf_if_led,
</#if>
};

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */

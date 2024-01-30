/**
 * \file
 *
 * \brief PAL: Physical Abstraction Layer
 *
 * Copyright (c) 2023 Atmel Corporation. All rights reserved.
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

#ifndef MPAL_H_INCLUDED
#define MPAL_H_INCLUDED

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */



/** \brief PAL_PLC interface */
/* @{ */
void pal_plc_init(void);
void pal_plc_process(void);
void pal_plc_set_callbacks(pal_callbacks_t *pal_cbs);

uint8_t pal_plc_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch);
uint8_t pal_plc_zct_get(uint32_t *pul_zct, uint16_t us_pch);
uint8_t pal_plc_timer_get(uint32_t *pul_timer, uint16_t us_pch);
uint8_t pal_plc_timer_get_extended(uint64_t *pull_timer);
uint8_t pal_plc_cd_get(uint8_t *puc_cd, uint8_t *puc_rssi, uint32_t *pul_time, uint8_t *puc_header, uint16_t us_pch);
uint8_t pal_plc_nl_get(uint8_t *puc_noise, uint16_t us_pch);
uint8_t pal_plc_agc_get(uint8_t *puc_mode, uint8_t *puc_gain, uint16_t us_pch);
uint8_t pal_plc_agc_set(uint8_t uc_mode, uint8_t uc_gain, uint16_t us_pch);
uint8_t pal_plc_ch_get(uint16_t *pus_pch);
uint8_t pal_plc_ch_set(uint16_t us_pch);

uint8_t pal_plc_data_request(x_pal_msg_tx_t *px_msg);

void pal_plc_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode);

uint8_t pal_plc_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);
uint8_t pal_plc_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);

uint16_t pal_plc_get_signal_capture(uint8_t *puc_dst, uint8_t uc_mode, uint32_t ul_time_start, uint32_t ul_duration);

uint8_t pal_plc_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration);

bool pal_plc_rm_minimum_quality(uint16_t us_chn, uint8_t uc_scheme, uint8_t uc_less_robust_mod);
uint8_t pal_plc_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2);


/* @} */

/** \brief PAL_RF interface */
/* @{ */
void pal_rf_init(void);
void pal_rf_process(void);
void pal_rf_set_callbacks(pal_callbacks_t *pal_cbs);

uint8_t pal_rf_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch);
uint8_t pal_rf_zct_get(uint32_t *pul_zct, uint16_t us_pch);
uint8_t pal_rf_timer_get(uint32_t *pul_timer, uint16_t us_pch);
uint8_t pal_rf_timer_get_extended(uint64_t *pull_timer);
uint8_t pal_rf_nl_get(uint8_t *puc_noise, uint16_t us_pch);
uint8_t pal_rf_cca_get(uint8_t *puc_channel_state);
uint8_t pal_rf_ch_get(uint16_t *pus_pch);
uint8_t pal_rf_ch_set(uint16_t us_pch);

uint8_t pal_rf_data_request(x_pal_msg_tx_t *px_msg);

void pal_rf_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode);

uint8_t pal_rf_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);
uint8_t pal_rf_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);

uint8_t pal_rf_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration);

bool pal_rf_rm_minimum_quality(uint16_t us_chn, uint8_t uc_scheme, uint8_t uc_less_robust_mod);
uint8_t pal_rf_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2);


/* @} */

/** \brief PAL_SERIAL interface */
/* @{ */
void pal_serial_init(void);
void pal_serial_process(void);
void pal_serial_set_callbacks(pal_callbacks_t *pal_cbs);

uint8_t pal_serial_timer_get(uint32_t *pul_timer, uint16_t us_pch);
uint8_t pal_serial_timer_get_extended(uint64_t *pull_timer);

uint8_t pal_serial_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);
uint8_t pal_serial_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);

uint8_t pal_serial_data_request(x_pal_msg_tx_t *px_msg);

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* MPAL_H_INCLUDED */

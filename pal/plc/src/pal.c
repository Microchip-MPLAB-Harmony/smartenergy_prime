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

/* System includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Prime includes */
#include "pal.h"
#include "pal_rm.h"
#include "prime_hal_wrapper.h"
#include "coup_tx_config.h"
#ifdef PAL_MULTIPHY
#include "mpal.h"
#endif

#include "atpl360.h"
#include "atpl360_IB_db.h"
#include "atpl360_boot.h"
#include "atpl360_comm.h"
#include "atpl360_exception.h"
#include "general_defs.h"

/* #define PAL_DEBUG_ENABLE */
#ifdef PAL_DEBUG_ENABLE
#       define LOG_PAL_DEBUG(...)   printf(__VA_ARGS__ )
#else
#       define LOG_PAL_DEBUG(...)
#endif

#ifndef PAL_MULTIPHY
#define pal_plc_init pal_init
#define pal_plc_process pal_process
#define pal_plc_set_callbacks pal_set_callbacks
#define pal_plc_data_request pal_data_request
#define pal_plc_set_chn_req pal_set_chn_req
#define pal_plc_timer_get pal_timer_get
#define pal_plc_timer_get_extended pal_timer_get_extended
#define pal_plc_cd_get pal_cd_get
#define pal_plc_zct_get pal_zct_get
#define pal_plc_agc_get pal_agc_get
#define pal_plc_agc_set pal_agc_set
#define pal_plc_nl_get pal_nl_get
#define pal_plc_ch_get pal_ch_get
#define pal_plc_ch_set pal_ch_set
#define pal_plc_get_cfg pal_get_cfg
#define pal_plc_set_cfg pal_set_cfg
#define pal_plc_snr_get pal_snr_get
#define pal_plc_get_msg_duration pal_get_msg_duration
#define pal_plc_get_signal_capture pal_get_signal_capture
#endif

#ifdef PAL_PLC

/**
 * \weakgroup prime_pal_group
 * @{
 */

/* ! \name PRIME Buffer ID */
#define PAL_TX_NUM_BUFFERS    2
#define PAL_TX_BUFFER_0       TX_BUFFER_0
#define PAL_TX_BUFFER_1       TX_BUFFER_1

/* Delay in us between read of Host timer and PL360 timer (CS falling edge) */
/* The delay depends on CPU frequency, compiler and optimizations */
/* It doesn't affect to relative time between RX and TX (compensated) */
#define PAL_TIMER_SYNC_OFFSET    6

/* Maximum and minimum relative frequency between PL360 and host timers (F_host/F_360 [uQ1.24]) */
/* It is used to detect wrong timer reads */
#define SYNC_TIMER_REL_FREQ_MAX  0x01000D1B /* +200 PPM */
#define SYNC_TIMER_REL_FREQ_MIN  0x00FFF2E5 /* -200 PPM */

/* Time in us of chirp */
#define PHY_CHIRP_TIME                 (2048)
#define PHY_CHIRP_MODE_B_TIME          (2048 * 4)
#define PHY_CHIRP_MODE_BC_TIME         ((2048 * 5) + (2240 * 2))

/* Time in us of phy symbol */
#define PHY_SYMBOL_TIME                (2240)

/* Time in us of phy header */
#define PHY_HEADER_TIME                (4480)
#define PHY_HEADER_B_BC_TIME           (2240L * 4)

/** \brief Transmission parameters */
/* @{ */

static pal_callbacks_t sx_pal_app_callbacks;

/* ATPL360 component */
static atpl360_descriptor_t sx_atpl360_desc;

static uint32_t sul_hi_timer_ref;
static uint32_t sul_previous_timer_ref;

static uint8_t suc_last_rssi_val;
static uint8_t suc_last_cinr_val;
static bool spb_buf_id[PAL_TX_NUM_BUFFERS];
static uint8_t suc_sniffer_enabled;
static uint8_t suc_current_channel;
static uint8_t suc_error_info;
static uint16_t sus_current_user_band_plan;
static bool sb_exception_pend;

/* Maximum number of channels supported: Worst case between PHY and PAL */
static uint8_t suc_max_num_channels;

/* Attenuation to be applied in all transmission */
static uint8_t suc_phy_att;

#ifdef PAL_ENABLE_C11_CFG
/* PLCOUP011 limitation: Impedance detection only valid in channel 1 */
# define PAL_DETECT_IMPEDANCE_CHANNEL  1
static uint8_t suc_network_detected;
#endif
#ifdef PAL_ENABLE_PL460_FCC_1_5B_CFG
/* PL460 FCC-1.5B limitation: Impedance detection only valid in channel 1 */
# define PAL_DETECT_IMPEDANCE_CHANNEL  6
#endif

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
static uint8_t suc_prev_channel;
static bool sb_impedance_detected;
static uint8_t _set_current_txrx_channel(uint8_t uc_channel);
#endif

/* Synchronization between Host and PL360 timers */
static uint32_t sul_sync_pl360_ref;
static uint32_t sul_sync_host_ref;
static uint32_t sul_sync_timer_rel_freq;
static uint32_t sul_sync_int_id;
static uint32_t sul_sync_delay;
static bool sb_sync_update;
static bool sb_disable_time_sync;


/* Storage of physical parameters for latest message received */
static x_pal_plc_rx_phy_params_t sx_rx_phy_params;
/* @} */

/**
 * \brief Function to encode the channel in pch format
 *
 * \param uc_chn  		Channel
 * \return Channel in pch format
 */
static uint16_t _encode_pch(uint8_t uc_chn)
{
	uint16_t us_pch_temp;

	if (uc_chn <= 8) {
		us_pch_temp = 1 << (uc_chn - 1); /* Single channel */
	} else  {
		us_pch_temp = 3 << (uc_chn - 9); /* Double channel */
	}

	return us_pch_temp;
}

/**
 * \brief Function to decode the channel in pch format
 *
 * \param us_pch  		Channel
 * \return Channel in plc format
 */
static uint8_t _decode_pch(uint16_t us_pch)
{
	uint8_t uc_plc_channel;

	if (us_pch == 0) {
		return 0;
	}

	uc_plc_channel = 1;
	while ((us_pch & 0x0001) == 0) {
		us_pch >>= 1;
		uc_plc_channel++;
	}

	us_pch &= 0x03;
	/* Double channel, we must add 8 */
	if (us_pch == 3) {
		uc_plc_channel += 8;
	}

	return uc_plc_channel;
}

/**
 * \brief Function to get channel list.
 *
 * \return List of available channels
 */
static uint16_t _get_channel_list(void)
{
	/* Changed band plan */
	if (sus_current_user_band_plan != 0) {
		return (sus_current_user_band_plan);
	}

	/* All channels by default */
	uint16_t us_chn_list = CHANNEL1 | CHANNEL2 | CHANNEL3 | CHANNEL4 | CHANNEL5 | CHANNEL6 | CHANNEL7 | CHANNEL8;
#ifdef PAL_ENABLE_MULTICHANNEL
	if (suc_max_num_channels > 1) {
		us_chn_list |= (((uint16_t)(CHANNEL12 | CHANNEL23 | CHANNEL34 | CHANNEL45 | CHANNEL56 | CHANNEL67 | CHANNEL78)) << 8);
	}
#endif

#ifdef USER_BAND_PLAN
	/* Update list of available channels according to band plan */
	us_chn_list &= (0xff00 | USER_BAND_PLAN);
#endif
#ifdef PAL_ENABLE_MULTICHANNEL
#ifdef USER_BAND_PLAN_2CH
	/* Update list of available channels according to band plan */
	if (suc_max_num_channels > 1) {
		us_chn_list &= (0xff | (USER_BAND_PLAN_2CH << 8));
	}
#endif
#endif

	return (us_chn_list);
}

/**
 * \brief Function to set channel list.
 *
 * \param uc_new_chn_list    List of available channels
 * \param uc_num_channels    One or double channel
 */
static void _set_channel_list(uint8_t uc_new_chn_list, uint8_t uc_num_channels)
{
	uint16_t us_current_chn_list;
	uint16_t us_band_plan;
	uint16_t us_chn_list;

	/* Retrieve the current list of allowed channels */
	us_current_chn_list = _get_channel_list();

	if (uc_num_channels == 1) {
		us_chn_list = us_current_chn_list & 0xff00;
		us_chn_list |= uc_new_chn_list;
	} else {
		us_chn_list = us_current_chn_list & 0x00ff;
		us_chn_list |= (uc_new_chn_list << 8);
	}

	sus_current_user_band_plan = 0;

	/* Update list of available channels according to the compiled band plan */
	us_current_chn_list = _get_channel_list();

	us_band_plan = us_current_chn_list & us_chn_list;

	if (us_band_plan != 0) {
		sus_current_user_band_plan = us_band_plan;
	} else {
		sus_current_user_band_plan = us_current_chn_list;
	}
}

static void _handle_continuous_tx_mode(uint8_t uc_cont_tx_mode)
{
	tx_msg_t phy_msg;
	uint8_t uc_data = 0;

	/* Default parameters for message sending */
	phy_msg.uc_buffer_id = PAL_TX_BUFFER_0;
	phy_msg.uc_att_level = 0xFF;
	phy_msg.x_csma_mode.uc_disable_rx = 1;
	phy_msg.x_csma_mode.uc_sense_count = 0;
	phy_msg.x_csma_mode.uc_sense_delay_ms = 0;
	phy_msg.uc_mod_type = MODE_TYPE_B;
	phy_msg.uc_scheme = MOD_SCHEME_DBPSK;
	phy_msg.ul_tx_time = 0;
	phy_msg.us_data_len = 1;
	phy_msg.puc_data_buf = &uc_data; /* Trash */

	/* Check mode */
	if (uc_cont_tx_mode == 0) {
		/* Disable continuous tx mode */
		phy_msg.uc_tx_mode = TX_MODE_CANCEL;
		sx_atpl360_desc.send_data(&phy_msg);
	} else {
		/* Enable continuous tx mode */
		phy_msg.uc_tx_mode = TX_MODE_PREAMBLE_CONTINUOUS | TX_MODE_RELATIVE;
		sx_atpl360_desc.send_data(&phy_msg);
	}
}

static void _handler_reset(atpl360_exception_t exception)
{
	/* In case of reset, mark to restore channel */
	if (exception == ATPL360_EXCEPTION_RESET) {
		sb_exception_pend = true;
	}

	/* Mark error */
	suc_error_info |= (1 << exception);

	/* Increase number of errors, if possible */
	uint8_t uc_num_errors = suc_error_info >> 4;
	if (uc_num_errors < 0xF) {
		++uc_num_errors;
		suc_error_info &= 0x0F;
		suc_error_info += (uc_num_errors << 4);
	}
}

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL

/**
 * \brief Transmit dummy message for impedance detection.
 *
 */
static void _imp_detect_tx_msg(void)
{
	tx_msg_t x_tx_msg;
	uint8_t puc_data_buf[8];

	x_tx_msg.uc_buffer_id = PAL_TX_BUFFER_0;
	x_tx_msg.x_csma_mode.uc_disable_rx = true;
	x_tx_msg.x_csma_mode.uc_sense_count = 0;
	x_tx_msg.x_csma_mode.uc_sense_delay_ms = 0;
	x_tx_msg.uc_mod_type = MODE_TYPE_A;
	x_tx_msg.uc_scheme = MOD_SCHEME_DBPSK;
	x_tx_msg.uc_tx_mode = TX_MODE_RELATIVE;
	x_tx_msg.ul_tx_time = 0;
	x_tx_msg.us_data_len = 8;
	x_tx_msg.puc_data_buf = puc_data_buf;
	x_tx_msg.uc_att_level = 7;
	memset(puc_data_buf, 0xAA, 8);

	sx_atpl360_desc.send_data(&x_tx_msg);
}

#endif

static void _timer_sync_int_handler(uint32_t ul_id)
{
	if (sul_sync_int_id == ul_id) {
		/* Update timer synchronization in next pal_process() */
		sb_sync_update = true;
	}
}

static uint32_t _timer_sync_read(uint32_t *pul_time_pl360)
{
	uint32_t ul_time_host;
	uint32_t ul_basepri_prev = __get_BASEPRI();

	/* Enter critical region. Disable all interrupts except highest priority
	 * (<1: 0) to ensure constant delay between timers read */
	__set_BASEPRI(1 << (8 - __NVIC_PRIO_BITS));

	/* Read current Host and PL360 timers */
	pal_timer_get(&ul_time_host, 0);
	sx_atpl360_desc.get_config(ATPL360_TIME_REF_ID, pul_time_pl360, sizeof(uint32_t), true);

	/* Leave critical region */
	__set_BASEPRI(ul_basepri_prev);

	/* Adjust delay between Host and PL360 timers */
	ul_time_host += PAL_TIMER_SYNC_OFFSET;

	return ul_time_host;
}

__always_inline static void _timer_sync_init(void)
{
	bool b_int_programmed;

	if (sb_disable_time_sync) {
		sb_sync_update = false;
	} else {
		/* Get initial timer references */
		sul_sync_host_ref = _timer_sync_read(&sul_sync_pl360_ref);

		/* Initialize relative frequency F_host/F_360 to 1 [uQ1.24] */
		sul_sync_timer_rel_freq = 1UL << 24;

		/* Program first interrupt after 50 ms (5 us deviation with 100 PPM) */
		sul_sync_delay = 50000;
		b_int_programmed = prime_hal_timer_1us_set_int(sul_sync_delay, true, _timer_sync_int_handler, &sul_sync_int_id);
		if (b_int_programmed) {
			sb_sync_update = false;
		} else {
			sul_sync_delay = 0;
			sb_sync_update = true;
		}
	}
}

__always_inline static void _timer_sync_update(void)
{
	uint32_t ul_time_host;
	uint32_t ul_time_pl360;
	uint32_t ul_delay_host;
	uint32_t ul_delay_pl360;
	uint32_t ul_sync_timer_rel_freq;
	bool b_int_programmed = false;

	/* Get current host and PL360 timers */
	ul_time_host = _timer_sync_read(&ul_time_pl360);

	/* Compute delays from references (last read) */
	ul_delay_host = ul_time_host - sul_sync_host_ref;
	ul_delay_pl360 = ul_time_pl360 - sul_sync_pl360_ref;

	/* Compute relative frequency F_host/F_360 [uQ1.24] */
	ul_sync_timer_rel_freq = (uint32_t)div_round((uint64_t)ul_delay_host << 24, ul_delay_pl360);

	/* Check if relative frequency is consistent, otherwise timer read is wrong */
	if ((ul_sync_timer_rel_freq >= SYNC_TIMER_REL_FREQ_MIN) && (ul_sync_timer_rel_freq <= SYNC_TIMER_REL_FREQ_MAX)) {
		/* Update relative frequency and references */
		sul_sync_timer_rel_freq = ul_sync_timer_rel_freq;
		sul_sync_host_ref = ul_time_host;
		sul_sync_pl360_ref = ul_time_pl360;

		switch (sul_sync_delay) {
		case 0:
			/* Next sync after 50 ms (5 us deviation with 100 PPM) */
			sul_sync_delay = 50000;
			break;

		case 50000:
			/* Next sync after 250 ms (5 us deviation with 20 PPM) */
			sul_sync_delay = 250000;
			break;

		case 250000:
			/* Next sync after 1 second (5 us deviation with 5 PPM) */
			sul_sync_delay = 1000000;
			break;

		default:
			/* Next sync after 5 seconds (5 us deviation with 1 PPM) */
			sul_sync_delay = 5000000;
			break;
		}

		/* Program next interrupt */
		b_int_programmed = prime_hal_timer_1us_set_int(sul_sync_delay, true, _timer_sync_int_handler, &sul_sync_int_id);
	} else {
		prime_hal_debug_report(PAL_PLC_TIMER_SYNC_ERROR);

		_timer_sync_init();
	}

	if (b_int_programmed) {
		sb_sync_update = false;
	} else {
		sul_sync_delay = 0;
	}

	LOG_PAL_DEBUG("SYNC: delay_360=%u; delay_host=%u\r\n", ul_delay_pl360, ul_delay_host);
	LOG_PAL_DEBUG("SYNC: REF_OFFSET=%d, PPM=%d\r\n", (int32_t)(sul_sync_pl360_ref - sul_sync_host_ref), (int32_t)((((int64_t)sul_sync_timer_rel_freq - (1 << 24)) * 1000000) >> 24));
}

static uint32_t _time_pl360_to_host(uint32_t ul_time_pl360)
{
	int64_t sll_delay_aux;
	int32_t sl_delay_pl360;
	int32_t sl_delay_host;
	uint32_t ul_time_host;

	/* Compute PL360 delay time since last sinchronization */
	sl_delay_pl360 = (int32_t)(ul_time_pl360 - sul_sync_pl360_ref);

	/* Convert PL360 delay to Host delay (frequency deviation) */
	sll_delay_aux = (int64_t)sl_delay_pl360 * sul_sync_timer_rel_freq;
	sl_delay_host = (int32_t)((sll_delay_aux + (1UL << 23)) >> 24);

	/* Compute Host time */
	ul_time_host = sul_sync_host_ref + sl_delay_host;

	return ul_time_host;
}

static uint32_t _time_host_to_pl360(uint32_t ul_time_host)
{
	int64_t sll_delay_aux;
	int32_t sl_delay_host;
	int32_t sl_delay_pl360;
	uint32_t ul_time_pl360;

	/* Compute Host delay time since last synchronization */
	sl_delay_host = (int32_t)(ul_time_host - sul_sync_host_ref);

	/* Convert Host delay to PL360 delay (frequency deviation) */
	sll_delay_aux = (int64_t)sl_delay_host << 24;
	sl_delay_pl360 = (int32_t)div_round(sll_delay_aux, sul_sync_timer_rel_freq);

	/* Compute PL360 time */
	ul_time_pl360 = sul_sync_pl360_ref + sl_delay_pl360;

	return ul_time_pl360;
}

static void _data_confirm(tx_cfm_t *px_msg_cfm)
{

  if (px_msg_cfm->uc_tx_result == TX_RESULT_TIMEOUT) {
  	_timer_sync_init();
  }

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	if (!sb_impedance_detected) {
		sb_impedance_detected = true;

		/* Restore channel after TX for impedance detection */
		if (suc_current_channel != suc_prev_channel) {
			_set_current_txrx_channel(suc_prev_channel);
		}

		return;
	}
#endif

	if (sx_pal_app_callbacks.data_confirm) {
		x_pal_data_cfm_t x_data_cfm;

		x_data_cfm.tx_time = _time_pl360_to_host(px_msg_cfm->ul_tx_time);
		x_data_cfm.rms_calc = (uint16_t)px_msg_cfm->ul_rms_calc;
		x_data_cfm.mode = px_msg_cfm->uc_mod_type;

		switch (px_msg_cfm->uc_tx_result) {
		case TX_RESULT_BUSY_CH:
		case TX_RESULT_BUSY_RX:
			x_data_cfm.result = PAL_TX_RESULT_BUSY_CH;
			break;
		default:
			x_data_cfm.result = px_msg_cfm->uc_tx_result;
			break;
		}

		x_data_cfm.us_pch = _encode_pch(suc_current_channel);

		LOG_PAL_DEBUG("cfm time %u \n", x_data_cfm.tx_time);

		/* ATPL360 Reset management */
		uint8_t uc_id;
		if (px_msg_cfm->uc_tx_result == TX_RESULT_NO_TX) {
			/* Reset indication of buffers in use */
			for(uc_id = 0; uc_id < PAL_TX_NUM_BUFFERS; uc_id++) {
				if (spb_buf_id[uc_id]) {
					x_data_cfm.buff_id = uc_id;
					sx_pal_app_callbacks.data_confirm(&x_data_cfm);
					spb_buf_id[uc_id] = false;
				}
			}

		} else {
			for(uc_id = 0; uc_id < PAL_TX_NUM_BUFFERS; uc_id++) {
				if (spb_buf_id[uc_id] && (px_msg_cfm->uc_buffer_id == uc_id)) {
					x_data_cfm.buff_id = uc_id;
					spb_buf_id[uc_id] = false;
					sx_pal_app_callbacks.data_confirm(&x_data_cfm);
					break;
				}
			}
		}

		/* Required Signalling by spec. */
		prime_hal_plc_tx_signal();

		/*		LOG_PAL_DEBUG("\r\nPAL: plc_data_confirm res:%u (%u-%u)\r\n", (unsigned int)x_data_cfm.result, */
		/*				(unsigned int)x_data_cfm.buff_id, (unsigned int)px_msg_cfm->uc_buffer_id); */
	}
}

static void _data_indication(rx_msg_t *px_msg)
{
	sx_rx_phy_params.uc_ber_soft = px_msg->uc_ber_soft;
	sx_rx_phy_params.uc_ber_soft_max = px_msg->uc_ber_soft_max;
	sx_rx_phy_params.uc_cinr_avg = px_msg->uc_cinr_avg;
	sx_rx_phy_params.uc_cinr_min = px_msg->uc_cinr_min;
	sx_rx_phy_params.uc_header_type = px_msg->uc_header_type;
	sx_rx_phy_params.uc_imp_percent = px_msg->uc_imp_percent;
	sx_rx_phy_params.uc_mod_type = px_msg->uc_mod_type;
	sx_rx_phy_params.uc_nar_bnd_percent = px_msg->uc_nar_bnd_percent;
	sx_rx_phy_params.uc_rssi_avg = px_msg->uc_rssi_avg;
	sx_rx_phy_params.uc_scheme = px_msg->uc_scheme;
	sx_rx_phy_params.ul_evm_header_acum = px_msg->ul_evm_header_acum;
	sx_rx_phy_params.ul_evm_payload_acum = px_msg->ul_evm_payload_acum;
	sx_rx_phy_params.ul_rx_time = px_msg->ul_rx_time;
	sx_rx_phy_params.us_data_len = px_msg->us_data_len;
	sx_rx_phy_params.us_evm_header = px_msg->us_evm_header;
	sx_rx_phy_params.us_evm_payload = px_msg->us_evm_payload;

	if (sx_pal_app_callbacks.data_indication) {
		x_pal_data_ind_t x_data_ind;

		x_data_ind.buff_id = 0;
		x_data_ind.data_buf = px_msg->puc_data_buf;
		x_data_ind.data_len = px_msg->us_data_len;
		x_data_ind.header_type = px_msg->uc_header_type;
		x_data_ind.mode = px_msg->uc_mod_type;
		x_data_ind.rx_time = _time_pl360_to_host(px_msg->ul_rx_time);
		x_data_ind.scheme = px_msg->uc_scheme;
        x_data_ind.us_pch = _encode_pch(suc_current_channel);
		pal_plc_rm_max_modulation_rx_msg(px_msg, &x_data_ind.us_estimated_bitrate, &x_data_ind.uc_less_robust_mod, suc_max_num_channels);
		x_data_ind.uc_lqi = pal_plc_rm_get_lqi(px_msg->uc_cinr_avg);
		x_data_ind.ss_rssi = px_msg->uc_rssi_avg;

		/* Store last values of some fields */
		suc_last_rssi_val = px_msg->uc_rssi_avg;
		suc_last_cinr_val = px_msg->uc_cinr_min;

		sx_pal_app_callbacks.data_indication(&x_data_ind);

		/* Required signaling by spec. */
		prime_hal_plc_rx_signal();

		LOG_PAL_DEBUG("\r\nPAL: plc_data_indication len:%u bufId:%u\r\n", (unsigned int)x_data_ind.data_len, (unsigned int)x_data_ind.buff_id);
	}
}

/**
 * \brief Handler to receive data from ATPL360.
 */
static void _handler_serial_atpl360_event(uint8_t *puc_serial_data, uint16_t us_len)
{
	x_usi_serial_cmd_params_t x_usi_msg;

	if (suc_sniffer_enabled) {
		uint32_t ul_time_ini_pl360, ul_time_end_pl360, ul_duration;
		uint32_t ul_time_ini_host, ul_time_end_host;

		/* Get times from sniffer message (PL360 time) */
		ul_time_ini_pl360 = (uint32_t)puc_serial_data[19] << 24;
		ul_time_ini_pl360 += (uint32_t)puc_serial_data[20] << 16;
		ul_time_ini_pl360 += (uint32_t)puc_serial_data[21] << 8;
		ul_time_ini_pl360 += (uint32_t)puc_serial_data[22];
		ul_time_end_pl360 = (uint32_t)puc_serial_data[23] << 24;
		ul_time_end_pl360 += (uint32_t)puc_serial_data[24] << 16;
		ul_time_end_pl360 += (uint32_t)puc_serial_data[25] << 8;
		ul_time_end_pl360 += (uint32_t)puc_serial_data[26];
		ul_duration = ul_time_end_pl360 - ul_time_ini_pl360;

		/* Convert from PL360 to Host time */
		ul_time_ini_host = _time_pl360_to_host(ul_time_ini_pl360);
		ul_time_end_host = ul_time_ini_host + ul_duration;

		/* Overwrite times in sniffer message (Host time) */
		puc_serial_data[19] = (uint8_t)(ul_time_ini_host >> 24);
		puc_serial_data[20] = (uint8_t)(ul_time_ini_host >> 16);
		puc_serial_data[21] = (uint8_t)(ul_time_ini_host >> 8);
		puc_serial_data[22] = (uint8_t)(ul_time_ini_host);
		puc_serial_data[23] = (uint8_t)(ul_time_end_host >> 24);
		puc_serial_data[24] = (uint8_t)(ul_time_end_host >> 16);
		puc_serial_data[25] = (uint8_t)(ul_time_end_host >> 8);
		puc_serial_data[26] = (uint8_t)(ul_time_end_host);

		x_usi_msg.uc_protocol_type = PROTOCOL_SNIF_PRIME;
		x_usi_msg.ptr_buf = puc_serial_data;
		x_usi_msg.us_len = us_len;
		prime_hal_usi_send_cmd(&x_usi_msg);
	}
}

/**
 * \brief Handler to receive data from APP.
 */
//static bool _handler_serial_app_event(uint8_t *puc_serial_data, uint16_t us_len)
//{
//	if (suc_sniffer_enabled) {
//		sx_atpl360_desc.send_addons_cmd(puc_serial_data, us_len);
//	}
//
//	return true;
//}

/**
 * \brief Get PL360 binary addressing.
 *
 * \param pul_address   Pointer to store the initial address of PL360 binary data
 *
 * \return Size of PL360 binary file
 */
static uint32_t _get_pl360_bin_addressing(uint32_t *pul_address)
{
#ifndef ATPL360_BINARY_ADDRESS
	/* Base Node */
	uint32_t ul_bin_addr;
	uint8_t *puc_bin_start;
	uint8_t *puc_bin_end;

  #if defined (__CC_ARM)
	extern uint8_t atpl_bin_start[];
	extern uint8_t atpl_bin_end[];
	ul_bin_addr = (int)(atpl_bin_start - 1);
	puc_bin_start = atpl_bin_start - 1;
	puc_bin_end = atpl_bin_end;
  #elif defined (__GNUC__)
	extern uint8_t atpl_bin_start;
	extern uint8_t atpl_bin_end;
	ul_bin_addr = (int)&atpl_bin_start;
	puc_bin_start = (int)&atpl_bin_start;
	puc_bin_end = (int)&atpl_bin_end;
  #elif defined (__ICCARM__)
	#pragma section = "P_atpl_bin"
	extern uint8_t atpl_bin;
	ul_bin_addr = (int)&atpl_bin;
	puc_bin_start = __section_begin("P_atpl_bin");
	puc_bin_end = __section_end("P_atpl_bin");
  #else
	#error This compiler is not supported for now.
  #endif
	*pul_address = ul_bin_addr;
	return ((uint32_t)puc_bin_end - (uint32_t)puc_bin_start);

#else
	/* Service Node */
	*pul_address = ATPL360_BINARY_ADDRESS;
	return ATPL360_BINARY_LEN;
#endif
}

/**
 * \brief check the transmission/reception channels
 *
 * \param uc_channel   Channel to check
 *
 * \return True if there is no error, otherwise returns false
 */
static bool _check_txrx_channel(uint8_t uc_channel)
{
	uint16_t us_chn_list;
	uint8_t uc_tmp;

	us_chn_list = _get_channel_list();
	uc_tmp = uc_channel - 1;
	if (!(us_chn_list & (1 << uc_tmp))) {
		return(false);
	} else {
		return(true);
	}
}

/**
 * \brief set the transmission/reception channels
 *
 * \param uc_channel   Channel to set
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT
 */
static uint8_t _set_current_txrx_channel(uint8_t uc_channel)
{
	/* Check if channel is not in the channel list */
	if (!_check_txrx_channel(uc_channel)) {
#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
		/* Allow channel PAL_DETECT_IMPEDANCE_CHANNEL for impedance detection */
		if (sb_impedance_detected || (uc_channel != PAL_DETECT_IMPEDANCE_CHANNEL)) {
			return(PAL_CFG_INVALID_INPUT);
		}
#else
		return(PAL_CFG_INVALID_INPUT);
#endif
	}

	sx_atpl360_desc.set_config(ATPL360_REG_CHANNEL_CFG, &uc_channel, 1);

	pl360_prime_coup_tx_config(&sx_atpl360_desc, uc_channel);
	suc_current_channel = uc_channel;

	/* Initialize synchronization of PL360-Host timers when channel updated */
	_timer_sync_init();

#ifdef PAL_ENABLE_C11_CFG
	if (suc_network_detected && (suc_current_channel != PAL_DETECT_IMPEDANCE_CHANNEL)) {
		uint8_t uc_value;

		uc_value = 2;
		sx_atpl360_desc.set_config(ATPL360_REG_CFG_IMPEDANCE, &uc_value, 1);
	}
#endif

	return(PAL_CFG_SUCCESS);
}

/**
* \brief Set PL360 PHY correlation thresholds after PL360 initialization
*/
static void _set_correlation_thresholds(void)
{
	uint16_t pus_thresholds[6];

	/* Get original thresholds */
	//sx_atpl360_desc.get_config(ATPL360_REG_SYNC_THRESHOLDS, pus_thresholds, 12, true);

	/* Set 1/2 (Type A and Type B) thresholds to maximum value (disable CD activation at 1/2 preamble) */
	pus_thresholds[0] = 35000;
	pus_thresholds[1] = 35000;
	pus_thresholds[2] = 46000;
	pus_thresholds[3] = 46000;
	pus_thresholds[4] = 64000;
	pus_thresholds[5] = 64000;
/*
	pus_thresholds[0] = 12500;
	pus_thresholds[1] = 12500;
	pus_thresholds[2] = 15000;
	pus_thresholds[3] = 15000;
	pus_thresholds[4] = 30000;
	pus_thresholds[5] = 30000;
*/
	sx_atpl360_desc.set_config(ATPL360_REG_SYNC_THRESHOLDS, pus_thresholds, 12);
}


#ifdef PAL_DETECT_IMPEDANCE_CHANNEL

/**
 * \brief Start impedance detection procedure. Only needed if the coupling does
 * not support impedance detection for all channels. Otherwise this is not
 * needed because the impedance can be detected in every transmission. Switch to
 * PAL_DETECT_IMPEDANCE_CHANNEL channel and transmit dummy message for
 * impedance detection.
 *
 */
static void _imp_detect_start(void)
{
	/* Store previous channel to restore after TX for impedance detection */
	suc_prev_channel = suc_current_channel;

	/* Switch to required channel for impedance detection */
	sb_impedance_detected = false;
	if (suc_current_channel != PAL_DETECT_IMPEDANCE_CHANNEL) {
		_set_current_txrx_channel(PAL_DETECT_IMPEDANCE_CHANNEL);
	}

	/* Transmit dummy message for impedance detection */
	_imp_detect_tx_msg();
}

#endif

/**
 * \brief This function initializes PRIME PLC PAL layer
 *
 * \note This function will be invoked by function pal_init() and it allows the user
 * to configure PAL layer initialization. Users can adapt the PAL layer to every
 * hardware platform.
 * This is an example of usage.
 * ATMEL strongly recommends to consult with technical support to change any
 * parameter in this file.
 *
 */
void pal_plc_init(void)
{
	uint32_t ul_bin_addr;
	uint32_t ul_bin_size;
	atpl360_hal_wrapper_t x_atpl360_hal_wrp;
	atpl360_dev_callbacks_t x_atpl360_cbs;
	atpl360_res_t uc_ret;
	uint8_t uc_id;
	uint8_t uc_ini_tx_channel;

	/* Init ATPL360 component */
	x_atpl360_hal_wrp.plc_init = prime_hal_plc_init;
	x_atpl360_hal_wrp.plc_reset = prime_hal_plc_reset;
	x_atpl360_hal_wrp.plc_set_stby_mode = prime_hal_plc_set_stby_mode;
	x_atpl360_hal_wrp.plc_set_handler = prime_hal_plc_set_handler;
	x_atpl360_hal_wrp.plc_send_boot_cmd = prime_hal_plc_send_boot_cmd;
	x_atpl360_hal_wrp.plc_write_read_cmd = prime_hal_plc_send_wrrd_cmd;
	x_atpl360_hal_wrp.plc_enable_int = prime_hal_plc_enable_interrupt;
	x_atpl360_hal_wrp.plc_delay = prime_hal_plc_delay;
	x_atpl360_hal_wrp.plc_get_thw = prime_hal_plc_get_thermal_warning;
	atpl360_init(&sx_atpl360_desc, &x_atpl360_hal_wrp);

	/* Enable ATPL360 */
	ul_bin_size = _get_pl360_bin_addressing(&ul_bin_addr);
	uc_ret = atpl360_enable(ul_bin_addr, ul_bin_size);
	if (uc_ret == ATPL360_ERROR) {
		LOG_PAL_DEBUG("\r\nPAL: atpl360_enable call error!\r\n");

		sb_disable_time_sync = true;
#ifdef PAL_MULTIPHY
		prime_hal_debug_report(PHY_LAYER_PLC_NOT_AVAILABLE);
#else
		prime_hal_debug_report(CRITICAL_ERROR_PL360_FAILURE);
#endif
	} else {
		sb_disable_time_sync = false;
	}

	LOG_PAL_DEBUG("\r\nPAL: atpl360_enable ok\r\n");

	/* Callback configuration. Set NULL as Not used */
	x_atpl360_cbs.data_confirm = _data_confirm;
	x_atpl360_cbs.data_indication = _data_indication;
	x_atpl360_cbs.exception_event = _handler_reset;
	x_atpl360_cbs.addons_event = _handler_serial_atpl360_event;
	x_atpl360_cbs.debug_mode_cb = NULL;
	x_atpl360_cbs.sleep_mode_cb = NULL;
	sx_atpl360_desc.set_callbacks(&x_atpl360_cbs);

#ifdef _PRIME_SIM_
	suc_sniffer_enabled = 1;
#else
	suc_sniffer_enabled = 0;
#endif
	suc_error_info = 0;

	for(uc_id = 0; uc_id < PAL_TX_NUM_BUFFERS; uc_id++) {
		spb_buf_id[uc_id] = false;
	}

	sb_exception_pend = false;
	sus_current_user_band_plan = 0;

	sul_hi_timer_ref = 0;
	sul_previous_timer_ref = 0;

#ifdef PAL_ENABLE_MULTICHANNEL
	/* Get maximum number of channels allowed from PHY layer */
	if (!sx_atpl360_desc.get_config(ATPL360_REG_MAX_NUM_CHANNELS, &suc_max_num_channels, 1, true)) {
		/* If get_config fails, set maximum number of channels to 1 (Single Channel) */
		suc_max_num_channels = 1;
	}

	/* Set maximum number of channels to worst case between PHY and PAL */
	suc_max_num_channels = min(suc_max_num_channels, PAL_MAX_NUM_CHANNELS);

#else
	/* Only Single Channel allowed */
	suc_max_num_channels = 1;
#endif

#ifdef PAL_ENABLE_C11_CFG
	suc_network_detected = 0;
#endif

	/* Always initialize PAL with first channel in band plan */
#ifdef PAL_ENABLE_MULTICHANNEL
	for(uc_ini_tx_channel = CHANNEL1; uc_ini_tx_channel <= CHANNEL78; uc_ini_tx_channel++) {
#else
	for(uc_ini_tx_channel = CHANNEL1; uc_ini_tx_channel <= CHANNEL8; uc_ini_tx_channel++) {
#endif
		if (_check_txrx_channel(uc_ini_tx_channel)) {
			_set_current_txrx_channel(uc_ini_tx_channel);
			break;
		}
	}

	_set_correlation_thresholds();

	suc_phy_att = 0;

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	/* Start impedance detection procedure */
	/* Only if impedance detection not valid for all channels */
	_imp_detect_start();
#endif

	pal_plc_rm_init();
}

/**
 * \brief This function check phy messages
 *
 * \note This function will be invoked by function pal_process() and it allows the user
 * to start callbacks functions from phy layer to upper layers
 * This is an example of usage.
 * ATMEL strongly recommends to consult with technical support to change any
 * parameter in this file.
 *
 */
void pal_plc_process(void)
{
	/* Check if channel must be restored */
	if (sb_exception_pend == true) {
		/* Restart exception flag */
		sb_exception_pend = false;

		/* Restore channel configuration */
		_set_current_txrx_channel(suc_current_channel);

		_set_correlation_thresholds();

		sul_hi_timer_ref = 0;
		sul_previous_timer_ref = 0;
	}

	/* Check ATPL360 pending events */
	atpl360_handle_events();

	if (sb_sync_update) {
		/* Update synchronization between Host and PL360 timers */
		_timer_sync_update();
	}
}

/**
 * \brief This function links callback functions between upper layer and phy layer.
 *
 * \param pal_cbs      Pointer to callbacks structure
 *
 */
void pal_plc_set_callbacks(pal_callbacks_t *pal_cbs)
{
	/* Capture callbacks from upper layers */
	memcpy(&sx_pal_app_callbacks, pal_cbs, sizeof(pal_callbacks_t));

	/* Be sure this callback always is NULL */
	sx_pal_app_callbacks.switch_rf_ch = NULL;

	/* Set USI callback : Phy Sniffer Tool iface, Commands allowed
	   then callback funcions disabled, only sniffer port configured */
	prime_hal_usi_set_callback(PROTOCOL_SNIF_PRIME, NULL, PHY_SNIFFER_USI_PORT);
}

/**
 * \brief Send message using PAL layer
 *
 * \param px_msg      Pointer to transmission data struct
 *
 */
uint8_t pal_plc_data_request(x_pal_msg_tx_t *px_msg)
{
	tx_msg_t phy_msg;

	phy_msg.uc_buffer_id = (enum buffer_id)px_msg->buff_id;
	spb_buf_id[px_msg->buff_id] = true;
	phy_msg.x_csma_mode.uc_disable_rx = px_msg->disable_rx;
	phy_msg.x_csma_mode.uc_sense_count = px_msg->num_senses;
	phy_msg.x_csma_mode.uc_sense_delay_ms = px_msg->sense_delay_ms;
	phy_msg.uc_mod_type = (enum mode_types)px_msg->mode;
	phy_msg.uc_scheme = (enum mod_schemes)px_msg->scheme;
	phy_msg.uc_tx_mode = px_msg->time_mode;
	phy_msg.ul_tx_time = px_msg->time_delay;
	phy_msg.us_data_len = px_msg->data_len;
	phy_msg.puc_data_buf = px_msg->data_buf;
	phy_msg.uc_att_level = suc_phy_att + px_msg->att_level;

	/* adapt to timer mode for atpl360 */
	if (phy_msg.uc_tx_mode == TX_MODE_ABSOLUTE) {
		/* Convert absolute time from Host to PL360 time */
		phy_msg.ul_tx_time = _time_host_to_pl360(phy_msg.ul_tx_time);
	}

	LOG_PAL_DEBUG("req time %u \n", px_msg->time_delay);

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	if (!sb_impedance_detected) {
		return PAL_TX_RESULT_BUSY_TX;
	} else {
		return sx_atpl360_desc.send_data(&phy_msg);
	}
#else
		return sx_atpl360_desc.send_data(&phy_msg);
#endif
}

/**
 * \brief Send message using PHY Serial layer
 *
 * \param uc_buff_id   Buffer to be flushed
 *
 */
void pal_plc_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode)
{
	(void)(ul_time_sync);
	(void)(us_pch);
	(void)(uc_time_mode);
}

/**
 * \brief Get timer
 *
 * \param pul_timer      Pointer to timer value
 * \param us_pch         Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_timer_get(uint32_t *pul_timer, uint16_t us_pch)
{
	(void)(us_pch);
	uint32_t ul_time;

	ul_time = prime_hal_timer_1us_get();

	if (ul_time < sul_previous_timer_ref) {
		sul_hi_timer_ref++;
	}

	sul_previous_timer_ref = ul_time;

	*pul_timer = ul_time;

	LOG_PAL_DEBUG("timer get %u \n", *pul_timer);

	return PAL_CFG_SUCCESS;
}

/**
 * \brief Get extended timer
 *
 * \param pull_timer      Pointer to extended timer value
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_timer_get_extended(uint64_t *pull_timer)
{
	uint32_t ul_time;
	uint64_t ull_time;

	pal_plc_timer_get(&ul_time, 0);
	ull_time = ((uint64_t)sul_hi_timer_ref << 32) | ul_time;

	*pull_timer = ull_time;
	return PAL_CFG_SUCCESS;
}

/**
 * \brief Get carrier detect
 *
 * \param puc_cd        Pointer to carrier detect
 * \param puc_rssi      Pointer to RSSI
 * \param pul_time      Pointer to time
 * \param puc_header    Pointer to header type
 * \param us_pch        Physical channel to get the carrier detect
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_cd_get(uint8_t *puc_cd, uint8_t *puc_rssi, uint32_t *pul_time, uint8_t *puc_header, uint16_t us_pch)
{
	(void)us_pch;
	rx_cd_info_t x_cd_info;
	uint64_t ull_temp;
	uint32_t ul_hi_timer_ref;
	uint32_t ul_rx_time_end;
	uint32_t ul_time_10us;
	uint8_t uc_cd;
	uint8_t uc_header;
	uint8_t uc_rssi;
	bool b_result;

	/* Read Carrier Detect information from PL360 */
	b_result = sx_atpl360_desc.get_config(ATPL360_REG_RX_CD_INFO, &x_cd_info, sizeof(x_cd_info), true);

	if (!b_result) {
		return(PAL_CFG_INVALID_INPUT);
	}

	/* Convert RSSI to format from PRIME spec: 0: <=70 dBuV; 1: <=72 dBuV; ... 15: >98 dBuV */
	uc_rssi = max(x_cd_info.uc_rssi_avg, 69);
	uc_rssi -= 69;
	uc_rssi >>= 1;
	*puc_rssi = min(uc_rssi, 15);

	if (x_cd_info.uc_rx_state == CD_RX_IDLE) {
		/* Free channel */
		uc_cd = 0;
		uc_header = 0;
		ul_time_10us = 0;
	} else {
		/* Busy channel */
		uc_cd = 1;
		if (x_cd_info.uc_rx_state == CD_RX_PAYLOAD) {
			uc_header = 0;
		} else {
			uc_header = 1;
		}

		/* Check if there is overflow since last time ref read (assumed that last read time ref is previous to Rx end time) */
		ul_rx_time_end = x_cd_info.ul_rx_time_end;
		ul_rx_time_end = _time_pl360_to_host(ul_rx_time_end);
		ul_hi_timer_ref = sul_hi_timer_ref;
		if (ul_rx_time_end < sul_previous_timer_ref) {
			ul_hi_timer_ref++;
		}

		/* Convert time to extended mode (64 bits) */
		ull_temp = ((uint64_t)ul_hi_timer_ref << 32) + ul_rx_time_end;

		/* To adjust more, make round instead floor */
		ull_temp += 5;

		/* Convert time to 10us base */
		ul_time_10us = (uint32_t)((ull_temp / 10) & 0xffffffff);
	}

	/* Write output parameters */
	*puc_cd = uc_cd;
	*pul_time = ul_time_10us;
	*puc_header = uc_header;

	LOG_PAL_DEBUG(("CD %u\r\n", (unsigned int)uc_cd));

	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Get zero-cross time
 *
 * \param pul_zct      Pointer to zero-cross time (in 10s of microsec)
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_zct_get(uint32_t *pul_zct, uint16_t us_pch)
{
	(void)us_pch;

	/* Read ZC data */
	uint64_t ull_temp;
	uint32_t ul_time_ref;
	uint32_t ul_zc_time_1us;
	uint32_t ul_zc_time_10us;

	sx_atpl360_desc.get_config(ATPL360_REG_ZC_TIME, &ul_zc_time_1us, sizeof(ul_zc_time_1us), true);
	ul_zc_time_1us = _time_pl360_to_host(ul_zc_time_1us);

	pal_timer_get_extended(&ull_temp);

	ul_time_ref = (uint32_t)(ull_temp & 0xffffffff);

	ull_temp = ull_temp & (0xffffffff00000000);

	ull_temp = ull_temp & (0xffffffff00000000);
	if (ul_time_ref < ul_zc_time_1us) {
		ull_temp -= 0x100000000;
	}

	ull_temp += ul_zc_time_1us;

	/* To adjust more, make round instead floor */
	ull_temp += 5;

	ul_zc_time_10us = (uint32_t)((ull_temp / 10) & 0xffffffff);
	*pul_zct = ul_zc_time_10us;

	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Get Automatic Gain Mode (AGC) - Not supported in this release
 *
 * \param puc_mode     Pointer to Auto/Manual mode
 * \param puc_gain     Pointer to initial receiving gain in auto mode
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_agc_get(uint8_t *puc_mode, uint8_t *puc_gain, uint16_t us_pch)
{
	(void)(us_pch);

	*puc_mode = 0; /* manual mode is not supported */
	*puc_gain = 0;

	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Set Automatic Gain Mode (AGC) - Not supported in this release
 *
 * \param uc_mode     Auto/Manual mode
 * \param uc_gain     Initial receiving gain in auto mode
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_agc_set(uint8_t uc_mode, uint8_t uc_gain, uint16_t us_pch)
{
	(void)(uc_mode);
	(void)(uc_gain);
	(void)(us_pch);

	return(PAL_CFG_INVALID_INPUT); /* Not supported */
}

/**
 * \brief Get Noise Level (NL)
 *
 * \param puc_noise    Pointer to Noise Level
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_nl_get(uint8_t *puc_noise, uint16_t us_pch)
{
	(void)us_pch;

	/* cinr is in 1/4 db. */
	*puc_noise = (suc_last_rssi_val - (suc_last_cinr_val >> 2));

	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Get the channel/band
 *
 * \param us_pch       Pointer to the Physical channel
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_ch_get(uint16_t *pus_pch)
{
#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	if (!sb_impedance_detected) {
		/* Impedance detection in progress */
		/* Return channel to be restored after TX */
		if (_check_txrx_channel(suc_prev_channel)) {
			/* Return channel to be restored after TX */
			*pus_pch = _encode_pch(suc_prev_channel);
			return PAL_CFG_SUCCESS;
		} else {
			return PAL_CFG_INVALID_INPUT;
		}
	}
#endif

	*pus_pch = _encode_pch(suc_current_channel);
	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Set the channel/band
 *
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_ch_set(uint16_t us_pch)
{
	uint8_t uc_channel;
	uc_channel = _decode_pch(us_pch);

#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	if (!sb_impedance_detected) {
		/* Impedance detection in progress */
		/* Return result depending on availability of channel */
		if (_check_txrx_channel(uc_channel)) {
			/* Store channel to restore after TX */
			suc_prev_channel = uc_channel;
			return PAL_CFG_SUCCESS;
		} else {
			return PAL_CFG_INVALID_INPUT;
		}
	}
#endif

	return(_set_current_txrx_channel(uc_channel));
}

/**
 * \brief Get extended PHY layer parameter
 *
 * \param us_id       Parameter Identification
 * \param *p_val      Pointer to parameter value
 * \param us_len      Length of the parameter
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT
 */
uint8_t pal_plc_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	rx_cd_info_t x_cd_info;
	uint16_t us_360_id;

	(void)us_pch;

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_CONTINUOUS_TX_EN:
		/* Not needed */
		return(PAL_CFG_INVALID_INPUT);

	case PAL_ID_ZC_PERIOD:
		us_360_id = ATPL360_REG_ZC_PERIOD;
		break;

	case PAL_ID_HOST_VERSION:
		us_360_id = ATPL360_HOST_VERSION_ID;
		break;

	case PAL_ID_CFG_MAX_TXRX_NUM_CHANNELS:
		*(uint8_t *)p_val = suc_max_num_channels;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_TXRX_DOUBLE_CHANNEL_LIST:
		*(uint8_t *)p_val = (uint8_t)((_get_channel_list() >> 8) & 0xff);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_INFO_VERSION:
		us_360_id = ATPL360_REG_VERSION_NUM;
		break;

	case PAL_ID_PHY_SNIFFER_EN:
		*(uint8_t *)p_val = suc_sniffer_enabled;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_AUTODETECT_BRANCH:
		us_360_id = ATPL360_REG_CFG_AUTODETECT_IMPEDANCE;
		break;

	case PAL_ID_CFG_IMPEDANCE:
		us_360_id = ATPL360_REG_CFG_IMPEDANCE;
		break;

	case PAL_ID_CFG_TXRX_CHANNEL:
#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
		if (!sb_impedance_detected) {
			/* Impedance detection in progress */
			/* Return channel to be restored after TX */
			if (_check_txrx_channel(suc_prev_channel)) {
				/* Return channel to be restored after TX */
				*(uint8_t *)p_val = suc_prev_channel;
				return PAL_CFG_SUCCESS;
			} else {
				return PAL_CFG_INVALID_INPUT;
			}
		}
#endif
		*(uint8_t *)p_val = suc_current_channel;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_TXRX_CHANNEL_LIST:
		*(uint8_t *)p_val = (uint8_t)(_get_channel_list() & 0xff);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_NETWORK_DETECTION:
#ifdef PAL_ENABLE_C11_CFG
		*(uint8_t *)p_val = suc_network_detected;
#else
		*(uint8_t *)p_val = 0;
#endif
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_ATTENUATION:
		 *(uint8_t *)p_val = suc_phy_att;
		return (PAL_CFG_SUCCESS);

	case PAL_ID_RX_PAYLOAD_LEN_SYM:
		us_360_id = ATPL360_REG_RX_PAY_SYMBOLS;
		break;

	case PAL_ID_INFO_DEVICE:
		us_360_id = ATPL360_REG_MODEL;
		break;

	case PAL_ID_REMAINING_FRAME_DURATION:
		if (sx_atpl360_desc.get_config(ATPL360_REG_RX_CD_INFO, &x_cd_info, sizeof(x_cd_info), true)) {
			if (x_cd_info.uc_rx_state == CD_RX_IDLE) {
				*(uint32_t *)p_val = 0;
			} else {
				*(uint32_t *)p_val = x_cd_info.ul_rx_time_end - x_cd_info.ul_current_time;
			}

			return(PAL_CFG_SUCCESS);
		} else {
			return(PAL_CFG_INVALID_INPUT);
		}

	case PAL_ID_PLC_RX_PHY_PARAMS:
		memcpy(p_val, &sx_rx_phy_params, us_len);
		return(PAL_CFG_SUCCESS);

	default:
		if (us_id >= 0xFD00) {
			if (us_id != 0xFEEE) {
				us_360_id = (us_id & 0x00FF) | 0x4000;
			} else {
				*(uint8_t *)p_val = suc_error_info;
				return(PAL_CFG_SUCCESS);
			}
		} else {
			*(uint8_t *)p_val = 0;
			return(PAL_CFG_SUCCESS);
		}
	}

	/* Get from phy layer */
	if (sx_atpl360_desc.get_config(us_360_id, p_val, us_len, true)) {
		return(PAL_CFG_SUCCESS);
	} else {
		return(PAL_CFG_INVALID_INPUT);
	}
}

/**
 * \brief Set extended PHY layer parameter
 *
 * \param us_id       Parameter Identification
 * \param *p_val      Pointer to parameter value
 * \param us_len      Length of the parameter
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT
 */
uint8_t pal_plc_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	uint16_t us_360_id;
	uint8_t uc_chn_list;
	uint8_t uc_channel;
	uint8_t uc_cont_tx_mode;
#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
	uint8_t uc_value;
#endif

	(void)us_pch;

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_CONTINUOUS_TX_EN:
		uc_cont_tx_mode = *(uint8_t *)p_val;
		_handle_continuous_tx_mode(uc_cont_tx_mode);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_ZC_PERIOD:
		us_360_id = ATPL360_REG_ZC_PERIOD;
		break;

	case PAL_ID_HOST_VERSION:
		us_360_id = ATPL360_HOST_VERSION_ID;
		break;

	case PAL_ID_CFG_TXRX_DOUBLE_CHANNEL_LIST:
		uc_chn_list = (*(uint8_t *)p_val);
		_set_channel_list(uc_chn_list, 2);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_INFO_VERSION:
		us_360_id = ATPL360_REG_VERSION_NUM;
		break;

	case PAL_ID_PHY_SNIFFER_EN:
#ifdef ATPL360_ADDONS_ENABLE
		suc_sniffer_enabled = *(uint8_t *)p_val;
#endif
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_AUTODETECT_BRANCH:
		us_360_id = ATPL360_REG_CFG_AUTODETECT_IMPEDANCE;
		break;

	case PAL_ID_CFG_IMPEDANCE:
		us_360_id = ATPL360_REG_CFG_IMPEDANCE;
		break;

	case PAL_ID_CFG_TXRX_CHANNEL:
		uc_channel = (*(uint8_t *)p_val);
#ifdef PAL_DETECT_IMPEDANCE_CHANNEL
		if (!sb_impedance_detected) {
			/* Impedance detection in progress */
			/* Return result depending on availability of channel */
			if (_check_txrx_channel(uc_channel)) {
				/* Store channel to restore after TX */
				suc_prev_channel = uc_channel;
				return PAL_CFG_SUCCESS;
			} else {
				return PAL_CFG_INVALID_INPUT;
			}
		}
#endif
		return(_set_current_txrx_channel(uc_channel));

	case PAL_ID_CFG_TXRX_CHANNEL_LIST:
		uc_chn_list = (*(uint8_t *)p_val);
		_set_channel_list(uc_chn_list, 1);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_RX_PAYLOAD_LEN_SYM:
		us_360_id = ATPL360_REG_RX_PAY_SYMBOLS;
		break;

	case PAL_ID_CFG_MAX_TXRX_NUM_CHANNELS:
	case PAL_ID_INFO_DEVICE:
	case PAL_ID_REMAINING_FRAME_DURATION:
	case PAL_ID_PLC_RX_PHY_PARAMS:
		/* Read only */
		return(PAL_CFG_INVALID_INPUT);

	case PAL_ID_NETWORK_DETECTION:
#ifdef PAL_ENABLE_C11_CFG
		suc_network_detected = (*(uint8_t *)p_val);
		/* Force VLO mode if network detected */
		if (suc_network_detected && (suc_current_channel != PAL_DETECT_IMPEDANCE_CHANNEL)) {
			uc_value = 2;
			sx_atpl360_desc.set_config(ATPL360_REG_CFG_IMPEDANCE, &uc_value, 1);
		}
#endif
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_ATTENUATION:
		suc_phy_att = (*(uint8_t *)p_val);
		return (PAL_CFG_SUCCESS);

	default:
		return(PAL_CFG_INVALID_INPUT);
	}

	/* Set in phy layer */
	if (sx_atpl360_desc.set_config(us_360_id, p_val, us_len)) {
		return(PAL_CFG_SUCCESS);
	} else {
		return(PAL_CFG_INVALID_INPUT);
	}
}

/**
 * \brief Get PLC message duration
 *
 * \param us_pch       Physical channel used
 * \param ul_msg_len   Message length
 * \param uc_scheme    Modulation scheme of message
 * \param uc_mode      Indicates if the message to transmit is type A, type B or type BC
 * \param pul_duration Pointer to message duration in microseconds(output)
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration)
{
	uint32_t ul_frame_duration;
	uint32_t ul_tx_time_chirp_header;
	uint16_t us_frame_len;
	uint8_t uc_symbol_size = 0;

	(void)us_pch;

	ul_frame_duration = 0;
	us_frame_len = us_msg_len;

	if (uc_mode == PAL_MODE_TYPE_A) {
		/* There are 7 bytes inside the header */
		if (us_frame_len < 7) {
			us_frame_len = 0;
		} else {
			us_frame_len -= 7;
		}
	}

	if (us_frame_len != 0) {
		switch (uc_scheme) {
		case PAL_PLC_DBPSK:
			uc_symbol_size = 12;
			break;

		case PAL_PLC_DQPSK:
			uc_symbol_size = 24;
			break;

		case PAL_PLC_D8PSK:
			uc_symbol_size = 36;
			break;

		case PAL_PLC_DBPSK_C:
			uc_symbol_size = 6;
			/* Increase a byte for flushing */
			if (us_frame_len) {
				us_frame_len++;
			}
			break;

		case PAL_PLC_DQPSK_C:
			uc_symbol_size = 12;
			/* Increase a byte for flushing */
			if (us_frame_len) {
				us_frame_len++;
			}
			break;

		case PAL_PLC_D8PSK_C:
			uc_symbol_size = 18;
			/* Increase a byte for flushing */
			if (us_frame_len) {
				us_frame_len++;
			}
			break;

		case PAL_PLC_R_DBPSK:
			uc_symbol_size = 6;
			/* Increase a byte for flushing */
			if (us_frame_len) {
				us_frame_len++;
			}
			break;

		case PAL_PLC_R_DQPSK:
			uc_symbol_size = 12;
			/* Increase a byte for flushing */
			if (us_frame_len) {
				us_frame_len++;
			}
			break;

		default:
			/* scheme error */
			*pul_duration = 0;
			return(PAL_CFG_INVALID_INPUT);
			break;
		}
	}

	/* Update tx frame duration */
	ul_frame_duration =  us_frame_len / uc_symbol_size;
	if (us_frame_len % uc_symbol_size) {
		ul_frame_duration++;
	}

	/* adjust ROB mode */
	if (uc_scheme & 0x08) {
		ul_frame_duration <<= 2;
	}

	ul_frame_duration *= PHY_SYMBOL_TIME;
	switch (uc_mode) {
	case PAL_MODE_TYPE_A:
		ul_tx_time_chirp_header = PHY_CHIRP_TIME + PHY_HEADER_TIME;
		break;

	case PAL_MODE_TYPE_B:
		ul_tx_time_chirp_header = PHY_CHIRP_MODE_B_TIME + PHY_HEADER_B_BC_TIME;
		break;

	case PAL_MODE_TYPE_BC:
		ul_tx_time_chirp_header = PHY_CHIRP_MODE_BC_TIME + PHY_HEADER_B_BC_TIME;
		break;

	default:
		ul_tx_time_chirp_header = PHY_CHIRP_MODE_BC_TIME + PHY_HEADER_B_BC_TIME;
		break;
	}

	/* Adjust chirp and header for PHY frame */
	ul_frame_duration += ul_tx_time_chirp_header;

	*pul_duration = ul_frame_duration;

	return(PAL_CFG_SUCCESS);

}

/**
 * \brief Convert QT value to Signal Noise Ratio (SNR)
 *
 * \param puc_snr      SNR parameter
 * \param uc_qt        QT parameter to get SNR level
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_plc_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch)
{
	(void)us_pch;

	if (uc_qt == 0) {
		*puc_snr = 0;
		return(PAL_CFG_INVALID_INPUT);
	} else {
		*puc_snr = uc_qt / 12 + 1;
		if (*puc_snr > 7) {
			*puc_snr  = 7;
		}

		return(PAL_CFG_SUCCESS);
	}
}

/**
 * \brief Get Capture Noise Data
 *
 * \param puc_dst          Pointer to destiny buffer to store data
 * \param uc_mode          Capture Mode (See 'Capture Mode Bit Mask' in aplt360_comm.h file)
 * \param ul_time_start    Start time in us based on PL360 timer reference
 * \param ul_duration      Duration time in us
 *
 * \return Size in bytes of data capture.
 */
uint16_t pal_plc_get_signal_capture(uint8_t *puc_dst, uint8_t uc_mode, uint32_t ul_time_start, uint32_t ul_duration)
{
	uint8_t *puc_ptr;
	signal_capture_st_info_t x_status_info;
	uint8_t puc_param[9];
	uint8_t uc_idx;

	sx_atpl360_desc.get_config(ATPL360_REG_SIGNAL_CAPTURE_STATUS, &x_status_info, 2, true);

	/* Check status */
	while (x_status_info.uc_status == SIGNAL_CAPTURE_RUNNING) {
		sx_atpl360_desc.get_config(ATPL360_REG_SIGNAL_CAPTURE_STATUS, &x_status_info, 2, true);
		prime_hal_plc_delay(DELAY_TREF_MS, 5);
	}

	/* Launch Capture */
	puc_ptr = puc_param;
	*puc_ptr++ = uc_mode;
	*puc_ptr++ = (uint8_t)(ul_time_start >> 24);
	*puc_ptr++ = (uint8_t)(ul_time_start >> 16);
	*puc_ptr++ = (uint8_t)(ul_time_start >> 8);
	*puc_ptr++ = (uint8_t)(ul_time_start);
	*puc_ptr++ = (uint8_t)(ul_duration >> 24);
	*puc_ptr++ = (uint8_t)(ul_duration >> 16);
	*puc_ptr++ = (uint8_t)(ul_duration >> 8);
	*puc_ptr++ = (uint8_t)(ul_duration);
	sx_atpl360_desc.set_config(ATPL360_REG_SIGNAL_CAPTURE_START, puc_param, sizeof(puc_param));

	/* Check status */
	sx_atpl360_desc.get_config(ATPL360_REG_SIGNAL_CAPTURE_STATUS, &x_status_info, 2, true);
	while (x_status_info.uc_status != SIGNAL_CAPTURE_READY) {
		sx_atpl360_desc.get_config(ATPL360_REG_SIGNAL_CAPTURE_STATUS, &x_status_info, 2, true);
		prime_hal_plc_delay(DELAY_TREF_MS, 5);
	}

	/* Read Noise Data */
	puc_ptr = puc_dst;
	uc_idx = 0;
	while (uc_idx < x_status_info.uc_num_frags) {
		sx_atpl360_desc.set_config(ATPL360_REG_SIGNAL_CAPTURE_FRAGMENT, &uc_idx, 1);
		uc_idx++;
		sx_atpl360_desc.get_config(ATPL360_REG_SIGNAL_CAPTURE_DATA, puc_ptr, SIGNAL_CAPTURE_FRAG_SIZE, true);
		puc_ptr += SIGNAL_CAPTURE_FRAG_SIZE;
	}

	return (uint16_t)(puc_ptr - puc_dst);
}

#endif /* PAL_PLC */

/* @} */

/**
 *
 * \file
 *
 * \brief PRIME RF Phy Abstraction Layer for RF215
 *
 * Copyright (c) 2023 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */


#include <string.h>

/* Prime includes */
#include "pal.h"
#include "pal_rf_rm.h"
#include "prime_hal_wrapper.h"
#ifdef PAL_MULTIPHY
#include "mpal.h"
#endif

#include "at86rf.h"
#include "ieee_15_4_sun_fsk.h"
#include "ieee_15_4_sun_ofdm.h"

/* #define PAL_DEBUG_ENABLE */
#ifdef PAL_DEBUG_ENABLE
#       define LOG_PAL_DEBUG(...)   printf(__VA_ARGS__ )
#else
#       define LOG_PAL_DEBUG(...)
#endif

#ifndef PAL_MULTIPHY
#define pal_rf_init pal_init
#define pal_rf_process pal_process
#define pal_rf_set_callbacks pal_set_callbacks
#define pal_rf_data_request pal_data_request
#define pal_rf_set_chn_req pal_set_chn_req
#define pal_rf_timer_get pal_timer_get
#define pal_rf_timer_get_extended pal_timer_get_extended
#define pal_rf_zct_get pal_zct_get
#define pal_rf_nl_get pal_nl_get
#define pal_rf_cca_get pal_cca_get
#define pal_rf_ch_get pal_ch_get
#define pal_rf_ch_set pal_ch_set
#define pal_rf_get_cfg pal_get_cfg
#define pal_rf_set_cfg pal_set_cfg
#define pal_rf_snr_get pal_snr_get
#define pal_rf_get_msg_duration pal_get_msg_duration
#endif


#ifdef PAL_RF

/**
 * \weakgroup prime_pal_group
 * @{
 */


static uint32_t sul_exception_reset;
static uint32_t sul_exception_spi;
static uint32_t sul_exception_tot;

static uint32_t sul_hi_timer_ref;
static uint32_t sul_previous_timer_ref;

/* Bit to indicate radio channel */
#define RF_CHANNEL_BIT  0x200

/* Macro to configure frequency hopping channel */
#define RF_FREQ_HOPPING_CHANNEL      0x03FF

static pal_callbacks_t sx_pal_app_callbacks;

static uint16_t sus_current_channel;

static uint16_t sus_num_rf_channels;

static uint8_t suc_rf_sniffer_enabled;

static uint8_t suc_rf_phy_mode;

#ifdef RF_FREQUENCY_HOPPING_ENABLED
#define MAX_NUM_RF_CHANNELS          512

#define MAX_NUM_BYTES_RF_CHANNELS    (MAX_NUM_RF_CHANNELS / 8)

#define MAX_CHANNELS_BCN_SEQUENCE    32

static uint8_t suc_bits_sequence[MAX_NUM_BYTES_RF_CHANNELS];

static uint16_t sus_hopping_sequence_length;

static uint8_t suc_bits_beacon_sequence[MAX_NUM_BYTES_RF_CHANNELS];

static uint8_t suc_hopping_bcn_sequence_length;

static uint16_t sus_current_rx_fh_channel;

static uint16_t sus_next_rx_fh_channel;
#endif

/* RF Modulation parameters (FSK: FEC; OFDM: MCS) used for transmission */
static at86rf_mod_frame_params_t sx_rf_mod_params;

/**
 * \brief AT86RF TX confirm callback
 *
 * \param uc_trx_id TRX identifier
 * \param px_tx_cfm Pointer to TX confirm
 */
static void _at86rf_tx_cfm_cb(uint8_t uc_trx_id, at86rf_tx_cfm_t *px_tx_cfm)
{
	UNUSED(uc_trx_id);

	if (sx_pal_app_callbacks.data_confirm) {
		at86rf_tx_cfm_res_t uc_tx_res;
		x_pal_data_cfm_t x_data_cfm;

        x_data_cfm.us_pch = sus_current_channel | RF_CHANNEL_BIT;
        x_data_cfm.mode = PAL_MODE_TYPE_RF;

		/* Time of TX frame end */
		x_data_cfm.tx_time = px_tx_cfm->ul_tx_time_ini;
		x_data_cfm.buff_id = px_tx_cfm->uc_tx_id;

		/* Translate TX result to PAL Transmission Results */
		uc_tx_res = px_tx_cfm->uc_tx_res;
		switch (uc_tx_res) {
		case AT86RF_TX_SUCCESS:
			x_data_cfm.result = PAL_TX_RESULT_SUCCESS;
			break;

		case AT86RF_TX_BUSY_RX:
		case AT86RF_TX_BUSY_CHN:
		case AT86RF_TX_CANCEL_BY_RX:
			x_data_cfm.result = PAL_TX_RESULT_BUSY_CH;
			break;

		case AT86RF_TX_BUSY_TX:
		case AT86RF_TX_FULL_BUFFERS:
		case AT86RF_TX_TRX_SLEPT:
			x_data_cfm.result = PAL_TX_RESULT_BUSY_TX;
			break;

		case AT86RF_TX_INVALID_LEN:
			x_data_cfm.result = PAL_TX_RESULT_INV_LENGTH;
			break;

		case AT86RF_TX_INVALID_TRX_ID:
		case AT86RF_TX_INVALID_PARAM:
			x_data_cfm.result = PAL_TX_RESULT_INV_PARAM;
			break;

		case AT86RF_TX_ERROR_UNDERRUN:
		case AT86RF_TX_TIMEOUT:
			x_data_cfm.result = PAL_TX_RESULT_TIMEOUT;
			break;

		case AT86RF_TX_ABORTED:
		case AT86RF_TX_CANCELLED:
		default:
			x_data_cfm.result = PAL_TX_RESULT_PHY_ERROR;
			break;
		}

		LOG_PAL_DEBUG("PalRfDataConfirm (%hhu). Time %u\r\n", uc_tx_res, px_tx_cfm->ul_tx_time_ini);

		/* Notify data confirm */
		sx_pal_app_callbacks.data_confirm(&x_data_cfm);
	}

}

/**
 * \brief AT86RF RX indication callback
 *
 * \param uc_trx_id TRX identifier
 * \param px_rx_ind Pointer to RX indication
 */
static void _at86rf_rx_ind_cb(uint8_t uc_trx_id, at86rf_rx_ind_t *px_rx_ind)
{
	UNUSED(uc_trx_id);

	if (sx_pal_app_callbacks.data_indication) {
		x_pal_data_ind_t x_data_ind;

		x_data_ind.data_buf = px_rx_ind->puc_data;
		x_data_ind.rx_time = px_rx_ind->ul_rx_time_ini;
		x_data_ind.buff_id = 0;
		x_data_ind.us_pch = sus_current_channel | RF_CHANNEL_BIT;
		pal_rf_rm_max_modulation_rx_msg(px_rx_ind->sc_rssi_dBm, &x_data_ind.us_estimated_bitrate, &x_data_ind.uc_less_robust_mod);
		x_data_ind.uc_lqi = pal_rf_rm_get_lqi(px_rx_ind->sc_rssi_dBm);
		x_data_ind.ss_rssi = px_rx_ind->sc_rssi_dBm;
		x_data_ind.data_len = px_rx_ind->us_psdu_len;
		x_data_ind.header_type = (x_data_ind.data_buf[0] >> 4) & 0x03;
		x_data_ind.mode = PAL_MODE_TYPE_RF;
		x_data_ind.scheme = px_rx_ind->x_mod_params.x_fsk.uc_fec_enabled;

		LOG_PAL_DEBUG("PalRfDataIndication. %hu bytes. Time %u. RSSI %hhd\r\n", px_rx_ind->us_psdu_len, px_rx_ind->ul_rx_time_ini, px_rx_ind->sc_rssi_dBm);

		/* Notify data indication */
		sx_pal_app_callbacks.data_indication(&x_data_ind);
	}


}

#ifdef RF_FREQUENCY_HOPPING_ENABLED
/**
 * \brief AT86RF exception callback
 *
 * \param uc_exception_mask Exception type mask
 */
static void _at86rf_chn_switch_cb(uint8_t uc_trx_id, at86rf_tx_cfm_res_t uc_result)
{
	UNUSED(uc_trx_id);

	if (uc_result == AT86RF_TX_SUCCESS) {
		sus_current_rx_fh_channel = sus_next_rx_fh_channel;
	}

	if (sx_pal_app_callbacks.switch_rf_ch) {
		x_pal_switch_rf_chn_t x_switch_rf_chn;

		x_switch_rf_chn.us_new_channel = sus_current_rx_fh_channel | RF_CHANNEL_BIT;
		sx_pal_app_callbacks.switch_rf_ch(&x_switch_rf_chn);
	}

}
#endif

/**
 * \brief AT86RF exception callback
 *
 * \param uc_exception_mask Exception type mask
 */
static void _at86rf_exception_cb(uint8_t uc_exception_mask)
{
	sul_exception_tot++;
	if (uc_exception_mask & AT86RF_EXCEPTION_RESET) {
		sul_exception_reset++;
	}

	if (uc_exception_mask & AT86RF_EXCEPTION_SPI_ERR) {
		sul_exception_spi++;
	}

	LOG_PAL_DEBUG("AT86F Exception: 0x%02x\r\n", uc_exception_mask);
}

#ifdef AT86RF_ADDONS_ENABLE
/**
 * \brief Handler to receive data from RF215.
 */
static void _handler_serial_at86rf_event(uint8_t *puc_serial_data, uint16_t us_len)
{
	x_usi_serial_cmd_params_t x_usi_msg;

	if (suc_rf_sniffer_enabled) {
		x_usi_msg.uc_protocol_type = PROTOCOL_SNIF_PRIME;
		x_usi_msg.ptr_buf = puc_serial_data;
		x_usi_msg.us_len = us_len;
		prime_hal_usi_send_cmd(&x_usi_msg);
	}
}
#endif


#ifdef RF_FREQUENCY_HOPPING_ENABLED
static void _build_valid_channels_array(at86rf_phy_cfg_t *px_phy_cfg)
{
	uint16_t us_index;
	uint8_t uc_bits_ranges[MAX_NUM_BYTES_RF_CHANNELS];
	uint8_t uc_index;

	uint8_t auc_intervals[NUM_RANGES_CHANNELS_INCLUDED_SEQUENCE][2] = RANGES_CHANNELS_INCLUDED_SEQUENCE;
	uint16_t aus_bcn_channels[NUM_CHANNELS_BCN_SEQUENCE] = CHANNELS_BCN_SEQUENCE;
	uint16_t us_excluded_channels[NUM_CHANNELS_EXCLUDED] = CHANNELS_EXLUDED;

	memset(suc_bits_sequence, 0, MAX_NUM_BYTES_RF_CHANNELS);
	memset(uc_bits_ranges, 0, MAX_NUM_BYTES_RF_CHANNELS);

	/* First include all possible channels for the band */
	for (us_index = px_phy_cfg->x_chn_cfg.us_chn_num_min; us_index <= px_phy_cfg->x_chn_cfg.us_chn_num_max; us_index++) {
		uint8_t uc_byte, uc_bit;

		uc_byte = us_index / 8;
		uc_bit = us_index % 8;

		suc_bits_sequence[uc_byte] = suc_bits_sequence[uc_byte] | (1 << uc_bit);
	}

	if (px_phy_cfg->x_chn_cfg.us_chn_num_min2 != 0xFFFF) {
		for (us_index = px_phy_cfg->x_chn_cfg.us_chn_num_min2; us_index <= px_phy_cfg->x_chn_cfg.us_chn_num_max2; us_index++) {
			uint8_t uc_byte, uc_bit;

			uc_byte = us_index / 8;
			uc_bit = us_index % 8;

			suc_bits_sequence[uc_byte] = suc_bits_sequence[uc_byte] | (1 << uc_bit);
		}
	}

	/* Build BCN sequence */
	for (uc_index = 0; uc_index < NUM_CHANNELS_BCN_SEQUENCE; uc_index++) {
		uint8_t uc_byte, uc_bit;

		uc_byte = aus_bcn_channels[uc_index] / 8;
		uc_bit = aus_bcn_channels[uc_index] % 8;
		/* Checking that is a valid channel */
		if (suc_bits_sequence[uc_byte] & (1 << uc_bit)) {
			suc_bits_beacon_sequence[uc_byte] = suc_bits_beacon_sequence[uc_byte] | (1 << uc_bit);
		}
	}

	/* Start building main hopping sequence */
	/*  Intersects with the intervals defined in conf_pal.h */
	for (uc_index = 0; uc_index < NUM_RANGES_CHANNELS_INCLUDED_SEQUENCE; uc_index++) {
		for (us_index = auc_intervals[uc_index][0]; us_index <= auc_intervals[uc_index][1]; us_index++) {
			uint8_t uc_byte, uc_bit;

			uc_byte = us_index / 8;
			uc_bit = us_index % 8;

			uc_bits_ranges[uc_byte] = uc_bits_ranges[uc_byte] | (1 << uc_bit);
		}

	}

	for (uc_index = 0; uc_index < MAX_NUM_BYTES_RF_CHANNELS; uc_index++) {
		suc_bits_sequence[uc_index] &= uc_bits_ranges[uc_index];
	}


	/* Exclude the channels */
	for (uc_index = 0; uc_index < NUM_CHANNELS_EXCLUDED; uc_index++) {
		uint8_t uc_byte, uc_bit;

		uc_byte = us_excluded_channels[uc_index] / 8;
		uc_bit = us_excluded_channels[uc_index] % 8;

		suc_bits_sequence[uc_byte] = suc_bits_sequence[uc_byte] & (~(1 << uc_bit));
	}
}

static void _set_rx_fh_channel(void)
{
	uint16_t us_channel;

	for (us_channel = 0; us_channel < MAX_NUM_RF_CHANNELS; us_channel++) {
		uint8_t uc_bit, uc_byte;

		uc_byte = us_channel / 8;
		uc_bit = us_channel % 8;

		if (suc_bits_beacon_sequence[uc_byte] & (1 << uc_bit)) {
			sus_current_rx_fh_channel = us_channel;
			sus_next_rx_fh_channel = us_channel;
			break;
		}
	}
}
#endif

static void _update_phy_configuration(void)
{
	at86rf_phy_cfg_t x_phy_cfg_09;

	/* Get PHY configuration */
	at86rf_pib_get(AT86RF_TRX_RF09_ID, AT86RF_PIB_PHY_CONFIG, &x_phy_cfg_09);

	suc_rf_phy_mode = x_phy_cfg_09.uc_phy_mod;
	if (suc_rf_phy_mode == AT86RF_PHY_MOD_FSK) {
		/* FSK: FEC disabled by default */
		sx_rf_mod_params.x_fsk.uc_fec_enabled = AT86RF_FSK_FEC_OFF;
	} else if (suc_rf_phy_mode == AT86RF_PHY_MOD_OFDM) {
		/* OFDM: MCS most robust by default (depending on option) */
		switch (x_phy_cfg_09.u_mod_cfg.x_ofdm.uc_opt) {
		case AT86RF_OFDM_OPT_4:
			sx_rf_mod_params.x_ofdm.uc_mcs = AT86RF_OFDM_MCS_2;
			break;

		case AT86RF_OFDM_OPT_3:
			sx_rf_mod_params.x_ofdm.uc_mcs = AT86RF_OFDM_MCS_1;
			break;

		case AT86RF_OFDM_OPT_2:
		case AT86RF_OFDM_OPT_1:
		default:
			sx_rf_mod_params.x_ofdm.uc_mcs = AT86RF_OFDM_MCS_0;
			break;
		}
	}

#ifdef RF_FREQUENCY_HOPPING_ENABLED
	_build_valid_channels_array(&x_phy_cfg_09);
	sus_current_channel = RF_FREQ_HOPPING_CHANNEL;
	sus_hopping_sequence_length = MAC_HOPPING_SEQUENCE_LENGTH;
	suc_hopping_bcn_sequence_length = MAC_HOPPING_BCN_SEQUENCE_LENGTH;
	_set_rx_fh_channel();
#else
	/* Always initialize PAL RF to min channel */
	sus_current_channel = x_phy_cfg_09.x_chn_cfg.us_chn_num_min;
#endif

	sus_num_rf_channels = x_phy_cfg_09.x_chn_cfg.us_chn_num_max - x_phy_cfg_09.x_chn_cfg.us_chn_num_min + 1;
	if (x_phy_cfg_09.x_chn_cfg.us_chn_num_min2 != 0xFFFF) {
		sus_num_rf_channels += x_phy_cfg_09.x_chn_cfg.us_chn_num_max2 - x_phy_cfg_09.x_chn_cfg.us_chn_num_min2 + 1;
	}
}

/** \brief Transmission parameters */
/* @{ */
void pal_rf_init(void)
{
	at86rf_hal_wrapper_t x_at86rf_hal_wrp;
	at86rf_callbacks_t x_at86rf_callbacks;
	at86rf_phy_ini_params_t x_phy_ini_params_09;
	at86rf_res_t uc_init_res;

	/* Initialize exception counters */
	sul_exception_reset = 0;
	sul_exception_spi = 0;
	sul_exception_tot = 0;

	sul_hi_timer_ref = 0;
	sul_previous_timer_ref = 0;

	/* Fill AT86RF HAL wrapper */
	x_at86rf_hal_wrp.rf_init = prime_hal_prf_if_init;
	x_at86rf_hal_wrp.rf_reset = prime_hal_prf_if_reset;
	x_at86rf_hal_wrp.rf_enable_int = prime_hal_prf_if_enable_interrupt;
	x_at86rf_hal_wrp.rf_set_handler = prime_hal_prf_if_set_handler;
	x_at86rf_hal_wrp.rf_send_spi_cmd = prime_hal_prf_if_send_spi_cmd;
	x_at86rf_hal_wrp.rf_is_spi_busy = prime_hal_prf_if_is_spi_busy;
	x_at86rf_hal_wrp.rf_led = prime_hal_prf_if_led;

	x_at86rf_hal_wrp.timer_get = prime_hal_timer_1us_get;
	x_at86rf_hal_wrp.timer_enable_int = prime_hal_timer_1us_enable_interrupt;
	x_at86rf_hal_wrp.timer_set_int = prime_hal_timer_1us_set_int;
	x_at86rf_hal_wrp.timer_cancel_int = prime_hal_timer_1us_cancel_int;

	/* Fill AT86RF callbacks */
	x_at86rf_callbacks.rf_exception_cb = _at86rf_exception_cb;
	x_at86rf_callbacks.rf_tx_cfm_cb = _at86rf_tx_cfm_cb;
	x_at86rf_callbacks.rf_rx_ind_cb = _at86rf_rx_ind_cb;
#ifdef RF_FREQUENCY_HOPPING_ENABLED
	x_at86rf_callbacks.rf_chn_switch_cb = _at86rf_chn_switch_cb;
#else
	x_at86rf_callbacks.rf_chn_switch_cb = NULL;
#endif
#ifdef AT86RF_ADDONS_ENABLE
	x_at86rf_callbacks.rf_addon_event_cb = _handler_serial_at86rf_event;
#endif
#ifdef _PRIME_SIM_
	suc_rf_sniffer_enabled = 1;
#else
	suc_rf_sniffer_enabled = 0;
#endif

	/* Initialize AT86RF component */
	at86rf_init(&x_at86rf_hal_wrp, &x_at86rf_callbacks);

	/* Initial PHY configuration (RF09, Sub-1GHz) */
	x_phy_ini_params_09.us_band_opm = RF_PHY_OPERATION_MODE;
	x_phy_ini_params_09.us_chn_num_ini = 0;

	/* Enable AT86RF component */
	uc_init_res = at86rf_enable(&x_phy_ini_params_09, NULL);

	/* Get PHY configuration and update variables accordingly */
	_update_phy_configuration();

	pal_rf_rm_init();

	if (uc_init_res == AT86RF_SUCCESS) {
		LOG_PAL_DEBUG("PalRfInitialize OK\r\n");
	} else {
		LOG_PAL_DEBUG("PalRfInitialize ERROR (%hhu)\r\n", uc_init_res);
#ifdef PAL_MULTIPHY
		prime_hal_debug_report(PHY_LAYER_RF_NOT_AVAILABLE);
#else
		prime_hal_debug_report(CRITICAL_ERROR_RF215_FAILURE);
#endif
	}
}

/**
 * \brief This function links callback functions between upper layer and phy layer.
 *
 * \param pal_cbs      Pointer to callbacks structure
 *
 */
void pal_rf_set_callbacks(pal_callbacks_t *pal_cbs)
{
	/* Capture callbacks from upper layers */
	memcpy(&sx_pal_app_callbacks, pal_cbs, sizeof(pal_callbacks_t));

#ifdef AT86RF_ADDONS_ENABLE
	/* Set USI callback : Phy Sniffer iface */
	prime_hal_usi_set_callback(PROTOCOL_SNIF_PRIME, NULL, PHY_SNIFFER_USI_PORT);
#endif
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
void pal_rf_process(void)
{
	at86rf_event_handler();
}


/**
 * \brief Send message using PAL layer
 *
 * \param px_msg      Pointer to transmission data struct
 *
 */
uint8_t pal_rf_data_request(x_pal_msg_tx_t *px_msg)
{
	at86rf_tx_params_t x_tx_params;

	/* TX parameters from upper layer */
	x_tx_params.puc_data = px_msg->data_buf;
	x_tx_params.us_psdu_len = px_msg->data_len;
	x_tx_params.ul_tx_time = px_msg->time_delay;
	x_tx_params.uc_txpwr_att = px_msg->att_level;
	x_tx_params.uc_tx_id = px_msg->buff_id;
	x_tx_params.uc_cw = px_msg->num_senses;
	x_tx_params.b_cancel_by_rx = false;

	/* TX channel */
	x_tx_params.us_chn_num = px_msg->us_pch & (~RF_CHANNEL_BIT);

	if (px_msg->disable_rx) {
		/* Forced mode: At least Energy above threshold CCA Mode is
		 * needed to comply with RF regulations */
		x_tx_params.uc_cca_mode = AT86RF_CCA_MODE_1;
	} else {
		/* CSMA used: Energy above threshold and carrier sense CCA Mode */
		x_tx_params.uc_cca_mode = AT86RF_CCA_MODE_3;
	}

	/* Get PHY configuration to set FEC in FSK or MCS in OFDM */
	if (suc_rf_phy_mode == AT86RF_PHY_MOD_FSK) {
		x_tx_params.x_mod_params.x_fsk.uc_fec_enabled = (at86rf_fsk_fec_t)px_msg->scheme;
	} else {
		x_tx_params.x_mod_params = sx_rf_mod_params;
	}

	/* Absolute time mode */
	switch (px_msg->time_mode) {
	case PAL_TX_MODE_ABSOLUTE:
		x_tx_params.uc_time_mode = AT86RF_TX_TIME_ABS;
		break;

	case PAL_TX_MODE_RELATIVE:
		if (px_msg->time_delay) {
			x_tx_params.uc_time_mode = AT86RF_TX_TIME_REL;
		} else {
			x_tx_params.uc_time_mode = AT86RF_TX_TIME_INST;
		}
		break;

	case PAL_TX_MODE_CANCEL:
		x_tx_params.uc_time_mode = AT86RF_TX_CANCEL;
		break;
	}

	LOG_PAL_DEBUG("PalRfTxRequest. %hu bytes. Time %u\r\n", x_tx_params.us_psdu_len, x_tx_params.ul_tx_time);

	/* Send TX request to RF PHY */
	at86rf_tx_req(AT86RF_TRX_RF09_ID, &x_tx_params);

	return PAL_TX_RESULT_PROCESS;
}


/**
 * \brief Send message using PHY Serial layer
 *
 * \param uc_buff_id   Buffer to be flushed
 *
 */
void pal_rf_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode)
{
	at86rf_set_chn_req_params_t x_params;

	x_params.ul_time = ul_time_sync;
	x_params.uc_time_mode = (at86rf_tx_time_mode_t) uc_time_mode;
	x_params.us_chn_num = us_pch & (~RF_CHANNEL_BIT);

#ifdef RF_FREQUENCY_HOPPING_ENABLED
	sus_next_rx_fh_channel = x_params.us_chn_num;
#endif

	at86rf_set_chn_req(AT86RF_TRX_RF09_ID, &x_params);
}

/**
 * \brief Get timer
 *
 * \param pul_timer      Pointer to timer value
 * \param us_pch         Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_timer_get(uint32_t *pul_timer, uint16_t us_pch)
{
	UNUSED(us_pch);
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
uint8_t pal_rf_timer_get_extended(uint64_t *pull_timer)
{
	uint32_t ul_time;
	uint64_t ull_time;

	pal_rf_timer_get(&ul_time, RF_CHANNEL_BIT);
	ull_time = ((uint64_t)sul_hi_timer_ref << 32) | ul_time;

	*pull_timer = ull_time;
	return PAL_CFG_SUCCESS;
}

/**
 * \brief Get zero-cross time
 *
 * \param pul_zct      Pointer to zero-cross time (in 10s of microsec)
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_zct_get(uint32_t *pul_zct, uint16_t us_pch)
{
	(void)us_pch;

	*pul_zct = 0;
	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Get Noise Level (NL)
 *
 * \param puc_noise    Pointer to Noise Level
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_nl_get(uint8_t *puc_noise, uint16_t us_pch)
{
	UNUSED(us_pch);

	*puc_noise = 0;

	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Make clear channel assessment mode 1
 *
* \param *puc_channel_state    Channel state (0: busy, 1: free)
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_cca_get(uint8_t *puc_channel_state)
{
	at86rf_cca_res_t uc_cca_result;

	/* Perform CCA Mode 3 (carrier sense + energy detection) */
	uc_cca_result = at86rf_cca_req(AT86RF_TRX_RF09_ID, AT86RF_CCA_MODE_3);
	if (uc_cca_result == AT86RF_CCA_FREE) {
		/* Free channel */
		*puc_channel_state = 1;
	} else {
		/* Busy channel */
		*puc_channel_state = 0;
	}

	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Get the channel/band
 *
 * \param us_pch       Pointer to the Physical channel
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_ch_get(uint16_t *pus_pch)
{
	*pus_pch = sus_current_channel | RF_CHANNEL_BIT;
	return(PAL_CFG_SUCCESS);
}

/**
 * \brief Set the channel/band
 *
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_ch_set(uint16_t us_pch)
{
	uint16_t us_chn;
	us_chn = us_pch & (~RF_CHANNEL_BIT);

	if (RF_FREQ_HOPPING_CHANNEL == us_chn) {
#ifdef RF_FREQUENCY_HOPPING_ENABLED
		sus_current_channel = RF_FREQ_HOPPING_CHANNEL;
		return(PAL_CFG_SUCCESS);
#else
		return(PAL_CFG_INVALID_INPUT);
#endif
	} else {
		/* Set in phy layer */
    	if (at86rf_pib_set(AT86RF_TRX_RF09_ID, AT86RF_PIB_PHY_CHANNEL_NUM, &us_chn) == AT86RF_SUCCESS) {
			/* Store valid channel */
			sus_current_channel = us_chn;
			return(PAL_CFG_SUCCESS);
		}
	}

    return(PAL_CFG_INVALID_INPUT);
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
uint8_t pal_rf_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	at86rf_pib_attr_t us_rf_id;
	UNUSED(us_len);
	UNUSED(us_pch);

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_INFO_VERSION:
		us_rf_id = AT86RF_PIB_FW_VERSION;
		break;

	case PAL_ID_PHY_SNIFFER_EN:
		*(uint8_t *)p_val = suc_rf_sniffer_enabled;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_TXRX_CHANNEL:
		*(uint16_t *)p_val = sus_current_channel;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_RX_PAYLOAD_LEN_SYM:
		us_rf_id = AT86RF_PIB_PHY_RX_PAY_SYMBOLS;
		break;

	case PAL_ID_CSMA_RF_SENSE_TIME:
		us_rf_id = AT86RF_PIB_PHY_CCA_ED_DURATION_US;
		break;

	case PAL_ID_UNIT_BACKOFF_PERIOD:
		us_rf_id = AT86RF_PIB_MAC_UNIT_BACKOFF_PERIOD;
		break;

	case PAL_ID_INFO_DEVICE:
		us_rf_id = AT86RF_PIB_DEVICE_ID;
		break;

	case PAL_ID_RF_DEFAULT_SCHEME:
		if ((RF_RM_MODE == RF_RM_AUTOMATIC) || (RF_RM_MODE == RF_RM_FORCED_ON)) {
			*(uint8_t *)p_val = PAL_RF_FSK_FEC_ON;
		} else {
			*(uint8_t *)p_val = PAL_RF_FSK_FEC_OFF;
		}

		return(PAL_CFG_SUCCESS);

	case PAL_ID_RF_NUM_CHANNELS:
		*(uint16_t *)p_val = sus_num_rf_channels;
		return(PAL_CFG_SUCCESS);

#ifdef RF_FREQUENCY_HOPPING_ENABLED
	case PAL_ID_RF_BITS_HOPPING_SEQUENCE:
		memcpy(p_val, &suc_bits_sequence, us_len);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_RF_BITS_BCN_HOPPING_SEQUENCE:
		memcpy(p_val, &suc_bits_beacon_sequence, us_len);
		return(PAL_CFG_SUCCESS);

	case PAL_ID_RF_MAC_HOPPING_SEQUENCE_LENGTH:
		*(uint16_t *)p_val = sus_hopping_sequence_length;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_RF_MAC_HOPPING_BCN_SEQUENCE_LENGTH:
		*(uint8_t *)p_val = suc_hopping_bcn_sequence_length;
		return(PAL_CFG_SUCCESS);
#endif

	case PAL_ID_MAX_PHY_PACKET_SIZE:
		*(uint16_t *)p_val = AT86RF215_MAX_PSDU_LEN;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_TURNAROUND_TIME:
		us_rf_id = AT86RF_PIB_PHY_TURNAROUND_TIME;
		break;

	case PAL_ID_PHY_TX_POWER:
		*(int16_t *)p_val = 14;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_FSK_FEC_ENABLED:
		if (suc_rf_phy_mode == AT86RF_PHY_MOD_FSK) {
			*(uint8_t *)p_val = pal_rf_rm_get_scheme();
			return(PAL_CFG_SUCCESS);
		} else {
			return(PAL_CFG_INVALID_INPUT);
		}

	case PAL_ID_PHY_FSK_FEC_INTERLEAVING_RSC:
		*(uint8_t *)p_val = false;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_FSK_FEC_SCHEME:
		*(uint16_t *)p_val = 0;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_FSK_PREAMBLE_LENGTH:
		*(uint16_t *)p_val = 8;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_SUN_FSK_SFD:
		*(uint16_t *)p_val = 0;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_FSK_SCRAMBLE_PSDU:
		*(uint8_t *)p_val = true;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_PHY_CCA_DURATION:
		us_rf_id = AT86RF_PIB_PHY_CCA_ED_DURATION_SYMBOLS;
		break;

	case PAL_ID_PHY_CCA_THRESHOLD:
		us_rf_id = AT86RF_PIB_PHY_CCA_ED_THRESHOLD_SENSITIVITY;
		break;

	case PAL_ID_RF_PHY_BAND_OPERATING_MODE:
		us_rf_id = AT86RF_PIB_PHY_BAND_OPERATING_MODE;
		break;

	default:
		return(PAL_CFG_INVALID_INPUT);
	}

	/* Get from phy layer */
	if (at86rf_pib_get(AT86RF_TRX_RF09_ID, us_rf_id, p_val) == AT86RF_SUCCESS) {
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
uint8_t pal_rf_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	at86rf_pib_attr_t us_rf_id;
	UNUSED(us_len);
	UNUSED(us_pch);

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_INFO_VERSION:
		us_rf_id = AT86RF_PIB_FW_VERSION;
		break;

	case PAL_ID_PHY_SNIFFER_EN:
#ifdef AT86RF_ADDONS_ENABLE
		suc_rf_sniffer_enabled = *(uint8_t *)p_val;
#endif
		return(PAL_CFG_SUCCESS);

	case PAL_ID_CFG_TXRX_CHANNEL:
		if (RF_FREQ_HOPPING_CHANNEL == *(uint16_t *)p_val) {
#ifdef RF_FREQUENCY_HOPPING_ENABLED
			sus_current_channel = RF_FREQ_HOPPING_CHANNEL;
			return(PAL_CFG_SUCCESS);
#else
			return(PAL_CFG_INVALID_INPUT);
#endif
		} else {
			us_rf_id = AT86RF_PIB_PHY_CHANNEL_NUM;
		}

		break;

	case PAL_ID_RX_PAYLOAD_LEN_SYM:
		us_rf_id = AT86RF_PIB_PHY_RX_PAY_SYMBOLS;
		break;

	case PAL_ID_PHY_FSK_FEC_ENABLED:
		if (suc_rf_phy_mode == AT86RF_PHY_MOD_FSK) {
			pal_rf_rm_set_scheme(*(uint8_t *)p_val);
			return(PAL_CFG_SUCCESS);
		} else {
			return(PAL_CFG_INVALID_INPUT);
		}

	case PAL_ID_PHY_CCA_DURATION:
		us_rf_id = AT86RF_PIB_PHY_CCA_ED_DURATION_SYMBOLS;
		break;

	case PAL_ID_PHY_CCA_THRESHOLD:
		us_rf_id = AT86RF_PIB_PHY_CCA_ED_THRESHOLD_SENSITIVITY;
		break;

	case PAL_ID_RF_PHY_BAND_OPERATING_MODE:
		us_rf_id = AT86RF_PIB_PHY_BAND_OPERATING_MODE;
		break;

	case PAL_ID_INFO_DEVICE:
	case PAL_ID_CSMA_RF_SENSE_TIME:
	case PAL_ID_UNIT_BACKOFF_PERIOD:
	case PAL_ID_RF_DEFAULT_SCHEME:
	case PAL_ID_RF_NUM_CHANNELS:
	case PAL_ID_RF_BITS_HOPPING_SEQUENCE:
	case PAL_ID_RF_BITS_BCN_HOPPING_SEQUENCE:
	case PAL_ID_RF_MAC_HOPPING_SEQUENCE_LENGTH:
	case PAL_ID_RF_MAC_HOPPING_BCN_SEQUENCE_LENGTH:
	case PAL_ID_MAX_PHY_PACKET_SIZE:
	case PAL_ID_TURNAROUND_TIME:
	case PAL_ID_PHY_TX_POWER:
	case PAL_ID_PHY_FSK_FEC_INTERLEAVING_RSC:
	case PAL_ID_PHY_FSK_FEC_SCHEME:
	case PAL_ID_PHY_FSK_PREAMBLE_LENGTH:
	case PAL_ID_PHY_SUN_FSK_SFD:
	case PAL_ID_PHY_FSK_SCRAMBLE_PSDU:
		/* Read only */
		return(PAL_CFG_INVALID_INPUT);

	default:
		return(PAL_CFG_INVALID_INPUT);
	}

	/* Set in phy layer */
	if (at86rf_pib_set(AT86RF_TRX_RF09_ID, us_rf_id, p_val) == AT86RF_SUCCESS) {
		/* Store valid channel */
		if (us_rf_id == AT86RF_PIB_PHY_CHANNEL_NUM) {
			sus_current_channel = *(uint16_t *)p_val;
		} else if (us_rf_id == AT86RF_PIB_PHY_BAND_OPERATING_MODE) {
			/* Get PHY configuration and update variables accordingly */
			_update_phy_configuration();
		}

		return(PAL_CFG_SUCCESS);
	} else {
		return(PAL_CFG_INVALID_INPUT);
	}
}

/**
 * \brief Get RF message duration
 *
 * \param us_pch       Physical channel used
 * \param ul_msg_len   Message length
 * \param uc_scheme    Modulation scheme of message
 * \param uc_mode      Indicates if the message to transmit is type A, type B or type BC
 * \param pul_duration Pointer to message duration in microseconds (output)
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration)
{
	at86rf_mod_frame_params_t x_rf_mod_params;
	at86rf_res_t uc_res;

	(void)us_pch;
	(void)uc_mode;

	/* Get PHY configuration to set FEC in FSK or MCS in OFDM */
	if (suc_rf_phy_mode == AT86RF_PHY_MOD_FSK) {
		x_rf_mod_params.x_fsk.uc_fec_enabled = (at86rf_fsk_fec_t)uc_scheme;
	} else {
		x_rf_mod_params = sx_rf_mod_params;
	}

	uc_res = at86rf_get_msg_duration(AT86RF_TRX_RF09_ID, &x_rf_mod_params, us_msg_len, pul_duration);

	if (uc_res == AT86RF_SUCCESS) {
		return(PAL_CFG_SUCCESS);
	} else {
		return(PAL_CFG_INVALID_INPUT);
	}
}

/**
 * \brief Convert QT value to Signal Noise Ratio (SNR)
 *
 * \param puc_snr      SNR parameter
 * \param uc_qt        QT parameter to get SNR level
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_rf_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch)
{
	UNUSED(puc_snr);
	UNUSED(uc_qt);
	UNUSED(us_pch);

	return(PAL_CFG_SUCCESS);
}

#endif /* PAL_RF */

/* @} */




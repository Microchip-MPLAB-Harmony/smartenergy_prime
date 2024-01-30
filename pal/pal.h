/**
 * \file
 *
 * \brief PAL: Physical Abstraction Layer
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

#ifndef PAL_H_INCLUDED
#define PAL_H_INCLUDED

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

#include <stdint.h>
#include <stdbool.h>
#include "conf_pal.h"

/* Inclusion of the module MPAL to handle two or more physical layers */
#if (defined(PAL_PLC) && defined(PAL_RF)) || (defined(PAL_PLC) && defined(PAL_SERIAL))
#define PAL_MULTIPHY
#endif

/**
 * \ingroup prime_ng_group
 * \defgroup prime_pal_group Physical Abstraction Layer
 *
 * This module provides configuration and utils for the PAL layer,
 * which is the interface between the PLC PHY layer and the PRIME
 * MAC layer.
 *
 * @{
 */

/** \brief Physical PRIME mode of transmission */
/* @{ */
enum pal_prime_modes_ids {
	PAL_MODE_TYPE_A  = 0,
	PAL_MODE_TYPE_B  = 2,
	PAL_MODE_TYPE_BC = 3,
	PAL_MODE_TYPE_RF = 5,
	PAL_MODE_NOISE   = 0xFE,  /* Not in PL360 */
	PAL_MODE_TEST    = 0xFF,  /* Not in PL360 */
};
/* @} */

/** \brief Physical Layer Identifiers */
/* @{ */
typedef enum {
	PAL_PHY_PLC = 0,
	PAL_PHY_SER = 1,
	PAL_PHY_RF  = 2,
	PAL_PHY_UNKNOWN = 0xFF,
} phy_layer_t;
/* @} */

/** \brief PLC Physical Scheme of modulation */
/* @{ */
enum pal_scheme_ids {
	PAL_PLC_DBPSK        = 0x00,
	PAL_PLC_DQPSK        = 0x01,
	PAL_PLC_D8PSK        = 0x02,
	PAL_PLC_DBPSK_C      = 0x04,
	PAL_PLC_DQPSK_C      = 0x05,
	PAL_PLC_D8PSK_C      = 0x06,
	PAL_PLC_R_DBPSK      = 0x0C,
	PAL_PLC_R_DQPSK      = 0x0D,
};
/* @} */

/** \brief PLC Physical Scheme of modulation */
/* @{ */
enum pal_rf_scheme_ids {
	PAL_RF_FSK_FEC_OFF = 0,
	PAL_RF_FSK_FEC_ON = 1,
};
/* @} */

#define PAL_OUTDATED_INF    0x0F

/** \brief PLC Timer modes */
/* @{ */
/** Absolute TX scheduling mode (absolute TX time specified) */
#define PAL_TX_MODE_ABSOLUTE         0
/** Relative TX scheduling mode (delay for TX time specified) */
#define PAL_TX_MODE_RELATIVE         1
/** Cancel TX scheduling mode (cancel TX) */
#define PAL_TX_MODE_CANCEL           2
/* @} */

/** QT null value */
#define PAL_QT_UNKNOW                0xFF

/** \brief PAL config success result */
/* @{ */
#define PAL_CFG_SUCCESS              0
#define PAL_CFG_INVALID_INPUT        1
/* @} */

/** \brief TX Result values */
/* @{ */
/** Transmission result: already in process */
#define PAL_TX_RESULT_PROCESS        0
/** Transmission result: end successfully */
#define PAL_TX_RESULT_SUCCESS        1
/** Transmission result: invalid length error */
#define PAL_TX_RESULT_INV_LENGTH     2
/** Transmission result: busy channel error */
#define PAL_TX_RESULT_BUSY_CH        3
/** Transmission result: busy transmission error */
#define PAL_TX_RESULT_BUSY_TX        4
/** Transmission result: busy reception error */
#define PAL_TX_RESULT_BUSY_RX        5
/** Transmission result: invalid scheme error */
#define PAL_TX_RESULT_INV_SCHEME     6
/** Transmission result: timeout error */
#define PAL_TX_RESULT_TIMEOUT        7
/** Transmission result: invalid buffer identifier error */
#define PAL_TX_RESULT_INV_BUFFER     8
/** Transmission result: invalid Prime Mode error */
#define PAL_TX_RESULT_INV_PRIME_MODE 9
/** Transmission result: invalid transmission mode error */
#define PAL_TX_RESULT_INV_TX_MODE    10
/** Transmission result: transmission cancelled */
#define PAL_TX_RESULT_INV_CANCELLED  11
/** Transmission result: high temperature (>120�C) error (only with PL460/PL480) */
#define PAL_TX_RESULT_HIGH_TEMP_120  12
/** Transmission result: high temperature (>110�C) error (only with PL460/PL480) */
#define PAL_TX_RESULT_HIGH_TEMP_110  13
/** Transmission Result (only RF): invalid Parameter */
#define PAL_TX_RESULT_INV_PARAM      20
/** Transmission result: error in tx */
#define PAL_TX_RESULT_PHY_ERROR      0xFE
/** Transmission result: not use physical layer */
#define PAL_TX_RESULT_PHY_UNUSED     0xFF
/* @} */

/** Parameters requested from MAC layer */
enum pal_ids {
	/** Enable/Disable continuous transmission mode */
	PAL_ID_CONTINUOUS_TX_EN,
	/** Zero Crossing Period */
	PAL_ID_ZC_PERIOD,
	/** Host Controller version identifier */
	PAL_ID_HOST_VERSION,
	/** Maximum Transmission/Reception Number of Channels */
	PAL_ID_CFG_MAX_TXRX_NUM_CHANNELS,
	/** Attenuation to be applied to every message */
	PAL_ID_CFG_ATTENUATION,
	/** List of available transmission/reception channels, Double combination */
	PAL_ID_CFG_TXRX_DOUBLE_CHANNEL_LIST,
	/** Product identifier */
	PAL_ID_INFO_VERSION,
	/** Enable/disable PHY Sniffer */
	PAL_ID_PHY_SNIFFER_EN,
	/** Flag to enable branch auto detection */
	PAL_ID_CFG_AUTODETECT_BRANCH,
	/** When branch auto detection disabled, indicate impedance to use */
	PAL_ID_CFG_IMPEDANCE,
	/** Transmission/Reception Channel */
	PAL_ID_CFG_TXRX_CHANNEL,
	/** List of available transmission/reception channels (depends on coupling and band plan) */
	PAL_ID_CFG_TXRX_CHANNEL_LIST,
	/** RX Payload length in OFDM symbols */
	PAL_ID_RX_PAYLOAD_LEN_SYM,
	/** Duration of channel senses in CSMA RF */
	PAL_ID_CSMA_RF_SENSE_TIME,
	/** Duration of a unit backoff period in CSMA RF */
	PAL_ID_UNIT_BACKOFF_PERIOD,
	/** Network detection */
	PAL_ID_NETWORK_DETECTION,
	/** Device information */
	PAL_ID_INFO_DEVICE,
	/** Remaining duration of present frame */
	PAL_ID_REMAINING_FRAME_DURATION,
	/** Default scheme for RF */
	PAL_ID_RF_DEFAULT_SCHEME,
	/** Physical parameters for a received plc message */
	PAL_ID_PLC_RX_PHY_PARAMS,
	/** RF frequency hopping parameters */
	PAL_ID_RF_BITS_HOPPING_SEQUENCE,
	PAL_ID_RF_BITS_BCN_HOPPING_SEQUENCE,
	PAL_ID_RF_MAC_HOPPING_SEQUENCE_LENGTH,
	PAL_ID_RF_MAC_HOPPING_BCN_SEQUENCE_LENGTH,
	/** Number of supported RF channels */
	PAL_ID_RF_NUM_CHANNELS,
	/** RF PHY PIBs */
	PAL_ID_MAX_PHY_PACKET_SIZE,
	PAL_ID_TURNAROUND_TIME,
	PAL_ID_PHY_TX_POWER,
	PAL_ID_PHY_FSK_FEC_ENABLED,
	PAL_ID_PHY_FSK_FEC_INTERLEAVING_RSC,
	PAL_ID_PHY_FSK_FEC_SCHEME,
	PAL_ID_PHY_FSK_PREAMBLE_LENGTH,
	PAL_ID_PHY_SUN_FSK_SFD,
	PAL_ID_PHY_FSK_SCRAMBLE_PSDU,
	PAL_ID_PHY_CCA_DURATION,
	PAL_ID_PHY_CCA_THRESHOLD,
	/* RF Band and Operating Mode */
	PAL_ID_RF_PHY_BAND_OPERATING_MODE,
};

/** Data struct used for transmission */
typedef struct {
	/** Pointer to data buffer */
	uint8_t *data_buf;
	/** Delay for transmission in us */
	uint32_t time_delay;
	/** Length of the data buffer. */
	uint16_t data_len;
	/** Physical channel to transmit the message */
	uint16_t us_pch;
	/** Buffer identifier */
	uint8_t buff_id;
	/** Attenuation level with which the message must be transmitted */
	uint8_t att_level;
	/** Modulation scheme of last transmitted message */
	uint8_t scheme;
	/** TX Forced */
	uint8_t disable_rx;
	/** Type A, Type B, Type BC, Type Radio */
	uint8_t mode;
	/** Time mode: 0: Absolute mode, 1: Differential mode, 2: Cancel TX */
	uint8_t time_mode;
	/** Number of channel senses */
	uint8_t num_senses;
	/** Delay between channel senses in ms */
	uint8_t sense_delay_ms;
} x_pal_msg_tx_t;

/** Data struct used for confirm transmission */
typedef struct {
	/** Transmission time in us */
	uint32_t tx_time;
	/** RMS value emitted (valid only when txQRMode is enable) */
	uint16_t rms_calc;
	/** Physical channel where the message has been transmitted */
	uint16_t us_pch;
	/** Type mode: Type A, Type B, Type BC, Type Radio  */
	uint8_t mode;
	/** Number of the buffer used to tx */
	uint8_t buff_id;
	/** Result */
	uint8_t result;
} x_pal_data_cfm_t;

/** Data struct used for reception */
typedef struct {
	/** Pointer to local data buffer */
	uint8_t *data_buf;
	/** Reception time in us */
	uint32_t rx_time;
	/** Length of the data buffer */
	uint16_t data_len;
	/** Physical channel where the message has been received */
	uint16_t us_pch;
	/** Bitrate estimation in Kbs */
	uint16_t us_estimated_bitrate;
	/** Rssi coded as specified */
	int16_t ss_rssi;
	/** Buffer identifier */
	uint8_t buff_id;
	/** Modulation scheme of the last received message */
	uint8_t scheme;
	/** Type A, Type B, Type BC, Type Radio */
	uint8_t mode;
	/** Header Type of the last received message */
	uint8_t header_type;
	/** Less robust modulation */
	uint8_t uc_less_robust_mod;
	/** SNR/LQI */
	uint8_t uc_lqi;
} x_pal_data_ind_t;


typedef struct {
	uint16_t us_new_channel;
} x_pal_switch_rf_chn_t;

typedef struct {
	/** Accumulated Error Vector Magnitude for header */
	uint32_t ul_evm_header_acum;
	/** Accumulated Error Vector Magnitude for payload */
	uint32_t ul_evm_payload_acum;
	/** Reception time (start of message) referred to 1us PHY counter */
	uint32_t ul_rx_time;
	/** Error Vector Magnitude for header */
	uint16_t us_evm_header;
	/** Error Vector Magnitude for payload */
	uint16_t us_evm_payload;
	/** Length of the data buffer in bytes */
	uint16_t us_data_len;
	/** Modulation scheme of the received message */
	uint8_t uc_scheme;
	/** Type A, Type B or Type BC  */
	uint8_t uc_mod_type;
	/** Header Type of the received message */
	uint8_t uc_header_type;
	/** Average RSSI (Received Signal Strength Indication) */
	uint8_t uc_rssi_avg;
	/** Average CNIR (Carrier to Interference + Noise ratio) */
	uint8_t uc_cinr_avg;
	/** Minimum CNIR (Carrier to Interference + Noise ratio) */
	uint8_t uc_cinr_min;
	/** Average Soft BER (Bit Error Rate) */
	uint8_t uc_ber_soft;
	/** Maximum Soft BER (Bit Error Rate) */
	uint8_t uc_ber_soft_max;
	/** Percentage of carriers affected by narrow band noise */
	uint8_t uc_nar_bnd_percent;
	/** Percentage of symbols affected by impulsive noise */
	uint8_t uc_imp_percent;
} x_pal_plc_rx_phy_params_t;


/** PAL confirm transmission callback */
typedef void (*pal_data_confirm_cb_t)(x_pal_data_cfm_t *px_data_cfm);

/** PAL reception callback */
typedef void (*pal_data_indication_cb_t)(x_pal_data_ind_t *px_data_ind);

/** PAL switch RF channel callback */
typedef void (*pal_switch_rf_ch_cb_t)(x_pal_switch_rf_chn_t *px_switch_rf_chn);

/** Data struct used for PAL callbacks */
typedef struct TPalCallbacks {
	pal_data_confirm_cb_t data_confirm;
	pal_data_indication_cb_t data_indication;
	pal_switch_rf_ch_cb_t switch_rf_ch;
} pal_callbacks_t;

/** \brief PAL interface */
/* @{ */
void pal_init(void);
void pal_process(void);
void pal_set_callbacks(pal_callbacks_t *pal_cbs);

uint8_t pal_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch);   /* Not available in Serial PHY */
uint8_t pal_zct_get(uint32_t *pul_zct, uint16_t us_pch);   /* Only available in PLC */
uint8_t pal_timer_get(uint32_t *pul_timer, uint16_t us_pch);
uint8_t pal_timer_get_extended(uint64_t *pull_timer);
uint8_t pal_cd_get(uint8_t *puc_cd, uint8_t *puc_rssi, uint32_t *pul_time, uint8_t *puc_header, uint16_t us_pch);   /* Only available in PLC */
uint8_t pal_nl_get(uint8_t *puc_noise, uint16_t us_pch);   /* Not available in Serial PHY */
uint8_t pal_agc_get(uint8_t *puc_mode, uint8_t *puc_gain, uint16_t us_pch);   /* Only available in PLC */
uint8_t pal_agc_set(uint8_t uc_mode, uint8_t uc_gain, uint16_t us_pch);    /* Only available in PLC */
uint8_t pal_cca_get(uint8_t *puc_channel_state);    /* Only available in RF */
uint8_t pal_ch_get(uint16_t *pus_pch);    /* Not available in Serial PHY */
uint8_t pal_ch_set(uint16_t us_pch);      /* Not available in Serial PHY */

uint8_t pal_data_request(x_pal_msg_tx_t *px_msg);

void pal_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode);   /* Only available in RF */

uint8_t pal_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);
uint8_t pal_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch);   /* Not available in Serial PHY */

uint16_t pal_get_signal_capture(uint8_t *puc_dst, uint8_t uc_mode, uint32_t ul_time_start, uint32_t ul_duration); /* Only available in PLC */

uint8_t pal_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration); /* Not available in Serial PHY */

bool pal_rm_minimum_quality(uint16_t us_pch, uint8_t uc_scheme, uint8_t uc_less_robust_mod);  /* Not available in Serial PHY */
uint8_t pal_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2);  /* Not available in Serial PHY */

/* @} */

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
#endif /* PAL_H_INCLUDED */

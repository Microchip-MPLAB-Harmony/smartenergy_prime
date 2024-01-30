/**
 *
 * \file
 *
 * \brief PRIME RF Phy Abstraction Layer for RF215 Robust Management Module
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
#ifdef PAL_MULTIPHY
#include "mpal.h"
#endif

#ifndef PAL_MULTIPHY
#define pal_rf_rm_minimum_quality pal_rm_minimum_quality
#define pal_rf_rm_get_less_robust_mod pal_rm_get_less_robust_mod
#endif


#ifdef PAL_RF

/**
 * \weakgroup prime_pal_group
 * @{
 */

/*
 * Formula to obtain the RSSI threshold
 *
 * RSSI = ( ln((1 - FER) / FER) - b * N^2 - c * N - d ) / a
 *
 * FER = 0,005 (frame error rate)
 * N = 255 (number of bytes of the PHY frame)
 * a = 3,93 * 10^-1 = 0,393
 * b = 2,13 * 10^-5 = 0,0000213
 * c = -1,24 * 10^-2 = -0,0124
 * d = 3,95 * 10^1 = 39,5
 *
 * The operation gives -82,51
 * But we configure -89 (FER = 2% in 20-byte messages, FER = 10% in 255-byte messages)
 */

/* Bandwidth of every modulation */
static const uint8_t suc_bandwidth[16] = {
		50,  /* PAL_RF_FSK_FEC_OFF */
		25,  /* PAL_RF_FSK_FEC_ON */
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0    /* PAL_OUTDATED_INF */
};


static int16_t ssc_rssi_threshold[2];
static uint8_t suc_rm_mode;

/*
 * \brief function to get valid modulations
 *
 * \param sc_rssi                        Received RSSI
 * \param us_estimated_bitrate           Bitrate estimation in Kbs
 * \param uc_less_robust_mod             Less robust modulation
 *
 * \return Less robust modulation and estimated bitrate for a valid communication
 */
void pal_rf_rm_max_modulation_rx_msg(int8_t sc_rssi, uint16_t *us_estimated_bitrate, uint8_t *uc_less_robust_mod)
{
	uint8_t uc_best_mod = PAL_OUTDATED_INF;

	switch(suc_rm_mode) {
	case RF_RM_FORCED_OFF:
		if (sc_rssi >= ssc_rssi_threshold[PAL_RF_FSK_FEC_OFF]) {
			uc_best_mod = PAL_RF_FSK_FEC_OFF;
		}

		break;

	case RF_RM_FORCED_ON:
		if (sc_rssi >= ssc_rssi_threshold[PAL_RF_FSK_FEC_ON]) {
			uc_best_mod = PAL_RF_FSK_FEC_ON;
		}

		break;

	case RF_RM_AUTOMATIC:
	default:
		if (sc_rssi >= ssc_rssi_threshold[PAL_RF_FSK_FEC_OFF]) {
			uc_best_mod = PAL_RF_FSK_FEC_OFF;
		} else if (sc_rssi >= ssc_rssi_threshold[PAL_RF_FSK_FEC_ON]) {
			uc_best_mod = PAL_RF_FSK_FEC_ON;
		}
	}

	*uc_less_robust_mod = uc_best_mod;

	if (uc_best_mod != PAL_OUTDATED_INF) {
		*us_estimated_bitrate = suc_bandwidth[uc_best_mod];
	} else {
		*us_estimated_bitrate = 0;
	}
}

/*
 * \brief function to calculate the LQI
 *
 * \param sc_rssi     Received RSSI
 *
 * \return Link quality indication
 */
uint8_t pal_rf_rm_get_lqi(int8_t sc_rssi)
{
	uint8_t uc_lqi;

	if (sc_rssi > 80) {
		uc_lqi = 0xFE;
	} else {
		uc_lqi = sc_rssi + 174;
	}

	return uc_lqi;
}


/**
 * \brief Check if the modulation is good enough for a low FER in the given scheme
 *
 * \param us_pch              Channel of reception of the message
 * \param uc_scheme           Modulation to compare
 * \param uc_less_robust_mod  Less robust modulation
 *
 * \return true or false
 */
bool pal_rf_rm_minimum_quality(uint16_t us_pch, uint8_t uc_scheme, uint8_t uc_less_robust_mod)
{
	(void)us_pch;
	(void)uc_scheme;

	if ((uc_less_robust_mod == PAL_OUTDATED_INF) ||
		((uc_scheme == PAL_RF_FSK_FEC_OFF) && (uc_less_robust_mod == PAL_RF_FSK_FEC_ON))) {
		return false;
	} else {
		return true;
	}
}


/**
 * \brief return the less robust modulation for a channel from to given
 *
 * \param us_pch            Channel of the modulation
 * \param uc_mod1           Modulation 1 to compare
 * \param uc_mod2           Modulation 2 to compare
 *
 * \return uc_mod1 or uc_mod2 scheme for the given channel
 */
uint8_t pal_rf_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2)
{
	(void) us_pch;

	if (suc_bandwidth[uc_mod1] > suc_bandwidth[uc_mod2]) {
		return uc_mod1;
	} else {
		return uc_mod2;
	}
}

/**
 * \brief Get the configured scheme
 *
 * \return Configured scheme
 */
uint8_t pal_rf_rm_get_scheme(void)
{
	switch(suc_rm_mode) {
	case RF_RM_FORCED_OFF:
		return PAL_RF_FSK_FEC_OFF;

	case RF_RM_FORCED_ON:
		return PAL_RF_FSK_FEC_ON;

	case RF_RM_AUTOMATIC:
	default:
		return PAL_RF_FSK_FEC_OFF;
	}
}

/**
 * \brief Set the new scheme
 *
 * \param uc_scheme           Modulation to compare
 */
void pal_rf_rm_set_scheme(uint8_t uc_scheme)
{
	if (uc_scheme == PAL_RF_FSK_FEC_OFF) {
		suc_rm_mode = RF_RM_FORCED_OFF;
	} else {
		suc_rm_mode = RF_RM_FORCED_ON;
	}
}

/**
 * \brief Initialization function of the PAL RM module in PLC
 *
 */
void pal_rf_rm_init(void)
{
	suc_rm_mode = RF_RM_MODE;
	ssc_rssi_threshold[PAL_RF_FSK_FEC_OFF] = RF_THRESHOLD_FSK_FEC_OFF;
	ssc_rssi_threshold[PAL_RF_FSK_FEC_ON] = RF_THRESHOLD_FSK_FEC_ON;
}

#endif /* PAL_RF */

/* @} */




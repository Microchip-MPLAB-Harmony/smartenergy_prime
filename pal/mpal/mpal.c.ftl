/**
 *
 * \file
 *
 * \brief MPAL: Multiple Physical Abstraction Layer
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
#include "pal.h"

#ifdef PAL_MULTIPHY
#include "mpal.h"

/* PLC channels are defined from 1..511 (0x01..0x1FF) */
/* RF channels are defined from 512..1023 (0x200..0x3FF) */
/* Serial channel is defined as 1024 (0x400) */
#define IS_A_PLC_CHANNEL(x)     (x < 512)
#define IS_A_RADIO_CHANNEL(x)   ((x > 511) && (x < 1024))
#define IS_A_SERIAL_CHANNEL(x)  (x == 1024)

static pal_callbacks_t sx_mpal_app_callbacks;

/**
 *
 */
static void _mpal_data_confirm(x_pal_data_cfm_t *px_msg_cfm)
{
	if (sx_mpal_app_callbacks.data_confirm) {
		sx_mpal_app_callbacks.data_confirm(px_msg_cfm);
	}
}

/**
 *
 */
static void _mpal_data_indication(x_pal_data_ind_t *px_msg_ind)
{
	if (sx_mpal_app_callbacks.data_indication) {
		sx_mpal_app_callbacks.data_indication(px_msg_ind);
	}
}

#ifdef PAL_RF
static void _mpal_switch_rf_ch(x_pal_switch_rf_chn_t *px_switch_rf_chn)
{
	if (sx_mpal_app_callbacks.switch_rf_ch) {
		sx_mpal_app_callbacks.switch_rf_ch(px_switch_rf_chn);
	}
}
#endif
/**
 * \brief This function initializes PRIME PAL layer
 *
 * \note This function will be invoked by function pal_init() and it allows the user
 * to configure PAL layer initialization. Users can adapt the PAL layer to every
 * hardware platform.
 * This is an example of usage.
 * ATMEL strongly recommends to consult with technical support to change any
 * parameter in this file.
 *
 */
void pal_init(void)
{
#ifdef PAL_PLC
	pal_plc_init();
#endif
#ifdef PAL_RF
	pal_rf_init();
#endif
#ifdef PAL_SERIAL
	pal_serial_init();
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
void pal_process(void)
{
#ifdef PAL_PLC
	pal_plc_process();
#endif
#ifdef PAL_RF
	pal_rf_process();
#endif
#ifdef PAL_SERIAL
	pal_serial_process();
#endif
}

/**
 * \brief This function links callback functions between upper layer and phy layer.
 *
 * \param pal_cbs      Pointer to callbacks structure
 *
 */
void pal_set_callbacks(pal_callbacks_t *pal_cbs)
{
	pal_callbacks_t x_pal_app_callbacks;
	memcpy(&sx_mpal_app_callbacks, pal_cbs, sizeof(pal_callbacks_t));

	x_pal_app_callbacks.data_confirm = _mpal_data_confirm;
	x_pal_app_callbacks.data_indication = _mpal_data_indication;

#ifdef PAL_PLC
	x_pal_app_callbacks.switch_rf_ch = NULL;
	pal_plc_set_callbacks(&x_pal_app_callbacks);
#endif
#ifdef PAL_RF
	x_pal_app_callbacks.switch_rf_ch = _mpal_switch_rf_ch;
	pal_rf_set_callbacks(&x_pal_app_callbacks);
#endif
#ifdef PAL_SERIAL
	x_pal_app_callbacks.switch_rf_ch = NULL;
	pal_serial_set_callbacks(&x_pal_app_callbacks);
#endif
}

/**
 * \brief Send message using PAL layer
 *
 * \param px_msg      Pointer to transmission data struct
 *
 */
uint8_t pal_data_request(x_pal_msg_tx_t *px_msg)
{
	if (IS_A_PLC_CHANNEL(px_msg->us_pch)) {
#ifdef PAL_PLC
		return pal_plc_data_request(px_msg);
#endif
	} else if (IS_A_RADIO_CHANNEL(px_msg->us_pch)) {
#ifdef PAL_RF
		return pal_rf_data_request(px_msg);
#endif
	} else if (IS_A_SERIAL_CHANNEL(px_msg->us_pch)) {
#ifdef PAL_SERIAL
		return pal_serial_data_request(px_msg);
#endif
	}

	return PAL_TX_RESULT_PHY_ERROR;
}

/**
 * \brief Get timer
 *
 * \param pul_timer      Pointer to timer value
 * \param us_pch         Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
void pal_set_chn_req(uint32_t ul_time_sync, uint16_t us_pch, uint8_t uc_time_mode)
{
#ifdef PAL_RF
	pal_rf_set_chn_req(ul_time_sync, us_pch, uc_time_mode);
#else
	(void)ul_time_sync;
	(void)us_pch;
	(void)uc_time_mode;
#endif
}

/**
 * \brief Get timer
 *
 * \param pul_timer      Pointer to timer value
 * \param us_pch         Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_timer_get(uint32_t *pul_timer, uint16_t us_pch)
{
	(void)(us_pch);

#if defined(PAL_PLC)
	return pal_plc_timer_get(pul_timer, us_pch);
#elif defined(PAL_RF)
	return pal_rf_timer_get(pul_timer, us_pch);
#elif defined(PAL_SERIAL)
	return pal_serial_timer_get(pul_timer, us_pch);
#else
	*pul_timer = 0;
	return(PAL_CFG_INVALID_INPUT);
#endif
}

/**
 * \brief Get extended timer
 *
 * \param pull_timer      Pointer to extended timer value
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_timer_get_extended(uint64_t *pull_timer)
{
#if defined(PAL_PLC)
	return pal_plc_timer_get_extended(pull_timer);
#elif defined(PAL_RF)
	return pal_rf_timer_get_extended(pull_timer);
#elif defined(PAL_SERIAL)
	return pal_serial_timer_get_extended(pull_timer);
#endif
}

/**
 * \brief Get carrier detect
 *
 * \param puc_cd        Pointer to carrier detect
 * \param puc_rssi      Pointer to RSSI
 * \param pul_time      Pointer to time
 * \param puc_header    Pointer to header type
 * \param us_pch        Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_cd_get(uint8_t *puc_cd, uint8_t *puc_rssi, uint32_t *pul_time, uint8_t *puc_header, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_cd_get(puc_cd, puc_rssi, pul_time, puc_header, us_pch);
#endif
	}

	*puc_cd = 0;
	*puc_rssi = 0;
	*pul_time = 0;
	*puc_header = 0;
	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Get zero-cross time
 *
 * \param pul_zct      Pointer to zero-cross time (in 10s of microsec)
 * \param us_pch       Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_zct_get(uint32_t *pul_zct, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_zct_get(pul_zct, us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_zct_get(pul_zct, us_pch);
#endif
	}

	*pul_zct = 0;
	return(PAL_CFG_INVALID_INPUT);
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
uint8_t pal_agc_get(uint8_t *puc_mode, uint8_t *puc_gain, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_agc_get(puc_mode, puc_gain, us_pch);
#endif
	}

	*puc_mode = 0;
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
uint8_t pal_agc_set(uint8_t uc_mode, uint8_t uc_gain, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_agc_set(uc_mode, uc_gain, us_pch);
#endif
	}

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
uint8_t pal_nl_get(uint8_t *puc_noise, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_nl_get(puc_noise, us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_nl_get(puc_noise, us_pch);
#endif
	}

	*puc_noise = 0;
	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Make clear channel assessment mode 1
 *
* \param *puc_channel_state    Channel state (0: busy, 1: free)
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_cca_get(uint8_t *puc_channel_state)
{
#ifdef PAL_RF
	return pal_rf_cca_get(puc_channel_state);
#else
	return(PAL_CFG_INVALID_INPUT);
#endif
}

/**
 * \brief Get the channel/band
 *
 * \param us_pch       Pointer to the Physical channel
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_ch_get(uint16_t *pus_pch)
{
	if (IS_A_PLC_CHANNEL(*pus_pch)) {
#ifdef PAL_PLC
		return pal_plc_ch_get(pus_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(*pus_pch)) {
#ifdef PAL_RF
		return pal_rf_ch_get(pus_pch);
#endif
	}

	*pus_pch = 0xFFFF;
	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Set the channel/band
 *
 * \param us_pch      Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_ch_set(uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_ch_set(us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_ch_set(us_pch);
#endif
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
uint8_t pal_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_get_cfg(us_id, p_val, us_len, us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_get_cfg(us_id, p_val, us_len, us_pch);
#endif
	} else if (IS_A_SERIAL_CHANNEL(us_pch)) {
#ifdef PAL_SERIAL
		return pal_serial_get_cfg(us_id, p_val, us_len, us_pch);
#endif
	}

	return(PAL_CFG_INVALID_INPUT);
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
uint8_t pal_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_set_cfg(us_id, p_val, us_len, us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_set_cfg(us_id, p_val, us_len, us_pch);
#endif
	} else if (IS_A_SERIAL_CHANNEL(us_pch)) {
#ifdef PAL_SERIAL
		return pal_serial_set_cfg(us_id, p_val, us_len, us_pch);
#endif
	}

	return(PAL_CFG_INVALID_INPUT);
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
uint8_t pal_snr_get(uint8_t *puc_snr, uint8_t uc_qt, uint16_t us_pch)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
		return pal_plc_snr_get(puc_snr, uc_qt, us_pch);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_snr_get(puc_snr, uc_qt, us_pch);
#endif
	}

	*puc_snr = 0;
	return(PAL_CFG_INVALID_INPUT);
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
uint16_t pal_get_signal_capture(uint8_t *puc_dst, uint8_t uc_mode, uint32_t ul_time_start, uint32_t ul_duration)
{
#ifdef PAL_PLC
	return pal_plc_get_signal_capture(puc_dst, uc_mode, ul_time_start, ul_duration);
#else
	return 0;
#endif
}

/**
 * \brief Get message duration
 *
 * \param us_pch       Physical channel used
 * \param ul_msg_len   Message length
 * \param uc_scheme    Modulation scheme of message
 * \param uc_mode      Indicates if the message to transmit is type A, type B or type BC
 * \param uc_use       Indicates if the duration is for CSMA or for TBLOCK
 * \param pul_duration Pointer to message duration in microseconds (output)
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_get_msg_duration(uint16_t us_pch, uint16_t us_msg_len, uint8_t uc_scheme, uint8_t uc_mode, uint32_t *pul_duration)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
    	return pal_plc_get_msg_duration(us_pch, us_msg_len, uc_scheme, uc_mode, pul_duration);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_get_msg_duration(us_pch, us_msg_len, uc_scheme, uc_mode, pul_duration);
#endif
	}

	*pul_duration = 0;
	return(PAL_CFG_INVALID_INPUT);
}

/**
 * \brief Check if the modulation is good enough for a low FER in the given scheme
 *
 * \param us_chn              Channel of reception of the message
 * \param uc_scheme           Modulation to compare
 * \param uc_less_robust_mod  Less robust modulation
 *
 * \return true or false
 */
bool pal_rm_minimum_quality(uint16_t us_pch, uint8_t uc_scheme, uint8_t uc_less_robust_mod)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
    	return pal_plc_rm_minimum_quality(us_pch, uc_scheme, uc_less_robust_mod);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_rm_minimum_quality(us_pch, uc_scheme, uc_less_robust_mod);
#endif
	}

	return true;
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
uint8_t pal_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2)
{
	if (IS_A_PLC_CHANNEL(us_pch)) {
#ifdef PAL_PLC
    	return pal_plc_rm_get_less_robust_mod(us_pch, uc_mod1, uc_mod2);
#endif
	} else if (IS_A_RADIO_CHANNEL(us_pch)) {
#ifdef PAL_RF
		return pal_rf_rm_get_less_robust_mod(us_pch, uc_mod1, uc_mod2);
#endif
	}

	return 0x0F;
}

#endif  /* PAL_MULTIPHY */

/* @} */

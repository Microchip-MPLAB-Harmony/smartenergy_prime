/**
 *
 * \file
 *
 * \brief PRIME Serial Phy Abstraction Layer
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
#include "prime_hal_wrapper.h"
#ifdef PAL_MULTIPHY
#include "mpal.h"
#endif

/* #define PAL_DEBUG_ENABLE */
#ifdef PAL_DEBUG_ENABLE
#       define LOG_PAL_DEBUG(...)   printf(__VA_ARGS__ )
#else
#       define LOG_PAL_DEBUG(...)
#endif

#ifndef PAL_MULTIPHY
#define pal_serial_init pal_init
#define pal_serial_process pal_process
#define pal_serial_set_callbacks pal_set_callbacks
#define pal_serial_data_request pal_data_request
#define pal_serial_timer_get pal_timer_get
#define pal_serial_timer_get_extended pal_timer_get_extended
#define pal_serial_get_cfg pal_get_cfg
#endif


#ifdef PAL_SERIAL

#include "phy_serial.h"

/**
 * \weakgroup prime_pal_group
 * @{
 */

/* Indicate serial channel */
#define SERIAL_CHANNEL  0x400

static uint32_t sul_hi_timer_ref;
static uint32_t sul_previous_timer_ref;

static uint8_t suc_serial_sniffer_enabled;

static pal_callbacks_t sx_pal_app_callbacks;

static void _serial_data_confirm(xPhySerMsgTxResult_t *px_msg_cfm)
{
	if (sx_pal_app_callbacks.data_confirm) {
		x_pal_data_cfm_t x_data_cfm;

		x_data_cfm.tx_time = px_msg_cfm->tx_time;
		x_data_cfm.rms_calc = (uint16_t)px_msg_cfm->rms_calc;
		x_data_cfm.mode = px_msg_cfm->mode;
		x_data_cfm.buff_id = px_msg_cfm->buff_id;
		x_data_cfm.us_pch = SERIAL_CHANNEL;

		switch (px_msg_cfm->result) {
		case PHY_SER_TX_RESULT_SUCCESS:
			x_data_cfm.result = PAL_TX_RESULT_SUCCESS;
			break;

		case PHY_SER_TX_RESULT_BUSY_TX:
			x_data_cfm.result = PAL_TX_RESULT_BUSY_TX;
			break;

		default:
			x_data_cfm.result = PAL_TX_RESULT_PHY_ERROR;
			break;
		}

		/* Notify data confirm */
		sx_pal_app_callbacks.data_confirm(&x_data_cfm);
	}
}

static void _serial_data_indication(xPhySerMsgRx_t *px_msg)
{
	if (sx_pal_app_callbacks.data_indication) {
		x_pal_data_ind_t x_data_ind;

		x_data_ind.buff_id = 0;
		x_data_ind.data_buf = px_msg->data_buf;
		x_data_ind.data_len = px_msg->data_len;
		x_data_ind.header_type = px_msg->header_type;
		x_data_ind.mode = px_msg->mode;
		x_data_ind.rx_time = px_msg->rx_time;
		x_data_ind.scheme = px_msg->scheme;
        x_data_ind.us_pch = SERIAL_CHANNEL;
		x_data_ind.uc_less_robust_mod = px_msg->scheme;
		x_data_ind.us_estimated_bitrate = 20;
		x_data_ind.uc_lqi = ((px_msg->cinr_avg + 12) / 4);
		x_data_ind.ss_rssi = px_msg->rssi_avg;

		sx_pal_app_callbacks.data_indication(&x_data_ind);
	}
}

#ifdef PHY_SERIAL_ADDONS_ENABLE
/**
 * \brief Handler to receive data from RF215.
 */
static void _handler_serial_event(uint8_t *puc_serial_data, uint16_t us_len)
{
	x_usi_serial_cmd_params_t x_usi_msg;

	if (suc_serial_sniffer_enabled) {
		x_usi_msg.uc_protocol_type = PROTOCOL_SNIF_PRIME;
		x_usi_msg.ptr_buf = puc_serial_data;
		x_usi_msg.us_len = us_len;
		prime_hal_usi_send_cmd(&x_usi_msg);
	}
}
#endif

/** \brief Initialization */
/* @{ */
void pal_serial_init(void)
{
	phy_serial_init();

	/* Callback configuration */
	phy_serial_callbacks_t phy_ser_cbs;
	phy_ser_cbs.phy_ser_data_confirm = _serial_data_confirm;
	phy_ser_cbs.phy_ser_data_indication = _serial_data_indication;
#ifdef PHY_SERIAL_ADDONS_ENABLE
	phy_ser_cbs.phy_ser_addon_event = _handler_serial_event;
#endif
	phy_serial_set_callbacks(&phy_ser_cbs);

#ifdef _PRIME_SIM_
	suc_serial_sniffer_enabled = 1;
#else
	suc_serial_sniffer_enabled = 0;
#endif

	LOG_PAL_DEBUG("PalSerialInitialize OK\r\n");
}

/**
 * \brief This function links callback functions between upper layer and phy layer.
 *
 * \param pal_cbs      Pointer to callbacks structure
 *
 */
void pal_serial_set_callbacks(pal_callbacks_t *pal_cbs)
{
	/* Capture callbacks from upper layers */
	memcpy(&sx_pal_app_callbacks, pal_cbs, sizeof(pal_callbacks_t));

#ifdef PHY_SERIAL_ADDONS_ENABLE
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
void pal_serial_process(void)
{
	phy_serial_process();
}


/**
 * \brief Send message using PAL layer
 *
 * \param px_msg      Pointer to transmission data struct
 *
 */
uint8_t pal_serial_data_request(x_pal_msg_tx_t *px_msg)
{
	return phy_serial_tx_frame((xPhySerMsgTx_t *)px_msg);
}

/**
 * \brief Get timer
 *
 * \param pul_timer      Pointer to timer value
 * \param us_pch         Physical channel used
 *
 * \return PAL_CFG_SUCCESS if there is no error, otherwise returns PAL_CFG_INVALID_INPUT.
 */
uint8_t pal_serial_timer_get(uint32_t *pul_timer, uint16_t us_pch)
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
uint8_t pal_serial_timer_get_extended(uint64_t *pull_timer)
{
	uint32_t ul_time;
	uint64_t ull_time;

	pal_serial_timer_get(&ul_time, SERIAL_CHANNEL);
	ull_time = ((uint64_t)sul_hi_timer_ref << 32) | ul_time;

	*pull_timer = ull_time;
	return PAL_CFG_SUCCESS;
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
uint8_t pal_serial_get_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
    (void)us_len;
    (void)us_pch;

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_PHY_SNIFFER_EN:
		*(uint8_t *)p_val = suc_serial_sniffer_enabled;
		return(PAL_CFG_SUCCESS);

	case PAL_ID_INFO_DEVICE:
    	*(uint16_t *)p_val = 0x0400;
        return(PAL_CFG_SUCCESS);

    default:
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
uint8_t pal_serial_set_cfg(uint16_t us_id, void *p_val, uint16_t us_len, uint16_t us_pch)
{
	(void)us_len;
    (void)us_pch;

	/* Check identifier */
	switch (us_id) {
	case PAL_ID_PHY_SNIFFER_EN:
#ifdef PHY_SERIAL_ADDONS_ENABLE
		suc_serial_sniffer_enabled = *(uint8_t *)p_val;
#endif
        return(PAL_CFG_SUCCESS);

    default:
        ;
    }

	return(PAL_CFG_INVALID_INPUT);
}

#endif /* PAL_SERIAL */

/* @} */




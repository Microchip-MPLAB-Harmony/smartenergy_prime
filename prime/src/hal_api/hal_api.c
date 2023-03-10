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
#include <asf.h>

#include "hal.h"
#include "hal_private.h"
#include "flash_efc.h"
#if (!PIC32CX)
#include "wdt.h"
#else
#include "dwdt.h"
#endif

#include "conf_app_example.h"
#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
#if BOARD == ATPL360ASB
	#include "vim878.h"
#elif (BOARD == ATPL360AMB) || (BOARD == ATPL360MB)
	#include "c0216CiZ.h"
#elif (BOARD == PIC32CXMTSH_DB) || (BOARD == PIC32CXMTC_DB)
	#include "cl010.h"
#else
	#include "c42364a.h"
#endif
#endif
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

/* Function declarations */
void __user_low_level_init(void);

/**
 * \brief Low level initialization called from Reset_Handler, before calling
 * to main and before data initialization. This function should not use
 * initialized data.
 */
void __user_low_level_init(void)
{
#if BOARD == PIC32CXMTSH_DB || BOARD == PIC32CXMTC_DB || BOARD == PIC32CXMTG_EK
	/* Enable coprocessor peripherals to allow access to PIOD */
	uint32_t read_reg;

	/* Assert coprocessor reset and reset its peripheral */
	read_reg = RSTC->RSTC_MR;
	read_reg &= ~(RSTC_MR_KEY_Msk | RSTC_MR_CPEREN | RSTC_MR_CPROCEN);
	read_reg |= RSTC_MR_KEY_PASSWD;
	RSTC->RSTC_MR = read_reg;

	/* Enables Coprocessor Bus Master Clock */
	PMC->PMC_SCER = PMC_SCER_CPBMCK | PMC_SCER_CPKEY_PASSWD;

	/* Set coprocessor clock prescaler */
	read_reg = PMC->PMC_CPU_CKR;
	read_reg &= ~PMC_CPU_CKR_CPPRES_Msk;
	PMC->PMC_CPU_CKR = read_reg | PMC_CPU_CKR_CPPRES(1);

	/*  Wait to PMC_SR_CPMCKRDY = 1 */
	while ((PMC->PMC_SR & PMC_SR_CPMCKRDY) == 0) {
	}

	/* Release coprocessor peripheral reset */
	RSTC->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN);

	/* Set coprocessor clock prescaler */
	read_reg = PMC->PMC_CPU_CKR;
	read_reg &= ~PMC_CPU_CKR_CPPRES_Msk;
	PMC->PMC_CPU_CKR = read_reg | PMC_CPU_CKR_CPPRES(0);

	/*  Wait to PMC_SR_CPMCKRDY = 1 */
	while ((PMC->PMC_SR & PMC_SR_CPMCKRDY) == 0) {
	}
#endif

	/* Critical initialization of PLC pins */
	hal_plc_crit_init();
}

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
 * \brief Initalize hardware abstraction layer interface
 *
 */
void hal_init(void)
{
	uint32_t *ptr_nvic_cpr0;
	bool b_is_int_disabled;

	/* Clear pending interrupts */
	ptr_nvic_cpr0 = (uint32_t *)NVIC_ICPR0;
	*ptr_nvic_cpr0 = 0xFFFFFFFF;

#ifndef HAL_ATPL360_INTERFACE
	/* reset hardware MODEM */
	hal_plc_reset();
#endif

	/* set critical region */
	b_is_int_disabled = __get_PRIMASK();
	if (!b_is_int_disabled) {
		Disable_global_interrupt();
	}

#ifndef HAL_ATPL360_INTERFACE
#ifdef SAME70
	hal_net_freq_init();
#endif
#endif

	/* init USI module */
	hal_usi_init();

	/* end critical region */
	if (!b_is_int_disabled) {
		Enable_global_interrupt();
	}

#if PIC32CX
	/* Disable Write Protection */
	efc_disable_write_protection(SEFC0);

	/* Unlock Key Bus */
	efc_unlock_key_bus_transfer(SEFC0);

	/* Set User Signature Rights */
	efc_set_usr_rights(SEFC0, EEFC_USR_WRENUSB1 | EEFC_USR_RDENUSB1);
#endif

#ifndef DISABLE_RESET_HANDLING
	/* Init reset handler module */
	hal_reset_handler_init();
#endif
#ifndef DISABLE_PIB_HANDLING
	/* Init PIB module */
	hal_pib_init();
#endif

	/* Initialize timer of 1us service */
	timer_1us_init();
}

/**
 * \brief Process hardware layer interface
 *
 */
void hal_process(void)
{
#ifndef HAL_ATPL360_INTERFACE
#ifdef SAME70
	hal_net_freq_process();
#endif
#endif

	hal_usi_process();
}

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

/**
 * \brief Report a debug error
 *
 * \param ul_err_type     Type of error
 */
void hal_debug_report(uint32_t ul_err_type)
{
#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
#if (BOARD == ATPL360AMB) || (BOARD == ATPL360MB)
	char puc_app[16];
	c0216CiZ_set_cursor(C0216CiZ_LINE_DOWN, 0);
	/* Show error */
	sprintf(puc_app, "Error %10u", ul_err_type);
	c0216CiZ_show((const char *)puc_app);
#elif (BOARD == PIC32CXMTSH_DB) || (BOARD == PIC32CXMTC_DB)
	ul_err_type = ul_err_type % 100000000; /* Only eight digits in the display */
	cl010_show_numeric_string(CL010_LINE_UP, (const uint8_t *)&ul_err_type);
#else
	ul_err_type = ul_err_type % 10000; /* Only four digits in the display */
	c42364a_show_numeric_dec((int32_t)ul_err_type);
#endif
#endif

#ifdef PRIME_DEBUG_REPORT
	modem_debug_report(ul_err_type);
#endif

	if (ul_err_type == CRITICAL_ERROR) {
		while (1) {
#ifndef _PRIME_SIM_
			/* Restart watchdog */
#if !PIC32CX
			wdt_restart(WDT);
#else
			dwdt_restart(DWDT, WDT0_ID);
#endif
#endif

#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
#if (BOARD == ATPL360AMB) || (BOARD == ATPL360MB)
			c0216CiZ_set_cursor(C0216CiZ_LINE_UP, 0);
			c0216CiZ_show((const char *)"CRITICAL ERROR");
#elif (BOARD == PIC32CXMTSH_DB)
			cl010_show_icon(CL010_ICON_PHASE_1);
			cl010_show_icon(CL010_ICON_PHASE_2);
			cl010_show_icon(CL010_ICON_PHASE_3);
			cl010_show_icon(CL010_ICON_PF);
			cl010_clear_icon(CL010_ICON_COMM_SIGNAL_LOW);
			cl010_clear_icon(CL010_ICON_COMM_SIGNAL_MED);
			cl010_clear_icon(CL010_ICON_COMM_SIGNAL_HIG);
			cl010_show_numeric_string(CL010_LINE_UP, (const uint8_t *)"EEEEEEEE");
#endif
#else
#if (BOARD == PL360BN) || (BOARD == ATPL360AMB) || (BOARD == ATPL360MB) || (BOARD == PL360G55CF_EK) || (BOARD == PIC32CXMTG_EK)
			LED_On(LED0);
			LED_On(LED1);
#endif
#endif
			delay_ms(500);
		}
	}
}

/**
 *  \brief Configure microcontroller supply monitor SUPC and browndown detector.
 */
void hal_setup_supply_monitor(uint32_t ul_monitoring_rate, uint32_t ul_monitor_threshold)
{
#if (!PIC32CX)
	/* Enable sam4c brownout detector */
	supc_enable_brownout_detector(SUPC);
	/* enable sam4c browout detector reset */
	supc_enable_brownout_reset(SUPC);
#endif

#if (!SAMG)
	/* enable and configure configure voltage monitor  */
	supc_enable_voltage_regulator(SUPC);
#endif

	/* configure sampling */
	supc_set_monitor_sampling_period(SUPC, ul_monitoring_rate);
	/* Set Monitor Threshold */
#if (!PIC32CX)
	supc_set_monitor_threshold(SUPC, ul_monitor_threshold);
#else
	supc_set_monitor_threshold(SUPC, (supc_sm_thrshld_level_t)ul_monitor_threshold);
#endif
	/* enable reset monitor if voltage monitor is under threshold voltage */
	supc_enable_monitor_reset(SUPC);

	/* Wait 30ms to be sure that voltage is stable */
	delay_ms(30);
}

/**
 * \brief Setup the watchdog
 *
 * \param ul_watchdog_time   Watchdog time in microseconds
 */
void hal_watchdog_setup(uint32_t ul_watchdog_time)
{
#if (!PIC32CX)
	uint32_t timeout_value;
	uint32_t wdt_mode;

	/* get value to init wdog from time in us. */
	timeout_value = wdt_get_timeout_value(ul_watchdog_time, BOARD_FREQ_SLCK_XTAL);

#ifdef WATCHDOG_ENABLE_RESET
	/* Configure WDT to trigger a reset. */
	wdt_mode = WDT_MR_WDRSTEN | /* Enable WDT reset. */
#if (BOARD != SAME70_XPLAINED) && (BOARD != PL360BN)
			WDT_MR_WDRPROC | /* WDT fault resets processor only. */
#endif
			WDT_MR_WDDBGHLT | /* WDT stops in debug state. */
			WDT_MR_WDIDLEHLT; /* WDT stops in idle state. */
	/* Initialize WDT with the given parameters. */
	wdt_init(WDT, wdt_mode, timeout_value, timeout_value);
#else
	/* Configure WDT to trigger an interruption. */
	wdt_mode = WDT_MR_WDFIEN | /* Enable WDT interrupt. */
#if (BOARD != SAME70_XPLAINED) && (BOARD != PL360BN)
			WDT_MR_WDRPROC | /* WDT fault resets processor only. */
#endif
			WDT_MR_WDDBGHLT | /* WDT stops in debug state. */
			WDT_MR_WDIDLEHLT; /* WDT stops in idle state. */
	/* Initialize WDT with the given parameters. */
	wdt_init(WDT, wdt_mode, timeout_value, timeout_value);

	/* Configure and enable WDT interrupt. */
	NVIC_DisableIRQ(WDT_IRQn);
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_SetPriority(WDT_IRQn, 0);
	NVIC_EnableIRQ(WDT_IRQn);
#endif /* WATCHDOG_ENABLE_RESET */
#else /* PIC32CX */
	dwdt_cfg_t dwdt_cfg;

#ifdef WATCHDOG_ENABLE_RESET
	/* Configure DWDT to trigger a reset */
	dwdt_cfg.ul_slck = CHIP_FREQ_SLCK_RC;
	dwdt_cfg.ul_prescaler = WDT0_IL_PRESC_RATIO128;
	dwdt_cfg.ul_mode =  WDT0_MR_WDIDLEHLT | /* WDT stops in idle state. */
				WDT0_MR_WDDBG0HLT | /* WDT stops in core 0 debug state. */
				WDT0_MR_WDDBG1HLT | /* WDT stops in core 1 debug state. */
				WDT0_MR_PERIODRST; /* WDT enables period reset */
	dwdt_cfg.ul_time = (ul_watchdog_time / 1000); /* Period time in ms */
	dwdt_cfg.ul_rep_time = 0; /* Repeat threshold time in ms */
	dwdt_cfg.ul_intlvl_time = 0; /* Interrupt threshold time in ms */
	/* Initialize DWDT with the given parameters. */
	if (dwdt_init(DWDT, WDT0_ID, &dwdt_cfg)) {
		while (1) {
			/* Invalid timeout values, error. */
		}
	}

#else
	/* Configure DWDT to trigger an interrupt */
	dwdt_cfg.ul_slck = CHIP_FREQ_SLCK_RC;
	dwdt_cfg.ul_prescaler = WDT0_IL_PRESC_RATIO128;
	dwdt_cfg.ul_mode =  WDT0_MR_WDIDLEHLT | /* WDT stops in idle state. */
				WDT0_MR_WDDBG0HLT | /* WDT stops in core 0 debug state. */
				WDT0_MR_WDDBG1HLT;  /* WDT stops in core 1 debug state. */
	dwdt_cfg.ul_time = (ul_watchdog_time / 1000); /* Period time in ms */
	dwdt_cfg.ul_rep_time = 0; /* Repeat threshold time in ms */
	dwdt_cfg.ul_intlvl_time = 0; /* Interrupt threshold time in ms */
	/* Initialize DWDT with the given parameters. */
	if (dwdt_init(DWDT, WDT0_ID, &dwdt_cfg)) {
		while (1) {
			/* Invalid timeout values, error. */
		}
	}
#endif /* WATCHDOG_ENABLE_RESET */
#endif /* PIC32CX */
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

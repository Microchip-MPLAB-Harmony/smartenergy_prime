/**
 * \file
 *
 * \brief PRIME_API : API joins a library's existing interface into a global interface
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
#include "prime_hal_wrapper.h"
#include "pal.h"
#include "mac.h"
#include "cl_null.h"
#include "mngp.h"
#include "conf_prime_stack.h"
#if USED_SSCS == SSCS_432
#include "cl_432.h"
#elif USED_SSCS == SSCS_IPV6
#include "cl_ipv6.h"
#else
#error No SSCS defined
#endif
#include "conf_mngp.h"
#include "conf_pal.h"
#include "conf_mac.h"

#include "prime_api.h"
#include "prime_hal_wrapper.h"

/* ASF includes */
#include "interrupt.h"

#include "atpl360.h"

#ifdef PAL_ENABLE_SER_PHY
#include "phy_serial.h"
#include "conf_phy_serial.h"
#endif

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/**
 * \weakgroup prime_api_group
 * @{
 */
#ifdef PRIME_API_SEPARATED_APPS
#ifdef __GNUC__
/** \brief Initialize segments */
/* @{ */
extern uint32_t _szero;
extern uint32_t _ezero;
/* @} */

/**
 * \brief PRIME data startup
 */
static void _prime_data_startup(void)
{
	uint32_t *pDest;

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) {
		*pDest++ = 0;
	}
}

#endif /* __GNUC__ */

#ifdef __ICCARM__
#pragma section = ".bss"

/**
 * \brief PRIME data startup
 */
static void _prime_data_startup(void)
{
	uint8_t *bss_start, *bss_end;
	uint32_t n;

	bss_start = __section_begin(".bss");
	bss_end = __section_end(".bss");

	/* Clear the zero-initialized data section */
	n = bss_end - bss_start;
	while (n--) {
		*bss_start++ = 0;
	}
}

#endif /* __ICCARM__ */
#endif /* PRIME_API_SEPARATED_APPS */

/**
 * \brief Set PRIME version
 *
 * \param mac_info    Pointer to MAC information
 */
static void _set_prime_version(mac_version_info_t *mac_info)
{
	uint8_t uc_size_config, uc_size_info;
	uint8_t uc_copy_len;

	memset(mac_info, 0, sizeof(mac_version_info_t));

	/* Update MODEL */
	uc_size_config = sizeof(PRIME_FW_MODEL);
	uc_size_info = sizeof(mac_info->fw_model);
	if (uc_size_config < uc_size_info) {
		uc_copy_len = uc_size_config;
	} else {
		uc_copy_len = uc_size_info;
	}

	memcpy(mac_info->fw_model, PRIME_FW_MODEL, uc_copy_len);
	mac_info->pib_model = PRIME_PIB_MODEL;

	/* Update VENDOR */
	uc_size_config = sizeof(PRIME_FW_VENDOR);
	uc_size_info = sizeof(mac_info->fw_vendor);
	if (uc_size_config < uc_size_info) {
		uc_copy_len = uc_size_config;
	} else {
		uc_copy_len = uc_size_info;
	}

	memcpy(mac_info->fw_vendor, PRIME_FW_VENDOR, uc_copy_len);
	mac_info->pib_vendor = PRIME_PIB_VENDOR;

	/* Update VERSION */
	uc_size_config = sizeof(PRIME_FW_VERSION);
	uc_size_info = sizeof(mac_info->fw_version);
	if (uc_size_config < uc_size_info) {
		uc_copy_len = uc_size_config;
	} else {
		uc_copy_len = uc_size_info;
	}

	memcpy(mac_info->fw_version, PRIME_FW_VERSION, uc_copy_len);
}

/**
 * \brief PRIME stack initialization
 */
void prime_stack_init(void *px_hal_api)
{
	mac_version_info_t mac_info;

	/* set critical region */
	__set_BASEPRI( 2 << (8 - __NVIC_PRIO_BITS));

#ifdef PRIME_API_SEPARATED_APPS
	_prime_data_startup();
#endif
	/* Set PRIME HAL wrapper */
	prime_hal_config(px_hal_api);

	/* Set PRIME version from config file*/
	_set_prime_version(&mac_info);

	/* Initialize PAL layer */
	pal_init();

#ifdef PAL_ENABLE_SER_PHY
	phy_ser_init((uint8_t)PHY_USI_PORT);
#endif

#ifdef MAC_SNIFFER_USI_PORT
	mac_sniffer_if_init(MAC_SNIFFER_USI_PORT);
#endif

	/* Initialize MAC layer */
#ifdef MAC_SECURITY_PROFILE
	mac_init(&mac_info, (uint8_t)MAC_SECURITY_PROFILE);
#else
	mac_init(&mac_info, 0);
#endif

	/* Initialize CONV layer */
	cl_null_init();
#if USED_SSCS == SSCS_432
	cl_432_init();
#elif USED_SSCS == SSCS_IPV6
	cl_ipv6_init();
#endif

	/* Initialize Management Plane */
	mngp_init(&mac_info, (uint8_t)MNGP_SPROF_USI_PORT);

	/* set critical region */
	__set_BASEPRI(0);
}

/**
 * \brief PRIME stack process
 */
void prime_stack_process(void)
{
	/* set critical region */
	__set_BASEPRI( 3 << (8 - __NVIC_PRIO_BITS));

	/* Process PRIME layers */
	pal_process();

	mac_process();

#ifdef PAL_ENABLE_SER_PHY
	phy_ser_process();
#endif

#if PRIME_MODE == PRIME_SN
	/* Process MNG layer */
	mngp_process();
#endif

	/* set critical region */
	__set_BASEPRI(0);
}

/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */

// /**
//  * \file
//  *
//  * \brief HAL_FU: Firmware Upgrade
//  *
//  * Copyright (c) 2023 Atmel Corporation. All rights reserved.
//  *
//  * \asf_license_start
//  *
//  * \page License
//  *
//  * Redistribution and use in source and binary forms, with or without
//  * modification, are permitted provided that the following conditions are met:
//  *
//  * 1. Redistributions of source code must retain the above copyright notice,
//  *    this list of conditions and the following disclaimer.
//  *
//  * 2. Redistributions in binary form must reproduce the above copyright notice,
//  *    this list of conditions and the following disclaimer in the documentation
//  *    and/or other materials provided with the distribution.
//  *
//  * 3. The name of Atmel may not be used to endorse or promote products derived
//  *    from this software without specific prior written permission.
//  *
//  * 4. This software may only be redistributed and used in connection with an
//  *    Atmel microcontroller product.
//  *
//  * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
//  * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
//  * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
//  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  * POSSIBILITY OF SUCH DAMAGE.
//  *
//  * \asf_license_stop
//  *
//  */
// #include "string.h"
// #include "conf_hal.h"
// #include "flash_efc.h"
// #include "hal_private.h"
// #include "asf.h"
// #include "conf_app_example.h"
// #ifdef HAL_FU_ENABLE_SIGNATURE
// #include "mbedtls/ecp.h"
// #include "mbedtls/ecdsa.h"
// #include "mbedtls/sha256.h"
// #endif

// /* / @cond 0 */
// /**INDENT-OFF**/
// #ifdef __cplusplus
// extern "C" {
// #endif
// /**INDENT-ON**/
// /* / @endcond */

// /** \brief FU CRC calculation states */
// /* @{ */
// enum fu_crc_state_val {
// 	FU_CRC_STATE_IDLE          = 0,
// 	FU_CRC_STATE_CALCULATING   = 1,
// };
// /* @} */

// #define FW_NO_SIGN                   0
// #define FW_SIGN_RSA3072_SHA256       1
// #define FW_SIGN_ECDSA_SHA256         2

// /* ATMEL Public KEY, Curve: SECP256K1 */
// const uint8_t puc_atmel_ecc_public_key[65] = {0x04, 0x14, 0x29, 0x55, 0x8e, 0xf0, 0x9a, 0x1b,
// 					      0xb3, 0x96, 0x31, 0x92, 0xe5, 0xe5, 0xd5, 0xbd,
// 					      0xb6, 0xbe, 0x96, 0xc4, 0x0e, 0x8e, 0x60, 0xd4,
// 					      0xda, 0x3d, 0x54, 0xdc, 0x22, 0xda, 0x8f, 0xe2,
// 					      0x09, 0xd3, 0xe2, 0x02, 0xa2, 0xf7, 0x39, 0xa0,
// 					      0xe6, 0x80, 0xb5, 0x88, 0xbb, 0xc0, 0x18, 0xf9,
// 					      0x73, 0x53, 0x52, 0x2d, 0x1b, 0x3b, 0x7a, 0x7e,
// 					      0x20, 0xbc, 0x5f, 0x5f, 0x9e, 0x73, 0x1c, 0x77,
// 					      0xcf };

// /** Callback function pointer for CRC result */
// static void (*_crc_cb_function)(uint32_t ul_crc);

// /** Callback function pointer for signature and binary result */
// static void (*_signature_image_cb_function)(hal_fu_verif_result_t uc_result);

// /** Callback function pointer for FU result */
// static void (*_fu_result_cb_function)(hal_fu_result_t *uc_result);

// /** Internal var to manage memory address in FU process */
// static uint32_t sul_fw_mem_address;

// /** FU information data */
// static hal_fu_info_t x_fu_data;

// /** CRC calculation state */
// static uint8_t suc_crc_state;

// /** 2 FU regions */
// static x_fu_region_cfg_t *spx_fu_region_cfg;
// static uint8_t suc_region_to_fu;

// #ifdef PRIME_NUM_REGIONS
// /** 2 FW apps: MAC and PHY */
// static uint8_t suc_app_to_fu;
// #endif

// /**
//  * \brief Erase internal flash region in use
//  *
//  * \param x_fu_region    Pointer to FU Region configuration
//  *
//  */
// static uint32_t sul_flash;
// static uint32_t sul_flash_end;
// static bool deleteActive;
// static void _erase_flash_region(x_fu_region_cfg_t *x_fu_region)
// {
// 	sul_flash = x_fu_region->ul_address;
// 	sul_flash_end = (uint32_t)(sul_flash + x_fu_region->ul_size);

// 	deleteActive = true;

// 	flash_unlock(sul_flash, sul_flash_end, 0, 0);
// }

// #ifdef PRIME_NUM_REGIONS
// /**
//  * \brief Verify if the received image is valid
//  *
//  * \return True if the image is valid, otherwise false
//  */
// static bool _verify_image(void)
// {
// 	uint32_t pul_fw_data;
// 	uint32_t pul_exec_data;
// 	uint16_t us_exec_vendor;
// 	uint16_t us_exec_model;
// 	uint16_t us_fw_vendor;
// 	uint16_t us_fw_model;
// 	uint8_t puc_fw_data[192];
// 	uint8_t puc_exec_data[192];
// 	uint8_t puc_image_id[PRIME_METADATA_SIZE];


// 	/* Get FW data */
// 	pul_fw_data = sul_fw_mem_address;

// 	/* Extract image identifier from FW data */
// 	memcpy(puc_image_id, (uint8_t *)(pul_fw_data + x_fu_data.image_size - x_fu_data.sign_length - PRIME_METADATA_SIZE), sizeof(puc_image_id));

// 	/* Check image identifier */
// 	if (memcmp(puc_image_id, PRIME_MAC13_BIN, sizeof(PRIME_MAC13_BIN))) {
// 		if (memcmp(puc_image_id, PRIME_MAC14_BIN, sizeof(PRIME_MAC14_BIN))) {
// #ifdef HAL_ATPL360_INTERFACE
// 			if (memcmp(puc_image_id, PRIME_PHY_BIN, sizeof(PRIME_PHY_BIN))) {
// 				if (memcmp(puc_image_id, PRIME_APP_BIN, sizeof(PRIME_APP_BIN))) {
// 					return false;
// 				} else {
// 					suc_app_to_fu = PRIME_MAIN_APP;
// 				}
// 			} else {
// 				suc_app_to_fu = PRIME_PHY_APP;
// 			}
// #else /* Not HAL_ATPL360_INTERFACE */
// 			if (memcmp(puc_image_id, PRIME_APP_BIN, sizeof(PRIME_APP_BIN))) {
// 				return false;
// 			} else {
// 				suc_app_to_fu = PRIME_MAIN_APP;
// 			}
// #endif /* HAL_ATPL360_INTERFACE */
// 		} else {
// 			suc_app_to_fu = PRIME_MAC14_APP;
// 		}
// 	} else {
// 		suc_app_to_fu = PRIME_MAC13_APP;
// 	}

// 	/* Set image identifier */
// 	gpbr_write(GPBR4, (uint32_t)(suc_app_to_fu));

// 	/* If upgrading MAC, further checks on the image are needed */
// 	if ((suc_app_to_fu != PRIME_MAC13_APP) && (suc_app_to_fu != PRIME_MAC14_APP)) {
// 		return true;
// 	}

// 	/* Load first page in execution for MAC */
// 	if (suc_app_to_fu == PRIME_MAC13_APP) {
// 		pul_exec_data = PRIME_MAC13_FLASH_LOCATION;
// 	} else {
// 		pul_exec_data = PRIME_MAC14_FLASH_LOCATION;
// 	}

// 	memcpy(puc_exec_data, (uint8_t *)pul_exec_data, x_fu_data.page_size);

// 	/* Load first page from FW data */
// 	memcpy(puc_fw_data, (uint8_t *)pul_fw_data, x_fu_data.page_size);

// 	/* Extract vendor */
// 	us_fw_vendor = (uint16_t)puc_fw_data[0] + (uint16_t)(puc_fw_data[1] << 8);
// 	us_exec_vendor = (uint16_t)puc_exec_data[0] + (uint16_t)(puc_exec_data[1] << 8);

// 	/* Check vendor */
// 	if (us_fw_vendor != us_exec_vendor) {
// 		return false;
// 	} else {
// 		/* Extract model */
// 		us_fw_model = (uint16_t)puc_fw_data[2] + (uint16_t)(puc_fw_data[3] << 8);
// 		us_exec_model = (uint16_t)puc_exec_data[2] + (uint16_t)(puc_exec_data[3] << 8);

// 		/* Check model */
// 		if (us_fw_model != us_exec_model) {
// 			return false;
// 		} else {
// 			return true;
// 		}
// 	}
// }
// #endif

 #include <string.h>

#include "definitions.h"
#include "device.h"

void SRV_FU_DataRead(uint32_t address, uint8_t *buffer, uint16_t size)
{
	uint32_t readAddress;

	readAddress = sul_fw_mem_address + address;
	if (size) {
		memcpy(buffer, (uint8_t *)readAddress, size);
	}
}

uint8_t SRV_FU_DataWrite(uint32_t address, uint8_t *buffer, uint16_t size)
{
	uint8_t result = 0;
	uint32_t writeAddress;


	writeAddress = sul_fw_mem_address + address;

	/* Number of bytes is 8 bytes alignment */
	if(size % 8) {
		size = ((size / 8) + 1) * 8;
	}

	if (size) {
		uint32_t resultFlash;

		resultFlash = flash_write(writeAddress, buffer, size, 0);

		if (resultFlash == FLASH_RC_OK) {
			result = 1;
		} else {
			result = 0;
		}
	}

	return result;
}

void SRV_FU_CfgRead(void *dst, uint16_t size)
{
	uint32_t bufferValue[4];
	uint32_t *pointerBuffer;

	pointerBuffer = (uint32_t *)bufferValue;
	*pointerBuffer++ = SUPC_GPBRead(GPBR_REGS_0);
	*pointerBuffer++ = SUPC_GPBRead(GPBR_REGS_1);
	*pointerBuffer++ = SUPC_GPBRead(GPBR_REGS_2);
	*pointerBuffer = SUPC_GPBRead(GPBR_REGS_3);
	memcpy(dst, (uint8_t *)bufferValue, size);

}

uint8_t SRV_FU_CfgWrite(void *src, uint16_t size)
{
	uint32_t bufferValue[4];
	uint32_t *pointerBuffer;

	memcpy((uint8_t *)bufferValue, src, size);

	pointerBuffer = (uint32_t *)bufferValue;
	SUPC_GPBRWrite(GPBR_REGS_0, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_1, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_2, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_3, *pointerBuffer);

	return 1;
}

/**
 * \brief Start the FU process by initializing and unlocking the memory
 *
 * \param x_fu_info    FU information data
 *
 */
void SRV_FU_Start(hal_fu_info_t *x_fu_info)
{
	x_fu_data.image_size = x_fu_info->image_size;
	x_fu_data.page_size = x_fu_info->page_size;
	x_fu_data.sign_algorithm = HAL_FU_NO_SIGNATURE;
	x_fu_data.sign_length = 0;
#ifdef HAL_FU_ENABLE_SIGNATURE
	x_fu_data.sign_algorithm = x_fu_info->sign_algorithm;
	x_fu_data.sign_length = x_fu_info->sign_length;
#endif

	/* Get image region for FW */
	suc_region_to_fu = 0;
	spx_fu_region_cfg[0].in_use = true;

	/* erase internal flash pages */
	_erase_flash_region(&spx_fu_region_cfg[suc_region_to_fu]);

	/* Set CRC status */
	suc_crc_state = FU_CRC_STATE_IDLE;

	/* Set addres to FU */
	sul_fw_mem_address = spx_fu_region_cfg[suc_region_to_fu].ul_address;

#ifdef PRIME_NUM_REGIONS
	/* Reset prime app to FU */
	suc_app_to_fu = PRIME_INVALID_APP;

	/* Clear boot configuration */
	x_boot_info_cfg_t x_boot_cfg;
	hal_get_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	x_boot_cfg.ul_img_size = 0;
	x_boot_cfg.ul_orig_addr = 0;
	x_boot_cfg.ul_dest_addr = 0;
	x_boot_cfg.uc_pages_counter = 0;
	x_boot_cfg.uc_boot_state = 0;
	hal_set_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	/* Reset image identifier */
	gpbr_write(GPBR4, (uint32_t)suc_app_to_fu);
#endif

	return;
}

/**
 * \brief Finish the FU process and execute the new firmware
 *
 * \param uc_hal_res     Result of the FU process
 *
 */
void SRV_FU_End(hal_fu_result_t uc_hal_res)
{
	hal_fu_result_t uc_res;

	/* check callback is initialized */
	if (!_fu_result_cb_function) {
		return;
	}

	switch (uc_hal_res) {
	case HAL_FU_SUCCESS:
		uc_res = HAL_FU_SUCCESS;
		_fu_result_cb_function(&uc_res);
		break;

	case HAL_FU_CRC_ERROR:
		uc_res = HAL_FU_CRC_ERROR;
		_fu_result_cb_function(&uc_res);
		break;

	case HAL_FU_FW_REVERT:
		uc_res = HAL_FU_FW_REVERT;
		_fu_result_cb_function(&uc_res);
		break;

	case HAL_FU_FW_CONFIRM:
		uc_res = HAL_FU_FW_CONFIRM;
		_fu_result_cb_function(&uc_res);
		break;

	default:
		break;
	}
}

/**
 * \brief Execute the old firmware
 *
 */
void SRV_FU_Revert(void)
{
	hal_fu_result_t uc_res;

	uc_res = HAL_FU_FW_REVERT;
	_fu_result_cb_function(&uc_res);
}

/**
 * \brief Calculate the CRC of the new firmware
 *
 */
void SRV_FU_CrcCalculate(void)
{
	uint32_t pul_flash;
	uint32_t ul_crc;

	if (suc_crc_state == FU_CRC_STATE_CALCULATING) {
		return;
	}

	suc_crc_state = FU_CRC_STATE_CALCULATING;

	pul_flash = spx_fu_region_cfg[suc_region_to_fu].ul_address;
	ul_crc = hal_pcrc_calc_fu((uint8_t *)pul_flash, x_fu_data.image_size, 0);
	/* check pointer function */
	if (_crc_cb_function) {
		_crc_cb_function(ul_crc);
	}

	suc_crc_state = FU_CRC_STATE_IDLE;
}

/**
 * \brief Finish the FU process and execute the new firmware
 *
 * \param p_handler   CRC callback function pointer
 *
 */
void SRV_FU_CrcSetCallback(void (*p_handler)(uint32_t ul_crc))
{
	_crc_cb_function = p_handler;
}

/**
 * \brief Check the signature of the new firmware and the validity of the received binary file
 *
 */

#ifdef HAL_FU_ENABLE_SIGNATURE
void SRV_FU_SignatureImageCheck(void)
{
	uint8_t uc_algorithm;
	uint16_t us_signature_length;
	uint32_t pul_fw_data;
	uint32_t pul_fw_length;

	/* check if there is a callback function, otherwise, don't waste time */
	if (!_signature_image_cb_function) {
		return;
	}

	/* get signature params */
	uc_algorithm = x_fu_data.sign_algorithm;
	us_signature_length = x_fu_data.sign_length;

	/* Get FW data */
	pul_fw_data = spx_fu_region_cfg[suc_region_to_fu].ul_address;
	pul_fw_length = x_fu_data.image_size;

	if (uc_algorithm == FW_NO_SIGN) {
		/* Verify if this is a right image */
		if (_verify_image()) {
			_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
		} else {
			_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
		}
	} else if (uc_algorithm == FW_SIGN_ECDSA_SHA256) {
		/* This code assumes that curve secp_256k1 is used.*/
		/* It will produce a 71 byte signature */
		int32_t i_ret = 0;
		uint8_t puc_sha256_hash[32];
		uint8_t puc_signature[256];
		mbedtls_ecp_keypair ctx_verify;
		mbedtls_ecp_keypair st_ecp_public_key;

		/*Load signature from FW data. Signature data is concatenated to FW */
		memcpy(puc_signature, (uint8_t *)pul_fw_data + pul_fw_length - us_signature_length, us_signature_length);

		/*Initialize verify context*/
		mbedtls_ecdsa_init(&ctx_verify);

		/* Load ECC curve */
		mbedtls_ecp_group_load(&ctx_verify.grp, MBEDTLS_ECP_DP_SECP256K1);

		/*Load ATMEL public KEY */
		mbedtls_ecp_keypair_init(&st_ecp_public_key);
		mbedtls_ecp_point_read_binary(&ctx_verify.grp, &st_ecp_public_key.Q, puc_atmel_ecc_public_key, sizeof puc_atmel_ecc_public_key);
		mbedtls_ecp_copy(&ctx_verify.Q, &st_ecp_public_key.Q);

		/* Verify curve/key */
		if (mbedtls_ecp_check_pubkey(&ctx_verify.grp, &st_ecp_public_key.Q) != 0) {
			/* Not a valid public key! */
			_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
			mbedtls_ecp_keypair_free(&ctx_verify);
			return;
		}

		/* Compute FW SHA-256 Hash */
		mbedtls_sha256((uint8_t *)pul_fw_data, pul_fw_length - us_signature_length, puc_sha256_hash, 0);

		/* Check signature */
		i_ret = mbedtls_ecdsa_read_signature(&ctx_verify, puc_sha256_hash, 32, puc_signature, us_signature_length);
		if (i_ret < 0) {
			_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
		} else {
			/* Verify if this is a right image */
			if (_verify_image()) {
				_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
			} else {
				_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
			}
		}

		/* release memory */
		mbedtls_ecp_keypair_free(&ctx_verify);
		mbedtls_ecp_keypair_free(&st_ecp_public_key);
	} else {
		/*NOT IMPLEMENTED */
		_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
	}
}

#else
void SRV_FU_SignatureImageCheck(void)
{
	/* check pointer function */
	if (!_signature_image_cb_function) {
		return;
	}

#ifdef PRIME_NUM_REGIONS
	/* Verify if this is a right image */
	if (_verify_image()) {
		_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
	} else {
		_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
	}
#endif
}

#endif /* HAL_FU_ENABLE_SIGNATURE */

/**
 * \brief Set handler to signature result callback
 *
 * \param p_handler   Signature callback function pointer
 *
 */
void SRV_FU_SignatureImageCheckSetCallback(void (*p_handler)(hal_fu_verif_result_t uc_result))
{
	_signature_image_cb_function = p_handler;
}

/**
 * \brief Set handler to Firmware upgrade result callback
 *
 * \param p_handler      FU result callback function pointer
 *
 */
void SRV_FU_SetCallback(void (*p_handler)(hal_fu_result_t *uc_result))
{
	_fu_result_cb_function = p_handler;

}

/**
 * \brief Get bitmap with the information about the status of each page.
 *
 * \param puc_bitmap             Pointer to bitmap information
 * \param pus_num_rcv_pages      Pointer to number of pages received
 *
 * \return Size of bitmap. (Maximum value 1024 bytes). In case to return 0,
 * the buffer will be initialized internally.
 */
uint16_t SRV_FU_GetBitmap(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages)
{
	UNUSED(puc_bitmap);
	UNUSED(pus_num_rcv_pages);

	return 0;
}

/**
 * \brief Configure FU PRIME regions
 *
 * \param uc_num_regs       Number of PRIME regions
 * \param px_fu_reg         Pointer to region configurations
 *
 */
void SRV_FU_ConfigRegions(uint8_t uc_num_regs, x_fu_region_cfg_t *px_fu_reg)
{
	(void)uc_num_regs;

	spx_fu_region_cfg = px_fu_reg;
}

#ifdef PRIME_NUM_REGIONS
/**
 * \brief Swap FW
 *
 * \return 1 if swapped, 0 otherwise
 *
 */
uint8_t SRV_FU_Swap(void)
{
	x_fu_region_cfg_t x_exec_region_cfg;

	/* Check if the current stack is 1.3 */
	x_prime_mode_info_cfg_t x_board_info;
	hal_get_config_info(CONFIG_TYPE_MODE_PRIME, sizeof(x_board_info), (void *)&x_board_info);
	if (x_board_info.prime_version == PRIME_1_3) {
		/* Verify if this is a right image */
		if (_verify_image() == false) {
			/* Trigger reset, needed in FU 1.3 */
			return 1;
		}
	}

	/* Get image identifier */
	suc_app_to_fu = (uint8_t)gpbr_read(GPBR4);

	/* Check which app must be swapped */
	switch (suc_app_to_fu) {
	case PRIME_MAC13_APP:
		x_exec_region_cfg.ul_address = PRIME_MAC13_FLASH_LOCATION;
		x_exec_region_cfg.ul_size = PRIME_MAC13_SIZE;
		break;
	case PRIME_MAC14_APP:
		x_exec_region_cfg.ul_address = PRIME_MAC14_FLASH_LOCATION;
		x_exec_region_cfg.ul_size = PRIME_MAC14_SIZE;
		break;
#ifdef HAL_ATPL360_INTERFACE
	case PRIME_PHY_APP:
		x_exec_region_cfg.ul_address = PRIME_PHY_FLASH_LOCATION;
		x_exec_region_cfg.ul_size = PRIME_PHY_SIZE;
		break;
#endif
	case PRIME_MAIN_APP:
		x_exec_region_cfg.ul_address = PRIME_APP_FLASH_LOCATION;
		x_exec_region_cfg.ul_size = PRIME_APP_SIZE;
		break;
	case PRIME_INVALID_APP:
	default:
		return 0;
	}

	/* Update boot configuration */
	x_boot_info_cfg_t x_boot_cfg;
	hal_get_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	x_boot_cfg.ul_orig_addr = spx_fu_region_cfg[suc_region_to_fu].ul_address;
	x_boot_cfg.ul_dest_addr = x_exec_region_cfg.ul_address;
	x_boot_cfg.ul_img_size = x_exec_region_cfg.ul_size;
	/* Store the new boot configuration */
	hal_set_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	/* Store the image identifier */
	gpbr_write(GPBR4, (uint32_t)suc_app_to_fu);

	return 1;
}
#endif

void SRV_FU_Task(void)
{
	/* Erase used sectors */
	if(deleteActive) {

	    flash_erase_page(sul_flash, IFLASH_ERASE_PAGES_16);

		sul_flash += HAL_FLASH_16PAGE_SIZE; /* 16 pages */

		if (sul_flash >= sul_flash_end) {
			deleteActive = false;
		}
	}
}

/**
 * \brief Initialize module
 *
 */
void SRV_FU_Initialize(void)
{
	_signature_image_cb_function = NULL;
	_crc_cb_function = NULL;
	_fu_result_cb_function = NULL;

	deleteActive = false;
}

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

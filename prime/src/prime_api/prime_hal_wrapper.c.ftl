/*******************************************************************************
  PRIME Hardware Abstraction Layer Wrapper API Source
   
  Company:
    Microchip Technology Inc.

  File Name:
    prime_hal_wrapper.c

  Summary:
    PRIME Hardware Abstraction Layer Wrapper API Source File

  Description:
    This module contains the implementation of the API wrapper to be used by the
    PRIME stack when accessing the services connected to the hardware.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include "prime_hal_wrapper.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables
// *****************************************************************************
// *****************************************************************************

static HAL_API *halApi;

// *****************************************************************************
// *****************************************************************************
// Section: PRIME HAL Wrapper Interface Routines
// *****************************************************************************
// *****************************************************************************

void PRIME_HAL_WRP_Configure(HAL_API *halApi)
{
	halApi = hal_api_ptr;
}

void PRIME_HAL_WRP_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType)
{
	halApi->restart_system(resetType);
}

uint32_t PRIME_HAL_WRAPPER_PcrcCalculate((uint8_t *pData, size_t length,
    PCRC_HEADER_TYPE hdrType, PCRC_CRC_TYPE crcType, uint32_t initValue, 
    bool v14Mode)
{
	return halApi->pcrc_calc(pData, length, hdrType, crcType, initValue, v14Mode);
}
    
void PRIME_HAL_WRP_PcrcConfigureSNA(uint8_t *sna)
{
	halApi->pcrc_config_sna(sna);
}

bool PRIME_HAL_WRP_GetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, 
    void* pData)
{
	return halApi->get_config_info(infoType, size, pData);
}

/* @} */

<#if primePal.PRIME_PAL_PLC_EN == true>
/** \brief PLC interface */
/* @{ */
void prime_hal_plc_init(void)
{
	return halApi->set_config_info(infoType, size, pData);
}

SRV_USI_HANDLE PRIME_HAL_WRP_UsiOpen(const SYS_MODULE_INDEX index)
{
    return halApi->usi_open(index);
}

void PRIME_HAL_WRP_UsiSetCallback(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, 
    SRV_USI_CALLBACK callback)
{
    halApi->usi_set_callback(handle, protocol, callback);
}

void PRIME_HAL_WRP_UsiSend(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, 
    uint8_t *data, size_t length)
{
	halApi->usi_send(handle, protocol, data, length);
}

void PRIME_HAL_WRP_DebugReport(SRV_LOG_REPORT_LEVEL logLevel, 
    SRV_LOG_REPORT_CODE code, const char *info, ...)
{
	halApi->debug_report(logLevel, code, info);
}

bool prime_hal_plc_send_boot_cmd(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read)
{
	halApi->pib_get_request(pibAttrib);
}

void PRIME_HAL_WRP_PIBGetRequestSetCallback(USER_PIB_GET_REQUEST_CALLBACK callback)
{
	halApi->pib_get_request_set_callback(callback);
}

void PRIME_HAL_WRP_PibSetRequest(uint16_t pibAttrib, void *pibValue, 
    uint8_t pibSize)
{
	halApi->pib_set_request(pibAttrib, pibValue, pibSize);
}

void PRIME_HAL_WRP_PIBSetRequestSetCallback(USER_PIB_SET_REQUEST_CALLBACK callback)
{
	halApi->pib_set_request_set_callback(callback);
}

uint32_t PRIME_HAL_WRP_RngGet(void)
{
	return halApi->rng_get();
}

int32_t PRIME_HAL_WRP_AesCmacDirect(uint8_t *input, uint32_t inputLen, 
    uint8_t *outputMac, uint8_t *key)
{
    return halApi->aes_cmac_direct(input, inputLen, outputMac, key);
}

int32_t PRIME_HAL_WRP_AesCcmSetkey(uint8_t *key)
{
    return halApi->aes_ccm_set_key(key);
}

int32_t PRIME_HAL_WRP_AesCcmEncryptAndTag(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, uint8_t *tag, 
    uint32_t tagLen)
{
    return halApi->aes_ccm_encrypt_tag(data, dataLen, iv, ivLen, aad, 
        aadLen, tag, tagLen);
}

int32_t PRIME_HAL_WRP_AesCcmAuthDecrypt(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, 
    uint8_t *tag, uint32_t tagLen)
{
    return halApi->aes_ccm_auth_decrypt(data, dataLen, iv, ivLen, aad, 
        aadLen,  tag, tagLen);
}

void PRIME_HAL_WRP_AesWrapKey(const uint8_t *key, uint32_t keyLen, 
    const uint8_t *in, uint32_t inLen, uint8_t *out)
{
    halApi->aes_wrap_key(key, keyLen, in, inLen, out);
}

bool PRIME_HAL_WRP_AesUnwrapKey(const uint8_t *key, uint32_t keyLen, 
    const uint8_t *in, uint32_t inLen, uint8_t *out)
{
    return halApi->aes_unwrap_key(key, keyLen, in, inLen, out);
}



/** \brief Firmware Upgrade Interface */
/* @{ */
void prime_hal_fu_data_read(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
	halApi->fu_data_read(addr, puc_buf, us_size);
}

uint8_t prime_hal_fu_data_write(uint32_t addr, uint8_t *puc_buf, uint16_t us_size)
{
	return halApi->fu_data_write(addr, puc_buf, us_size);
}

void prime_hal_fu_data_cfg_read(void *pv_dst, uint16_t us_size)
{
	halApi->fu_data_cfg_read(pv_dst, us_size);
}

uint8_t prime_hal_fu_data_cfg_write(void *pv_src, uint16_t us_size)
{
	return halApi->fu_data_cfg_write(pv_src, us_size);
}

void prime_hal_fu_start(hal_fu_info_t *x_fu_info)
{
	halApi->fu_start(x_fu_info);
}

void prime_hal_fu_end(hal_fu_result_t uc_hal_res)
{
	halApi->fu_end(uc_hal_res);
}

void prime_hal_fu_revert(void)
{
	halApi->fu_revert();
}

void prime_hal_fu_crc_calculate(void)
{
	halApi->fu_crc_calculate();
}

void prime_hal_fu_crc_set_callback(void (*p_handler)(uint32_t ul_crc))
{
	halApi->fu_crc_set_callback(p_handler);
}

uint16_t prime_hal_fu_get_bitmap(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages)
{
	return halApi->fu_get_bitmap(puc_bitmap, pus_num_rcv_pages);
}

void prime_hal_fu_signature_image_check(void)
{
	halApi->fu_signature_image_check();
}

void prime_hal_fu_signature_image_check_set_callback(void (*p_handler)(hal_fu_verif_result_t uc_result))
{
	halApi->fu_signature_image_check_set_callback(p_handler);
}

void prime_hal_swap_stack(uint8_t uc_traffic)
{
	halApi->swap_stack(uc_traffic);
}

/** \brief Timer 1us Interface */
/* @{ */
uint32_t prime_hal_timer_1us_get(void)
{
	return halApi->timer_1us_get();
}

void prime_hal_timer_1us_enable_interrupt(bool b_enable)
{
	halApi->timer_1us_enable_interrupt(b_enable);
}

bool prime_hal_timer_1us_set_int(uint32_t ul_time_us, bool b_relative, void (*p_handler)(uint32_t), uint32_t *pul_int_id)
{
	return halApi->timer_1us_set_int(ul_time_us, b_relative, p_handler, pul_int_id);
}

bool prime_hal_timer_1us_cancel_int(uint32_t ul_int_id)
{
	return halApi->timer_1us_cancel_int(ul_int_id);
}
/* @} */

<#if primePal.PRIME_PAL_RF_EN == true>
/** \brief RF Interface */
/* @{ */
uint8_t prime_hal_prf_if_init(void)
{
	return p_hal_api->prf_if_init();
}

void prime_hal_prf_if_reset(void)
{
	p_hal_api->prf_if_reset();
}

void prime_hal_prf_if_enable_interrupt(bool b_enable)
{
	p_hal_api->prf_if_enable_interrupt(b_enable);
}

void prime_hal_prf_if_set_handler(void (*p_handler)(void))
{
	p_hal_api->prf_if_set_handler(p_handler);
}

bool prime_hal_prf_if_send_spi_cmd(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode)
{
	return p_hal_api->prf_if_send_spi_cmd(puc_data_buf, us_addr, us_len, uc_mode);
}

bool prime_hal_prf_if_is_spi_busy(void)
{
	return p_hal_api->prf_if_is_spi_busy();
}

void prime_hal_prf_if_led(uint8_t uc_led_id, bool b_led_on)
{
	p_hal_api->prf_if_led(uc_led_id, b_led_on);
}
</#if>

	/* @} */


/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */

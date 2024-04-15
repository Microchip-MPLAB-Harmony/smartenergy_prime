/*******************************************************************************
  PRIME Hardware Abstraction Layer API Header 
   
  Company:
    Microchip Technology Inc.

  File Name:
    hal_api.h

  Summary:
    PRIME Hardware Abstraction Layer API Header File

  Description:
    This module contains configuration and utils for the interface between the
    services connected to the hardware and the PRIME stack.
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

#ifndef HAL_API_H_INCLUDE
#define HAL_API_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "service/storage/srv_storage.h"
#include "service/user_pib/srv_user_pib.h"
#include "service/reset_handler/srv_reset_handler.h"
#include "service/pcrc/srv_pcrc.h"
#include "service/random/srv_random.h"
#include "service/log_report/srv_log_report.h"
#include "service/usi/srv_usi.h"
#include "service/security/aes_wrapper.h"
#include "service/security/cipher_wrapper.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Restart system

  Summary:
    Function pointer to request a system restart.

  Description:
    This function pointer is used to request a system restart.

  Remarks:
    Related to Reset Handler service.
*/
typedef void (*HAL_RESTART_SYSTEM)(SRV_RESET_HANDLER_RESET_CAUSE resetType);

// *****************************************************************************
/* Calculate CRC

  Summary:
    Function pointer to request the calculation of a CRC.

  Description:
    This function pointer is used to request the calculation of a CRC.

  Remarks:
    Related to PCRC service.
*/
typedef uint32_t (*HAL_PCRC_CALCULATE)(uint8_t *pData, size_t length,
    PCRC_HEADER_TYPE hdrType, PCRC_CRC_TYPE crcType, uint32_t initValue, 
    bool v14Mode);

// *****************************************************************************
/* Configure subnetwork address for CRC calculation

  Summary:
    Function pointer to set the subnetwork address for the calculation of a CRC.

  Description:
    This function pointer is used to set the subnetwork address for the 
    calculation of a CRC.

  Remarks:
    Related to PCRC service.
*/
typedef void (*HAL_PCRC_CONFIGURE_SNA)(uint8_t *sna);

// *****************************************************************************
/* Get configuration information

  Summary:
    Function pointer to get configuration information stored externally.

  Description:
    This function pointer is used to get configuration information stored 
    externally.

  Remarks:
    Related to PRIME Storage service.
*/
typedef bool (*HAL_GET_CONFIG_INFO)(SRV_STORAGE_TYPE infoType, uint8_t size, 
    void *pData);

// *****************************************************************************
/* Set configuration information

  Summary:
    Function pointer to set configuration information stored externally.

  Description:
    This function pointer is used to set configuration information stored 
    externally.

  Remarks:
    Related to PRIME Storage service.
*/
typedef bool (*HAL_SET_CONFIG_INFO)(SRV_STORAGE_TYPE infoType, uint8_t size, 
    void *pData);

// *****************************************************************************
/* Open an USI instance

  Summary:
    Function pointer to request to open an USI instance.

  Description:
    This function pointer is used to request to open an USI instance.

  Remarks:
    Related to USI service.
*/
typedef SRV_USI_HANDLE (*HAL_USI_OPEN)(const SYS_MODULE_INDEX index);

// *****************************************************************************
/* Register USI callback function

  Summary:
    Function pointer to request the registration of a USI callback function.

  Description:
    This function pointer is used to request the registration of a USI callback 
    function.

  Remarks:
    Related to USI service.
*/
typedef void (*HAL_USI_SET_CALLBACK)(SRV_USI_HANDLE handle, 
    SRV_USI_PROTOCOL_ID protocol, SRV_USI_CALLBACK callback);

// *****************************************************************************
/* Request to send a USI message

  Summary:
    Function pointer to request the sending of a USI message.

  Description:
    This function pointer is used to request the sending of a USI message.

  Remarks:
    Related to USI service.
*/
typedef void (*HAL_USI_SEND)(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, 
    uint8_t *data, size_t length);

// *****************************************************************************
/* Report an error for the debug log

  Summary:
    Function pointer to request the reporting of an error for the debug log.

  Description:
    This function pointer is used to request the reporting of an error for the 
    debug log.

  Remarks:
    Related to Log Report service.
*/
typedef void (*HAL_DEBUG_REPORT)(SRV_LOG_REPORT_LEVEL logLevel, 
    SRV_LOG_REPORT_CODE code, const char *info, ...);

// *****************************************************************************
/* Get a user PIB attribute

  Summary:
    Function pointer to request a user PIB attribute.

  Description:
    This function pointer is used to request a user PIB attribute.

  Remarks:
    Related to PRIME User PIB service.
*/
typedef void (*HAL_PIB_GET_REQUEST)(uint16_t pibAttrib);

// *****************************************************************************
/* Register a callback function to get a user PIB attribute

  Summary:
    Function pointer to request the registration of the callback function to get 
    a user PIB attribute.

  Description:
    This function pointer is used to request the registration of the callback 
    function to get a user PIB attribute.

  Remarks:
    Related to PRIME User PIB service.
*/
typedef void (*HAL_PIB_GET_REQUEST_SET_CALLBACK)(SRV_USER_PIB_GET_REQUEST_CALLBACK callback);

// *****************************************************************************
/* Set a user PIB attribute

  Summary:
    Function pointer to set a user PIB attribute.

  Description:
    This function pointer is used to set a user PIB attribute.

  Remarks:
    Related to PRIME User PIB service.
*/
typedef void (*HAL_PIB_SET_REQUEST)(uint16_t pibAttrib, void *pibValue, uint8_t pibSize);

// *****************************************************************************
/* Register a callback function to set a user PIB attribute

  Summary:
    Function pointer to request the registration of the callback function to set 
    a user PIB attribute.

  Description:
    This function pointer is used to request the registration of the callback 
    function to set a user PIB attribute.

  Remarks:
    Related to PRIME User PIB service.
*/
typedef void (*HAL_PIB_SET_REQUEST_SET_CALLBACK)(SRV_USER_PIB_SET_REQUEST_CALLBACK callback);

// *****************************************************************************
/* Get a random number

  Summary:
    Function pointer to request a random number.

  Description:
    This function pointer is used to request a random number.

  Remarks:
    Related to Random service.
*/
typedef uint32_t (*HAL_RNG_GET)(void);

// *****************************************************************************
/*  Perform AES-CMAC

  Summary:
    Function pointer to perform AES-CMAC to generate the MAC in single step 
    without initialization.

  Description:
    This function pointer is used to perform AES-CMAC to generate the MAC in 
    single step without initialization.

  Remarks:
    Related to Security service.
*/
typedef int32_t (*HAL_AES_CMAC_DIRECT)(uint8_t *input, uint32_t inputLen, 
    uint8_t *outputMac, uint8_t *key);

// *****************************************************************************
/*  Set the encryption key for AES-CCM

  Summary:
    Function pointer to initialize the AES-CCM context and set the encryption 
    key.

  Description:
    This function pointer is used to initialize the AES-CCM context and set the 
    16-byte encryption key.

  Remarks:
    Related to Security service.
*/
typedef int32_t (*HAL_AES_CCM_SET_KEY)(uint8_t *key);

// *****************************************************************************
/*  Perform AES-CCM authenticated encryption of a buffer

  Summary:
    Function pointer to perform AES-CCM authenticated encryption of a buffer.

  Description:
    This function pointer is used to perform AES-CCM authenticated encryption 
    of a buffer.

  Remarks:
    Related to Security service.
*/
typedef int32_t (*HAL_AES_CCM_ENCRYPT_TAG)(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, uint8_t *tag, 
    uint32_t tagLen);

// *****************************************************************************
/*  Perform AES-CCM authenticated decryption of a buffer

  Summary:
    Function pointer to perform AES-CCM authenticated decryption of a buffer.

  Description:
    This function pointer is used to perform AES-CCM authenticated decryption 
    of a buffer.

  Remarks:
    Related to Security service.
*/
typedef int32_t (*HAL_AES_CCM_AUTH_DECRYPT)(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, uint8_t *tag, 
    uint32_t tagLen);

// *****************************************************************************
/*  Wrap a key with AES Key Wrap Algorithm

  Summary:
    Function pointer to wrap a key with AES Key Wrap Algorithm.

  Description:
    This function pointer is used to wrap a key using AES Key Wrap Algorithm.

  Remarks:
    Related to Security service.
*/
typedef void (*HAL_AES_WRAP_KEY)(const uint8_t *key, uint32_t keyLen, 
    const uint8_t *in, uint32_t inLen, uint8_t *out);

// *****************************************************************************
/*  Unwrap a key with AES Key Wrap Algorithm

  Summary:
    Function pointer to unwrap a key with AES Key Wrap Algorithm.

  Description:
    This function pointer is used to unwrap a key using AES Key Wrap Algorithm.

  Remarks:
    Related to Security service.
*/
typedef bool (*HAL_AES_UNWRAP_KEY)(const uint8_t *key, uint32_t keyLen, const uint8_t *in, 
    uint32_t inLen, uint8_t *out);


// Related to FU service - TBD
typedef void (*hal_fu_data_read_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef uint8_t (*hal_fu_data_write_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
typedef void (*hal_fu_data_cfg_read_t)(void *pv_dst, uint16_t us_size);
typedef uint8_t (*hal_fu_data_cfg_write_t)(void *pv_src, uint16_t us_size);
typedef void (*hal_fu_start_t)(hal_fu_info_t *x_fu_info);
typedef void (*hal_fu_end_t)(hal_fu_result_t uc_hal_res);
typedef void (*hal_fu_revert_t)(void);
typedef void (*hal_fu_crc_calculate_t)(void);
typedef void (*hal_fu_crc_set_callback_t)(void (*p_handler)(uint32_t ul_crc));
typedef void (*hal_fu_signature_image_check_t)(void);
typedef void (*hal_fu_signature_image_check_set_callback_t)(void (*p_handler)(hal_fu_verif_result_t uc_result));
typedef uint16_t (*hal_fu_get_bitmap_t)(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages);
typedef void (*hal_swap_stack_t)(uint8_t uc_traffic);

// Related to PAL - TBD

<#if primePal.PRIME_PAL_PLC_EN == true>
typedef bool (*hal_plc_send_boot_cmd_t)(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read);
typedef bool (*hal_plc_send_wrrd_cmd_t)(uint8_t uc_cmd, void *px_spi_data, void *px_spi_status_info);
typedef void (*hal_plc_enable_interrupt_t)(bool enable);
typedef void (*hal_plc_delay_t)(uint8_t uc_tref, uint32_t ul_delay);
typedef bool (*hal_plc_set_stby_mode_t)(bool sleep);
typedef bool (*hal_plc_get_thermal_warning_t)(void);
</#if>


// Related to Time Management - TBD
/* Timer of 1us service interface */
typedef uint32_t (*hal_timer_1us_get_t)(void);
typedef bool (*hal_timer_1us_set_int_t)(uint32_t ul_time_us, bool b_relative, void (*p_handler)(uint32_t), uint32_t *pul_int_id);
typedef bool (*hal_timer_1us_cancel_int_t)(uint32_t ul_int_id);
typedef void (*hal_timer_1us_enable_interrupt_t)(bool b_enable);

<#if primePal.PRIME_PAL_RF_EN == true>
typedef uint8_t (*hal_prf_if_init_t)(void);
typedef void (*hal_prf_if_reset_t)(void);
typedef void (*hal_prf_if_enable_interrupt_t)(bool b_enable);
typedef void (*hal_prf_if_set_handler_t)(void (*p_handler)(void));
typedef bool (*hal_prf_if_send_spi_cmd_t)(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode);
typedef bool (*hal_prf_if_is_spi_busy_t)(void);
typedef void (*hal_prf_if_led_t)(uint8_t uc_led_id, bool b_led_on);
</#if>

// *****************************************************************************
/* HAL API functions structure

  Summary:
    Structure with HAL functions for the API.

  Description:
    This structure defines the list of available functions in the HAL API.

  Remarks:
    None.
*/
typedef struct {
	HAL_RESTART_SYSTEM restart_system;

	HAL_PCRC_CALCULATE pcrc_calc;
	HAL_PCRC_CONFIGURE_SNA pcrc_config_sna;
    
    HAL_GET_CONFIG_INFO get_config_info;
	HAL_SET_CONFIG_INFO set_config_info;
    
    HAL_USI_OPEN usi_open;
	HAL_USI_SET_CALLBACK usi_set_callback;
	HAL_USI_SEND usi_send;
    
    HAL_DEBUG_REPORT debug_report;
    
    HAL_PIB_GET_REQUEST pib_get_request;
	HAL_PIB_GET_REQUEST_SET_CALLBACK pib_get_request_set_callback;
	HAL_PIB_SET_REQUEST pib_set_request;
	HAL_PIB_SET_REQUEST_SET_CALLBACK pib_set_request_set_callback;
    
    HAL_RNG_GET rng_get;
    
    HAL_AES_CMAC_DIRECT aes_cmac_direct;
    HAL_AES_CCM_SET_KEY aes_ccm_set_key;
    HAL_AES_CCM_ENCRYPT_TAG aes_ccm_encrypt_tag;
    HAL_AES_CCM_AUTH_DECRYPT aes_ccm_auth_decrypt;
    HAL_AES_WRAP_KEY aes_wrap_key;
    HAL_AES_UNWRAP_KEY aes_unwrap_key;
   
	hal_fu_data_read_t fu_data_read;
	hal_fu_data_write_t fu_data_write;
	hal_fu_data_cfg_read_t fu_data_cfg_read;
	hal_fu_data_cfg_write_t fu_data_cfg_write;
	hal_fu_start_t fu_start;
	hal_fu_end_t fu_end;
	hal_fu_revert_t fu_revert;
	hal_fu_crc_calculate_t fu_crc_calculate;
	hal_fu_crc_set_callback_t fu_crc_set_callback;
	hal_fu_signature_image_check_t fu_signature_image_check;
	hal_fu_signature_image_check_set_callback_t fu_signature_image_check_set_callback;
	hal_fu_get_bitmap_t fu_get_bitmap;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_init_t plc_init;
	hal_plc_reset_t plc_reset;
	hal_plc_set_handler_t plc_set_handler;
	hal_plc_tx_signal_t plc_tx_signal;
	hal_plc_rx_signal_t plc_rx_signal;
</#if>

	hal_get_config_info_t get_config_info;
	hal_set_config_info_t set_config_info;

    hal_usi_open_t usi_open;
	hal_usi_set_callback_t usi_set_callback;
	hal_usi_send_cmd_t usi_send_cmd;

	hal_trng_read_t trng_read;

	hal_debug_report_t debug_report;

	hal_net_get_freq_t net_get_freq;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_send_boot_cmd_t plc_send_boot_cmd;
	hal_plc_send_wrrd_cmd_t plc_send_wrrd_cmd;
	hal_plc_enable_interrupt_t plc_enable_int;
	hal_plc_delay_t plc_delay;
</#if>

#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init_t nwk_recovery_init;
	hal_nwk_recovery_read_t nwk_recovery_read;
	hal_nwk_recovery_write_t nwk_recovery_write;
#endif

	hal_pib_get_request_t pib_get_request;
	hal_pib_get_request_set_callback_t pib_get_request_set_callback;
	hal_pib_set_request_t pib_set_request;
	hal_pib_set_request_set_callback_t pib_set_request_set_callback;

	hal_aes_init_t aes_init;
	hal_aes_set_callback_t aes_set_callback;
	hal_aes_key_t aes_key;
	hal_aes_crypt_t aes_crypt;
	hal_swap_stack_t swap_stack;

<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_set_stby_mode_t plc_set_stby_mode;
	hal_plc_get_thermal_warning_t plc_get_thermal_warning;
</#if>

	/* New functions must be added at the end */


	hal_timer_1us_get_t timer_1us_get;
	hal_timer_1us_set_int_t timer_1us_set_int;
	hal_timer_1us_cancel_int_t timer_1us_cancel_int;
	hal_timer_1us_enable_interrupt_t timer_1us_enable_interrupt;
    
    /* New functions must be added at the end */

<#if primePal.PRIME_PAL_RF_EN == true>
	hal_prf_if_init_t prf_if_init;
	hal_prf_if_reset_t prf_if_reset;
	hal_prf_if_enable_interrupt_t prf_if_enable_interrupt;
	hal_prf_if_set_handler_t prf_if_set_handler;
	hal_prf_if_send_spi_cmd_t prf_if_send_spi_cmd;
	hal_prf_if_is_spi_busy_t prf_if_is_spi_busy;
	hal_prf_if_led_t prf_if_led;
</#if>

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* HAL_API_H_INCLUDE */

/*******************************************************************************
 End of File
*/

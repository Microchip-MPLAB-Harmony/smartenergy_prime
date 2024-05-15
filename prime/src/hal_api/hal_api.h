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
#include "service/time_management/srv_time_management.h"
#include "stack/pal/pal_types.h"

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
typedef size_t (*HAL_USI_SEND)(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol,
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

// *****************************************************************************
/*  Gets the time in a 64 bit variable in microseconds


  Summary:
    Function pointer to get the value of a counter and convert it in a 64 bit
    variable in microseconds.

  Description:
    This function pointer is used to get the value of a counter and convert it
    in a 64 bit variable in microseconds.

  Remarks:
    Related to Time Management service.
*/
typedef uint64_t (*HAL_TIMER_GetTimeUS64)(void);

// *****************************************************************************
/*  Gets the time in a 32 bit variable in microseconds


  Summary:
    Function pointer to get the value of a counter and convert it in a 32 bit
    variable in microseconds.

  Description:
    This function pointer is used to get the value of a counter and convert it
    in a 32 bit variable in microseconds.

  Remarks:
    Related to Time Management service.
*/
typedef uint32_t (*HAL_TIMER_GetTimeUS)(void);

// *****************************************************************************
/* Register a callback function with the time system service

   Summary:
        Function pointer to register a function with the time system service
        to be called back when the requested number of microseconds have expired
        (either once or repeatedly).

   Description:
        Creates a timer object and registers a function with it to be called back
        when the requested delay (specified in microseconds) has completed.  The
        caller must identify if the timer should call the function once or repeatedly
        every time the given delay period expires.

   Returns:
        SYS_TIME_HANDLE - A valid timer object handle if the call succeeds.
                      SYS_TIME_HANDLE_INVALID if it fails.

*/
typedef SYS_TIME_HANDLE (*HAL_TIMER_CallbackRegisterUS)(SYS_TIME_CALLBACK callback,
                        uintptr_t context, uint32_t us, SYS_TIME_CALLBACK_TYPE type);


// Related to FU service - TBD
// typedef void (*hal_fu_data_read_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
// typedef uint8_t (*hal_fu_data_write_t)(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
// typedef void (*hal_fu_data_cfg_read_t)(void *pv_dst, uint16_t us_size);
// typedef uint8_t (*hal_fu_data_cfg_write_t)(void *pv_src, uint16_t us_size);
// typedef void (*hal_fu_start_t)(hal_fu_info_t *x_fu_info);
// typedef void (*hal_fu_end_t)(hal_fu_result_t uc_hal_res);
// typedef void (*hal_fu_revert_t)(void);
// typedef void (*hal_fu_crc_calculate_t)(void);
// typedef void (*hal_fu_crc_set_callback_t)(void (*p_handler)(uint32_t ul_crc));
// typedef void (*hal_fu_signature_image_check_t)(void);
// typedef void (*hal_fu_signature_image_check_set_callback_t)(void (*p_handler)(hal_fu_verif_result_t uc_result));
// typedef uint16_t (*hal_fu_get_bitmap_t)(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages);
// typedef void (*hal_swap_stack_t)(uint8_t uc_traffic);

// Related to PAL - TBD
typedef SYS_MODULE_OBJ (*HAL_PAL_INITIALIZE)(const SYS_MODULE_INDEX index);
typedef void (*HAL_PAL_TASKS)(SYS_MODULE_OBJ object);
typedef SYS_STATUS (*HAL_PAL_STATUS)(SYS_MODULE_OBJ object);
typedef void (*HAL_PAL_CALLBACK_REGISTER)(PAL_CALLBACKS *pCallbacks);
typedef uint8_t (*HAL_PAL_DATA_REQUEST)(PAL_MSG_REQUEST_DATA *pData);
typedef uint8_t (*HAL_PAL_GET_SNR)(uint16_t pch, uint8_t *snr, uint8_t qt);
typedef uint8_t (*HAL_PAL_GET_ZCT)(uint16_t pch, uint32_t *zct);
typedef uint8_t (*HAL_PAL_GET_TIMER)(uint16_t pch, uint32_t *timer);
typedef uint8_t (*HAL_PAL_GET_TIMER_EXTENDED)(uint16_t pch, uint64_t *timer);
typedef uint8_t (*HAL_PAL_GET_CD)(uint16_t pch, uint8_t *cd, uint8_t *rssi, uint32_t *time, uint8_t *header);
typedef uint8_t (*HAL_PAL_GET_NL)(uint16_t pch, uint8_t *noise);
typedef uint8_t (*HAL_PAL_GET_AGC)(uint16_t pch, uint8_t *mode, uint8_t *gain);
typedef uint8_t (*HAL_PAL_SET_AGC)(uint16_t pch, uint8_t mode, uint8_t gain);
typedef uint8_t (*HAL_PAL_GET_CCA)(uint16_t pch, uint8_t *pState);
typedef uint8_t (*HAL_PAL_GET_CHANNEL)(uint16_t *pPch, uint16_t channelReference);
typedef uint8_t (*HAL_PAL_SET_CHANNEL)(uint16_t pch);
typedef void (*HAL_PAL_PROGRAM_CHANNEL_SWITCH)(uint16_t pch, uint32_t timeSync, uint8_t timeMode);
typedef uint8_t (*HAL_PAL_GET_CONFIGURATION)(uint16_t pch, uint16_t id, void *val, uint16_t length);
typedef uint8_t (*HAL_PAL_SET_CONFIGURATION)(uint16_t pch, uint16_t id, void *val, uint16_t length);
typedef uint16_t (*HAL_PAL_GET_SIGNAL_CAPTURE)(uint16_t pch, uint8_t *noiseCapture, uint8_t mode, uint32_t timeStart, uint32_t duration);
typedef uint8_t (*HAL_PAL_GET_MSG_DURATION)(uint16_t pch, uint16_t length, PAL_SCHEME scheme, uint8_t mode, uint32_t *duration);
typedef bool (*HAL_PAL_CHECK_MINIMUM_QUALITY)(uint16_t pch, uint8_t reference, uint8_t modulation);
typedef uint8_t (*HAL_PAL_GET_LESS_ROBUST_MODULATION)(uint16_t pch, uint8_t mod1, uint8_t mod2);

typedef void (*TBD)(void);
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

    HAL_TIMER_GetTimeUS64 timer_get_us64;
    HAL_TIMER_GetTimeUS timer_get_us;
    HAL_TIMER_CallbackRegisterUS timer_callback_register_us;

    TBD tbd1; //hal_fu_data_read_t fu_data_read;
    TBD tbd2; //hal_fu_data_write_t fu_data_write;
    TBD tbd3; //hal_fu_data_cfg_read_t fu_data_cfg_read;
    TBD tbd4; //hal_fu_data_cfg_write_t fu_data_cfg_write;
    TBD tbd5; //hal_fu_start_t fu_start;
    TBD tbd6; //hal_fu_end_t fu_end;
    TBD tbd7; //hal_fu_revert_t fu_revert;
    TBD tbd8; //hal_fu_crc_calculate_t fu_crc_calculate;
    TBD tbd9; //hal_fu_crc_set_callback_t fu_crc_set_callback;
    TBD tbd10; //hal_fu_signature_image_check_t fu_signature_image_check;
    TBD tbd11; //hal_fu_signature_image_check_set_callback_t fu_signature_image_check_set_callback;
    TBD tbd12; //hal_fu_get_bitmap_t fu_get_bitmap;

    TBD tbd13; //hal_net_get_freq_t net_get_freq;

    TBD tbd14; //hal_nwk_recovery_init_t nwk_recovery_init;
    TBD tbd15; //hal_nwk_recovery_read_t nwk_recovery_read;
    TBD tbd16; //hal_nwk_recovery_write_t nwk_recovery_write;

    HAL_PAL_INITIALIZE hal_pal_initialize;
    HAL_PAL_TASKS hal_pal_tasks;
    HAL_PAL_STATUS hal_pal_status;
    HAL_PAL_CALLBACK_REGISTER hal_pal_callback_register;
    HAL_PAL_DATA_REQUEST hal_pal_data_request;
    HAL_PAL_GET_SNR hal_pal_get_snr;
    HAL_PAL_GET_ZCT hal_pal_get_zct;
    HAL_PAL_GET_TIMER hal_pal_get_timer;
    HAL_PAL_GET_TIMER_EXTENDED hal_pal_get_timer_extended;
    HAL_PAL_GET_CD hal_pal_get_cd;
    HAL_PAL_GET_NL hal_pal_get_nl;
    HAL_PAL_GET_AGC hal_pal_get_agc;
    HAL_PAL_SET_AGC hal_pal_set_agc;
    HAL_PAL_GET_CCA hal_pal_get_cca;
    HAL_PAL_GET_CHANNEL hal_pal_get_channel;
    HAL_PAL_SET_CHANNEL hal_pal_set_channel;
    HAL_PAL_PROGRAM_CHANNEL_SWITCH hal_pal_program_channel_switch;
    HAL_PAL_GET_CONFIGURATION hal_pal_get_configuration;
    HAL_PAL_SET_CONFIGURATION hal_pal_set_configuration;
    HAL_PAL_GET_SIGNAL_CAPTURE hal_pal_get_signal_capture ;
    HAL_PAL_GET_MSG_DURATION hal_pal_get_msg_duration;
    HAL_PAL_CHECK_MINIMUM_QUALITY hal_pal_check_minimum_quality;
    HAL_PAL_GET_LESS_ROBUST_MODULATION hal_pal_get_less_robust_modulation;

    /* New functions must be added at the end */

} HAL_API;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* HAL_API_H_INCLUDE */

/*******************************************************************************
 End of File
*/

/*******************************************************************************
  PRIME Hardware Abstraction Layer API Source 
   
  Company:
    Microchip Technology Inc.

  File Name:
    hal_api.c

  Summary:
    PRIME Hardware Abstraction Layer API Source File

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include "hal_api.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* HAL API functions 

  Summary:
    HAL API functions.

  Description:
    This structure contains the list of available functions in the HAL API.

  Remarks:
    The functions in this structure correspond to the required services by the 
    PRIME stack.
 */
 
const HAL_API hal_api = {
	SRV_RESET_HANDLER_RestartSystem,
    
	SRV_PCRC_GetValue,
	SRV_PCRC_ConfigureSNA,
    
    SRV_STORAGE_GetConfigInfo,
	SRV_STORAGE_SetConfigInfo,
    
    SRV_USI_Open,
	SRV_USI_CallbackRegister,
	SRV_USI_Send_Message,
    
    SRV_LOG_REPORT_Message_With_Code,
    
    SRV_USER_PIB_GetRequest,
	SRV_USER_PIB_GetRequestCallbackRegister,
	SRV_USER_PIB_SetRequest,
	SRV_USER_PIB_SetRequestCallbackRegister,
    
    SRV_RANDOM_Get32bits,
    
    CIPHER_Wrapper_AesCmacDirect,
    CIPHER_Wrapper_AesCcmSetkey,
    CIPHER_Wrapper_AesCcmEncryptAndTag,
    CIPHER_Wrapper_AesCcmAuthDecrypt,
    AES_Wrapper_WrapKey,
    AES_Wrapper_UnwrapKey
    
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
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_init,
	hal_plc_reset,
	hal_plc_set_handler,
	hal_plc_tx_signal,
	hal_plc_rx_signal,
</#if>
	SRV_STORAGE_GetConfigInfo,
	SRV_STORAGE_SetConfigInfo,
    SRV_USI_Open,
	SRV_USI_CallbackRegister,
	SRV_USI_Send_Message,
	SRV_RANDOM_Get32bits,
	SRV_LOG_REPORT_Message_With_Code,
	hal_net_get_freq,
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_send_boot_cmd,
	hal_plc_send_wrrd_cmd,
	hal_plc_enable_interrupt,
	hal_plc_delay,
</#if>
#ifdef HAL_NWK_RECOVERY_INTERFACE
	hal_nwk_recovery_init,
	hal_nwk_recovery_read,
	hal_nwk_recovery_write,
#endif
	SRV_USER_PIB_GetRequest,
	SRV_USER_PIB_GetRequestCallbackRegister,
	SRV_USER_PIB_SetRequest,
	SRV_USER_PIB_SetRequestCallbackRegister,
	hal_aes_init,
	hal_aes_set_callback,
	hal_aes_key,
	hal_aes_crypt,
	hal_swap_stack,
<#if primePal.PRIME_PAL_PLC_EN == true>
	hal_plc_set_stby_mode,
	hal_plc_get_thermal_warning,
</#if>
	timer_1us_get,
	timer_1us_set_int,
	timer_1us_cancel_int,
	timer_1us_enable_interrupt,
<#if primePal.PRIME_PAL_RF_EN == true>
	prf_if_init,
	prf_if_reset,
	prf_if_enable_interrupt,
	prf_if_set_handler,
	prf_if_send_spi_cmd,
	prf_if_is_spi_busy,
	prf_if_led,
</#if>
};

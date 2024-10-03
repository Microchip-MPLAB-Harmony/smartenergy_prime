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

/*******************************************************************************
  PRIME Reset Handler Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_firmware_upgrade.c

  Summary:
    PRIME Firmware Upgrade Service Interface Header File.

  Description:
    The Firmware Upgrade service provides the handling of the firmare upgrade 
    and version swap for PRIME. This file provides the interface definition for 
    this service.
*******************************************************************************/

///DOM-IGNORE-BEGIN
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


#include <string.h>

#include "definitions.h"
#include "srv_firmware_upgrade.h"
#include "srv_firmware_upgrade_local.h"
#include "device.h"

#include "driver/memory/drv_memory.h"
#include "service/pcrc/srv_pcrc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

/* Callback function pointers */
static SRV_FU_CRC_CB SRV_FU_CrcCallback;
static SRV_FU_IMAGE_VERIFY_CB SRV_FU_ImageVerifyCallback;
static SRV_FU_RESULT_CB SRV_FU_ResultCallback;
static SRV_FU_VERSION_SWAP_CB SRV_FU_SwapCallback;
static SRV_FU_MEM_TRANSFER_CB SRV_FU_MemTransferCallback;

// /** Internal var to manage memory address in FU process */
// static uint32_t sul_fw_mem_address;

/* 
Changes to get from MCC
*/
#define PRIME_FU_MEM_DRV
#define PRIME_FU_MEM_INSTANCE
#define PRIME_FU_MEM_OFFSET     0x0000
#define PRIME_FU_MEM_SIZE       0x80000

#define MEMORY_WRITE_SIZE       0x100
#define MAX_BUFFER_READ_SIZE    0x100
    


SYS_MEDIA_GEOMETRY *nvmGeometry = NULL;

/* NVM Data buffer */
static CACHE_ALIGN uint8_t pMemWrite[MEMORY_WRITE_SIZE];

static CACHE_ALIGN uint8_t pBuffInput[MAX_BUFFER_READ_SIZE];

static CACHE_ALIGN SRV_FU_MEM_INFO memInfo;

static SRV_FU_INFO fuData;

static SRV_FU_CRC_STATE crcState;

static uint32_t crcReadAddress;

static uint32_t crcSize;

static uint32_t crcRemainSize;

static uint32_t calculatedCrc;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void lSRV_FU_TransferHandler
(
    DRV_MEMORY_EVENT event,
    DRV_MEMORY_COMMAND_HANDLE commandHandle,
    uintptr_t context
)
{
    SRV_FU_MEM_TRANSFER_RESULT transferResult;
    SRV_FU_MEM_TRANSFER_CMD transferCmd;
    SRV_FU_MEM_INFO *mInfo = (SRV_FU_MEM_INFO *)context;

    switch(event)
    {
        case DRV_MEMORY_EVENT_COMMAND_COMPLETE:
            mInfo->state = SRV_FU_MEM_STATE_CMD_WAIT;
            transferResult = SRV_FU_MEM_TRANSFER_OK;
            break;

        case DRV_MEMORY_EVENT_COMMAND_ERROR:
        default:
            mInfo->state = SRV_FU_MEM_STATE_ERROR;
            transferResult = SRV_FU_MEM_TRANSFER_ERROR;
            break;
    }

    if (commandHandle == mInfo->eraseHandle) 
    {
        transferCmd = SRV_FU_MEM_TRANSFER_CMD_ERASE;
    }
    else if (commandHandle == mInfo->readHandle) 
    {
        if (memInfo.state == SRV_FU_CALCULATE_CRC_BLOCK)
        {
            /* Calculating CRC.... no callback*/
            crcState = SRC_FU_CRC_CALCULATING;
            return;
        }
        else
        {
            transferCmd = SRV_FU_MEM_TRANSFER_CMD_READ;
        }
    }
    else if (commandHandle == mInfo->writeHandle)
    {
        if (transferResult == SRV_FU_MEM_TRANSFER_OK)
        {
            /* Continue with next transfer, not callback */
            memInfo.state = SRV_FU_MEM_STATE_WRITE_ONE_BLOCK;
            return;
        }
        else
        {
            transferCmd = SRV_FU_MEM_TRANSFER_CMD_WRITE;
        }
    }
    else
    {
        transferCmd = SRV_FU_MEM_TRANSFER_CMD_BAD;
    }

    if (SRV_FU_MemTransferCallback != NULL) 
    {
        SRV_FU_MemTransferCallback(transferCmd, transferResult);
    }   
}


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_NVM_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Function Declarations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
void lSRV_FU_EraseFuRegion(void)
{
    
    DRV_MEMORY_AsyncErase(memInfo.memoryHandle, &memInfo.eraseHandle, memInfo.eraseBlockStart, memInfo.numFuRegionEraseBlocks);

	memInfo.state = SRV_FU_MEM_STATE_ERASE_FLASH;
}

// *****************************************************************************
// *****************************************************************************
// Section: Reset Handler Service Interface Implementation
// *****************************************************************************
// *****************************************************************************
//void SRV_FU_ConfigRegions(SRV_FU_REGION_CGF *fuRegion);
// bool SRV_FU_SwapFirmware(void);
// void SRV_FU_VerifyImage(void);

void SRV_FU_Initialize(void)
{
	SRV_FU_CrcCallback = NULL;
	SRV_FU_ImageVerifyCallback = NULL;
	SRV_FU_ResultCallback = NULL;
	SRV_FU_SwapCallback = NULL;
    SRV_FU_MemTransferCallback = NULL;

	
    memInfo.iniFuRegion = PRIME_FU_MEM_OFFSET;
    memInfo.sizeFuRegion = PRIME_FU_MEM_SIZE;

	memInfo.state = SRV_FU_MEM_STATE_OPEN_DRIVER;
}


void SRV_FU_Tasks(void)
{

   /* Check the application's current state. */
    switch ( memInfo.state )
    {
        case SRV_FU_MEM_STATE_OPEN_DRIVER:
        {
            memInfo.memoryHandle = DRV_MEMORY_Open(DRV_MEMORY_INDEX_0, DRV_IO_INTENT_READWRITE);

            if (DRV_HANDLE_INVALID != memInfo.memoryHandle)
            {
                DRV_MEMORY_TransferHandlerSet(memInfo.memoryHandle, lSRV_FU_TransferHandler, (uintptr_t)&memInfo);
                memInfo.state = SRV_FU_MEM_STATE_GEOMETRY_GET;
            }

            break;
        }

        case SRV_FU_MEM_STATE_GEOMETRY_GET:
        {
            nvmGeometry = DRV_MEMORY_GeometryGet(memInfo.memoryHandle);

            if (nvmGeometry == NULL)
            {
                memInfo.state = SRV_FU_MEM_UNITIALIZED;
                break;
            }

			memInfo.eraseBlockStart = (memInfo.iniFuRegion / nvmGeometry->geometryTable[SYS_MEDIA_GEOMETRY_TABLE_ERASE_ENTRY].blockSize);

            memInfo.numFuRegionEraseBlocks = (memInfo.sizeFuRegion / nvmGeometry->geometryTable[SYS_MEDIA_GEOMETRY_TABLE_ERASE_ENTRY].blockSize);

			memInfo.writePageSize = nvmGeometry->geometryTable[SYS_MEDIA_GEOMETRY_TABLE_WRITE_ENTRY].blockSize;
			memInfo.readPageSize = nvmGeometry->geometryTable[SYS_MEDIA_GEOMETRY_TABLE_READ_ENTRY].blockSize;

            memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
            break;
        }


        case SRV_FU_MEM_STATE_ERASE_FLASH:
        {
            if (DRV_MEMORY_COMMAND_HANDLE_INVALID == memInfo.eraseHandle)
            {
                if (SRV_FU_MemTransferCallback != NULL) 
                {
                    SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_ERASE, SRV_FU_MEM_TRANSFER_ERROR);
                }   
                
                memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
            }

            break;
        }

		case SRV_FU_MEM_STATE_READ_MEMORY:
		{
			if (DRV_MEMORY_COMMAND_HANDLE_INVALID == memInfo.readHandle)
			{
                if (SRV_FU_MemTransferCallback != NULL) 
                {
                    SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_READ, SRV_FU_MEM_TRANSFER_ERROR);
                }   
                
                memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
			}
			break;
		}
		
		case SRV_FU_MEM_STATE_WRITE_ONE_BLOCK:
		{
            uint32_t block;
            uint32_t offset;
            uint32_t bytesToCopy;

            if (memInfo.writeSize == 0)
            {
                if (SRV_FU_MemTransferCallback != NULL) 
                {
                    SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_READ, SRV_FU_MEM_TRANSFER_OK);
                }   
                
                memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
                break;
            }
            
            block = memInfo.writeAddress / memInfo.writePageSize;
            memInfo.recoverAddress = block * memInfo.writePageSize;
            offset = memInfo.writeAddress - memInfo.recoverAddress;

            if ((memInfo.writePageSize - offset) < memInfo.writeSize)
            {
                bytesToCopy = memInfo.writePageSize - offset;
            }
            else
            {
                bytesToCopy = memInfo.writeSize;
            }

            memset( pMemWrite, 0xff, memInfo.writePageSize);
            memcpy( &pMemWrite[offset], &pBuffInput[memInfo.bytesWritten] , memInfo.writeSize);

            DRV_MEMORY_AsyncWrite(memInfo.memoryHandle, &memInfo.writeHandle, pMemWrite, block, 1);
 
			if (DRV_MEMORY_COMMAND_HANDLE_INVALID == memInfo.writeHandle)
			{
                if (SRV_FU_MemTransferCallback != NULL) 
                {
                    SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_READ, SRV_FU_MEM_TRANSFER_ERROR);
                }   
                
                memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
			}
            else
            {
                memInfo.writeAddress += bytesToCopy;
                memInfo.writeSize -= bytesToCopy;
                memInfo.bytesWritten += bytesToCopy;
                
                memInfo.state = SRV_FU_MEM_STATE_WRITE_WAIT_END;
            }

            break;
		}

        case SRV_FU_CALCULATE_CRC_BLOCK:
        {
            if (crcState == SRC_FU_CRC_CALCULATING)
            {
                calculatedCrc = SRV_PCRC_GetValue(pBuffInput, crcSize, PCRC_HT_FU, PCRC_CRC32,
                                     calculatedCrc);

                if (crcRemainSize > 0)
                {
                    uint32_t blockStart, nBlock;
                    uint32_t bytesPagesRead;

                    crcState = SRV_FU_CRC_WAIT_READ_BLOCK;

                    if (crcRemainSize < MAX_BUFFER_READ_SIZE)
                    {
                        crcSize = crcRemainSize;
                    }
                    else
                    {
                        crcSize = MAX_BUFFER_READ_SIZE;
                    }
                    
                    blockStart = crcReadAddress / memInfo.readPageSize;
                    nBlock = crcSize / memInfo.readPageSize;

                    bytesPagesRead = nBlock * memInfo.readPageSize;
                    /* Aling CRC size with the readPageSize */
                    if (crcSize > bytesPagesRead)
                    {
                        if (((nBlock + 1) * memInfo.readPageSize) <= MAX_BUFFER_READ_SIZE)
                        {
                            nBlock++;
                        }
                        else
                        {
                            /* Cannot read everything, we reduced the size of the Crc calculated
                            this time */
                            crcSize = bytesPagesRead;
                        }
                    }

                    DRV_MEMORY_AsyncRead(memInfo.memoryHandle, &memInfo.readHandle, pBuffInput, blockStart, nBlock);
                    
                    crcReadAddress += crcSize;
                    crcRemainSize -= crcSize;
                }
                else
                {
                    /* check pointer function */
                    if (SRV_FU_CrcCallback) {
                        SRV_FU_CrcCallback(calculatedCrc);
                    }

                    crcState = SRV_FU_CRC_ILDE;
                    memInfo.state = SRV_FU_MEM_STATE_CMD_WAIT;
                }
            }
            
            break;
        }
        
        case SRV_FU_MEM_STATE_XFER_WAIT:
        case SRV_FU_MEM_STATE_SUCCESS:
        case SRV_FU_MEM_STATE_WRITE_WAIT_END:
        case SRV_FU_MEM_STATE_CMD_WAIT:
        case SRV_FU_MEM_UNITIALIZED:
        default:
        {
            break;
        }
	}
}


void SRV_FU_DataRead(uint32_t address, uint8_t *buffer, uint16_t size)
{
	uint32_t readAddress;
	uint32_t blockStart, nBlock;

	readAddress = memInfo.iniFuRegion + address;

	blockStart = readAddress / memInfo.readPageSize;
	nBlock = size / memInfo.readPageSize;

	DRV_MEMORY_AsyncRead(memInfo.memoryHandle, &memInfo.readHandle, (void *) buffer, blockStart, nBlock);
	
	memInfo.state = SRV_FU_MEM_STATE_READ_MEMORY;
}



void SRV_FU_DataWrite(uint32_t address, uint8_t *buffer, uint16_t size)
{

    if (size > MAX_BUFFER_READ_SIZE)
    {
        if (SRV_FU_MemTransferCallback != NULL) 
        {
            SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_WRITE, SRV_FU_MEM_TRANSFER_ERROR);
        }

        return;
    }
	
    if (memInfo.writePageSize > MEMORY_WRITE_SIZE)
    {
        if (SRV_FU_MemTransferCallback != NULL) 
        {
            SRV_FU_MemTransferCallback(SRV_FU_MEM_TRANSFER_CMD_WRITE, SRV_FU_MEM_TRANSFER_ERROR);
        }
        return;
    }

    memInfo.writeAddress = memInfo.iniFuRegion + address;
    memInfo.writeSize = size;
    memInfo.bytesWritten = 0;

    memcpy(pBuffInput, buffer, size);
    
    memInfo.state = SRV_FU_MEM_STATE_WRITE_ONE_BLOCK;
}


void SRV_FU_CfgRead(void *dst, uint16_t size)
{
	uint32_t bufferValue[4];
	uint32_t *pointerBuffer;

	pointerBuffer = (uint32_t *)bufferValue;
	*pointerBuffer++ = SUPC_GPBRRead(GPBR_REGS_0);
	*pointerBuffer++ = SUPC_GPBRRead(GPBR_REGS_1);
	*pointerBuffer++ = SUPC_GPBRRead(GPBR_REGS_2);
	*pointerBuffer = SUPC_GPBRRead(GPBR_REGS_3);
	memcpy(dst, (uint8_t *)bufferValue, size);

}

void SRV_FU_CfgWrite(void *src, uint16_t size)
{
	uint32_t bufferValue[4];
	uint32_t *pointerBuffer;

	memcpy((uint8_t *)bufferValue, src, size);

	pointerBuffer = (uint32_t *)bufferValue;
	SUPC_GPBRWrite(GPBR_REGS_0, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_1, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_2, *pointerBuffer++);
	SUPC_GPBRWrite(GPBR_REGS_3, *pointerBuffer);
}

/**
 * \brief Start the FU process by initializing and unlocking the memory
 *
 * \param x_fu_info    FU information data
 *
 */
void SRV_FU_Start(SRV_FU_INFO *fuInfo)
{
	fuData.imageSize = fuInfo->imageSize;
	fuData.pageSize = fuInfo->pageSize;
	fuData.signAlgorithm = SRV_FU_SIGNATURE_ALGO_NO_SIGNATURE;
	fuData.signLength = 0;
#ifdef HAL_FU_ENABLE_SIGNATURE
	fuData.signAlgorithm = fuInfo->signAlgorithm;
	fuData.signLength = fuInfo->signLength;
#endif

	/* erase internal flash pages */
	lSRV_FU_EraseFuRegion();

	/* Set CRC status */
	crcState = SRV_FU_CRC_ILDE;

#ifdef PRIME_NUM_REGIONS
	// /* Reset prime app to FU */
	// suc_app_to_fu = PRIME_INVALID_APP;

	// /* Clear boot configuration */
	// x_boot_info_cfg_t x_boot_cfg;
	// hal_get_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	// x_boot_cfg.ul_img_size = 0;
	// x_boot_cfg.ul_orig_addr = 0;
	// x_boot_cfg.ul_dest_addr = 0;
	// x_boot_cfg.uc_pages_counter = 0;
	// x_boot_cfg.uc_boot_state = 0;
	// hal_set_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
	// /* Reset image identifier */
	// gpbr_write(GPBR4, (uint32_t)suc_app_to_fu);
#endif

	return;
}

/**
 * \brief Finish the FU process and execute the new firmware
 *
 * \param uc_hal_res     Result of the FU process
 *
 */
void SRV_FU_End(SRV_FU_RESULT fuResult)
{
	SRV_FU_RESULT result;

	/* check callback is initialized */
	if (!SRV_FU_ResultCallback) {
		return;
	}

	switch (fuResult)
    {
	    case SRV_FU_RESULT_SUCCESS:
            result = SRV_FU_RESULT_SUCCESS;
            SRV_FU_ResultCallback(result);
            break;

    	case SRV_FU_RESULT_CRC_ERROR:
            result = SRV_FU_RESULT_CRC_ERROR;
            SRV_FU_ResultCallback(result);
            break;

    	case SRV_FU_RESULT_FW_REVERT:
            result = SRV_FU_RESULT_FW_REVERT;
            SRV_FU_ResultCallback(result);
            break;

    	case SRV_FU_RESULT_FW_CONFIRM:
            result = SRV_FU_RESULT_FW_CONFIRM;
            SRV_FU_ResultCallback(result);
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
	SRV_FU_RESULT result;

	result = SRV_FU_RESULT_FW_REVERT;
	SRV_FU_ResultCallback(result);
}

/**
 * \brief Calculate the CRC of the new firmware
 *
 */
void SRV_FU_CalculateCrc(void)
{
	uint32_t blockStart, nBlock;
    uint32_t bytesPagesRead;

	if (crcState != SRV_FU_CRC_ILDE) {
		return;
	}

	crcState = SRV_FU_CRC_WAIT_READ_BLOCK;

	crcReadAddress = memInfo.iniFuRegion;
    crcRemainSize = fuData.imageSize;

	if (crcRemainSize < MAX_BUFFER_READ_SIZE)
    {
        crcSize = crcRemainSize;
    }
    else
    {
        crcSize = MAX_BUFFER_READ_SIZE;
    }
    
    blockStart = crcReadAddress / memInfo.readPageSize;
	nBlock = crcSize / memInfo.readPageSize;

    bytesPagesRead = nBlock * memInfo.readPageSize;
    /* Aling CRC size with the readPageSize */
    if (crcSize > bytesPagesRead)
    {
        if (((nBlock + 1) * memInfo.readPageSize) <= MAX_BUFFER_READ_SIZE)
        {
            nBlock++;
        }
        else
        {
            /* Cannot read everything, we reduced the size of the Crc calculated
            this time */
            crcSize = bytesPagesRead;
        }
    }

	DRV_MEMORY_AsyncRead(memInfo.memoryHandle, &memInfo.readHandle, pBuffInput, blockStart, nBlock);
	
	crcReadAddress += crcSize;
    crcRemainSize -= crcSize;

    memInfo.state = SRV_FU_CALCULATE_CRC_BLOCK;
    
    /* CRC Initial */
    calculatedCrc = 0;
}

/**
 *
 */
void SRV_FU_RegisterCallbackCrc(SRV_FU_CRC_CB callback)
{
	SRV_FU_CrcCallback = callback;
}

/**
 */
void SRV_FU_RegisterCallbackVerify(SRV_FU_IMAGE_VERIFY_CB callback)
{
	SRV_FU_ImageVerifyCallback = callback;
}

/**
 */
void SRV_FU_RegisterCallbackFuResult(SRV_FU_RESULT_CB callback)
{
	SRV_FU_ResultCallback = callback;

}

/**
 */
uint16_t SRV_FU_GetBitmap(uint8_t *bitmap, uint32_t *numRxPages)
{
	(void)bitmap;
	(void)numRxPages;

	return 0;
}

/**
 */
void SRV_FU_SwapVersion(SRV_FU_TRAFFIC_VERSION trafficVersion)
{
	/* check callback is initialized */
	if (SRV_FU_SwapCallback) {
		SRV_FU_SwapCallback(trafficVersion);
	}
}

/**
 */
void SRV_FU_RegisterCallbackSwapVersion(SRV_FU_VERSION_SWAP_CB callback)
{
	SRV_FU_SwapCallback = callback;
}






// /**
//  * \brief Check the signature of the new firmware and the validity of the received binary file
//  *
//  */

// #ifdef HAL_FU_ENABLE_SIGNATURE
// void SRV_FU_SignatureImageCheck(void)
// {
// 	uint8_t uc_algorithm;
// 	uint16_t us_signature_length;
// 	uint32_t pul_fw_data;
// 	uint32_t pul_fw_length;

// 	/* check if there is a callback function, otherwise, don't waste time */
// 	if (!_signature_image_cb_function) {
// 		return;
// 	}

// 	/* get signature params */
// 	uc_algorithm = x_fu_data.sign_algorithm;
// 	us_signature_length = x_fu_data.sign_length;

// 	/* Get FW data */
// 	pul_fw_data = spx_fu_region_cfg[suc_region_to_fu].ul_address;
// 	pul_fw_length = x_fu_data.image_size;

// 	if (uc_algorithm == FW_NO_SIGN) {
// 		/* Verify if this is a right image */
// 		if (_verify_image()) {
// 			_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
// 		} else {
// 			_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
// 		}
// 	} else if (uc_algorithm == FW_SIGN_ECDSA_SHA256) {
// 		/* This code assumes that curve secp_256k1 is used.*/
// 		/* It will produce a 71 byte signature */
// 		int32_t i_ret = 0;
// 		uint8_t puc_sha256_hash[32];
// 		uint8_t puc_signature[256];
// 		mbedtls_ecp_keypair ctx_verify;
// 		mbedtls_ecp_keypair st_ecp_public_key;

// 		/*Load signature from FW data. Signature data is concatenated to FW */
// 		memcpy(puc_signature, (uint8_t *)pul_fw_data + pul_fw_length - us_signature_length, us_signature_length);

// 		/*Initialize verify context*/
// 		mbedtls_ecdsa_init(&ctx_verify);

// 		/* Load ECC curve */
// 		mbedtls_ecp_group_load(&ctx_verify.grp, MBEDTLS_ECP_DP_SECP256K1);

// 		/*Load ATMEL public KEY */
// 		mbedtls_ecp_keypair_init(&st_ecp_public_key);
// 		mbedtls_ecp_point_read_binary(&ctx_verify.grp, &st_ecp_public_key.Q, puc_atmel_ecc_public_key, sizeof puc_atmel_ecc_public_key);
// 		mbedtls_ecp_copy(&ctx_verify.Q, &st_ecp_public_key.Q);

// 		/* Verify curve/key */
// 		if (mbedtls_ecp_check_pubkey(&ctx_verify.grp, &st_ecp_public_key.Q) != 0) {
// 			/* Not a valid public key! */
// 			_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
// 			mbedtls_ecp_keypair_free(&ctx_verify);
// 			return;
// 		}

// 		/* Compute FW SHA-256 Hash */
// 		mbedtls_sha256((uint8_t *)pul_fw_data, pul_fw_length - us_signature_length, puc_sha256_hash, 0);

// 		/* Check signature */
// 		i_ret = mbedtls_ecdsa_read_signature(&ctx_verify, puc_sha256_hash, 32, puc_signature, us_signature_length);
// 		if (i_ret < 0) {
// 			_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
// 		} else {
// 			/* Verify if this is a right image */
// 			if (_verify_image()) {
// 				_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
// 			} else {
// 				_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
// 			}
// 		}

// 		/* release memory */
// 		mbedtls_ecp_keypair_free(&ctx_verify);
// 		mbedtls_ecp_keypair_free(&st_ecp_public_key);
// 	} else {
// 		/*NOT IMPLEMENTED */
// 		_signature_image_cb_function(HAL_FU_SIGNATURE_FAIL);
// 	}
// }

// #else
// void SRV_FU_SignatureImageCheck(void)
// {
// 	/* check pointer function */
// 	if (!_signature_image_cb_function) {
// 		return;
// 	}

// #ifdef PRIME_NUM_REGIONS
// 	/* Verify if this is a right image */
// 	if (_verify_image()) {
// 		_signature_image_cb_function(HAL_FU_VERIF_SUCCESS);
// 	} else {
// 		_signature_image_cb_function(HAL_FU_IMAGE_FAIL);
// 	}
// #endif
// }

// #endif /* HAL_FU_ENABLE_SIGNATURE */

// /**
//  * \brief Configure FU PRIME regions
//  *
//  * \param uc_num_regs       Number of PRIME regions
//  * \param px_fu_reg         Pointer to region configurations
//  *
//  */
// void SRV_FU_ConfigRegions(uint8_t uc_num_regs, x_fu_region_cfg_t *px_fu_reg)
// {
// 	(void)uc_num_regs;

// 	spx_fu_region_cfg = px_fu_reg;
// }

// #ifdef PRIME_NUM_REGIONS
// /**
//  * \brief Swap FW
//  *
//  * \return 1 if swapped, 0 otherwise
//  *
//  */
// uint8_t SRV_FU_Swap(void)
// {
// 	x_fu_region_cfg_t x_exec_region_cfg;

// 	/* Check if the current stack is 1.3 */
// 	x_prime_mode_info_cfg_t x_board_info;
// 	hal_get_config_info(CONFIG_TYPE_MODE_PRIME, sizeof(x_board_info), (void *)&x_board_info);
// 	if (x_board_info.prime_version == PRIME_1_3) {
// 		/* Verify if this is a right image */
// 		if (_verify_image() == false) {
// 			/* Trigger reset, needed in FU 1.3 */
// 			return 1;
// 		}
// 	}

// 	/* Get image identifier */
// 	suc_app_to_fu = (uint8_t)gpbr_read(GPBR4);

// 	/* Check which app must be swapped */
// 	switch (suc_app_to_fu) {
// 	case PRIME_MAC13_APP:
// 		x_exec_region_cfg.ul_address = PRIME_MAC13_FLASH_LOCATION;
// 		x_exec_region_cfg.ul_size = PRIME_MAC13_SIZE;
// 		break;
// 	case PRIME_MAC14_APP:
// 		x_exec_region_cfg.ul_address = PRIME_MAC14_FLASH_LOCATION;
// 		x_exec_region_cfg.ul_size = PRIME_MAC14_SIZE;
// 		break;
// #ifdef HAL_ATPL360_INTERFACE
// 	case PRIME_PHY_APP:
// 		x_exec_region_cfg.ul_address = PRIME_PHY_FLASH_LOCATION;
// 		x_exec_region_cfg.ul_size = PRIME_PHY_SIZE;
// 		break;
// #endif
// 	case PRIME_MAIN_APP:
// 		x_exec_region_cfg.ul_address = PRIME_APP_FLASH_LOCATION;
// 		x_exec_region_cfg.ul_size = PRIME_APP_SIZE;
// 		break;
// 	case PRIME_INVALID_APP:
// 	default:
// 		return 0;
// 	}

// 	/* Update boot configuration */
// 	x_boot_info_cfg_t x_boot_cfg;
// 	hal_get_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
// 	x_boot_cfg.ul_orig_addr = spx_fu_region_cfg[suc_region_to_fu].ul_address;
// 	x_boot_cfg.ul_dest_addr = x_exec_region_cfg.ul_address;
// 	x_boot_cfg.ul_img_size = x_exec_region_cfg.ul_size;
// 	/* Store the new boot configuration */
// 	hal_set_config_info(CONFIG_TYPE_BOOT_INFO, sizeof(x_boot_cfg), &x_boot_cfg);
// 	/* Store the image identifier */
// 	gpbr_write(GPBR4, (uint32_t)suc_app_to_fu);

// 	return 1;
// }
// #endif


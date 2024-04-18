/*******************************************************************************
  PRIME Hardware Abstraction Layer Wrapper API Header
   
  Company:
    Microchip Technology Inc.

  File Name:
    prime_hal_wrapper.h

  Summary:
    PRIME Hardware Abstraction Layer Wrapper API Header File

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

#ifndef PRIME_HAL_WRAPPER_H_INCLUDE
#define PRIME_HAL_WRAPPER_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "stack/prime/hal_api/hal_api.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: API Functions
// *****************************************************************************
// *****************************************************************************
<#if primePal.PRIME_PAL_PLC_EN == true>
//******************************************************************************
/* Function:
    void PRIME_HAL_WRP_Configure(HAL_API *halApi)
    
  Summary:
    Configure the PRIME HAL Wrapper.
bool prime_hal_plc_send_boot_cmd(uint16_t us_cmd, uint32_t ul_addr, uint32_t ul_data_len, uint8_t *puc_data_buf, uint8_t *puc_data_read);
bool prime_hal_plc_send_wrrd_cmd(uint8_t uc_cmd, void *px_spi_data, void *px_spi_status_info);
void prime_hal_plc_enable_interrupt(bool enable);
void prime_hal_plc_delay(uint8_t uc_tref, uint32_t ul_delay);

  Precondition:
    None.

  Parameters:
    halApi       - Pointr to the HAL API.

  Returns:
    None.

  Example:
    <code>
    void prime_stack_init(void *p_hal_api)
    {
        PRIME_HAL_WRP_Configure(p_hal_api);
    }
    </code>

  Remarks:
    None.
*/
void PRIME_HAL_WRP_Configure(HAL_API *halApi);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType)

  Summary:
    Trigger a system restart.

  Description:
    This routine triggers a system restart. 

  Precondition:
    None.
    
  Parameters:
    resetType      - Type of reset

  Returns:
    None.

  Example:
    <code>
    PRIME_HAL_WRP_RestartSystem(RESET_HANDLER_SOFTWARE_RESET);
    </code>

  Remarks:
    Related to Reset Handler service.
*/

void PRIME_HAL_WRP_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType);

// *****************************************************************************
/* Function:
    uint32_t PRIME_HAL_WRAPPER_PcrcCalculate(
      uint8_t *pData, 
      size_t length,
      PCRC_HEADER_TYPE hdrType, 
      PCRC_CRC_TYPE crcType,
      uint32_t initValue,
      bool v14Mode
    );
    
    (uint8_t *pData, size_t length,
    PCRC_HEADER_TYPE hdrType, PCRC_CRC_TYPE crcType, uint32_t initValue, 
    bool v14Mode);

  Summary:
    Obtains the CRC for a data stream.

  Description:
    This routine gets the CRC value (8, 16 or 32 bits, depending on arguments)
    of the data stream provided as argument.

  Precondition:
    None.

  Parameters:
    pData       - Pointer to buffer containing the data stream

    length      - Length of the data stream

    hdrType     - Header type to determine the method to obtain CRC

    crcType     - CRC type(8, 16 or 32 bits)

    initValue   - Initialization value for CRC computation 
    
    v14Mode     - True for PRIME v1.4 mode and otherwise false
 
  Returns:
    If successful, the routine returns a valid CRC value.
    If an error occurs, the return value is PCRC_INVALID. Error can occur if
    hdrType or crcType are wrong.
    Returned CRC is always a 32-bit value. For 8-bit or 16-bit CRC, it is casted 
    to 32-bit.

  Example:
    <code>
    uint32_t valueCrc32;
    
    valueCrc32 = PRIME_HAL_WRAPPER_PcrcCalculate(pData, length, 
                    PCRC_HT_BEACON, PCRC_CRC32);
    </code>

  Remarks:
    Related to PCRC service.
*/        
uint32_t PRIME_HAL_WRAPPER_PcrcCalculate((uint8_t *pData, size_t length,
    PCRC_HEADER_TYPE hdrType, PCRC_CRC_TYPE crcType, uint32_t initValue, 
    bool v14Mode);
    
// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_PcrcConfigureSNA(uint8_t *sna);

  Summary:
    Sets SNA value to be used as initial value on further CRC calculations.

  Description:
    This routine sets the value that will be used as the initial CRC value
    for computations related to PRIME stack, as stated on PRIME specification.

  Precondition:
    None.

  Parameters:
    sna    -    Pointer to buffer containing SNA value

  Returns:
    None.

  Example:
    <code>
    uint8_t sna[PCRC_SNA_SIZE];
    
    PRIME_HAL_WRP_PcrcConfigureSNA(sna);
    </code>

  Remarks:
    Related to PCRC service.
*/  
    
void PRIME_HAL_WRP_PcrcConfigureSNA(uint8_t *sna);

// *****************************************************************************
/* Function:
    bool PRIME_HAL_WRP_GetConfigInfo
    (
        SRV_STORAGE_TYPE infoType, 
        uint8_t size, 
        void* pData
    )

  Summary:
    Reads configuration information stored externally.

  Description:
    This routine reads configuration information stored externally.

  Precondition:
    None.

  Parameters:
    infoType - Configuration information type to read
    size     - Size in bytes of the configuration information to read
    pData    - Pointer to store the read configuration information

  Returns:
    The result of the read operation (true if success, false if error).

  Example:
    <code>
    uint8_t macInfo[8];

    if (PRIME_HAL_WRP_GetConfigInfo(SRV_STORAGE_TYPE_MAC_INFO, 8, macInfo))
    {

    }
    </code>

  Remarks:
    Related to PRIME Storage service.
*/
bool PRIME_HAL_WRP_GetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, 
    void* pData);

// *****************************************************************************
/* Function:
    bool PRIME_HAL_WRP_SetConfigInfo
    (
        SRV_STORAGE_TYPE infoType, 
        uint8_t size, 
        void* pData
    )

  Summary:
    Writes configuration information stored externally.

  Description:
    This routine writes configuration information stored externally.

  Precondition:
    None.

  Parameters:
    infoType - Configuration information type to write
    size     - Size in bytes of the configuration information to write
    pData    - Pointer to configuration information data to write

  Returns:
    The result of the write operation (true if success, false if error).

  Example:
    <code>
    uint8_t macInfo[8] = {0x55, 0xAA, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

    PRIME_HAL_WRP_SetConfigInfo(SRV_STORAGE_TYPE_MAC_INFO, 8, macInfo);
    </code>

  Remarks:
    Related to PRIME Storage service.
*/
bool PRIME_HAL_WRP_SetConfigInfo(SRV_STORAGE_TYPE infoType, uint8_t size, 
    void* pData);
    
// *****************************************************************************
/* Function:
    SRV_USI_HANDLE PRIME_HAL_WRP_UsiOpen(const SYS_MODULE_INDEX index)

  Summary:
    Opens the specified USI service instance and returns a handle to it.

  Description:
    This routine opens the specified USI service instance and provides a handle
    that must be provided to all other client-level operations to identify the
    caller and the instance of the service.

  Precondition:
    None.

  Parameters:
    index     - Index for the instance to be opened

  Returns:
    If successful, the routine returns a valid open-instance handle.
    If an error occurs, the return value is SRV_USI_HANDLE_INVALID.

  Example:
    <code>
    SRV_USI_HANDLE handle;

    handle = PRIME_HAL_WRP_UsiOpen(SRV_USI_INDEX_0);
    if (handle == SRV_USI_HANDLE_INVALID)
    {

    }
    </code>

  Remarks:
    Related to USI service.
*/
SRV_USI_HANDLE PRIME_HAL_WRP_UsiOpen(const SYS_MODULE_INDEX index);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_UsiSetCallback
    (
        SRV_USI_HANDLE handle,
        SRV_USI_PROTOCOL_ID protocol,
        SRV_USI_CALLBACK callback
    }

  Summary:
    Registers a function to be called back when a new message is received and
    it belongs to the specified USI protocol.

  Description:
    This function allows a client to register an event handling function to be
    called back when a new message is received and it belongs to the specified
    USI protocol.

  Precondition:
    None.

  Parameters:
    handle      - A valid open-instance handle, returned from SRV_USI_Open
    protocol    - Identifier of the protocol for which callback function will
                  be registered
    callback    - Pointer to the callback function

  Returns:
    None.

  Example:
    <code>
    void PRIME_USIEventHandler(uint8_t *pData, size_t length)
    {

    }

    PRIME_HAL_WRP_UsiSetCallback(handle, SRV_USI_PROT_ID_MNGP_PRIME, PRIME_USIEventHandler);
    </code>
    
  Remarks:
    Related to USI service.
*/
void PRIME_HAL_WRP_UsiSetCallback(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, 
    SRV_USI_CALLBACK callback);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_UsiSend
    (
        SRV_USI_HANDLE handle,
        SRV_USI_PROTOCOL_ID protocol, 
        uint8_t *data, 
        size_t length )

  Summary:
    Sends a message through serial interface (USI).

  Description:
    This function is used to send a message through USI. The message will be
    formated depending on the specified protocol and will be sent using the
    serial interface associated to the corresponding USI instance.

  Precondition:
    None.

  Parameters:
    handle      - A valid open-instance handle, returned from SRV_USI_Open
    protocol    - Identifier of the protocol for the message to send
    data        - Pointer to the data to send
    length      - Length of the data to send in bytes

  Returns:
    None

  Example:
    <code>
    uint8_t pData[] = "Message to send through USI";

    PRIME_HAL_WRP_UsiSend(handle, SRV_USI_PROT_ID_MNGP_PRIME, pData, sizeof(pData));
    </code>

  Remarks:
    Related to USI service.
  */
void PRIME_HAL_WRP_UsiSend(SRV_USI_HANDLE handle, SRV_USI_PROTOCOL_ID protocol, 
    uint8_t *data, size_t length);

//******************************************************************************
/* Function:
    void PRIME_HAL_WRP_DebugReport
    (
        SRV_LOG_REPORT_LEVEL logLevel,
        SRV_LOG_REPORT_CODE code,
        const char *info, ...)
        
  Summary:
    Reports an error/warning code with related information.

  Description:
    This function reports an error/warning code with related information.

  Precondition:
    None.

  Parameters:
    logLevel       - Log priority level
    code           - Code of the reported error/warning
    info           - Formatted description of the reported error/warning

  Returns:
    None.

  Example:
    <code>
    PRIME_HAL_WRP_DebugReport(SRV_LOG_REPORT_WARNING, 100, "Wrong input\r\n");
    </code>

  Remarks:
    Related to Log Report service.
*/
void PRIME_HAL_WRP_DebugReport(SRV_LOG_REPORT_LEVEL logLevel, 
    SRV_LOG_REPORT_CODE code, const char *info, ...);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_PibGetRequest(uint16_t pibAttrib)

  Summary:
    Get a user PIB.

  Description:
    This routine is used to get a user PIB.

  Precondition:
    None.
    
  Parameters:
    pibAttrib     - PIB attribute identifier

  Returns:
    None.

  Example:
    <code>
    static void PRIME_GetRequestHandler(uint8_t result, uint16_t pibAttrib, 
                                      void *pibValue, uint8_t pibSize)
    {
        if (getResult == true) 
        {
            ...
        }
    }
    
    PRIME_HAL_WRP_PibGetRequestSetCallback(PRIME_GetRequestHandler);

    PRIME_HAL_WRP_PibGetRequest(PIB_USER_RESET_INFO);
    </code>

  Remarks:
    Related to User PIB service.
*/
void PRIME_HAL_WRP_PibGetRequest(uint16_t us_pib_attrib);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_PIBGetRequestSetCallback
    ( 
        USER_PIB_GET_REQUEST_CALLBACK callback
    )

  Summary:
    Register a function to be called to get the requested user PIB.

  Description:
    This routine allows a client to register an event handling function to be
    called when a requested user PIB is returned.

  Precondition:
    None.
    
  Parameters:
    callback      - Pointer to the callback function

  Returns:
    None.

  Example:
    <code>
    static void PRIME_GetRequestHandler(uint8_t getResult, uint16_t pibAttrib, 
                                      void *pibValue, uint8_t pibSize)
    {
        if (getResult == true) 
        {
            ...
        }
    }
    
    PRIME_HAL_WRP_PIBGetRequestSetCallback(APP_GetRequestHandler);
    </code>

  Remarks:
    Related to User PIB service.
*/
void PRIME_HAL_WRP_PIBGetRequestSetCallback(USER_PIB_GET_REQUEST_CALLBACK callback);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_PibSetRequest
    (
        uint16_t pibAttrib, 
        void *pibValue, 
        uint8_t pibSize
    )

  Summary:
    Set a user PIB.

  Description:
    This routine is used to set a user PIB.

  Precondition:
    None.
    
  Parameters:
    pibAttrib     - PIB attribute identifier
    pibValue      - PIB attribute value
    pibSize       - PIB attribute value size

  Returns:
    None.

  Example:
    <code>
    static void PRIME_SetRequestHandler(uint8_t setResult)
    {
        if (setResult == true) 
        {
            ...
        }
    }
    
    uint32_t resetValue = 0;
        
    PRIME_HAL_WRP_PIBSetRequestSetCallback(PRIME_SetRequestHandler);

    PRIME_HAL_WRP_PibSetRequest(PIB_USER_RESET_INFO, &resetValue, 
        sizeof(resetValue));
    </code>

  Remarks:
    Related to User PIB service.
*/

void PRIME_HAL_WRP_PibSetRequest(uint16_t pibAttrib, void *pibValue, 
    uint8_t pibSize);

// *****************************************************************************
/* Function:
    void PRIME_HAL_WRP_PIBSetRequestSetCallback
    (
        USER_PIB_SET_REQUEST_CALLBACK callback
    )

  Summary:
    Register a function to be called to get the result of setting a user PIB.

  Description:
    This routine allows a client to register an event handling function to be
    called with the result of setting a user PIB.

  Precondition:
    None.
    
  Parameters:
    callback      - Pointer to the callback function

  Returns:
    None.

  Example:
    <code>
    static void PRIME_SetRequestHandler(uint8_t setResult)
    {
        if (setResult == true) 
        {
            ...
        }
    }
        
    PRIME_HAL_WRP_PIBSetRequestSetCallback(PRIME_SetRequestHandler);
    </code>

  Remarks:
    Related to User PIB service.
*/
void PRIME_HAL_WRP_PIBSetRequestSetCallback(USER_PIB_SET_REQUEST_CALLBACK callback);

//******************************************************************************
/* Function:
    uint32_t PRIME_HAL_WRP_RngGet(void)

  Summary:
    Returns a random value of 32 bits.

  Description:
    This function returns a random value of 32 bits.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    32-bit random value.

  Example:
    <code>
    uint32_t rndNum = PRIME_HAL_WRP_RngGet();
    </code>

  Remarks:
    Related to Random service.
*/
uint32_t PRIME_HAL_WRP_RngGet(void);

//******************************************************************************
/* Function:
    int32_t PRIME_HAL_WRP_AesCmacDirect
    (
        uint8_t *input, 
        uint32_t inputLen,
        uint8_t *outputMac, 
        uint8_t *key
    )

  Summary:
    Performs AES-CMAC to generate the MAC in single step without initialization.

  Description:
    This function performs AES-CMAC to generate the MAC in single step without
    initialization. The size of the key is 16 bytes. The size of the MAC is equal 
    to the AES block size (16 bytes).

  Precondition:
    None.

  Parameters:
    input     - Pointer to buffer holding the input data
    inputLen  - Length of the input data in bytes
    outputMac - Pointer to store the output data (MAC). The size of the buffer
                must be equal or larger than the AES block size (16 bytes).
    key       - Pointer to buffer holding the 16-byte key itself

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful process.
    - Any other value: Error in the process.

  Example:
    <code>
    int32_t ret;
    uint8_t in1[] = { some input data };
    uint8_t out1[16];
    uint8_t key[16] = { some key };

    ret = PRIME_HAL_WRP_AesCmacDirect(in1, sizeof(in1), out1, key);
    </code>

  Remarks:
    Related to Security service.
*/
int32_t PRIME_HAL_WRP_AesCmacDirect(uint8_t *input, uint32_t inputLen, 
    uint8_t *outputMac, uint8_t *key);

//******************************************************************************
/* Function:
    int32_t PRIME_HAL_WRP_AesCcmSetkey(uint8_t *key)

  Summary:
    Initializes the AES-CCM context and sets the encryption key.

  Description:
    This function initializes the AES-CCM context and sets the 16-byte
    encryption key.

  Precondition:
    None.

  Parameters:
    key - Pointer to buffer holding the 16-byte key itself

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful initialization.
    - Any other value: Error in the initialization.

  Example:
    <code>
    int32_t ret;
    uint8_t key[16] = { some key };
    ret = PRIME_HAL_WRP_AesCcmSetkey(key);
    </code>

  Remarks:
    Related to Security service.
*/

int32_t PRIME_HAL_WRP_AesCcmSetkey(uint8_t *key);

//******************************************************************************
/* Function:
    int32_t PRIME_HAL_WRP_AesCcmEncryptAndTag
    (
        uint8_t *data,
        uint32_t dataLen,
        uint8_t *iv, 
        uint32_t ivLen,
        uint8_t *aad, 
        uint32_t aadLen,
        uint8_t *tag, 
        uint32_t tagLen
    )

  Summary:
    Performs AES-CCM authenticated encryption of a buffer.

  Description:
    This function performs AES-CCM authenticated encryption of a buffer.

  Precondition:
    Key must be set earlier with a call to PRIME_HAL_WRP_AesCcmSetkey.

  Parameters:
    data    - Pointer to buffer holding the input plain data. The output
              ciphered data is stored in the same buffer.
    dataLen - Length of the input/output data in bytes
    iv      - Pointer to initialization vector (nonce)
    ivLen   - Length of the nonce in bytes
    aad     - Pointer to additional authentication data field
    aadLen  - Length of additional authentication data in bytes
    tag     - Pointer to store the authentication tag
    tagLen  - Length of the authentication tag in bytes

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful encryption and authentication.
    - Any other value: Error in the encryption.

  Example:
    <code>
    int32_t ret;
    uint8_t nonce[] = { some initialization nonce };
    uint8_t data[] = { some plain message };
    uint8_t aad[] = { some additional authentication data };
    uint8_t tag[];
    uint8_t key[16] = { some key };

    ret = PRIME_HAL_WRP_AesCcmSetkey(key);
    ret = PRIME_HAL_WRP_AesCcmEncryptAndTag(data, sizeof(data),
                                            nonce, sizeof(nonce),
                                            aad, sizeof(aad),
                                            tag, sizeof(tag));
    </code>

  Remarks:
    Related to Security service.
*/

int32_t PRIME_HAL_WRP_AesCcmEncryptAndTag(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, 
    uint8_t *tag, uint32_t tagLen);

//******************************************************************************
/* Function:
    int32_t PRIME_HAL_WRP_AesCcmAuthDecrypt
    (
        uint8_t *data, 
        uint32_t dataLen,
        uint8_t *iv, 
        uint32_t ivLen,
        uint8_t *aad, 
        uint32_t aadLen,
        uint8_t *tag, 
        uint32_t tagLen
    )

  Summary:
    Performs AES-CCM authenticated decryption of a buffer.

  Description:
    This function performs a AES-CCM authenticated decryption of a buffer.

  Precondition:
    Key must be set earlier with a call to PRIME_HAL_WRP_AesCcmSetkey.

  Parameters:
    data    - Pointer to buffer holding the input ciphered data. The output
              plain data is stored in the same buffer.
    dataLen - Length of the input/output data in bytes
    iv      - Pointer to initialization vector (nonce)
    ivLen   - Length of the nonce in bytes
    aad     - Pointer to additional authentication data field
    aadLen  - Length of additional authentication data in bytes
    tag     - Pointer to buffer holding the authentication tag
    tagLen  - Length of the authentication tag in bytes

  Returns:
    - CIPHER_WRAPPER_RETURN_GOOD: Successful decryption and authentication.
    - Any other value: Error in the decryption or authentication.

  Example:
    <code>
    int32_t ret;
    uint8_t nonce[] = { some initialization nonce };
    uint8_t data[] = { some ciphered message };
    uint8_t aad[] = { some additional authentication data };
    uint8_t tag[] = { authentication tag received for verification };
    uint8_t key[16] = { some key };

    ret = PRIME_HAL_WRP_AesCcmSetkey(key, sizeof(key));
    ret = PRIME_HAL_WRP_AesCcmAuthDecrypt(data, sizeof(data),
                                          nonce, sizeof(nonce),
                                          aad, sizeof(aad),
                                          tag, sizeof(tag));
    </code>

  Remarks:
    Related to Security service.
*/

int32_t PRIME_HAL_WRP_AesCcmAuthDecrypt(uint8_t *data, uint32_t dataLen,
    uint8_t *iv, uint32_t ivLen, uint8_t *aad, uint32_t aadLen, 
    uint8_t *tag, uint32_t tagLen);
    
//******************************************************************************
/* Function:
    void PRIME_HAL_WRP_AesWrapKey
    (
        const uint8_t *key, 
        uint32_t keyLen, 
        const uint8_t *in, 
        uint32_t inLen,
        uint8_t *out
    )
    
  Summary:
    Wraps a key with AES Key Wrap Algorithm.

  Description:
    This function wraps a key using AES Key Wrap Algorithm.

  Precondition:
    None.

  Parameters:
    key    - Pointer to buffer holding the AES key for the algorithm
    keyLen - Length of key in bytes
    in     - Pointer to buffer where the key to wrap is located
    inLen  - Length in bytes of the key to wrap
    out    - Pointer to buffer to store the wrapped key

  Returns:
    None.

  Example:
    <code>
    uint8_t in1[16];
    uint8_t out1[24];
    uint8_t key[] = { some 16, 24, or 32 byte key };

    PRIME_HAL_WRP_AesWrapKey(key, sizeof(key), in1, sizeof(in1), out1);
    </code>

  Remarks:
    The wrapped key is one byte longer than the plain key.
    Related to Security service.
*/
void PRIME_HAL_WRP_AesWrapKey(const uint8_t *key, uint32_t keyLen, 
    const uint8_t *in, uint32_t inLen, uint8_t *out);    

//******************************************************************************
/* Function:
    bool PRIME_HAL_WRP_AesUnwrapKey(
        const uint8_t *key, 
        uint32_t keyLen, 
        const uint8_t *in, 
        uint32_t inLen,
        uint8_t *out)
    
  Summary:
    Unwraps a key with AES Key Wrap Algorithm.

  Description:
    This function unwraps a key using AES Key Wrap Algorithm.

  Precondition:
    None.

  Parameters:
    key    - Pointer to buffer holding the AES key for the algorithm.
    keyLen - Length of key in bytes.
    in     - Pointer to buffer where the wrapped key is located.
    inLen  - Length in bytes of the key to unwrap.
    out    - Pointer to buffer to store the unwrapped key.

  Returns:
    True if key was correctly unwrapped. Otherwise, false.

  Example:
    <code>
    uint8_t in1[24];
    uint8_t out1[16];
    uint8_t key[] = { some 16, 24, or 32 byte key };

    PRIME_HAL_WRP_AesUnwrapKey(key, sizeof(key), in1, sizeof(in1), out1);
    </code>

  Remarks:
    The unwrapped key is one byte shorter than the wrapped key.
    Related to Security service.
*/    
bool PRIME_HAL_WRP_AesUnwrapKey(const uint8_t *key, uint32_t keyLen, 
    const uint8_t *in, uint32_t inLen, uint8_t *out);
    
// *****************************************************************************
/* Function:
    uint64_t PRIME_HAL_WRP_GetTimeUS64(void)

  Summary:
    This primitive gets the value of a counter and converts it in a 64 bit 
    variable in microseconds.

  Description:
    This primitive makes use of SYS_TIME service to get the value of the 
    microseconds counter in order to be able to set timeouts and perform delays.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Value of microseconds in 64 bits.

  Example:
    <code>
    uint64_t previousTimeUS = PRIME_HAL_WRP_GetTimeUS64();
 
    uint64_t newTimeUS = PRIME_HAL_WRP_GetTimeUS64();

    if ((newTimeUS - previousTimeUS) > 1000000)
    {
    }
    </code>

  Remarks:
    Related to Time Management service.
*/
uint64_t PRIME_HAL_WRP_GetTimeUS64(void); 

// *****************************************************************************
/* Function:
    uint32_t PRIME_HAL_WRP_GetTimeUS(void)

  Summary:
    This primitive gets the value of a counter and converts it in a 32 bit 
    variable in microseconds.

  Description:
    This primitive makes use of SYS_TIME service to get the value of the 
    microseconds counter in order to be able to set timeouts and perform delays.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Value of microseconds in 32 bits.

  Example:
    <code>
    uint32_t previousTimeUS = PRIME_HAL_WRP_GetTimeUS();
 
    uint32_t newTimeUS = PRIME_HAL_WRP_GetTimeUS();

    if ((newTimeUS - previousTimeUS) > 1000000)
    {
    }
    </code>

  Remarks:
    Related to Time Management service.
*/
uint32_t PRIME_HAL_WRP_GetTimeUS(void);    




/** \brief Firmware Upgrade Interface */
/* @{ */
void prime_hal_fu_data_read(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
uint8_t prime_hal_fu_data_write(uint32_t addr, uint8_t *puc_buf, uint16_t us_size);
void prime_hal_fu_data_cfg_read(void *pv_dst, uint16_t us_size);
uint8_t prime_hal_fu_data_cfg_write(void *pv_src, uint16_t us_size);
void prime_hal_fu_start(hal_fu_info_t *x_fu_info);
void prime_hal_fu_end(hal_fu_result_t uc_hal_res);
void prime_hal_fu_revert(void);
void prime_hal_fu_crc_calculate(void);
void prime_hal_fu_crc_set_callback(void (*p_handler)(uint32_t ul_crc));
uint16_t prime_hal_fu_get_bitmap(uint8_t *puc_bitmap, uint32_t *pus_num_rcv_pages);
void prime_hal_fu_signature_image_check(void);
void prime_hal_fu_signature_image_check_set_callback(void (*p_handler)(hal_fu_verif_result_t uc_result));
void prime_hal_swap_stack(uint8_t uc_traffic);
/* @} */


/** \brief PAL Interface */


/** \brief RF Interface */
/* @{ */
<#if primePal.PRIME_PAL_RF_EN == true>
uint8_t prime_hal_prf_if_init(void);
void prime_hal_prf_if_reset(void);
void prime_hal_prf_if_enable_interrupt(bool b_enable);
void prime_hal_prf_if_set_handler(void (*p_handler)(void));
bool prime_hal_prf_if_send_spi_cmd(uint8_t *puc_data_buf, uint16_t us_addr, uint16_t us_len, uint8_t uc_mode);
bool prime_hal_prf_if_is_spi_busy(void);
void prime_hal_prf_if_led(uint8_t uc_led_id, bool b_led_on);
</#if>
/* @} */

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* PRIME_HAL_WRAPPER_H_INCLUDE */

/*******************************************************************************
 End of File
*/

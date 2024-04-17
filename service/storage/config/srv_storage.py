# coding: utf-8
"""
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
"""

storage_helpkeyword = "mcc_h3_prime_storage_service_configurations"

def instantiateComponent(primeStorageComponent):
    
    Log.writeInfoMessage("Loading Storage PRIME Service")

    processor = Variables.get("__PROCESSOR")

    if ("PIC32CX" in processor) and ("MT" in processor):
        userSignatureBlock = primeStorageComponent.createIntegerSymbol("SRV_STORAGE_USER_SIGNATURE_BLOCK", None)
        userSignatureBlock.setLabel("User Signature Block")
        userSignatureBlock.setDescription("User Signature block index used to store non-volatile data")
        userSignatureBlock.setDefaultValue(1)
        userSignatureBlock.setMin(1)
        userSignatureBlock.setMax(6)
        userSignatureBlock.setHelp(storage_helpkeyword)

        userSignaturePage = primeStorageComponent.createIntegerSymbol("SRV_STORAGE_USER_SIGNATURE_PAGE", None)
        userSignaturePage.setLabel("User Signature Page")
        userSignaturePage.setDescription("User Signature page index used to store non-volatile data")
        userSignaturePage.setDefaultValue(0)
        userSignaturePage.setMin(0)
        userSignaturePage.setMax(7)
        userSignaturePage.setHelp(storage_helpkeyword)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Storage Service Files
    srvStorageSourceFile = primeStorageComponent.createFileSymbol("SRV_STORAGE_SOURCE", None)
    srvStorageSourceFile.setSourcePath("service/storage/srv_storage.c.ftl")
    srvStorageSourceFile.setOutputName("srv_storage.c")
    srvStorageSourceFile.setDestPath("service/storage")
    srvStorageSourceFile.setProjectPath("config/" + configName + "/service/storage/")
    srvStorageSourceFile.setType("SOURCE")
    srvStorageSourceFile.setMarkup(True)
    srvStorageSourceFile.setOverwrite(True)
    srvStorageSourceFile.setEnabled(True)

    srvStorageHeaderFile = primeStorageComponent.createFileSymbol("SRV_STORAGE_HEADER", None)
    srvStorageHeaderFile.setSourcePath("service/storage/srv_storage.h")
    srvStorageHeaderFile.setOutputName("srv_storage.h")
    srvStorageHeaderFile.setDestPath("service/storage")
    srvStorageHeaderFile.setProjectPath("config/" + configName + "/service/storage/")
    srvStorageHeaderFile.setType("HEADER")
    srvStorageHeaderFile.setMarkup(False)
    srvStorageHeaderFile.setOverwrite(True)
    srvStorageHeaderFile.setEnabled(True)

    #### FreeMaker System Files ######################################################

    srvStorageSystemDefFile = primeStorageComponent.createFileSymbol("SRV_STORAGE_SYSTEM_DEF", None)
    srvStorageSystemDefFile.setType("STRING")
    srvStorageSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvStorageSystemDefFile.setSourcePath("service/storage/templates/system/system_definitions.h.ftl")
    srvStorageSystemDefFile.setMarkup(True)

    srvStorageSystemInitFile = primeStorageComponent.createFileSymbol("SRV_STORAGE_SYSTEM_INIT", None)
    srvStorageSystemInitFile.setType("STRING")
    srvStorageSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvStorageSystemInitFile.setSourcePath("service/storage/templates/system/system_initialize.c.ftl")
    srvStorageSystemInitFile.setMarkup(True)

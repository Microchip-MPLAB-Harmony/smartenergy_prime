# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

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

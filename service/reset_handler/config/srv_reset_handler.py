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

def instantiateComponent(primeResetHandlerComponent):
    
    Log.writeInfoMessage("Loading Reset Handler PRIME Service")

    # Enable reset handling
    primeResetEn = primeResetHandlerComponent.createBooleanSymbol("RESET_HANDLING_EN", None)
    primeResetEn.setLabel("Enable reset handling")
    primeResetEn.setDescription("Enable/disable reset handling")
    primeResetEn.setVisible(True)
    primeResetEn.setDefaultValue(True)
    primeResetEn.setReadOnly(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Storage Service Files
    srvResetHandlerSourceFile = primeResetHandlerComponent.createFileSymbol("SRV_RESET_HANDLER_SOURCE", None)
    srvResetHandlerSourceFile.setSourcePath("service/reset_handler/srv_reset_handler.c.ftl")
    srvResetHandlerSourceFile.setOutputName("srv_reset_handler.c")
    srvResetHandlerSourceFile.setDestPath("service/reset_handler")
    srvResetHandlerSourceFile.setProjectPath("config/" + configName + "/service/reset_handler/")
    srvResetHandlerSourceFile.setType("SOURCE")
    srvResetHandlerSourceFile.setMarkup(True)
    srvResetHandlerSourceFile.setOverwrite(True)
    srvResetHandlerSourceFile.setEnabled(True)

    srvResetHandlerHeaderFile = primeResetHandlerComponent.createFileSymbol("SRV_RESET_HANDLER_HEADER", None)
    srvResetHandlerHeaderFile.setSourcePath("service/reset_handler/srv_reset_handler.h")
    srvResetHandlerHeaderFile.setOutputName("srv_reset_handler.h")
    srvResetHandlerHeaderFile.setDestPath("service/reset_handler")
    srvResetHandlerHeaderFile.setProjectPath("config/" + configName + "/service/reset_handler/")
    srvResetHandlerHeaderFile.setType("HEADER")
    srvResetHandlerHeaderFile.setMarkup(False)
    srvResetHandlerHeaderFile.setOverwrite(True)
    srvResetHandlerHeaderFile.setEnabled(True)
    
    #### FreeMaker System Files ######################################################

    srvResetHandlerSystemDefFile = primeResetHandlerComponent.createFileSymbol("SRV_RESET_HANDLER_SYSTEM_DEF", None)
    srvResetHandlerSystemDefFile.setType("STRING")
    srvResetHandlerSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvResetHandlerSystemDefFile.setSourcePath("service/reset_handler/templates/system/system_definitions.h.ftl")
    srvResetHandlerSystemDefFile.setMarkup(True)

    srvResetHandlerSystemInitFile = primeResetHandlerComponent.createFileSymbol("SRV_RESET_HANDLER_SYSTEM_INIT", None)
    srvResetHandlerSystemInitFile.setType("STRING")
    srvResetHandlerSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvResetHandlerSystemInitFile.setSourcePath("service/reset_handler/templates/system/system_initialize.c.ftl")
    srvResetHandlerSystemInitFile.setMarkup(True)

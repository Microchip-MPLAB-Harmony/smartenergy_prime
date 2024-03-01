# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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

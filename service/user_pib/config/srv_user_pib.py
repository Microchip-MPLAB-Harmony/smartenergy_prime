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

def instantiateComponent(primeUserPibComponent):
    
    Log.writeInfoMessage("Loading User PIBs PRIME Service")

    # Enable default PIB handling
    primeDefaultPibEn = primeUserPibComponent.createBooleanSymbol("DEFAULT_PIB_HANDLING_EN", None)
    primeDefaultPibEn.setLabel("Enable default PIB handling")
    primeDefaultPibEn.setDescription("Enable/disable the default PIB handling")
    primeDefaultPibEn.setVisible(True)
    primeDefaultPibEn.setDefaultValue(True)
    primeDefaultPibEn.setReadOnly(False)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Storage Service Files
    srvUserPibSourceFile = primeUserPibComponent.createFileSymbol("SRV_USER_PIB_SOURCE", None)
    srvUserPibSourceFile.setSourcePath("service/user_pib/srv_user_pib.c.ftl")
    srvUserPibSourceFile.setOutputName("srv_user_pib.c")
    srvUserPibSourceFile.setDestPath("service/user_pib")
    srvUserPibSourceFile.setProjectPath("config/" + configName + "/service/user_pib/")
    srvUserPibSourceFile.setType("SOURCE")
    srvUserPibSourceFile.setMarkup(True)
    srvUserPibSourceFile.setOverwrite(True)
    srvUserPibSourceFile.setEnabled(True)

    srvUserPibHeaderFile = primeUserPibComponent.createFileSymbol("SRV_USER_PIB_HEADER", None)
    srvUserPibHeaderFile.setSourcePath("service/user_pib/srv_user_pib.h")
    srvUserPibHeaderFile.setOutputName("srv_user_pib.h")
    srvUserPibHeaderFile.setDestPath("service/user_pib")
    srvUserPibHeaderFile.setProjectPath("config/" + configName + "/service/user_pib/")
    srvUserPibHeaderFile.setType("HEADER")
    srvUserPibHeaderFile.setMarkup(False)
    srvUserPibHeaderFile.setOverwrite(True)
    srvUserPibHeaderFile.setEnabled(True)
    
    #### FreeMaker System Files ######################################################

    srvUserPibSystemDefFile = primeUserPibComponent.createFileSymbol("SRV_USER_PIB_SYSTEM_DEF", None)
    srvUserPibSystemDefFile.setType("STRING")
    srvUserPibSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvUserPibSystemDefFile.setSourcePath("service/user_pib/templates/system/system_definitions.h.ftl")
    srvUserPibSystemDefFile.setMarkup(True)

    srvUserPibSystemInitFile = primeUserPibComponent.createFileSymbol("SRV_USER_PIB_SYSTEM_INIT", None)
    srvUserPibSystemInitFile.setType("STRING")
    srvUserPibSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvUserPibSystemInitFile.setSourcePath("service/user_pib/templates/system/system_initialize.c.ftl")
    srvUserPibSystemInitFile.setMarkup(True)

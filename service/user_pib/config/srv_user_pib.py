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

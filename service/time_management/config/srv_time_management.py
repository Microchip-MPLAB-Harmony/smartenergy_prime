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
def instantiateComponent(primeTimeManagementComponent):
    
    Log.writeInfoMessage("Loading Time Management service for PRIME")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    
    # Time Management Files
    pTimeMgmtSource = primeTimeManagementComponent.createFileSymbol("SRV_TIME_MANAGEMENT_SOURCE", None)
    pTimeMgmtSource.setSourcePath("service/time_management/srv_time_management.c")
    pTimeMgmtSource.setOutputName("srv_time_management.c")
    pTimeMgmtSource.setDestPath("service/time_management")
    pTimeMgmtSource.setProjectPath("config/" + configName + "/service/time_management/")
    pTimeMgmtSource.setType("SOURCE")
    
    pTimeMgmtHeader = primeTimeManagementComponent.createFileSymbol("SRV_TIME_MANAGEMENT_HEADER", None)
    pTimeMgmtHeader.setSourcePath("service/time_management/srv_time_management.h")
    pTimeMgmtHeader.setOutputName("srv_time_management.h")
    pTimeMgmtHeader.setDestPath("service/time_management")
    pTimeMgmtHeader.setProjectPath("config/" + configName + "/service/time_management/")
    pTimeMgmtHeader.setType("HEADER")

    #### FreeMaker System Files ######################################################

    pTimeMgmtSystemDefFile = primeTimeManagementComponent.createFileSymbol("SRV_TIME_MANAGEMENT_DEF", None)
    pTimeMgmtSystemDefFile.setType("STRING")
    pTimeMgmtSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pTimeMgmtSystemDefFile.setSourcePath("service/time_management/templates/system/definitions.h.ftl")
    pTimeMgmtSystemDefFile.setMarkup(True)
  

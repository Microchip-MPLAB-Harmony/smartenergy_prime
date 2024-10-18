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
prime_fu_helpkeyword = "prime_fu_configuration"

def instantiateComponent(primeFirmwareUpgradeComponent):
    
    Log.writeInfoMessage("Loading Firmware Upgrade Handler PRIME Service")

    # Link with memory instance
    primeFUMemDrv = primeFirmwareUpgradeComponent.createStringSymbol("PRIME_FU_MEM_DRV", None)
    primeFUMemDrv.setLabel("Driver Type")
    primeFUMemDrv.setDefaultValue("")
    primeFUMemDrv.setVisible(True)
    primeFUMemDrv.setReadOnly(True)
    primeFUMemDrv.setHelp(prime_fu_helpkeyword)
    
    primeFUMemInstance = primeFirmwareUpgradeComponent.createIntegerSymbol("PRIME_FU_MEM_INSTANCE", None)
    primeFUMemInstance.setLabel("Memory Instance")
    primeFUMemDrv.setReadOnly(True)
    primeFUMemInstance.setVisible(True)
    primeFUMemInstance.setHelp(prime_fu_helpkeyword)

    primeFUMemOffset = primeFirmwareUpgradeComponent.createIntegerSymbol("PRIME_FU_MEM_OFFSET", None)
    primeFUMemOffset.setLabel("Firmware Upgrade Memory Offset")
    primeFUMemOffset.setVisible(True)
    primeFUMemOffset.setEnabled(True)
    primeFUMemOffset.setHelp(prime_fu_helpkeyword)

    primeFUMemSize = primeFirmwareUpgradeComponent.createIntegerSymbol("PRIME_FU_MEM_SIZE", None)
    primeFUMemSize.setLabel("Firmware Upgrade Memory Size")
    primeFUMemSize.setVisible(True)
    primeFUMemSize.setEnabled(True)
    primeFUMemSize.setHelp(prime_fu_helpkeyword)
   
    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    # Firwmare upgrade files
    srvFUSourceFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_SOURCE", None)
    srvFUSourceFile.setSourcePath("service/firmware_upgrade/srv_firmware_upgrade.c.ftl")
    srvFUSourceFile.setOutputName("srv_firmware_upgrade.c")
    srvFUSourceFile.setDestPath("service/firmware_upgrade")
    srvFUSourceFile.setProjectPath("config/" + configName + "/service/firmware_upgrade/")
    srvFUSourceFile.setType("SOURCE")
    srvFUSourceFile.setMarkup(True)
    srvFUSourceFile.setOverwrite(True)
    srvFUSourceFile.setEnabled(True)

    srvFUHeaderFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_HEADER", None)
    srvFUHeaderFile.setSourcePath("service/firmware_upgrade/srv_firmware_upgrade.h")
    srvFUHeaderFile.setOutputName("srv_firmware_upgrade.h")
    srvFUHeaderFile.setDestPath("service/firmware_upgrade")
    srvFUHeaderFile.setProjectPath("config/" + configName + "/service/firmware_upgrade/")
    srvFUHeaderFile.setType("HEADER")
    srvFUHeaderFile.setMarkup(False)
    srvFUHeaderFile.setOverwrite(True)
    srvFUHeaderFile.setEnabled(True)

    srvFUHeaderLocalFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_HEADER_LOCAL", None)
    srvFUHeaderLocalFile.setSourcePath("service/firmware_upgrade/srv_firmware_upgrade_local.h")
    srvFUHeaderLocalFile.setOutputName("srv_firmware_upgrade_local.h")
    srvFUHeaderLocalFile.setDestPath("service/firmware_upgrade")
    srvFUHeaderLocalFile.setProjectPath("config/" + configName + "/service/firmware_upgrade/")
    srvFUHeaderLocalFile.setType("HEADER")
    srvFUHeaderLocalFile.setMarkup(False)
    srvFUHeaderLocalFile.setOverwrite(True)
    srvFUHeaderLocalFile.setEnabled(True)
    
    #### FreeMaker System Files ######################################################

    srvFUSystemDefFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_SYSTEM_DEF", None)
    srvFUSystemDefFile.setType("STRING")
    srvFUSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvFUSystemDefFile.setSourcePath("service/firmware_upgrade/templates/system/system_definitions.h.ftl")
    srvFUSystemDefFile.setMarkup(True)

    srvFUSystemInitFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_SYSTEM_INIT", None)
    srvFUSystemInitFile.setType("STRING")
    srvFUSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvFUSystemInitFile.setSourcePath("service/firmware_upgrade/templates/system/system_initialize.c.ftl")
    srvFUSystemInitFile.setMarkup(True)

    srvFUSystemTasksFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_SYSTEM_TASK", None)
    srvFUSystemTasksFile.setType("STRING")
    srvFUSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
    srvFUSystemTasksFile.setSourcePath("service/firmware_upgrade/templates/system/system_tasks.c.ftl")
    srvFUSystemTasksFile.setMarkup(True)



def onAttachmentConnected(source, target):
    localComponent = source["component"]
    connectID = source["id"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    targetID = target["id"]

    if connectID == "Memory_dep" :
        print("Connecting Memory_dep")
        primeFUMemDrv = localComponent.getSymbolByID("PRIME_FU_MEM_DRV")
        primeFUMemDrv.setValue(str(remoteID))
        primeFUMemInstance = localComponent.getSymbolByID("PRIME_FU_MEM_INSTANCE")
        primeFUMemInstance.setValue(remoteComponent.getInstanceNumber())

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    connectID = source["id"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    targetID = target["id"]

    if connectID == "Memory_dep" :
        print("Desconnecting Memory_dep")

        primeFUMemDrv = localComponent.getSymbolByID("PRIME_FU_MEM_DRV")
        primeFUMemDrv.clearValue()
        primeFUMemInstance = localComponent.getSymbolByID("PRIME_FU_MEM_INSTANCE")
        primeFUMemInstance.clearValue()


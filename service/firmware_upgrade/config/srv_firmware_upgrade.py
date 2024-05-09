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

    # Storage Service Files
    srvFUSourceFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_FU_SOURCE", None)
    srvFUSourceFile.setSourcePath("service/firmware_upgrade/srv_firmware_upgrade.c")
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
    
    #### FreeMaker System Files ######################################################

    srvFUSystemDefFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_RESET_HANDLER_SYSTEM_DEF", None)
    srvFUSystemDefFile.setType("STRING")
    srvFUSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    srvFUSystemDefFile.setSourcePath("service/firmware_upgrade/templates/system/system_definitions.h.ftl")
    srvFUSystemDefFile.setMarkup(True)

    srvFUSystemInitFile = primeFirmwareUpgradeComponent.createFileSymbol("SRV_RESET_HANDLER_SYSTEM_INIT", None)
    srvFUSystemInitFile.setType("STRING")
    srvFUSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    srvFUSystemInitFile.setSourcePath("service/firmware_upgrade/templates/system/system_initialize.c.ftl")
    srvFUSystemInitFile.setMarkup(True)

'''
15:59:15.987    INFO: {Harmony}<Harmony Database> -> satisfied: HarmonyCoreService, drv_memory_HarmonyCoreDependency
15:59:15.987    FINE: {Harmony}<Event>[Info]: Broadcasting event: ComponentActivatedEvent - drv_memory_0
15:59:15.987    FINE: {Harmony}<Event>[Info]: Broadcasting event: Graph.RepaintEvent - 
15:59:15.987    FINE: {Harmony}<Event>[Info]: Broadcasting event: RefreshUIEvent - 
15:59:15.988    FINE: {Harmony}<Event>[Info]: Broadcasting event: ComponentAttachmentStateChanged - [drv_memory_0:drv_media:DRV_MEDIA]
15:59:15.988    FINE: {Harmony}<Event>[Info]: Broadcasting event: Graph.RepaintEvent - 
15:59:15.988    FINE: {Harmony}<Event>[Info]: Broadcasting event: ComponentAttachmentStateChanged - [primeFirwmareUpgrade:Memory_dep:DRV_MEDIA]
15:59:15.988    FINE: {Harmony}<Event>[Info]: Broadcasting event: Graph.RepaintEvent - 
15:59:15.989    INFO: {Harmony}<Harmony Database> -> satisfied: Memory_dep, drv_media

primeFUMemDrv = primeFirmwareUpgradeComponent.createStringSymbol("PRIME_FU_MEM_DRV", None)
primeFUMemInstance = primeFirmwareUpgradeComponent.createIntegerSymbol("PRIME_FU_MEM_INSTANCE", None)

'''

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    connectID = source["id"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    targetID = target["id"]

    if connectID == "Memory_dep" :
        print("DD Debug: Connecting Memory_dep")
        print("DD Debug: Local component" + localComponent)
        print("DD Debug: Remote component " + str(remoteID) + " instance number " + str(remoteComponent.getInstanceNumber()))

    print("DD satisfied: " + connectID + ", " + targetID)

def onAttachmentDisconnected(source, target):
    localComponent = source["component"]
    localConnectID = source["id"]
    remoteComponent = target["component"]
    remoteComponentID = remoteComponent.getID()

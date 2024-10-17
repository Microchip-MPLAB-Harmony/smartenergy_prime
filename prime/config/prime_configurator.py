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

global primeServicesGroup

# Configuration parameters
global primeConfigMode
global primeConfigBnSlaveEn
global primeConfigProject
global primeConfigVersion
global primeConfigComment
global primeConfigFWVendor
global primeConfigFWModel
global primeConfigFWVersion
global primeConfigPIBVendor
global primeConfigPIBModel
global primeMacConfig
global primeConfigSecProfile
global primeConfigMaxNumNodes
global primeMngpConfig
global primeConfigMngpSprof
global primeConfigSprofUsiPort

# Memory Regions (SN)
global primeSNAppAddress
global primeSNPFWStack14Address
global primeSNPHYAddress
global primeSNPFWStack13Address
global primeSNAppSize
global primeSNPFWStack14Size
global primeSNPHYSize
global primeSNPFWStack13Size
global primeFUComment
global primeBNFUComment

global primeSNAppMetaData
global primeSNPFWStack14MetaData
global primeSNSNPHYMetaData
global primeSNPFWStack13MetaData

# Files
global pPrimeHalWrapperHeaderFile
global pPrimeHalWrapperSourceFile
global pMacHeaderFile
global pMngpHeaderFile
global pBmngDefsHeaderFile
global pBmngApiHeaderFile
global pClNullHeaderFile
global pClNullApiHeaderFile
global pCl432HeaderFile
global pCl432ApiHeaderFile
global pHalApiSourceFile
global pPalTypesFile
global pPalHeaderFile
global pPrimeHeaderFile
global pPrimeSourceFile
global pPrimeLocalHeaderFile

# Libraries
global pPrime13BnLibFile
global pPrime13SnLibFile
global pPrime14BnMLibFile
global pPrime14SnLibFile

# Linker Options
global pPrimeXc32LdPrepMacroSym

PRIME_USER_APP_OFFSET_HEX = "0x10000"
PRIME_USER_APP_SIZE_HEX = "0x40000"
PRIME_USER_APP_ID = "APPBIN"
PRIME_FW_STACK_14_OFFSET_HEX = "0x90000"
PRIME_FW_STACK_14_SIZE_HEX = "0x22000"
PRIME_FW_STACK_14_ID = "MAC14BIN"
PRIME_PHY_OFFSET_HEX = "0xB8000"
PRIME_PHY_SIZE_HEX = "0x18000"
PRIME_PHY_ID = "PHYBIN"
PRIME_FW_STACK_13_OFFSET_HEX = "0xD0000"
PRIME_FW_STACK_13_SIZE_HEX = "0x20000"
PRIME_FW_STACK_13_ID = "MAC13BIN"

PRIME_FW_STACK_RAM_SIZE = "0x0000B000"  # TBD !!!!!

def getFlashMemoryDescription():
    nodeIFLASH = ATDF.getNode("/avr-tools-device-file/devices/device/address-spaces/address-space/memory-segment@[type=\"flash\"]")
    if nodeIFLASH is not None:
        return (nodeIFLASH.getAttribute("start"), nodeIFLASH.getAttribute("size"))

    return (0, 0)

def getRamMemoryDescription():
    nodeIRAM = ATDF.getNode("/avr-tools-device-file/devices/device/address-spaces/address-space/memory-segment@[type=\"ram\"]")
    if nodeIRAM is not None:
        return (nodeIRAM.getAttribute("start"), nodeIRAM.getAttribute("size"))

    return (0, 0)

def setFwStackRamParams(primeMode, primeProject):
    global pPrimeXc32LdPrepMacroSym

    ramStartAddressHex, ramSizeHex = getRamMemoryDescription()

    if (primeMode == "SN"):
        # Service Node
        if (primeProject == "application project"):
            # Service Node App
            ramOrigin = ramStartAddressHex
            ramLength = hex(int(ramSizeHex, 0) - int(PRIME_FW_STACK_RAM_SIZE, 0))
        else:
            # FW Stack App
            ramOrigin = hex(int(ramStartAddressHex, 0) + int(ramSizeHex, 0) - int(PRIME_FW_STACK_RAM_SIZE, 0))
            ramLength = PRIME_FW_STACK_RAM_SIZE
    else:
        # Base Node App
        ramOrigin = ramStartAddressHex
        ramLength = ramSizeHex

    ram_origin  = "RAM_ORIGIN=" + ramOrigin
    ram_length  = "RAM_LENGTH=" + ramLength

    ramParams = (ram_origin + ";" + ram_length)
    pPrimeXc32LdPrepMacroSym.setValue(ramParams)

def createGroupServices():
    global primeServicesGroup
    primeServicesGroup = Database.findGroup("PRIME SERVICES")
    if (primeServicesGroup == None):
        primeServicesGroup = Database.createGroup("PRIME STACK", "PRIME SERVICES")
        primeServices = ["srvRandom", "srvQueue", "srvLogReport", "srv_pcrc", "srvSecurity", "primeStorage", "primeUserPib", "primeResetHandler", "primeTimeManagement"]
        for component in primeServices:
            primeServicesGroup.addComponent(component)
        Database.activateComponents(primeServices, "PRIME SERVICES")

    primeServicesRootGroup = ["primeFirwmareUpgrade"]
    Database.activateComponents(primeServicesRootGroup)
        
    # Debug traces enabled
    setVal("srvLogReport", "ENABLE_TRACES", True)

def primeAddBnFiles():
    # BN API files
    pPrimeHeaderFile.setEnabled(True)
    pPrimeSourceFile.setEnabled(True)
    pPrimeLocalHeaderFile.setEnabled(True)
    pPrimeHalWrapperHeaderFile.setEnabled(True)
    pPrimeHalWrapperSourceFile.setEnabled(True)
    pMacHeaderFile.setEnabled(True)
    pMngpHeaderFile.setEnabled(True)
    pBmngApiHeaderFile.setEnabled(True)
    pBmngDefsHeaderFile.setEnabled(True)
    pClNullHeaderFile.setEnabled(True)
    pClNullApiHeaderFile.setEnabled(True)
    pCl432HeaderFile.setEnabled(True)
    pCl432ApiHeaderFile.setEnabled(True)
    pHalApiSourceFile.setEnabled(True)
    pPalTypesFile.setEnabled(False)
    pPalHeaderFile.setEnabled(False)

    # No libraries by default
    pPrime13BnLibFile.setEnabled(False)
    pPrime13SnLibFile.setEnabled(False)
    pPrime14BnMLibFile.setEnabled(False)
    pPrime14SnLibFile.setEnabled(False)
    
    processor = Variables.get("__PROCESSOR")

    # BN library
    if (primeConfigVersion.getValue() == "1.3.6"):
        if (primeConfigBnSlaveEn.getValue() == True):
            pPrime13SnLibFile.setEnabled(True)
            pBmngApiHeaderFile.setEnabled(False)
            pBmngDefsHeaderFile.setEnabled(False)
        else:
            pPrime13BnLibFile.setEnabled(True)
    elif (primeConfigVersion.getValue() == "1.4"):
        if ("PIC32CX" in processor) and ("MT" in processor):
            pPrime14BnMLibFile.setEnabled(True)
        else:
            # Other libraries and platforms TBD
            pass
    else:
        # Unknown option: no library by default
        pass

def primeAddSnFiles():
    # BN only
    pBmngApiHeaderFile.setEnabled(False)
    pBmngDefsHeaderFile.setEnabled(False)

    # No libraries by default
    pPrime13BnLibFile.setEnabled(False)
    pPrime13SnLibFile.setEnabled(False)
    pPrime14BnMLibFile.setEnabled(False)
    pPrime14SnLibFile.setEnabled(False)

    if (primeConfigProject.getValue() == "bin project"):
        # SN API files in bin
        pPrimeHeaderFile.setEnabled(False)
        pPrimeSourceFile.setEnabled(False)
        pPrimeLocalHeaderFile.setEnabled(False)
        pPrimeHalWrapperHeaderFile.setEnabled(True)
        pPrimeHalWrapperSourceFile.setEnabled(True)
        pMacHeaderFile.setEnabled(True)
        pMngpHeaderFile.setEnabled(True)
        pClNullHeaderFile.setEnabled(True)
        pClNullApiHeaderFile.setEnabled(True)
        pCl432HeaderFile.setEnabled(True)
        pCl432ApiHeaderFile.setEnabled(True)
        pHalApiSourceFile.setEnabled(False)
        pPalTypesFile.setEnabled(True)
        pPalHeaderFile.setEnabled(True)

        # SN library
        if (primeConfigVersion.getValue() == "1.3.6"):
            pPrime13SnLibFile.setEnabled(True)
        elif (primeConfigVersion.getValue() == "1.4"):
            pPrime14SnLibFile.setEnabled(True)
        else:
            # Unknown option: no library by default
            pass
    elif (primeConfigProject.getValue() == "application project"):
        # SN API files in application
        pPrimeHeaderFile.setEnabled(True)
        pPrimeSourceFile.setEnabled(True)
        pPrimeLocalHeaderFile.setEnabled(True)
        pPrimeHalWrapperHeaderFile.setEnabled(False)
        pPrimeHalWrapperSourceFile.setEnabled(False)
        pMacHeaderFile.setEnabled(False)
        pMngpHeaderFile.setEnabled(False)
        pClNullHeaderFile.setEnabled(False)
        pClNullApiHeaderFile.setEnabled(False)
        pCl432HeaderFile.setEnabled(False)
        pCl432ApiHeaderFile.setEnabled(False)
        pHalApiSourceFile.setEnabled(True)
        pPalTypesFile.setEnabled(False)
        pPalHeaderFile.setEnabled(False)

        # No library in SN application
        pass
    else:
        # Unknown option: no library by default
        pass

def primeUpdateFiles(primeStackComponent):
    addPALFUComponent = False
    
    if (primeConfigMode.getValue() == "SN"):
        # Add files for SN
        primeAddSnFiles()
        if (primeConfigProject.getValue() == "application project"):
            addPALFUComponent = True

    elif (primeConfigMode.getValue() == "BN"):
        # Add files for BN
        primeAddBnFiles()
        # Add PRIME components
        addPALFUComponent = True

    if (addPALFUComponent == True):
        # Add PRIME components
        Database.activateComponents(["primePal"], "PRIME STACK")
        primeStackComponent.setDependencyEnabled("primePal_dep", True)
        primeStackComponent.setDependencyEnabled("primeFu_dep", True)
    else:
        # Remove PRIME components
        Database.deactivateComponents(["primePal"])
        primeStackComponent.setDependencyEnabled("primePal_dep", False)
        primeStackComponent.setDependencyEnabled("primeFu_dep", False)
    
        
def primeShowSprofUsiInstance(symbol, event):
    symbol.setVisible(event["value"])

    if (primeConfigMode.getValue() == "BN"):
        if (event["value"] == True):
            usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
            symbol.setMax(len(usiInstances) - 1)
    else:
        if (primeConfigProject.getValue() == "application project"):
            if (event["value"] == True):
                usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
                symbol.setMax(len(usiInstances) - 1)
        else:
            # In the SN bin project, there is no USI block
            symbol.setMax(10)

def primeShowSNAppConfiguration(primeVersion):
    # Valid options
    primeConfigMode.setVisible(True)
    primeConfigProject.setVisible(True)
    primeConfigComment.setVisible(True)
    primeConfigFWVersion.setVisible(True)
    primeMngpConfig.setVisible(True)
    primeConfigMngpSprof.setVisible(True)
    primeConfigSprofUsiPort.setVisible(True)

    primeSNAppAddress.setVisible(True)
    primeFUComment.setVisible(True)
    primeSNPHYAddress.setVisible(True)
    primeSNPFWStack14Address.setVisible(True)
    primeSNPFWStack13Address.setVisible(True)

    primeSNAppSize.setVisible(True)
    primeSNPHYSize.setVisible(True)
    primeSNPFWStack14Size.setVisible(True)
    primeSNPFWStack13Size.setVisible(True)

    primeSNAppMetaData.setVisible(True)
    primeSNPFWStack14MetaData.setVisible(True)
    primeSNSNPHYMetaData.setVisible(True)
    primeSNPFWStack13MetaData.setVisible(True)

    # Hide SN Application options
    primeConfigBnSlaveEn.setVisible(False)
    primeConfigBnSlaveEn.setValue(False)
    primeConfigVersion.setVisible(False)
    primeConfigFWVendor.setVisible(False)
    primeConfigFWModel.setVisible(False)
    primeConfigPIBVendor.setVisible(False)
    primeConfigPIBModel.setVisible(False)
    primeMacConfig.setVisible(False)
    primeConfigMaxNumNodes.setVisible(False)
    primeConfigSecProfile.setVisible(False)

    if (primeVersion == "1.4"):
        primeConfigFWVersion.setValue("HS14.01.01")
    else:
        primeConfigFWVersion.setValue("S13.01.01")

def primeShowSNBinConfiguration(primeVersion):
    # Valid options
    primeConfigProject.setVisible(True)
    primeConfigMode.setVisible(True)
    primeConfigVersion.setVisible(True)
    primeConfigFWVendor.setVisible(True)
    primeConfigFWModel.setVisible(True)
    primeConfigFWVersion.setVisible(True)
    primeConfigPIBVendor.setVisible(True)
    primeConfigPIBModel.setVisible(True)
    primeMacConfig.setVisible(True)
    primeConfigSecProfile.setVisible(True)

    # Hide SN Binary options
    primeConfigMaxNumNodes.setVisible(False)
    primeConfigMaxNumNodes.setValue(0)
    primeConfigComment.setVisible(False)
    primeConfigBnSlaveEn.setVisible(False)
    primeConfigBnSlaveEn.setValue(False)
    primeMngpConfig.setVisible(False)
    primeConfigMngpSprof.setVisible(False)
    primeConfigSprofUsiPort.setVisible(False)

    primeSNAppAddress.setVisible(False)
    primeFUComment.setVisible(False)
    primeSNPHYAddress.setVisible(False)

    primeSNAppSize.setVisible(False)
    primeSNPHYSize.setVisible(False)

    primeSNAppMetaData.setVisible(False)
    primeSNPFWStack14MetaData.setVisible(False)
    primeSNSNPHYMetaData.setVisible(False)
    primeSNPFWStack13MetaData.setVisible(False)

    if (primeVersion == "1.4"):
        # SN - v1.4
        primeConfigSecProfile.setReadOnly(False)
        primeConfigFWVersion.setValue("HS14.01.01")
        primeSNPFWStack14Address.setVisible(False)
        primeSNPFWStack14Size.setVisible(False)
        primeSNPFWStack13Address.setVisible(False)
        primeSNPFWStack13Size.setVisible(False)
    else:
        # SN - v1.3.6
        primeConfigSecProfile.setValue(0)
        primeConfigSecProfile.setReadOnly(True)
        primeConfigFWVersion.setValue("S13.01.01")
        primeSNPFWStack14Address.setVisible(False)
        primeSNPFWStack14Size.setVisible(False)
        primeSNPFWStack13Address.setVisible(True)
        primeSNPFWStack13Size.setVisible(True)

def primeShowBNConfiguration(primeVersion):
    # Valid options
    primeConfigMode.setVisible(True)
    primeConfigVersion.setVisible(True)
    primeConfigFWVendor.setVisible(True)
    primeConfigFWModel.setVisible(True)
    primeConfigFWVersion.setVisible(True)
    primeConfigPIBVendor.setVisible(True)
    primeConfigPIBModel.setVisible(True)
    primeMacConfig.setVisible(True)
    primeConfigMaxNumNodes.setVisible(False)
    primeMngpConfig.setVisible(True)
    primeConfigMngpSprof.setVisible(True)
    primeConfigSprofUsiPort.setVisible(True)
    primeConfigSecProfile.setVisible(True)
    primeFUComment.setVisible(True)

    # Hide BN options
    primeConfigProject.setVisible(False)
    primeConfigComment.setVisible(False)

    primeSNAppAddress.setVisible(False)
    primeSNPHYAddress.setVisible(False)
    primeSNPFWStack14Address.setVisible(False)
    primeSNPFWStack13Address.setVisible(False)

    primeSNAppSize.setVisible(False)
    primeSNPHYSize.setVisible(False)
    primeSNPFWStack14Size.setVisible(False)
    primeSNPFWStack13Size.setVisible(False)

    primeSNAppMetaData.setVisible(False)
    primeSNPFWStack14MetaData.setVisible(False)
    primeSNSNPHYMetaData.setVisible(False)
    primeSNPFWStack13MetaData.setVisible(False)

    if (primeVersion == "1.4"):
        # BN - v1.4
        primeConfigBnSlaveEn.setValue(False)
        primeConfigBnSlaveEn.setVisible(False)
        primeConfigSecProfile.setReadOnly(False)
        primeConfigFWVersion.setValue("HB14.01.01")
    else:
        # BN - v1.3.6
        primeConfigBnSlaveEn.setVisible(True)
        primeConfigSecProfile.setValue(0)
        primeConfigSecProfile.setReadOnly(True)
        primeConfigFWVersion.setValue("B13.01.01")

def primeUpdateConfiguration(symbol, event):
    primeMode = primeConfigMode.getValue()
    primeProject = primeConfigProject.getValue()
    primeVersion = primeConfigVersion.getValue()

    if (primeMode == "SN"):
        # Service Node
        if (primeProject == "application project"):
            # SN - Application project
            primeShowSNAppConfiguration(primeVersion)
            # Start address
            startAddress = primeSNAppAddress.getValue()
            # Enable PLC Phy driver static addressing
            phyStartAddress = int(primeSNPHYAddress.getValue(), 0)
            phySize = int(primeSNPHYSize.getValue(), 0)
            Database.sendMessage("drvPlcPhy", "SET_STATIC_ADDRESSING", {"enable":True, "address":phyStartAddress, "size":phySize})
        else:
            # SN - Bin project
            primeShowSNBinConfiguration(primeVersion)
            # Start address
            if (primeVersion == "1.4"):
                startAddress = primeSNPFWStack14Address.getValue()
            else:
                startAddress = primeSNPFWStack13Address.getValue()
    else:
        # Base Node
        primeShowBNConfiguration(primeVersion)
        # Start address
        memStartAddressHex, memSizeHex = getFlashMemoryDescription()
        startAddress = memStartAddressHex
        # Disable PLC Phy driver static addressing
        Database.sendMessage("drvPlcPhy", "SET_STATIC_ADDRESSING", {"enable":False, "address":0, "size":0})

    # Add files
    localComponent = symbol.getComponent()
    primeUpdateFiles(localComponent)

    # Set RAM linker properties
    setFwStackRamParams(primeMode, primeProject)
    # Set Application Start Address
    Database.sendMessage("core", "APP_START_ADDRESS", {"start_address":startAddress})

def instantiateComponent(primeStackConfigComponent):
    Log.writeInfoMessage("Loading Stack Configurator for PRIME")

    processor = Variables.get("__PROCESSOR")

    primeStackGroup = Database.findGroup("PRIME STACK")
    if (primeStackGroup == None):
        primeStackGroup = Database.createGroup(None, "PRIME STACK")
    Database.setActiveGroup("PRIME STACK")
    primeStackGroup.addComponent("prime_stack_config")

    # Create group for all PRIME SERVICES
    createGroupServices()

    # Enable PAL by default for SN (SN application is the default configuration)
    Database.activateComponents(["primePal"], "PRIME STACK")
    primeStackConfigComponent.setDependencyEnabled("primePal_dep", True)
    primeStackConfigComponent.setDependencyEnabled("primeFu_dep", True)
    
    # Configure PRIME Stack
    primeStackConfig = primeStackConfigComponent.createMenuSymbol("PRIME_Stack_Configuration", None)
    primeStackConfig.setLabel("PRIME Stack Configuration")
    primeStackConfig.setDescription("Configure the PRIME Stack general options")
    primeStackConfig.setVisible(True)

    # Select mode
    global primeConfigMode
    primeModes = ["BN", "SN"]
    primeConfigMode = primeStackConfigComponent.createComboSymbol("PRIME_MODE", primeStackConfig, primeModes)
    primeConfigMode.setLabel("PRIME mode")
    primeConfigMode.setDescription("Select the PRIME mode: base or service node")
    primeConfigMode.setVisible(True)
    primeConfigMode.setDefaultValue("SN")
    
    # Select type of project
    global primeConfigProject
    primeProjectOptions = ["application project", "bin project"]
    primeConfigProject = primeStackConfigComponent.createComboSymbol("PRIME_PROJECT", primeStackConfig, primeProjectOptions)
    primeConfigProject.setLabel("Type of PRIME project for separated applications")
    primeConfigProject.setDescription("Select the type of PRIME project for separated applications")
    primeConfigProject.setVisible(True)
    primeConfigProject.setDefaultValue("application project")

    memStartAddressHex, memSizeHex = getFlashMemoryDescription()

    global primeSNAppAddress
    primeSNAppAddress = primeStackConfigComponent.createStringSymbol("PRIME_SN_APP_ADDRESS", primeStackConfig)
    primeSNAppAddress.setLabel("User Application Address")
    primeSNAppAddress.setDescription("PRIME SN User Application region start address")
    primeSNAppAddress.setVisible(True)
    address = hex(int(memStartAddressHex, 0) + int(PRIME_USER_APP_OFFSET_HEX, 0))
    primeSNAppAddress.setDefaultValue(address)

    global primeSNAppSize
    primeSNAppSize = primeStackConfigComponent.createStringSymbol("PRIME_SN_APP_SIZE", primeStackConfig)
    primeSNAppSize.setLabel("User Application Size")
    primeSNAppSize.setDescription("PRIME SN User Application size in bytes")
    primeSNAppSize.setVisible(True)
    primeSNAppSize.setDefaultValue(PRIME_USER_APP_SIZE_HEX)

    global primeSNAppMetaData
    primeSNAppMetaData = primeStackConfigComponent.createStringSymbol("PRIME_SN_APP_METADATA", primeStackConfig)
    primeSNAppMetaData.setLabel("User Application ID")
    primeSNAppMetaData.setDescription("PRIME SN User Application Metadata")
    primeSNAppMetaData.setVisible(True)
    primeSNAppMetaData.setDefaultValue(PRIME_USER_APP_ID)

    global primeSNPFWStack14Address
    primeSNPFWStack14Address = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK14_ADDRESS", primeStackConfig)
    primeSNPFWStack14Address.setLabel("FW Stack v1.4 Address")
    primeSNPFWStack14Address.setDescription("PRIME v1.4 FW Stack region start address")
    primeSNPFWStack14Address.setVisible(True)
    address = hex(int(memStartAddressHex, 0) + int(PRIME_FW_STACK_14_OFFSET_HEX, 0))
    primeSNPFWStack14Address.setDefaultValue(address)

    global primeSNPFWStack14Size
    primeSNPFWStack14Size = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK14_SIZE", primeStackConfig)
    primeSNPFWStack14Size.setLabel("FW Stack v1.4 Size")
    primeSNPFWStack14Size.setDescription("PRIME SN FW Stack v1.4 size in bytes")
    primeSNPFWStack14Size.setVisible(True)
    primeSNPFWStack14Size.setDefaultValue(PRIME_FW_STACK_14_SIZE_HEX)

    global primeSNPFWStack14MetaData
    primeSNPFWStack14MetaData = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK14_METADATA", primeStackConfig)
    primeSNPFWStack14MetaData.setLabel("FW Stack v1.4 ID")
    primeSNPFWStack14MetaData.setDescription("FW Stack v1.4 Metadata")
    primeSNPFWStack14MetaData.setVisible(True)
    primeSNPFWStack14MetaData.setDefaultValue(PRIME_FW_STACK_14_ID)

    global primeSNPHYAddress
    primeSNPHYAddress = primeStackConfigComponent.createStringSymbol("PRIME_SN_PHY_ADDRESS", primeStackConfig)
    primeSNPHYAddress.setLabel("PHY Layer Address")
    primeSNPHYAddress.setDescription("PRIME PHY layer region start address")
    primeSNPHYAddress.setVisible(True)
    address = hex(int(memStartAddressHex, 0) + int(PRIME_PHY_OFFSET_HEX, 0))
    primeSNPHYAddress.setDefaultValue(address)

    global primeSNPHYSize
    primeSNPHYSize = primeStackConfigComponent.createStringSymbol("PRIME_SN_PHY_SIZE", primeStackConfig)
    primeSNPHYSize.setLabel("PHY Layer Size")
    primeSNPHYSize.setDescription("PRIME SN PHY Layer size in bytes")
    primeSNPHYSize.setVisible(True)
    primeSNPHYSize.setDefaultValue(PRIME_PHY_SIZE_HEX)

    global primeSNSNPHYMetaData
    primeSNSNPHYMetaData = primeStackConfigComponent.createStringSymbol("PRIME_SN_PHY_METADATA", primeStackConfig)
    primeSNSNPHYMetaData.setLabel("PHY Layer ID")
    primeSNSNPHYMetaData.setDescription("PHY Layer Metadata")
    primeSNSNPHYMetaData.setVisible(True)
    primeSNSNPHYMetaData.setDefaultValue(PRIME_PHY_ID)

    global primeSNPFWStack13Address
    primeSNPFWStack13Address = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK13_ADDRESS", primeStackConfig)
    primeSNPFWStack13Address.setLabel("FW Stack v1.3 Address")
    primeSNPFWStack13Address.setDescription("PRIME v1.3 FW Stack region start address")
    primeSNPFWStack13Address.setVisible(True)
    address = hex(int(memStartAddressHex, 0) + int(PRIME_FW_STACK_13_OFFSET_HEX, 0))
    primeSNPFWStack13Address.setDefaultValue(address)

    global primeSNPFWStack13Size
    primeSNPFWStack13Size = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK13_SIZE", primeStackConfig)
    primeSNPFWStack13Size.setLabel("FW Stack v1.3 Size")
    primeSNPFWStack13Size.setDescription("PRIME SN FW Stack v1.3 size in bytes")
    primeSNPFWStack13Size.setVisible(True)
    primeSNPFWStack13Size.setDefaultValue(PRIME_FW_STACK_13_SIZE_HEX)

    global primeSNPFWStack13MetaData
    primeSNPFWStack13MetaData = primeStackConfigComponent.createStringSymbol("PRIME_SN_FWSTACK13_METADATA", primeStackConfig)
    primeSNPFWStack13MetaData.setLabel("FW Stack v1.3 ID")
    primeSNPFWStack13MetaData.setDescription("PRIME SN FW Stack v1.3 Metadata")
    primeSNPFWStack13MetaData.setVisible(True)
    primeSNPFWStack13MetaData.setDefaultValue(PRIME_FW_STACK_13_ID)

    # The FW Image address is configured by PRIME FU Service
    global primeFUComment
    primeFUComment = primeStackConfigComponent.createCommentSymbol("PRIME_FU_COMMENT", primeStackConfig)
    primeFUComment.setLabel("*** PRIME FU address must be configured using the PRIME FU Service ***")
    primeFUComment.setVisible(True)

    # Dummy symbol to update files of the PRIME Stack
    primeStackAddressDummy = primeStackConfigComponent.createMenuSymbol("PRIME_Stack_Address_Dummy", None)
    primeStackAddressDummy.setLabel("")
    primeStackAddressDummy.setDescription("")
    primeStackAddressDummy.setVisible(False)
    primeStackAddressDummy.setDependencies(primeUpdateConfiguration, ["PRIME_SN_APP_ADDRESS", "PRIME_SN_FWSTACK14_ADDRESS", "PRIME_SN_FWSTACK13_ADDRESS"])

    # Enable BN slave
    global primeConfigBnSlaveEn
    primeConfigBnSlaveEn = primeStackConfigComponent.createBooleanSymbol("BN_SLAVE_EN", primeStackConfig)
    primeConfigBnSlaveEn.setLabel("Enable BN Slave")
    primeConfigBnSlaveEn.setDescription("Enable/disable the BN slave functionality")
    primeConfigBnSlaveEn.setVisible(False)
    primeConfigBnSlaveEn.setDefaultValue(False)

    # Select version
    global primeConfigVersion
    primeVersions = ["1.3.6", "1.4"]
    primeConfigVersion = primeStackConfigComponent.createComboSymbol("PRIME_VERSION", primeStackConfig, primeVersions)
    primeConfigVersion.setLabel("PRIME version")
    primeConfigVersion.setDescription("Select the PRIME version: 1.3.6 or 1.4")
    primeConfigVersion.setVisible(False)
    primeConfigVersion.setDefaultValue("1.4")

    # The SN application project cannot configure more than the previous parameters
    global primeConfigComment
    primeConfigComment = primeStackConfigComponent.createCommentSymbol("PRIME_CONFIG_COMMENT", primeStackConfig)
    primeConfigComment.setLabel("*** Configure the PRIME Stack of the Service Node in the bin project ***")
    primeConfigComment.setVisible(True)

    # Select PRIME FW vendor
    global primeConfigFWVendor
    primeConfigFWVendor = primeStackConfigComponent.createStringSymbol("PRIME_FW_VENDOR", primeStackConfig)
    primeConfigFWVendor.setLabel("PRIME FW Vendor")
    primeConfigFWVendor.setDescription("Select the PRIME FW vendor")
    primeConfigFWVendor.setVisible(False)
    primeConfigFWVendor.setDefaultValue("MCHP")

    # Select PRIME FW model
    global primeConfigFWModel
    primeConfigFWModel = primeStackConfigComponent.createStringSymbol("PRIME_FW_MODEL", primeStackConfig)
    primeConfigFWModel.setLabel("PRIME FW Model")
    primeConfigFWModel.setDescription("Select the PRIME FW model")
    primeConfigFWModel.setVisible(False)
    if ("PIC32CX" in processor) and ("MT" in processor):
        primeConfigFWModel.setDefaultValue("PIC32CXXPL460")
    elif ("ATSAME70" in processor):
        primeConfigFWModel.setDefaultValue("PL360BN")
    else:
        primeConfigFWModel.setDefaultValue("OTHER")

    # Select PRIME FW version
    global primeConfigFWVersion
    primeConfigFWVersion = primeStackConfigComponent.createStringSymbol("PRIME_FW_VERSION", primeStackConfig)
    primeConfigFWVersion.setLabel("PRIME FW Version")
    primeConfigFWVersion.setDescription("Select the PRIME FW version")
    primeConfigFWVersion.setVisible(False)
    primeConfigFWVersion.setDefaultValue("HS14.01.01\0\0\0\0\0\0")

    # Select PRIME PIB vendor
    global primeConfigPIBVendor
    primeConfigPIBVendor = primeStackConfigComponent.createHexSymbol("PRIME_PIB_VENDOR", primeStackConfig)
    primeConfigPIBVendor.setLabel("PRIME PIB Vendor")
    primeConfigPIBVendor.setDescription("Select the PRIME PIB vendor")
    primeConfigPIBVendor.setVisible(False)
    primeConfigPIBVendor.setMin(0x0000)
    primeConfigPIBVendor.setMax(0xFFFF)
    primeConfigPIBVendor.setDefaultValue(0x0000)

    # Select PRIME PIB model
    global primeConfigPIBModel
    primeConfigPIBModel = primeStackConfigComponent.createHexSymbol("PRIME_PIB_MODEL", primeStackConfig)
    primeConfigPIBModel.setLabel("PRIME PIB Model")
    primeConfigPIBModel.setDescription("Select the PRIME PIB model")
    primeConfigPIBModel.setVisible(False)
    primeConfigPIBModel.setMin(0x0000)
    primeConfigPIBModel.setMax(0xFFFF)
    if ("PIC32CX" in processor) and ("MT" in processor):
        primeConfigPIBModel.setDefaultValue(0x3941)
    elif ("ATSAME70" in processor):
        primeConfigPIBModel.setDefaultValue(0x3D3F)
    else:
        primeConfigPIBModel.setDefaultValue(0xFFFF)

    # Configure PRIME MAC Layer
    global primeMacConfig
    primeMacConfig = primeStackConfigComponent.createMenuSymbol("PRIME_MAC_Layer", None)
    primeMacConfig.setLabel("PRIME MAC Layer Configuration")
    primeMacConfig.setDescription("Configure the PRIME MAC Layer options")
    primeMacConfig.setVisible(False)

    # Select MAC security profile
    global primeConfigSecProfile
    primeConfigSecProfile = primeStackConfigComponent.createIntegerSymbol("MAC_SECURITY_PROFILE", primeMacConfig)
    primeConfigSecProfile.setLabel("MAC Security Profile")
    primeConfigSecProfile.setDescription("Select the PRIME MAC security profile")
    primeConfigSecProfile.setVisible(False)
    primeConfigSecProfile.setMin(0)
    primeConfigSecProfile.setMax(2)
    primeConfigSecProfile.setDefaultValue(0)

    # Select maximum number of nodes
    global primeConfigMaxNumNodes
    primeConfigMaxNumNodes = primeStackConfigComponent.createIntegerSymbol("NUM_MAX_NODES", primeMacConfig)
    primeConfigMaxNumNodes.setLabel("MAC Maximum Number of Nodes")
    primeConfigMaxNumNodes.setDescription("Select the maximum number of nodes supported by the Base Node")
    primeConfigMaxNumNodes.setVisible(False)
    primeConfigMaxNumNodes.setMin(0)
    primeConfigMaxNumNodes.setMax(2000)
    primeConfigMaxNumNodes.setDefaultValue(25)

    # Configure PRIME Management Plane
    global primeMngpConfig
    primeMngpConfig = primeStackConfigComponent.createMenuSymbol("PRIME_MNGP", None)
    primeMngpConfig.setLabel("PRIME Management Plane Configuration")
    primeMngpConfig.setDescription("Configure the PRIME Management Plane options")
    primeMngpConfig.setVisible(True)

    # Management Plane Serial Profile is always enabled
    global primeConfigMngpSprof
    primeConfigMngpSprof = primeStackConfigComponent.createCommentSymbol("MNGP_SPROFILE", primeMngpConfig)
    primeConfigMngpSprof.setLabel("Management Plane Serial Profile")
    primeConfigMngpSprof.setVisible(True)

    # Private symbol for USI instance of Serial Profile
    primeConfigSprof = primeStackConfigComponent.createBooleanSymbol("MNGP_SERIAL_PROFILE", primeConfigMngpSprof)
    primeConfigSprof.setVisible(False)
    primeConfigSprof.setDefaultValue(True)
    primeConfigSprof.setReadOnly(True)

    # Select serial profile USI port
    global primeConfigSprofUsiPort
    primeConfigSprofUsiPort = primeStackConfigComponent.createIntegerSymbol("MNGP_SPROF_USI_INSTANCE", primeConfigMngpSprof)
    primeConfigSprofUsiPort.setLabel("USI Instance")
    primeConfigSprofUsiPort.setDescription("USI instance used for the Management Plane Serial Profile")
    primeConfigSprofUsiPort.setVisible(True)
    primeConfigSprofUsiPort.setDefaultValue(0)
    primeConfigSprofUsiPort.setMax(0)
    primeConfigSprofUsiPort.setMin(0)
    primeConfigSprofUsiPort.setDependencies(primeShowSprofUsiInstance, ["MNGP_SERIAL_PROFILE"])

    # Dummy symbol to update files of the PRIME Stack
    primeStackDummy = primeStackConfigComponent.createMenuSymbol("PRIME_Stack_Dummy", None)
    primeStackDummy.setLabel("")
    primeStackDummy.setDescription("")
    primeStackDummy.setVisible(False)
    primeStackDummy.setDependencies(primeUpdateConfiguration, ["PRIME_MODE", "PRIME_PROJECT", "PRIME_VERSION"])

    #### Preprocessor-macros #####################################################

    # Set XC32-LD option to Modify RAM Start address and length
    global pPrimeXc32LdPrepMacroSym
    pPrimeXc32LdPrepMacroSym = primeStackConfigComponent.createSettingSymbol("PRIME_XC32_LINKER_PREPROC_MACROS", None)
    pPrimeXc32LdPrepMacroSym.setCategory("C32-LD")
    pPrimeXc32LdPrepMacroSym.setKey("preprocessor-macros")
    ramStartAddressHex, ramSizeHex = getRamMemoryDescription()
    ram_origin  = "RAM_ORIGIN=" + ramStartAddressHex
    ram_length  = "RAM_LENGTH=" + hex(int(ramSizeHex, 0) - int(PRIME_FW_STACK_RAM_SIZE, 0))
    ramParams = (ram_origin + ";" + ram_length)
    pPrimeXc32LdPrepMacroSym.setValue(ramParams)
    pPrimeXc32LdPrepMacroSym.setAppend(True, ";=")

    # Set Application Start Address
    Database.sendMessage("core", "APP_START_ADDRESS", {"start_address":(hex(int(memStartAddressHex, 0) + int(PRIME_USER_APP_OFFSET_HEX, 0)))})

    ############################################################################
    #### Code Generation ####
    ############################################################################

    configName = Variables.get("__CONFIGURATION_NAME")
    
    ##### PRIME API
    global pPrimeHeaderFile
    pPrimeHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_HEADER", None)
    pPrimeHeaderFile.setSourcePath("prime/src/prime_stack.h")
    pPrimeHeaderFile.setOutputName("prime_stack.h")
    pPrimeHeaderFile.setDestPath("stack/prime")
    pPrimeHeaderFile.setProjectPath("config/" + configName + "/stack/prime")
    pPrimeHeaderFile.setType("HEADER")
    pPrimeHeaderFile.setEnabled(False)
    
    global pPrimeSourceFile
    pPrimeSourceFile = primeStackConfigComponent.createFileSymbol("PRIME_SOURCE", None)
    pPrimeSourceFile.setSourcePath("prime/src/prime_stack.c.ftl")
    pPrimeSourceFile.setOutputName("prime_stack.c")
    pPrimeSourceFile.setDestPath("stack/prime")
    pPrimeSourceFile.setProjectPath("config/" + configName + "/stack/prime")
    pPrimeSourceFile.setType("SOURCE")
    pPrimeSourceFile.setEnabled(False)
    pPrimeSourceFile.setMarkup(True)
    
    global pPrimeLocalHeaderFile
    pPrimeLocalHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_LOCAL_HEADER", None)
    pPrimeLocalHeaderFile.setSourcePath("prime/src/prime_stack_local.h")
    pPrimeLocalHeaderFile.setOutputName("prime_stack_local.h")
    pPrimeLocalHeaderFile.setDestPath("stack/prime")
    pPrimeLocalHeaderFile.setProjectPath("config/" + configName + "/stack/prime")
    pPrimeLocalHeaderFile.setType("HEADER")
    pPrimeLocalHeaderFile.setEnabled(False)

    ##### PRIME API
    pPrimeApiHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_API_HEADER", None)
    pPrimeApiHeaderFile.setSourcePath("prime/src/prime_api/prime_api.h.ftl")
    pPrimeApiHeaderFile.setOutputName("prime_api.h")
    pPrimeApiHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiHeaderFile.setType("HEADER")
    pPrimeApiHeaderFile.setEnabled(True)
    pPrimeApiHeaderFile.setMarkup(True)

    pPrimeApiSourceFile = primeStackConfigComponent.createFileSymbol("PRIME_API_SOURCE", None)
    pPrimeApiSourceFile.setSourcePath("prime/src/prime_api/prime_api.c.ftl")
    pPrimeApiSourceFile.setOutputName("prime_api.c")
    pPrimeApiSourceFile.setDestPath("stack/prime/prime_api")
    pPrimeApiSourceFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiSourceFile.setType("SOURCE")
    pPrimeApiSourceFile.setEnabled(True)
    pPrimeApiSourceFile.setMarkup(True)

    pPrimeApiDefsHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_API_DEFS_HEADER", None)
    pPrimeApiDefsHeaderFile.setSourcePath("prime/src/prime_api/prime_api_defs.h")
    pPrimeApiDefsHeaderFile.setOutputName("prime_api_defs.h")
    pPrimeApiDefsHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeApiDefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiDefsHeaderFile.setType("HEADER")
    pPrimeApiDefsHeaderFile.setEnabled(True)

    pPrimeApiTypesHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_API_TYPES_HEADER", None)
    pPrimeApiTypesHeaderFile.setSourcePath("prime/src/prime_api/prime_api_types.h.ftl")
    pPrimeApiTypesHeaderFile.setOutputName("prime_api_types.h")
    pPrimeApiTypesHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeApiTypesHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiTypesHeaderFile.setType("HEADER")
    pPrimeApiTypesHeaderFile.setEnabled(True)
    pPrimeApiTypesHeaderFile.setMarkup(True);

    global pPrimeHalWrapperHeaderFile
    pPrimeHalWrapperHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_HAL_WRAPPER_HEADER", None)
    pPrimeHalWrapperHeaderFile.setSourcePath("prime/src/prime_api/prime_hal_wrapper.h")
    pPrimeHalWrapperHeaderFile.setOutputName("prime_hal_wrapper.h")
    pPrimeHalWrapperHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeHalWrapperHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeHalWrapperHeaderFile.setType("HEADER")
    pPrimeHalWrapperHeaderFile.setEnabled(False)

    global pPrimeHalWrapperSourceFile
    pPrimeHalWrapperSourceFile = primeStackConfigComponent.createFileSymbol("PRIME_HAL_WRAPPER_SOURCE", None)
    pPrimeHalWrapperSourceFile.setSourcePath("prime/src/prime_api/prime_hal_wrapper.c")
    pPrimeHalWrapperSourceFile.setOutputName("prime_hal_wrapper.c")
    pPrimeHalWrapperSourceFile.setDestPath("stack/prime/prime_api")
    pPrimeHalWrapperSourceFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeHalWrapperSourceFile.setType("SOURCE")
    pPrimeHalWrapperSourceFile.setEnabled(False)

    ##### MAC API
    global pMacHeaderFile
    pMacHeaderFile = primeStackConfigComponent.createFileSymbol("MAC_HEADER", None)
    pMacHeaderFile.setSourcePath("prime/src/mac/mac.h")
    pMacHeaderFile.setOutputName("mac.h")
    pMacHeaderFile.setDestPath("stack/prime/mac")
    pMacHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mac")
    pMacHeaderFile.setType("HEADER")
    pMacHeaderFile.setEnabled(False)

    pMacDefsHeaderFile = primeStackConfigComponent.createFileSymbol("MAC_DEFS_HEADER", None)
    pMacDefsHeaderFile.setSourcePath("prime/src/mac/mac_defs.h")
    pMacDefsHeaderFile.setOutputName("mac_defs.h")
    pMacDefsHeaderFile.setDestPath("stack/prime/mac")
    pMacDefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mac")
    pMacDefsHeaderFile.setType("HEADER")
    pMacDefsHeaderFile.setEnabled(True)

    pMacPibHeaderFile = primeStackConfigComponent.createFileSymbol("MAC_PIB_HEADER", None)
    pMacPibHeaderFile.setSourcePath("prime/src/mac/mac_pib.h")
    pMacPibHeaderFile.setOutputName("mac_pib.h")
    pMacPibHeaderFile.setDestPath("stack/prime/mac")
    pMacPibHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mac")
    pMacPibHeaderFile.setType("HEADER")
    pMacPibHeaderFile.setEnabled(True)

    ##### Management Plane API
    global pMngpHeaderFile
    pMngpHeaderFile = primeStackConfigComponent.createFileSymbol("MNGP_HEADER", None)
    pMngpHeaderFile.setSourcePath("prime/src/mngp/mngp.h.ftl")
    pMngpHeaderFile.setOutputName("mngp.h")
    pMngpHeaderFile.setDestPath("stack/prime/mngp")
    pMngpHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mngp")
    pMngpHeaderFile.setType("HEADER")
    pMngpHeaderFile.setEnabled(False)
    pMngpHeaderFile.setMarkup(True)

    global pBmngDefsHeaderFile
    pBmngDefsHeaderFile = primeStackConfigComponent.createFileSymbol("BMNG_DEFS_HEADER", None)
    pBmngDefsHeaderFile.setSourcePath("prime/src/mngp/bmng_defs.h")
    pBmngDefsHeaderFile.setOutputName("bmng_defs.h")
    pBmngDefsHeaderFile.setDestPath("stack/prime/mngp")
    pBmngDefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mngp")
    pBmngDefsHeaderFile.setType("HEADER")
    pBmngDefsHeaderFile.setEnabled(False)

    global pBmngApiHeaderFile
    pBmngApiHeaderFile = primeStackConfigComponent.createFileSymbol("BMNG_API_HEADER", None)
    pBmngApiHeaderFile.setSourcePath("prime/src/mngp/bmng_api.h")
    pBmngApiHeaderFile.setOutputName("bmng_api.h")
    pBmngApiHeaderFile.setDestPath("stack/prime/mngp")
    pBmngApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mngp")
    pBmngApiHeaderFile.setType("HEADER")
    pBmngApiHeaderFile.setEnabled(False)

    ##### Convergence Layer API
    global pClNullHeaderFile
    pClNullHeaderFile = primeStackConfigComponent.createFileSymbol("CL_NULL_HEADER", None)
    pClNullHeaderFile.setSourcePath("prime/src/conv/sscs/null/cl_null.h")
    pClNullHeaderFile.setOutputName("cl_null.h")
    pClNullHeaderFile.setDestPath("stack/prime/conv/sscs/null")
    pClNullHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/null")
    pClNullHeaderFile.setType("HEADER")
    pClNullHeaderFile.setEnabled(False)

    global pClNullApiHeaderFile
    pClNullApiHeaderFile = primeStackConfigComponent.createFileSymbol("CL_NULL_API_HEADER", None)
    pClNullApiHeaderFile.setSourcePath("prime/src/conv/sscs/null/cl_null_api.h.ftl")
    pClNullApiHeaderFile.setOutputName("cl_null_api.h")
    pClNullApiHeaderFile.setDestPath("stack/prime/conv/sscs/null")
    pClNullApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/null")
    pClNullApiHeaderFile.setType("HEADER")
    pClNullApiHeaderFile.setEnabled(False)
    pClNullApiHeaderFile.setMarkup(True)

    global pCl432HeaderFile
    pCl432HeaderFile = primeStackConfigComponent.createFileSymbol("CL_432_HEADER", None)
    pCl432HeaderFile.setSourcePath("prime/src/conv/sscs/iec_4_32/cl_432.h")
    pCl432HeaderFile.setOutputName("cl_432.h")
    pCl432HeaderFile.setDestPath("stack/prime/conv/sscs/iec_4_32")
    pCl432HeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/iec_4_32")
    pCl432HeaderFile.setType("HEADER")
    pCl432HeaderFile.setEnabled(False)

    global pCl432ApiHeaderFile
    pCl432ApiHeaderFile = primeStackConfigComponent.createFileSymbol("CL_432_API_HEADER", None)
    pCl432ApiHeaderFile.setSourcePath("prime/src/conv/sscs/iec_4_32/cl_432_api.h.ftl")
    pCl432ApiHeaderFile.setOutputName("cl_432_api.h")
    pCl432ApiHeaderFile.setDestPath("stack/prime/conv/sscs/iec_4_32")
    pCl432ApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/iec_4_32")
    pCl432ApiHeaderFile.setType("HEADER")
    pCl432ApiHeaderFile.setEnabled(False)
    pCl432ApiHeaderFile.setMarkup(True)

    pCl432DefsHeaderFile = primeStackConfigComponent.createFileSymbol("CL_432_DEFS_HEADER", None)
    pCl432DefsHeaderFile.setSourcePath("prime/src/conv/sscs/iec_4_32/cl_432_defs.h.ftl")
    pCl432DefsHeaderFile.setOutputName("cl_432_defs.h")
    pCl432DefsHeaderFile.setDestPath("stack/prime/conv/sscs/iec_4_32")
    pCl432DefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/iec_4_32")
    pCl432DefsHeaderFile.setType("HEADER")
    pCl432DefsHeaderFile.setEnabled(True)
    pCl432DefsHeaderFile.setMarkup(True)

    ##### HAL API
    pHalApiHeaderFile = primeStackConfigComponent.createFileSymbol("HAL_API_HEADER", None)
    pHalApiHeaderFile.setSourcePath("prime/src/hal_api/hal_api.h")
    pHalApiHeaderFile.setOutputName("hal_api.h")
    pHalApiHeaderFile.setDestPath("stack/prime/hal_api")
    pHalApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/hal_api")
    pHalApiHeaderFile.setType("HEADER")
    pHalApiHeaderFile.setEnabled(True)

    global pHalApiSourceFile
    pHalApiSourceFile = primeStackConfigComponent.createFileSymbol("HAL_API_SOURCE", None)
    pHalApiSourceFile.setSourcePath("prime/src/hal_api/hal_api.c")
    pHalApiSourceFile.setOutputName("hal_api.c")
    pHalApiSourceFile.setDestPath("stack/prime/hal_api")
    pHalApiSourceFile.setProjectPath("config/" + configName + "/stack/prime/hal_api")
    pHalApiSourceFile.setType("SOURCE")
    pHalApiSourceFile.setEnabled(True)

    global pPalTypesFile
    pPalTypesFile = primeStackConfigComponent.createFileSymbol("PAL_TYPES", None)
    pPalTypesFile.setSourcePath("pal/pal_types.h")
    pPalTypesFile.setOutputName("pal_types.h")
    pPalTypesFile.setDestPath("stack/pal")
    pPalTypesFile.setProjectPath("config/" + configName + "/stack/pal")
    pPalTypesFile.setType("HEADER")
    pPalTypesFile.setEnabled(False)

    global pPalHeaderFile
    pPalHeaderFile = primeStackConfigComponent.createFileSymbol("PAL_HEADER", None)
    pPalHeaderFile.setSourcePath("pal/pal.h")
    pPalHeaderFile.setOutputName("pal.h")
    pPalHeaderFile.setDestPath("stack/pal")
    pPalHeaderFile.setProjectPath("config/" + configName + "/stack/pal")
    pPalHeaderFile.setType("HEADER")
    pPalHeaderFile.setEnabled(False)

    ##### PRIME LIBRARIES
    global pPrime13BnLibFile
    pPrime13BnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_3_BN_LIBRARY", None)
    pPrime13BnLibFile.setSourcePath("prime/libs/prime13_lib_bn.a")
    pPrime13BnLibFile.setOutputName("prime13_lib_bn.a")
    pPrime13BnLibFile.setDestPath("stack/prime/libs")
    pPrime13BnLibFile.setEnabled(False)

    global pPrime13SnLibFile
    pPrime13SnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_3_SN_LIBRARY", None)
    pPrime13SnLibFile.setSourcePath("prime/libs/prime13_lib_sn.a")
    pPrime13SnLibFile.setOutputName("prime13_lib_sn.a")
    pPrime13SnLibFile.setDestPath("stack/prime/libs")
    pPrime13SnLibFile.setEnabled(False)

    global pPrime14BnMLibFile
    pPrime14BnMLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_4_BN_M_LIBRARY", None)
    pPrime14BnMLibFile.setSourcePath("prime/libs/prime14_lib_bn_m.a")
    pPrime14BnMLibFile.setOutputName("prime14_lib_bn_m.a")
    pPrime14BnMLibFile.setDestPath("stack/prime/libs")
    pPrime14BnMLibFile.setEnabled(False)

    global pPrime14SnLibFile
    pPrime14SnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_4_SN_LIBRARY", None)
    pPrime14SnLibFile.setSourcePath("prime/libs/prime14_lib_sn.a")
    pPrime14SnLibFile.setOutputName("prime14_lib_sn.a")
    pPrime14SnLibFile.setDestPath("stack/prime/libs")
    pPrime14SnLibFile.setEnabled(False)

    ##### PRIME STACK TEMPLATES
    
    primeStackSystemConfigFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_CONFIGURATION", None)
    primeStackSystemConfigFile.setType("STRING")
    primeStackSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
    primeStackSystemConfigFile.setSourcePath("prime/templates/system/configuration.h.ftl")
    primeStackSystemConfigFile.setMarkup(True)

    primeStackSystemDefFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_DEF", None)
    primeStackSystemDefFile.setType("STRING")
    primeStackSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    primeStackSystemDefFile.setSourcePath("prime/templates/system/definitions.h.ftl")
    primeStackSystemDefFile.setMarkup(True)
    
    primeStackSystemDefObjFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_DEF_OBJ", None)
    primeStackSystemDefObjFile.setType("STRING")
    primeStackSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    primeStackSystemDefObjFile.setSourcePath("prime/templates/system/definitions_objects.h.ftl")
    primeStackSystemDefObjFile.setMarkup(True)
    
    primeStackSystemInitDataFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_INIT_DATA", None)
    primeStackSystemInitDataFile.setType("STRING")
    primeStackSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
    primeStackSystemInitDataFile.setSourcePath("prime/templates/system/initialize_data.c.ftl")
    primeStackSystemInitDataFile.setMarkup(True)

    primeSystemInitFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_INIT", None)
    primeSystemInitFile.setType("STRING")
    primeSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
    primeSystemInitFile.setSourcePath("prime/templates/system/initialize.c.ftl")
    primeSystemInitFile.setMarkup(True)

    primeSystemTasksFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_SYS_TASK", None)
    primeSystemTasksFile.setType("STRING")
    primeSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
    primeSystemTasksFile.setSourcePath("prime/templates/system/system_tasks.c.ftl")
    primeSystemTasksFile.setMarkup(True)

def destroyComponent(primeStackConfigComponent):
    # Set Application Start Address
    memStartAddressHex, memSizeHex = getFlashMemoryDescription()
    Database.sendMessage("core", "APP_START_ADDRESS", {"start_address":memStartAddressHex})

    Database.deactivateComponents(["prime_config"])

################################################################################
#### Business Logic ####
################################################################################

#Set symbols of other components
def setVal(component, symbol, value):
    triggerDict = {"Component":component,"Id":symbol, "Value":value}
    if(Database.sendMessage(component, "SET_SYMBOL", triggerDict) == None):
        print("Set Symbol Failure" + component + ":" + symbol + ":" + str(value))
        return False
    else:
        return True

#Handle messages from other components
def handleMessage(messageID, args):
    retDict= {}
    if (messageID == "SET_SYMBOL"):
        print("handleMessage: Set Symbol in PRIME Stack")
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict

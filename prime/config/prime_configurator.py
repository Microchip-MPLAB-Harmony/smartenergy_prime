"""*****************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

def primeAddBnFiles():

    global pPrimeApiSourceFile
    global pPrimeApiAsFile
    global pPrimeHalWrapperHeaderFile
    global pPrimeHalWrapperSourceFile
    global pMacHeaderFile
    global pMngpHeaderFile
    global pBmngApiHeaderFile
    global pBmngDefsHeaderFile
    global pClNullHeaderFile
    global pClNullApiHeaderFile
    global pCl432HeaderFile
    global pCl432ApiHeaderFile
    global pHalApiSourceFile
    global pPrime13BnLibFile
    global pPrime13BnSlaveLibFile
    global pPrime13SnLibFile
    global pPrime14BnLibFile
    global pPrime14SnLibFile
    global primeConfigVersion
    global primeConfigBnSlaveEn
    
    # SN only 
    pPrimeApiAsFile.setEnabled(False)
    
    # BN API files
    pPrimeApiSourceFile.setEnabled(True)
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
    
    # No libraries by default
    pPrime13BnLibFile.setEnabled(False)
    pPrime13BnSlaveLibFile.setEnabled(False)
    pPrime13SnLibFile.setEnabled(False)
    pPrime14BnLibFile.setEnabled(False)
    pPrime14SnLibFile.setEnabled(False)
    
    # BN library
    if (primeConfigVersion.getValue() == "1.3.6"): 
        if (primeConfigBnSlaveEn.getValue() == True):
            pPrime13BnSlaveLibFile.setEnabled(True)
        else:
            pPrime13BnLibFile.setEnabled(True)
    elif (primeConfigVersion.getValue() == "1.4"): 
        pPrime14BnLibFile.setEnabled(True)
    else:
        # Unknown option: no library by default
        pass

def primeAddSnFiles():

    global pPrimeApiSourceFile
    global pPrimeApiAsFile
    global pPrimeHalWrapperHeaderFile
    global pPrimeHalWrapperSourceFile
    global pMacHeaderFile
    global pMngpHeaderFile
    global pBmngApiHeaderFile
    global pBmngDefsHeaderFile
    global pClNullHeaderFile
    global pClNullApiHeaderFile
    global pCl432HeaderFile
    global pCl432ApiHeaderFile
    global pHalApiSourceFile
    global pPrime13BnLibFile
    global pPrime13BnSlaveLibFile
    global pPrime13SnLibFile
    global pPrime14BnLibFile
    global pPrime14SnLibFile
    global primeConfigVersion
    global primeConfigProject
    
    # BN only
    pBmngApiHeaderFile.setEnabled(False)
    pBmngDefsHeaderFile.setEnabled(False)
    pClNullApiHeaderFile.setEnabled(False)
    pCl432ApiHeaderFile.setEnabled(False)
    
    # No libraries by default
    pPrime13BnLibFile.setEnabled(False)
    pPrime13BnSlaveLibFile.setEnabled(False)
    pPrime13SnLibFile.setEnabled(False)
    pPrime14BnLibFile.setEnabled(False)
    pPrime14SnLibFile.setEnabled(False)
    
    if (primeConfigProject.getValue() == "bin project"):
        # SN API files in bin
        pPrimeApiSourceFile.setEnabled(True)
        pPrimeApiAsFile.setEnabled(True)
        pPrimeHalWrapperHeaderFile.setEnabled(True)
        pPrimeHalWrapperSourceFile.setEnabled(True)
        pMacHeaderFile.setEnabled(True)
        pMngpHeaderFile.setEnabled(True)
        pClNullHeaderFile.setEnabled(True)
        pCl432HeaderFile.setEnabled(True)
        pHalApiSourceFile.setEnabled(False)
        
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
        pPrimeApiSourceFile.setEnabled(False)
        pPrimeApiAsFile.setEnabled(False)
        pPrimeHalWrapperHeaderFile.setEnabled(False)
        pPrimeHalWrapperSourceFile.setEnabled(False)
        pMacHeaderFile.setEnabled(False)
        pMngpHeaderFile.setEnabled(False)
        pClNullHeaderFile.setEnabled(False)
        pCl432HeaderFile.setEnabled(False)
        pHalApiSourceFile.setEnabled(True)
        
        # No library in SN application
        pass
    else:
        # Unknown option: no library by default
        pass

def primeChangeMode(symbol, event):

    global primeConfigMode
    global primeConfigProject
    global primeConfigVersion
    global primeConfigOperationMode
    
    if (event["value"] == "SN"):
        # Need to config project type before showing further options
        primeHideAllOptions()
        primeConfigMode.setVisible(True)
        primeConfigProject.setValue("application project")
        primeConfigProject.setVisible(True)
        primeConfigVersion.setVisible(True)
        primeConfigOperationMode.setVisible(True)
        primeConfigComment.setVisible(True)
    else:
        # Config everything
        primeShowAllOptions()
        primeConfigProject.setVisible(False)
        primeConfigComment.setVisible(False)
        # Add files for BN
        primeAddBnFiles()
        
def primeAddBnSlave(symbol, event):
    # Add files for BN
    primeAddBnFiles()

def primeChangeProject(symbol, event):

    global primeConfigVersion
    global primeConfigOperationMode
    global primeConfigMaxNumNodes
    global primeConfigBnSlaveEn
    
    if (event["value"] == "application project"):
        # Application project for SN: only config version and operation mode
        primeHideAllOptions()
        primeConfigMode.setVisible(True)
        primeConfigProject.setVisible(True)
        primeConfigVersion.setVisible(True)
        primeConfigOperationMode.setVisible(True)
        primeConfigComment.setVisible(True)
    elif (event["value"] == "bin project"):
        # bin project for SN: config everything except number of nodes
        primeShowAllOptions()
        primeConfigMaxNumNodes.setVisible(False)
        primeConfigMaxNumNodes.setValue(0)
        primeConfigComment.setVisible(False)
        primeConfigBnSlaveEn.setVisible(False)
    else:
        # Unknown option: hide all options
        primeHideAllOptions()
        primeConfigMode.setVisible(True)
        
    # Add files for SN 
    primeAddSnFiles()
        
def primeChangeOperationMode(symbol, event):
    # Adjust the version name (hybrid or not)
    primeChangeFWVersion()

def primeChangeConfigVersion(symbol, event):
    global primeConfigOperationMode
    global primeConfigSecProfile
    global primeConfigBnSlaveEn
    
    # No security and no hybrid or RF in 1.3
    # No BN slave in 1.4
    if (event["value"] == "1.3.6"):
        primeConfigOperationMode.setValue("PLC")
        primeConfigOperationMode.setReadOnly(True)
        primeConfigSecProfile.setValue(0)
        primeConfigSecProfile.setReadOnly(True)
        primeConfigBnSlaveEn.setReadOnly(False)
    elif (event["value"] == "1.4"):
        primeConfigOperationMode.setValue("Hybrid")
        primeConfigOperationMode.setReadOnly(False)
        primeConfigSecProfile.setReadOnly(False)
        primeConfigBnSlaveEn.setReadOnly(True)
    else:
        #Unknown option
        primeConfigOperationMode.setValue("Hybrid")
        primeConfigOperationMode.setReadOnly(False)
        primeConfigSecProfile.setReadOnly(False)
        primeConfigBnSlaveEn.setReadOnly(True)
    
    primeChangeFWVersion()

def primeChangeFWVersion():
    global primeConfigMode
    global primeConfigVersion
    global primeConfigBnSlaveEn
    
    # 1.4 versions are always hybrid
    if (primeConfigMode.getValue() == "SN"):
        if (primeConfigVersion.getValue() == "1.4"):
            primeConfigFWVersion.setValue("HS14.01.01") 
        else:
            primeConfigFWVersion.setValue("S13.01.01") 
        
        # Add files for SN 
        primeAddSnFiles()
    else:
        if (primeConfigVersion.getValue() == "1.4"):
            primeConfigFWVersion.setValue("HB14.01.01") 
        else:
            primeConfigFWVersion.setValue("B13.01.01") 
        
        # Add files for BN 
        primeAddBnFiles()
 
def primeShowMacSnifferUsiInstance(symbol, event):
    global primeConfigMode
    global primeConfigOperationMode
    
    symbol.setVisible(event["value"])
    
    if (primeConfigMode.getValue() == "BN"):
        if (event["value"] == True):
            usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
            symbol.setMax(len(usiInstances) - 1)
    else:
        if (primeConfigOperationMode.getValue() == "application project"):
            if (event["value"] == True):
                usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
                symbol.setMax(len(usiInstances) - 1)
        else:
            # In the SN bin project, there is no USI block
            symbol.setMax(10)

def primeShowSprofUsiInstance(symbol, event):
    global primeConfigMode
    global primeConfigOperationMode
    
    symbol.setVisible(event["value"])
    
    if (primeConfigMode.getValue() == "BN"):
        if (event["value"] == True):
            usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
            symbol.setMax(len(usiInstances) - 1)
    else:
        if (primeConfigOperationMode.getValue() == "application project"):
            if (event["value"] == True):
                usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
                symbol.setMax(len(usiInstances) - 1)
        else:
            # In the SN bin project, there is no USI block
            symbol.setMax(10)

def primeShowAllOptions():

    global primeConfigMode
    global primeConfigBnSlaveEn
    global primeConfigVersion
    global primeConfigOperationMode
    global primeConfigFWVendor
    global primeConfigFWModel
    global primeConfigFWVersion
    global primeConfigPIBVendor
    global primeConfigPIBModel
    global primeConfigSecProfile
    global primeMacConfig
    global primeConfigSecProfile
    global primeConfigMaxNumNodes
    global primeConfigMacSniffer
    global primeConfigMacSnifferUsiPort
    global primeMngpConfig
    global primeConfigMngpSprof
    global primeConfigSprofUsiPort
    
    primeChangeFWVersion()
    
    primeConfigMode.setVisible(True)
    primeConfigBnSlaveEn.setVisible(True)
    primeConfigVersion.setVisible(True)
    primeConfigOperationMode.setVisible(True)
    primeConfigFWVendor.setVisible(True)
    primeConfigFWModel.setVisible(True)
    primeConfigFWVersion.setVisible(True)
    primeConfigPIBVendor.setVisible(True)
    primeConfigPIBModel.setVisible(True)
    primeConfigSecProfile.setVisible(True)
    primeMacConfig.setVisible(True)
    primeConfigSecProfile.setVisible(True)
    primeConfigMaxNumNodes.setVisible(True)
    primeConfigMacSniffer.setVisible(True)
    if (primeConfigMacSniffer.getValue() == True):
        primeConfigMacSnifferUsiPort.setVisible(True)
    else:
        primeConfigMacSnifferUsiPort.setVisible(False)
    primeMngpConfig.setVisible(True)
    primeConfigMngpSprof.setVisible(True)
    primeConfigSprofUsiPort.setVisible(True)
    
def primeHideAllOptions():

    global primeConfigMode
    global primeConfigBnSlaveEn
    global primeConfigVersion
    global primeConfigOperationMode
    global primeConfigFWVendor
    global primeConfigFWModel
    global primeConfigFWVersion
    global primeConfigPIBVendor
    global primeConfigPIBModel
    global primeConfigSecProfile
    global primeMacConfig
    global primeConfigSecProfile
    global primeConfigMaxNumNodes
    global primeConfigMacSniffer
    global primeConfigMacSnifferUsiPort
    global primeMngpConfig
    global primeConfigMngpSprof
    global primeConfigSprofUsiPort
    
    primeConfigMode.setVisible(False)
    primeConfigBnSlaveEn.setVisible(False)
    primeConfigVersion.setVisible(False)
    primeConfigOperationMode.setVisible(False)
    primeConfigFWVendor.setVisible(False)
    primeConfigFWModel.setVisible(False)
    primeConfigFWVersion.setVisible(False)
    primeConfigPIBVendor.setVisible(False)
    primeConfigPIBModel.setVisible(False)
    primeConfigSecProfile.setVisible(False)
    primeMacConfig.setVisible(False)
    primeConfigSecProfile.setVisible(False)
    primeConfigMaxNumNodes.setVisible(False)
    primeConfigMacSniffer.setVisible(False)
    primeConfigMacSnifferUsiPort.setVisible(False)
    primeMngpConfig.setVisible(False)
    primeConfigMngpSprof.setVisible(False)
    primeConfigSprofUsiPort.setVisible(False)
    
    
def instantiateComponent(primeStackConfigComponent):
    Log.writeInfoMessage("Loading Stack Configurator for PRIME")
    
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
    primeConfigMode.setDependencies(primeChangeMode, ["PRIME_MODE"])
    
    # Enable BN slave
    global primeConfigBnSlaveEn
    primeConfigBnSlaveEn = primeStackConfigComponent.createBooleanSymbol("BN_SLAVE_EN", primeStackConfig)
    primeConfigBnSlaveEn.setLabel("Enable BN Slave")
    primeConfigBnSlaveEn.setDescription("Enable/disable the BN slave functionality")
    primeConfigBnSlaveEn.setVisible(False)
    primeConfigBnSlaveEn.setDefaultValue(False)
    primeConfigBnSlaveEn.setReadOnly(True)
    primeConfigBnSlaveEn.setDependencies(primeAddBnSlave, ["BN_SLAVE_EN"])
    
    # Select type of project 
    global primeConfigProject
    primeProjectOptions = ["application project", "bin project"]
    primeConfigProject = primeStackConfigComponent.createComboSymbol("PRIME_PROJECT", primeStackConfig, primeProjectOptions)
    primeConfigProject.setLabel("Type of PRIME project for separated applications")
    primeConfigProject.setDescription("Select the type of PRIME project for separated applications")
    primeConfigProject.setVisible(True)
    primeConfigProject.setDefaultValue("application project")
    primeConfigProject.setDependencies(primeChangeProject, ["PRIME_PROJECT"])
    
    # Select version 
    global primeConfigVersion
    primeVersions = ["1.3.6", "1.4"]
    primeConfigVersion = primeStackConfigComponent.createComboSymbol("PRIME_VERSION", primeStackConfig, primeVersions)
    primeConfigVersion.setLabel("PRIME version")
    primeConfigVersion.setDescription("Select the PRIME version: 1.3.6 or 1.4")
    primeConfigVersion.setVisible(True)
    primeConfigVersion.setDefaultValue("1.4")
    primeConfigVersion.setDependencies(primeChangeConfigVersion, ["PRIME_VERSION"])    
    
    # Select operation mode 
    global primeConfigOperationMode
    primeOperationModes = ["Hybrid", "PLC", "RF"]
    primeConfigOperationMode = primeStackConfigComponent.createComboSymbol("PRIME_OPERATION_MODE", primeStackConfig, primeOperationModes)
    primeConfigOperationMode.setLabel("PRIME operation mode")
    primeConfigOperationMode.setDescription("Select the PRIME operation mode: Hybrid, only PLC or only RF")
    primeConfigOperationMode.setVisible(True)
    primeConfigOperationMode.setDefaultValue("Hybrid")
    primeConfigOperationMode.setDependencies(primeChangeOperationMode, ["PRIME_OPERATION_MODE"]) 

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
    primeConfigFWModel.setDefaultValue("PIC32CXMTG")    
    
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
    primeConfigPIBModel.setDefaultValue(0x3940)
    
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
    primeConfigMaxNumNodes.setDefaultValue(15)
    
    # Enable MAC sniffer
    global primeConfigMacSniffer
    primeConfigMacSniffer = primeStackConfigComponent.createBooleanSymbol("MAC_SNIFFER_EN", primeMacConfig)
    primeConfigMacSniffer.setLabel("Enable PRIME MAC Sniffer")
    primeConfigMacSniffer.setDescription("Enable/disable the MAC sniffer through USI")
    primeConfigMacSniffer.setVisible(False)
    primeConfigMacSniffer.setDefaultValue(False)
    
    # Select MAC sniffer USI port
    global primeConfigMacSnifferUsiPort
    primeConfigMacSnifferUsiPort = primeStackConfigComponent.createIntegerSymbol("MAC_SNIFFER_USI_PORT", primeConfigMacSniffer)
    primeConfigMacSnifferUsiPort.setLabel("USI Instance")
    primeConfigMacSnifferUsiPort.setDescription("USI instance used for PRIME MAC Sniffer")
    primeConfigMacSnifferUsiPort.setVisible(False)
    primeConfigMacSnifferUsiPort.setDefaultValue(0)
    primeConfigMacSnifferUsiPort.setMax(0)
    primeConfigMacSnifferUsiPort.setMin(0)
    primeConfigMacSnifferUsiPort.setDependencies(primeShowMacSnifferUsiInstance, ["MAC_SNIFFER_EN"])
    
    # Configure PRIME Management Plane
    global primeMngpConfig
    primeMngpConfig = primeStackConfigComponent.createMenuSymbol("PRIME_MNGP", None)
    primeMngpConfig.setLabel("PRIME Management Plane Configuration")
    primeMngpConfig.setDescription("Configure the PRIME Management Plane options")
    primeMngpConfig.setVisible(False)
    
    # Management Plane Serial Profile is always enabled
    global primeConfigMngpSprof
    primeConfigMngpSprof = primeStackConfigComponent.createCommentSymbol("MNGP_SPROFILE", primeMngpConfig)
    primeConfigMngpSprof.setLabel("Management Plane Serial Profile")
    primeConfigMngpSprof.setVisible(False)

    # Private symbol for USI instance of Serial Profile
    primeConfigSprof = primeStackConfigComponent.createBooleanSymbol("MNGP_SERIAL_PROFILE", primeConfigMngpSprof)
    primeConfigSprof.setVisible(False)
    primeConfigSprof.setDefaultValue(True)
    primeConfigSprof.setReadOnly(True)
    
    # Select serial profile USI port
    global primeConfigSprofUsiPort
    primeConfigSprofUsiPort = primeStackConfigComponent.createIntegerSymbol("MNGP_SPROF_USI_PORT", primeConfigMngpSprof)
    primeConfigSprofUsiPort.setLabel("USI Instance")
    primeConfigSprofUsiPort.setDescription("USI instance used for the Management Plane Serial Profile")
    primeConfigSprofUsiPort.setVisible(False)
    primeConfigSprofUsiPort.setDefaultValue(0)
    primeConfigSprofUsiPort.setMax(0)
    primeConfigSprofUsiPort.setMin(0)
    primeConfigSprofUsiPort.setDependencies(primeShowSprofUsiInstance, ["MNGP_SERIAL_PROFILE"])


    ############################################################################
    #### Code Generation ####
    ############################################################################
    
    configName = Variables.get("__CONFIGURATION_NAME")
    
    ##### PRIME API
    pPrimeApiHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_API_HEADER", None)
    pPrimeApiHeaderFile.setSourcePath("prime/src/prime_api/prime_api.h")
    pPrimeApiHeaderFile.setOutputName("prime_api.h")
    pPrimeApiHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiHeaderFile.setType("HEADER")
    pPrimeApiHeaderFile.setEnabled(True)
    #pPrimeApiHeaderFile.setMarkup(True)
    
    global pPrimeApiSourceFile
    pPrimeApiSourceFile = primeStackConfigComponent.createFileSymbol("PRIME_API_SOURCE", None)
    pPrimeApiSourceFile.setSourcePath("prime/src/prime_api/prime_api.c")
    pPrimeApiSourceFile.setOutputName("prime_api.c")
    pPrimeApiSourceFile.setDestPath("stack/prime/prime_api")
    pPrimeApiSourceFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiSourceFile.setType("SOURCE")
    pPrimeApiSourceFile.setEnabled(False)
    #pPrimeApiSourceFile.setMarkup(True)
    
    global pPrimeApiAsFile
    pPrimeApiAsFile = primeStackConfigComponent.createFileSymbol("PRIME_API_ASSEMBLER_FILE", None)
    pPrimeApiAsFile.setSourcePath("prime/src/prime_api/prime_api_as.S")
    pPrimeApiAsFile.setOutputName("prime_api_as.S")
    pPrimeApiAsFile.setDestPath("stack/prime/prime_api")
    pPrimeApiAsFile.setEnabled(False)
    #pPrimeApiAsFile.setMarkup(True)
    
    pPrimeApiDefsHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_API_DEFS_HEADER", None)
    pPrimeApiDefsHeaderFile.setSourcePath("prime/src/prime_api/prime_api_defs.h")
    pPrimeApiDefsHeaderFile.setOutputName("prime_api_defs.h")
    pPrimeApiDefsHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeApiDefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeApiDefsHeaderFile.setType("HEADER")
    pPrimeApiDefsHeaderFile.setEnabled(True)
    
    global pPrimeHalWrapperHeaderFile
    pPrimeHalWrapperHeaderFile = primeStackConfigComponent.createFileSymbol("PRIME_HAL_WRAPPER_HEADER", None)
    pPrimeHalWrapperHeaderFile.setSourcePath("prime/src/prime_api/prime_hal_wrapper.h")
    pPrimeHalWrapperHeaderFile.setOutputName("prime_hal_wrapper.h")
    pPrimeHalWrapperHeaderFile.setDestPath("stack/prime/prime_api")
    pPrimeHalWrapperHeaderFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeHalWrapperHeaderFile.setType("HEADER")
    pPrimeHalWrapperHeaderFile.setEnabled(False)
    #pPrimeHalWrapperHeaderFile.setMarkup(True)
    
    global pPrimeHalWrapperSourceFile
    pPrimeHalWrapperSourceFile = primeStackConfigComponent.createFileSymbol("PRIME_HAL_WRAPPER_SOURCE", None)
    pPrimeHalWrapperSourceFile.setSourcePath("prime/src/prime_api/prime_hal_wrapper.c")
    pPrimeHalWrapperSourceFile.setOutputName("prime_hal_wrapper.c")
    pPrimeHalWrapperSourceFile.setDestPath("stack/prime/prime_api")
    pPrimeHalWrapperSourceFile.setProjectPath("config/" + configName + "/stack/prime/prime_api")
    pPrimeHalWrapperSourceFile.setType("SOURCE")
    pPrimeHalWrapperSourceFile.setEnabled(False)
    #pPrimeApiSourceFile.setMarkup(True)
    
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
    pMngpHeaderFile.setSourcePath("prime/src/mngp/mngp.h")
    pMngpHeaderFile.setOutputName("mngp.h")
    pMngpHeaderFile.setDestPath("stack/prime/mngp")
    pMngpHeaderFile.setProjectPath("config/" + configName + "/stack/prime/mngp")
    pMngpHeaderFile.setType("HEADER")
    pMngpHeaderFile.setEnabled(False)
    
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
    pClNullApiHeaderFile.setSourcePath("prime/src/conv/sscs/null/cl_null_api.h")
    pClNullApiHeaderFile.setOutputName("cl_null_api.h")
    pClNullApiHeaderFile.setDestPath("stack/prime/conv/sscs/null")
    pClNullApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/null")
    pClNullApiHeaderFile.setType("HEADER")
    pClNullApiHeaderFile.setEnabled(False)
    #pClNullApiHeaderFile.setMarkup(True)
    
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
    pCl432ApiHeaderFile.setSourcePath("prime/src/conv/sscs/iec_4_32/cl_432_api.h")
    pCl432ApiHeaderFile.setOutputName("cl_432_api.h")
    pCl432ApiHeaderFile.setDestPath("stack/prime/conv/sscs/iec_4_32")
    pCl432ApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/iec_4_32")
    pCl432ApiHeaderFile.setType("HEADER")
    pCl432ApiHeaderFile.setEnabled(False)
    #pCl432ApiHeaderFile.setMarkup(True)
    
    pCl432DefsHeaderFile = primeStackConfigComponent.createFileSymbol("CL_432_DEFS_HEADER", None)
    pCl432DefsHeaderFile.setSourcePath("prime/src/conv/sscs/iec_4_32/cl_432_defs.h")
    pCl432DefsHeaderFile.setOutputName("cl_432_defs.h")
    pCl432DefsHeaderFile.setDestPath("stack/prime/conv/sscs/iec_4_32")
    pCl432DefsHeaderFile.setProjectPath("config/" + configName + "/stack/prime/conv/sscs/iec_4_32")
    pCl432DefsHeaderFile.setType("HEADER")
    pCl432DefsHeaderFile.setEnabled(True)
    #pCl432DefsHeaderFile.setMarkup(True)
    
    ##### HAL API
    pHalApiHeaderFile = primeStackConfigComponent.createFileSymbol("HAL_API_HEADER", None)
    pHalApiHeaderFile.setSourcePath("prime/src/hal_api/hal_api.h")
    pHalApiHeaderFile.setOutputName("hal_api.h")
    pHalApiHeaderFile.setDestPath("stack/prime/hal_api")
    pHalApiHeaderFile.setProjectPath("config/" + configName + "/stack/prime/hal_api")
    pHalApiHeaderFile.setType("HEADER")
    pHalApiHeaderFile.setEnabled(True)
    #pHalApiHeaderFile.setMarkup(True)
    
    global pHalApiSourceFile
    pHalApiSourceFile = primeStackConfigComponent.createFileSymbol("HAL_API_SOURCE", None)
    pHalApiSourceFile.setSourcePath("prime/src/hal_api/hal_api.c")
    pHalApiSourceFile.setOutputName("hal_api.c")
    pHalApiSourceFile.setDestPath("stack/prime/hal_api")
    pHalApiSourceFile.setProjectPath("config/" + configName + "/stack/prime/hal_api")
    pHalApiSourceFile.setType("SOURCE")
    pHalApiSourceFile.setEnabled(True)
    #pHalApiSourceFile.setMarkup(True)
     
    ##### PRIME LIBRARIES
    global pPrime13BnLibFile
    pPrime13BnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_3_BN_LIBRARY", None)
    pPrime13BnLibFile.setSourcePath("prime/libs/prime13_lib_bn.a")
    pPrime13BnLibFile.setOutputName("prime13_lib_bn.a")
    pPrime13BnLibFile.setDestPath("stack/prime/libs")
    pPrime13BnLibFile.setEnabled(False)
    
    global pPrime13BnSlaveLibFile
    pPrime13BnSlaveLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_3_BN_SLAVE_LIBRARY", None)
    pPrime13BnSlaveLibFile.setSourcePath("prime/libs/prime13_lib_bn_slave.a")
    pPrime13BnSlaveLibFile.setOutputName("prime13_lib_bn_slave.a")
    pPrime13BnSlaveLibFile.setDestPath("stack/prime/libs")
    pPrime13BnSlaveLibFile.setEnabled(False)
    
    global pPrime13SnLibFile
    pPrime13SnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_3_SN_LIBRARY", None)
    pPrime13SnLibFile.setSourcePath("prime/libs/prime13_lib_sn.a")
    pPrime13SnLibFile.setOutputName("prime13_lib_sn.a")
    pPrime13SnLibFile.setDestPath("stack/prime/libs")
    pPrime13SnLibFile.setEnabled(False)
    
    global pPrime14BnLibFile
    pPrime14BnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_4_BN_LIBRARY", None)
    pPrime14BnLibFile.setSourcePath("prime/libs/prime14_lib_bn.a")
    pPrime14BnLibFile.setOutputName("prime14_lib_bn.a")
    pPrime14BnLibFile.setDestPath("stack/prime/libs")
    pPrime14BnLibFile.setEnabled(False)
    
    global pPrime14SnLibFile
    pPrime14SnLibFile = primeStackConfigComponent.createLibrarySymbol("PRIME_1_4_SN_LIBRARY", None)
    pPrime14SnLibFile.setSourcePath("prime/libs/prime14_lib_sn.a")
    pPrime14SnLibFile.setOutputName("prime14_lib_sn.a")
    pPrime14SnLibFile.setDestPath("stack/prime/libs")
    pPrime14SnLibFile.setEnabled(False)
    
#### FreeMaker System Files ######################################################

    #primeStackSystemConfigFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_CONFIGURATION", None)
    #primeStackSystemConfigFile.setType("STRING")
    #primeStackSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
    #primeStackSystemConfigFile.setSourcePath("prime/templates/system/configuration.h.ftl")
    #primeStackSystemConfigFile.setMarkup(True)

    #primeStackSystemDefFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_DEF", None)
    #primeStackSystemDefFile.setType("STRING")
    #primeStackSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    #primeStackSystemDefFile.setSourcePath("prime/templates/system/definitions.h.ftl")
    #primeStackSystemDefFile.setMarkup(True)

    #primeSystemInitFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_INIT", None)
    #primeSystemInitFile.setType("STRING")
    #primeSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
    #primeSystemInitFile.setSourcePath("prime/templates/system/initialize.c.ftl")
    #primeSystemInitFile.setMarkup(True)

    #primeSystemTasksFile = primeStackConfigComponent.createFileSymbol("PRIME_STACK_SYS_TASK", None)
    #primeSystemTasksFile.setType("STRING")
    #primeSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
    #primeSystemTasksFile.setSourcePath("prime/templates/system/system_tasks.c.ftl")
    #primeSystemTasksFile.setMarkup(True)


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
        print("handleMessage: Set Symbol")
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict

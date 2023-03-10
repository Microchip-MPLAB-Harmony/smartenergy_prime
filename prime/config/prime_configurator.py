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
    else:
        # Config everything
        primeShowAllOptions()
        primeConfigProject.setVisible(False)

def primeChangeProject(symbol, event):

    global primeConfigVersion
    global primeConfigOperationMode
    global primeConfigMaxNumNodes
    
    if (event["value"] == "application project"):
        # Application project for SN: only config version and operation mode
        primeHideAllOptions()
        primeConfigMode.setVisible(True)
        primeConfigProject.setVisible(True)
        primeConfigVersion.setVisible(True)
        primeConfigOperationMode.setVisible(True)
    elif (event["value"] == "bin project"):
        # bin project for SN: config everything except number of nodes
        primeShowAllOptions()
        primeConfigMaxNumNodes.setVisible(False)
        primeConfigMaxNumNodes.setValue(0)
    else:
        # Unknown option: hide all options
        primeHideAllOptions()
        primeConfigMode.setVisible(True)
        
def primeChangeOperationMode(symbol, event):
    # Adjust the version name (hybrid or not)
    primeChangeFWVersion()

def primeChangeConfigVersion(symbol, event):
    global primeConfigOperationMode
    global primeConfigSecProfile
    
    # No security and no hybrid or RF in 1.3
    if (event["value"] == "1.3.6"):
        primeConfigOperationMode.setValue("PLC")
        primeConfigOperationMode.setReadOnly(True)
        primeConfigSecProfile.setValue(0)
        primeConfigSecProfile.setReadOnly(True)
    elif (event["value"] == "1.4"):
        primeConfigOperationMode.setValue("Hybrid")
        primeConfigOperationMode.setReadOnly(False)
        primeConfigSecProfile.setReadOnly(False)
    else:
        #Unknown option
        primeConfigOperationMode.setValue("Hybrid")
        primeConfigOperationMode.setReadOnly(False)
        primeConfigSecProfile.setReadOnly(False)
    
    primeChangeFWVersion()

def primeChangeFWVersion():
    global primeConfigMode
    global primeConfigVersion
    
    # 1.4 versions are always hybrid
    if (primeConfigMode.getValue() == "SN"):
        if (primeConfigVersion.getValue() == "1.4"):
            primeConfigFWVersion.setValue("HS14.01.01") 
        else:
            primeConfigFWVersion.setValue("S13.01.01") 
    else:
        if (primeConfigVersion.getValue() == "1.4"):
            primeConfigFWVersion.setValue("HB14.01.01") 
        else:
            primeConfigFWVersion.setValue("B13.01.01") 
 
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
    
    # Configure PRIME Management Plane Layer
    global primeMngpConfig
    primeMngpConfig = primeStackConfigComponent.createMenuSymbol("PRIME_MNGP_Layer", None)
    primeMngpConfig.setLabel("PRIME Management Plane Layer Configuration")
    primeMngpConfig.setDescription("Configure the PRIME Management Plane Layer options")
    primeMngpConfig.setVisible(False)
    
    # Management Plane Serial Profile is always enabled
    global primeConfigMngpSprof
    primeConfigMngpSprof = primeStackConfigComponent.createCommentSymbol("MNGP_SPROFILE", primeMngpConfig)
    primeConfigMngpSprof.setLabel("Management Place Serial Profile")
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

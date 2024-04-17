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

global primePalConfig
global primePalConfigOperationMode
global primePalPlcStatus
global primePalRfStatus
global primePalSerialStatus

global pMpalHeaderFile
global pMpalSourceFile

INACTIVE = 0
ACTIVE = 1

def instantiateComponent(primePalConfigComponent):
    Log.writeInfoMessage("Loading PAL Configurator for PRIME")
    
    primePalGroup = Database.findGroup("PRIME PAL")
    if (primePalGroup == None):
        primeStackGroup = Database.findGroup("PRIME STACK")
        if (primeStackGroup == None):
            primePalGroup = Database.createGroup(None, "PRIME PAL")
            Database.setActiveGroup("PRIME PAL")
            primePalGroup.addComponent("prime_pal_config")
        else: 
            primePalGroup = Database.createGroup("PRIME STACK", "PRIME PAL")
    
    # Configure PRIME PAL
    global primePalConfig
    primePalConfig = primePalConfigComponent.createMenuSymbol("PRIME_PAL_Configuration", None)
    primePalConfig.setLabel("PRIME PAL Configuration")
    primePalConfig.setDescription("Configure the PRIME PAL")
    primePalConfig.setVisible(True)
    
    # Select operation mode
    global primePalConfigOperationMode
    primePalOperationModes = ["-- Select a PRIME operation mode from list --", "Hybrid", "PLC", "RF", "PLC+Serial"]
    primePalConfigOperationMode = primePalConfigComponent.createComboSymbol("PRIME_PAL_OPERATION_MODE", primePalConfig, primePalOperationModes)
    primePalConfigOperationMode.setLabel("PRIME PAL operation mode")
    primePalConfigOperationMode.setDescription("Select the PRIME PAL operation mode: Hybrid, only PLC, only RF, or PLC with Serial")
    primePalConfigOperationMode.setVisible(True)
    primePalConfigOperationMode.setReadOnly(False)
    primePalConfigOperationMode.setDependencies(primePalChangeOperationMode, ["PRIME_PAL_OPERATION_MODE"]) 
    
    # Status of PAL PLC
    global primePalPlcStatus
    primePalPlcStatus = primePalConfigComponent.createIntegerSymbol("PRIME_PAL_PLC_STATUS", None)
    primePalPlcStatus.setVisible(False)
    primePalPlcStatus.setDefaultValue(INACTIVE)
    
    # Status of PAL RF
    global primePalRfStatus
    primePalRfStatus = primePalConfigComponent.createIntegerSymbol("PRIME_PAL_RF_STATUS", None)
    primePalRfStatus.setVisible(False)
    primePalRfStatus.setDefaultValue(INACTIVE)
    
    # Status of PAL Serial
    global primePalSerialStatus
    primePalSerialStatus = primePalConfigComponent.createIntegerSymbol("PRIME_PAL_SERIAL_STATUS", None)
    primePalSerialStatus.setVisible(False)
    primePalSerialStatus.setDefaultValue(INACTIVE)

    ############################################################################
    #### Code Generation ####
    ############################################################################
    
    configName = Variables.get("__CONFIGURATION_NAME")
    
    ##### PRIME PAL
    global pPrimePalHeaderFile
    pPrimePalHeaderFile = primePalConfigComponent.createFileSymbol("PRIME_PAL_HEADER", None)
    pPrimePalHeaderFile.setSourcePath("pal/pal.h")
    pPrimePalHeaderFile.setOutputName("pal.h")
    pPrimePalHeaderFile.setDestPath("stack/pal")
    pPrimePalHeaderFile.setProjectPath("config/" + configName + "/stack/pal")
    pPrimePalHeaderFile.setType("HEADER")
    pPrimePalHeaderFile.setEnabled(True)
    
    ##### MPAL
    global pMpalHeaderFile
    pMpalHeaderFile = primePalConfigComponent.createFileSymbol("MPAL_HEADER", None)
    pMpalHeaderFile.setSourcePath("pal/mpal/mpal.h")
    pMpalHeaderFile.setOutputName("mpal.h")
    pMpalHeaderFile.setDestPath("stack/pal/mpal")
    pMpalHeaderFile.setProjectPath("config/" + configName + "/stack/pal/mpal")
    pMpalHeaderFile.setType("HEADER")
    pMpalHeaderFile.setEnabled(False)
    
    global pMpalSourceFile
    pMpalSourceFile = primePalConfigComponent.createFileSymbol("MPAL_SOURCE", None)
    pMpalSourceFile.setSourcePath("pal/mpal/mpal.c.ftl")
    pMpalSourceFile.setOutputName("mpal.c")
    pMpalSourceFile.setDestPath("stack/pal/mpal")
    pMpalSourceFile.setProjectPath("config/" + configName + "/stack/pal/mpal")
    pMpalSourceFile.setType("SOURCE")
    pMpalSourceFile.setEnabled(False)
    pMpalSourceFile.setMarkup(True)
    
#### FreeMaker System Files ######################################################

    primePalSystemDefFile = primePalConfigComponent.createFileSymbol("PRIME_PAL_DEF", None)
    primePalSystemDefFile.setType("STRING")
    primePalSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    primePalSystemDefFile.setSourcePath("pal/templates/system/definitions.h.ftl")
    primePalSystemDefFile.setMarkup(True)
    
def destroyComponent(primePalConfigComponent):
    # Deactivate PAL components
    primePalPlcDisable()
    primePalRfDisable()
    primePalSerialDisable() 
    primeRemoveMpalFiles()
    Database.deactivateComponents(["prime_pal_config"])
    
def primePalChangeOperationMode(symbol, event):
    if (event["value"] == "Hybrid"):
        # Add PLC
        primePalPlcEnable()
        # Add RF    
        primePalRfEnable()
        # Remove Serial 
        primePalSerialDisable()   
        # Add MPAL
        primeAddMpalFiles()
    elif (event["value"] == "PLC"):
        # Add PLC
        primePalPlcEnable()
        # Remove RF
        primePalRfDisable()
        # Remove Serial 
        primePalSerialDisable()   
        # Remove MPAL
        primeRemoveMpalFiles()        
    elif (event["value"] == "RF"):
        # Add RF    
        primePalRfEnable()
        # Remove PLC
        primePalPlcDisable()
        # Remove Serial 
        primePalSerialDisable() 
        # Remove MPAL
        primeRemoveMpalFiles()        
    elif (event["value"] == "PLC+Serial"):
        # Add PLC
        primePalPlcEnable()
        # Add Serial   
        primePalSerialEnable()
        # Remove RF
        primePalRfDisable()
        # Add MPAL
        primeAddMpalFiles()   
    else:
        # Deactivate PAL components
        primePalPlcDisable()
        primePalRfDisable()
        primePalSerialDisable() 
        primeRemoveMpalFiles()

def primePalPlcEnable():
    primePalGroup = Database.findGroup("PRIME PAL")
    if (primePalPlcStatus.getValue() == INACTIVE):
        primePalGroup.addComponent("primePalPlc")
        Database.activateComponents(["primePalPlc"], "PRIME PAL")
        primePalPlcStatus.setValue(ACTIVE)

def primePalPlcDisable():
    if (primePalPlcStatus.getValue() == ACTIVE):
        Database.deactivateComponents(["primePalPlc"])
        primePalPlcStatus.setValue(INACTIVE) 

def primePalRfEnable():
    primePalGroup = Database.findGroup("PRIME PAL")
    if (primePalRfStatus.getValue() == INACTIVE):
        primePalGroup.addComponent("primePalRf")
        Database.activateComponents(["primePalRf"], "PRIME PAL")
        primePalRfStatus.setValue(ACTIVE)
        
def primePalRfDisable():
    if (primePalRfStatus.getValue() == ACTIVE):
        Database.deactivateComponents(["primePalRf"])
        primePalRfStatus.setValue(INACTIVE) 

def primePalSerialEnable():
    primePalGroup = Database.findGroup("PRIME PAL")
    if (primePalSerialStatus.getValue() == INACTIVE):
        primePalGroup.addComponent("primePalSerial")
        Database.activateComponents(["primePalSerial"], "PRIME PAL")
        primePalSerialStatus.setValue(ACTIVE)
        
def primePalSerialDisable():
    if (primePalSerialStatus.getValue() == ACTIVE):
        Database.deactivateComponents(["primePalSerial"])
        primePalSerialStatus.setValue(INACTIVE) 
        
def primeAddMpalFiles():
    pMpalHeaderFile.setEnabled(True)
    pMpalSourceFile.setEnabled(True)
    
def primeRemoveMpalFiles():
    pMpalHeaderFile.setEnabled(False)
    pMpalSourceFile.setEnabled(False)

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
        print "handleMessage: Set Symbol in PRIME PAL Configurator"
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
        if (args["Id"] == "PRIME_PAL_OPERATION_MODE"):
            primePalConfigOperationMode.setVisible(False)
            primePalConfig.setVisible(False)
    elif (messageID == "PRIME_PAL_NONE"):
        print "handleMessage: Remove all PRIME PAL layers"
        primePalConfigOperationMode.setVisible(False)
        primePalConfig.setVisible(False)
        # Deactivate PAL components
        primePalConfigOperationMode.setValue("-- Select a PRIME operation mode from list --")
        retDict= {"Return": "Success"}
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict


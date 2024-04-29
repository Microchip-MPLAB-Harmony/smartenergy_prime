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
prime_pal_helpkeyword = "prime_pal_configuration"

global primePalPlcSrcFile
global primePalPlcHdrFile
global primePalPlcLocalHdrFile
global primePalPlcRmSrcFile
global primePalPlcRmHdrFile

global primePalRfSrcFile
global primePalRfHdrFile
global primePalRfLocalHdrFile
global primePalRfRmSrcFile
global primePalRfRmHdrFile

global primePalSerialSrcFile
global primePalSerialHdrFile
global primePalSerialLocalHdrFile

def enablePalPlc(palComponent, value):
    primePalPlcSrcFile.setEnabled(value)
    primePalPlcHdrFile.setEnabled(value)
    primePalPlcLocalHdrFile.setEnabled(value)
    primePalPlcRmSrcFile.setEnabled(value)
    primePalPlcRmHdrFile.setEnabled(value)
    palComponent.setDependencyEnabled("primePalPlc", value)

    if value & palComponent.getDependencyEnabled("primePalUSI"):
        Database.activateComponents(["srv_psniffer"])
    else:
        Database.deactivateComponents(["srv_psniffer"]) 

def enablePalRf(palComponent, value):
    primePalRfSrcFile.setEnabled(value)
    primePalRfHdrFile.setEnabled(value)
    primePalRfLocalHdrFile.setEnabled(value)
    primePalRfRmSrcFile.setEnabled(value)
    primePalRfRmHdrFile.setEnabled(value)
    palComponent.setDependencyEnabled("primePalRf", value)

    if value & palComponent.getDependencyEnabled("primePalUSI"):
        Database.activateComponents(["srv_rsniffer"])
    else:
        Database.deactivateComponents(["srv_rsniffer"]) 

def enablePalSerial(palComponent, value):
    primePalSerialSrcFile.setEnabled(value)
    primePalSerialHdrFile.setEnabled(value)
    primePalSerialLocalHdrFile.setEnabled(value)
    palComponent.setDependencyEnabled("primePalSerial", value)

    if value & palComponent.getDependencyEnabled("primePalUSI"):
        Database.activateComponents(["srv_psniffer"])
    else:
        Database.deactivateComponents(["srv_psniffer"]) 

def enablePhySniffer(palComponent, value):
    palComponent.setDependencyEnabled("primePalUSI", value)
    
    if value and palComponent.getDependencyEnabled("primePalPlc"):
        Database.activateComponents(["srv_psniffer"])
    else:
        Database.deactivateComponents(["srv_psniffer"]) 
        
    if value & palComponent.getDependencyEnabled("primePalRf"):
        Database.activateComponents(["srv_rsniffer"])
    else:
        Database.deactivateComponents(["srv_rsniffer"]) 
        
    if value & palComponent.getDependencyEnabled("primePalSerial"):
        Database.activateComponents(["srv_psniffer"])
    else:
        Database.deactivateComponents(["srv_psniffer"]) 

def showUSISymbol(symbol, event):
    symbol.setVisible(event["value"])

    if (event["value"] == True):
        usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
        symbol.setMax(len(usiInstances) - 1)

def showSymbol(symbol, event):
    symbol.setVisible(event["value"])

def updatePalDependencies(symbol, event):
    idSymbol = event["id"]
    localComponent = symbol.getComponent()

    if (idSymbol == "PRIME_PAL_PLC_EN"):
        enablePalPlc(localComponent, event["value"])
    elif (idSymbol == "PRIME_PAL_RF_EN"):
        enablePalRf(localComponent, event["value"])
    elif (idSymbol == "PRIME_PAL_SERIAL_EN"):
        enablePalSerial(localComponent, event["value"])
    elif (idSymbol == "PRIME_PAL_PHY_SNIFFER"):
        enablePhySniffer(localComponent, event["value"])

def freqHopGetChannelList(rangeValues):
    channels = []
    rangeSplit = rangeValues.split(",")
    for channel in rangeSplit:
        subRangeSplit = channel.split("-")
        if len(subRangeSplit) == 1:
            channels.append(int(channel))
        else:
            rangeIni = int(subRangeSplit[0])
            rangeEnd = int(subRangeSplit[1])
            channelRange = range(rangeIni, rangeEnd + 1)
            for chnInRange in channelRange:
                channels.append(chnInRange)

    channelList = ",".join(map(str,channels))
    return channelList

def freqHopUpdateRanges(symbol, event):
    rangeValues = event["value"]
    if (rangeValues):
        channelList = freqHopGetChannelList(rangeValues)
        Database.setSymbolValue("primePal", "PRIME_PAL_RF_FREQ_HOPPING_RANGE_VALUES", channelList)
        print("CHRIS dbg -> freqHopUpdateRanges: {}".format(channelList))
            
def freqHopUpdateBcnRanges(symbol, event):
    rangeValues = event["value"]
    if (rangeValues):
        channelList = freqHopGetChannelList(rangeValues)
        Database.setSymbolValue("primePal", "PRIME_PAL_RF_FREQ_HOPPING_BCN_RANGE_VALUES", channelList)
        print("CHRIS dbg -> freqHopUpdateBcnRanges: {}".format(channelList))

def instantiateComponent(primePalComponent):
    Log.writeInfoMessage("Loading PAL for PRIME")

    primePalComponent.setDependencyEnabled("primePalRf", False)
    primePalComponent.setDependencyEnabled("primePalSerial", False)
    primePalComponent.setDependencyEnabled("primePalUSI", False)
    
    # Configure PRIME PAL
    primePalInterfaces = primePalComponent.createMenuSymbol("PRIME_PAL_INTERFACES", None)
    primePalInterfaces.setLabel("Select PHY layer interfaces")
    primePalInterfaces.setDescription("Enable interfaces to be used through the PRIME PAL")
    primePalInterfaces.setVisible(True)
    primePalInterfaces.setHelp(prime_pal_helpkeyword)

    primePalPlc = primePalComponent.createBooleanSymbol("PRIME_PAL_PLC_EN", primePalInterfaces)
    primePalPlc.setLabel("Enable PLC PHY interface")
    primePalPlc.setDefaultValue(True)
    primePalPlc.setHelp(prime_pal_helpkeyword)

    primePalPlcChnComment = primePalComponent.createCommentSymbol("PRIME_PAL_PLC_CHN_COMMENT", primePalPlc)
    primePalPlcChnComment.setVisible(True)
    primePalPlcChnComment.setLabel("***Review PLC PHY driver configuration to set PLC PHY Channels according to the PRIME Band Plan***")
    primePalPlcChnComment.setDependencies(showSymbol, ["PRIME_PAL_PLC_EN"])

    global primePalPlcPVDDMonitor
    primePalPlcPVDDMonitor = primePalComponent.createBooleanSymbol("PRIME_PAL_PLC_PVDD_MONITOR", primePalPlc)
    primePalPlcPVDDMonitor.setLabel("PVDD Monitor")
    primePalPlcPVDDMonitor.setDefaultValue(False)
    primePalPlcPVDDMonitor.setHelp(prime_pal_helpkeyword)
    if Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE") == "PL360":
        primePalPlcPVDDMonitor.setReadOnly(True)
    primePalPlcPVDDMonitor.setDependencies(showSymbol, ["PRIME_PAL_PLC_EN"])

    primePalRf = primePalComponent.createBooleanSymbol("PRIME_PAL_RF_EN", primePalInterfaces)
    primePalRf.setLabel("Enable RF PHY interface")
    primePalRf.setDefaultValue(False)
    primePalRf.setHelp(prime_pal_helpkeyword)

    primePalRfFreqHopping = primePalComponent.createBooleanSymbol("PRIME_PAL_RF_FREQ_HOPPING", primePalRf)
    primePalRfFreqHopping.setLabel("Enable Frequency Hopping")
    primePalRfFreqHopping.setVisible(False)
    primePalRfFreqHopping.setDefaultValue(False)
    primePalRfFreqHopping.setHelp(prime_pal_helpkeyword)
    primePalRfFreqHopping.setDependencies(showSymbol, ["PRIME_PAL_RF_EN"])

    primePalRfFreqHoppingChnRange = primePalComponent.createStringSymbol("PRIME_PAL_RF_FREQ_HOPPING_RANGE", primePalRfFreqHopping)
    primePalRfFreqHoppingChnRange.setLabel("Channel Sequence")
    primePalRfFreqHoppingChnRange.setVisible(False)
    primePalRfFreqHoppingChnRange.setHelp(prime_pal_helpkeyword)
    primePalRfFreqHoppingChnRange.setDependencies(showSymbol, ["PRIME_PAL_RF_FREQ_HOPPING"])

    primePalRfFreqHoppingChnRangeValues = primePalComponent.createStringSymbol("PRIME_PAL_RF_FREQ_HOPPING_RANGE_VALUES", primePalRfFreqHopping)
    primePalRfFreqHoppingChnRangeValues.setLabel("")
    primePalRfFreqHoppingChnRangeValues.setVisible(False)
    primePalRfFreqHoppingChnRangeValues.setDependencies(freqHopUpdateRanges, ["PRIME_PAL_RF_FREQ_HOPPING_RANGE"])

    primePalRfFreqHoppingChnBcnRange = primePalComponent.createStringSymbol("PRIME_PAL_RF_FREQ_HOPPING_BCN_RANGE", primePalRfFreqHopping)
    primePalRfFreqHoppingChnBcnRange.setLabel("BCN Channel Sequence")
    primePalRfFreqHoppingChnBcnRange.setVisible(False)
    primePalRfFreqHoppingChnBcnRange.setHelp(prime_pal_helpkeyword)
    primePalRfFreqHoppingChnBcnRange.setDependencies(showSymbol, ["PRIME_PAL_RF_FREQ_HOPPING"])

    primePalRfFreqHoppingChnBcnRangeValues = primePalComponent.createStringSymbol("PRIME_PAL_RF_FREQ_HOPPING_BCN_RANGE_VALUES", primePalRfFreqHopping)
    primePalRfFreqHoppingChnBcnRangeValues.setLabel("")
    primePalRfFreqHoppingChnBcnRangeValues.setVisible(False)
    primePalRfFreqHoppingChnBcnRangeValues.setDependencies(freqHopUpdateBcnRanges, ["PRIME_PAL_RF_FREQ_HOPPING_BCN_RANGE"])

    primePalSerial = primePalComponent.createBooleanSymbol("PRIME_PAL_SERIAL_EN", primePalInterfaces)
    primePalSerial.setLabel("Enable Serial PHY interface")
    primePalSerial.setDefaultValue(False)
    primePalSerial.setHelp(prime_pal_helpkeyword)

    primePalPhySniffer = primePalComponent.createBooleanSymbol("PRIME_PAL_PHY_SNIFFER", None)
    primePalPhySniffer.setLabel("Enable PRIME PHY sniffer")
    primePalPhySniffer.setDefaultValue(False)
    primePalPhySniffer.setHelp(prime_pal_helpkeyword)

    primePalPlcUSIInstance = primePalComponent.createIntegerSymbol("PRIME_PAL_PHY_SNIFFER_USI_INSTANCE", primePalPhySniffer)
    primePalPlcUSIInstance.setLabel("USI Instance")
    primePalPlcUSIInstance.setDefaultValue(0)
    primePalPlcUSIInstance.setMax(0)
    primePalPlcUSIInstance.setMin(0)
    primePalPlcUSIInstance.setVisible(False)
    primePalPlcUSIInstance.setHelp(prime_pal_helpkeyword)
    primePalPlcUSIInstance.setDependencies(showUSISymbol, ["PRIME_PAL_PHY_SNIFFER"])

    primePalDummy = primePalComponent.createMenuSymbol("PRIME_PAL_DUMMY", None)
    primePalDummy.setLabel("")
    primePalDummy.setDescription("")
    primePalDummy.setVisible(False)
    primePalDummy.setHelp(prime_pal_helpkeyword)
    primePalDummy.setDependencies(updatePalDependencies, ["PRIME_PAL_PLC_EN", "PRIME_PAL_RF_EN", "PRIME_PAL_SERIAL_EN", "PRIME_PAL_PHY_SNIFFER"])
    
    ############################################################################
    #### Code Generation ####
    ############################################################################
    
    configName = Variables.get("__CONFIGURATION_NAME")
    
    ##### PRIME PAL
    primePalSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_SOURCE", None)
    primePalSrcFile.setSourcePath("pal/pal.c.ftl")
    primePalSrcFile.setOutputName("pal.c")
    primePalSrcFile.setDestPath("stack/pal")
    primePalSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalSrcFile.setType("SOURCE")
    primePalSrcFile.setMarkup(True)

    primePalHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_HEADER", None)
    primePalHdrFile.setSourcePath("pal/pal.h")
    primePalHdrFile.setOutputName("pal.h")
    primePalHdrFile.setDestPath("stack/pal")
    primePalHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalHdrFile.setType("HEADER")

    primePalHdrLocalFile = primePalComponent.createFileSymbol("PRIME_PAL_HEADER_LOCAL", None)
    primePalHdrLocalFile.setSourcePath("pal/pal_local.h.ftl")
    primePalHdrLocalFile.setOutputName("pal_local.h")
    primePalHdrLocalFile.setDestPath("stack/pal")
    primePalHdrLocalFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalHdrLocalFile.setType("SOURCE")
    primePalHdrLocalFile.setMarkup(True)

    ##### PRIME PAL PLC
    global primePalPlcSrcFile
    primePalPlcSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_PLC_SOURCE", None)
    primePalPlcSrcFile.setSourcePath("pal/plc/pal_plc.c.ftl")
    primePalPlcSrcFile.setOutputName("pal_plc.c")
    primePalPlcSrcFile.setDestPath("stack/pal")
    primePalPlcSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalPlcSrcFile.setType("SOURCE")
    primePalPlcSrcFile.setMarkup(True)

    global primePalPlcHdrFile
    primePalPlcHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_PLC_HEADER", None)
    primePalPlcHdrFile.setSourcePath("pal/plc/pal_plc.h.ftl")
    primePalPlcHdrFile.setOutputName("pal_plc.h")
    primePalPlcHdrFile.setDestPath("stack/pal")
    primePalPlcHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalPlcHdrFile.setType("SOURCE")
    primePalPlcHdrFile.setMarkup(True)

    global primePalPlcLocalHdrFile
    primePalPlcLocalHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_PLC_LOCAL_HEADER", None)
    primePalPlcLocalHdrFile.setSourcePath("pal/plc/pal_plc_local.h.ftl")
    primePalPlcLocalHdrFile.setOutputName("pal_plc_local.h")
    primePalPlcLocalHdrFile.setDestPath("stack/pal")
    primePalPlcLocalHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalPlcLocalHdrFile.setType("SOURCE")
    primePalPlcLocalHdrFile.setMarkup(True)
    
    global primePalPlcRmSrcFile
    primePalPlcRmSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_PLC_SOURCE_RM", None)
    primePalPlcRmSrcFile.setSourcePath("pal/plc/pal_plc_rm.c")
    primePalPlcRmSrcFile.setOutputName("pal_plc_rm.c")
    primePalPlcRmSrcFile.setDestPath("stack/pal")
    primePalPlcRmSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalPlcRmSrcFile.setType("SOURCE")

    global primePalPlcRmHdrFile
    primePalPlcRmHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_PLC_HEADER_RM", None)
    primePalPlcRmHdrFile.setSourcePath("pal/plc/pal_plc_rm.h")
    primePalPlcRmHdrFile.setOutputName("pal_plc_rm.h")
    primePalPlcRmHdrFile.setDestPath("stack/pal")
    primePalPlcRmHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalPlcRmHdrFile.setType("SOURCE")

    ##### PRIME PAL RF
    global primePalRfSrcFile
    primePalRfSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_RF_SOURCE", None)
    primePalRfSrcFile.setSourcePath("pal/rf/pal_rf.c.ftl")
    primePalRfSrcFile.setOutputName("pal_rf.c")
    primePalRfSrcFile.setDestPath("stack/pal")
    primePalRfSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalRfSrcFile.setType("SOURCE")
    primePalRfSrcFile.setEnabled(False)
    primePalRfSrcFile.setMarkup(True)

    global primePalRfHdrFile
    primePalRfHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_RF_HEADER", None)
    primePalRfHdrFile.setSourcePath("pal/rf/pal_rf.h.ftl")
    primePalRfHdrFile.setOutputName("pal_rf.h")
    primePalRfHdrFile.setDestPath("stack/pal")
    primePalRfHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalRfHdrFile.setType("SOURCE")
    primePalRfHdrFile.setEnabled(False)
    primePalRfHdrFile.setMarkup(True)

    global primePalRfLocalHdrFile
    primePalRfLocalHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_RF_LOCAL_HEADER", None)
    primePalRfLocalHdrFile.setSourcePath("pal/rf/pal_rf_local.h.ftl")
    primePalRfLocalHdrFile.setOutputName("pal_rf_local.h")
    primePalRfLocalHdrFile.setDestPath("stack/pal")
    primePalRfLocalHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalRfLocalHdrFile.setType("SOURCE")
    primePalRfLocalHdrFile.setEnabled(False)
    primePalRfLocalHdrFile.setMarkup(True)
    
    global primePalRfRmSrcFile
    primePalRfRmSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_RF_SOURCE_RM", None)
    primePalRfRmSrcFile.setSourcePath("pal/rf/pal_rf_rm.c")
    primePalRfRmSrcFile.setOutputName("pal_rf_rm.c")
    primePalRfRmSrcFile.setDestPath("stack/pal")
    primePalRfRmSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalRfRmSrcFile.setType("SOURCE")
    primePalRfRmSrcFile.setEnabled(False)

    global primePalRfRmHdrFile
    primePalRfRmHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_RF_HEADER_RM", None)
    primePalRfRmHdrFile.setSourcePath("pal/rf/pal_rf_rm.h")
    primePalRfRmHdrFile.setOutputName("pal_rf_rm.h")
    primePalRfRmHdrFile.setDestPath("stack/pal")
    primePalRfRmHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalRfRmHdrFile.setType("SOURCE")
    primePalRfRmHdrFile.setEnabled(False)

    ##### PRIME PAL SERIAL
    global primePalSerialSrcFile
    primePalSerialSrcFile = primePalComponent.createFileSymbol("PRIME_PAL_SERIAL_SOURCE", None)
    primePalSerialSrcFile.setSourcePath("pal/serial/pal_serial.c")
    primePalSerialSrcFile.setOutputName("pal_serial.c")
    primePalSerialSrcFile.setDestPath("stack/pal")
    primePalSerialSrcFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalSerialSrcFile.setType("SOURCE")
    primePalSerialSrcFile.setEnabled(False)

    global primePalSerialHdrFile
    primePalSerialHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_SERIAL_HEADER", None)
    primePalSerialHdrFile.setSourcePath("pal/serial/pal_serial.h")
    primePalSerialHdrFile.setOutputName("pal_serial.h")
    primePalSerialHdrFile.setDestPath("stack/pal")
    primePalSerialHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalSerialHdrFile.setType("SOURCE")
    primePalSerialHdrFile.setEnabled(False)

    global primePalSerialLocalHdrFile
    primePalSerialLocalHdrFile = primePalComponent.createFileSymbol("PRIME_PAL_SERIAL_LOCAL_HEADER", None)
    primePalSerialLocalHdrFile.setSourcePath("pal/serial/pal_serial_local.h")
    primePalSerialLocalHdrFile.setOutputName("pal_serial_local.h")
    primePalSerialLocalHdrFile.setDestPath("stack/pal")
    primePalSerialLocalHdrFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalSerialLocalHdrFile.setType("SOURCE")
    primePalSerialLocalHdrFile.setEnabled(False)

    primePalTypesFile = primePalComponent.createFileSymbol("PRIME_PAL_TYPES", None)
    primePalTypesFile.setSourcePath("pal/pal_types.h")
    primePalTypesFile.setOutputName("pal_types.h")
    primePalTypesFile.setDestPath("stack/pal")
    primePalTypesFile.setProjectPath("config/" + configName + "/stack/pal")
    primePalTypesFile.setType("HEADER")

    
#### FreeMaker System Files ######################################################

    primePalSystemDefFile = primePalComponent.createFileSymbol("PRIME_PAL_DEF", None)
    primePalSystemDefFile.setType("STRING")
    primePalSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    primePalSystemDefFile.setSourcePath("pal/templates/system/definitions.h.ftl")
    primePalSystemDefFile.setMarkup(True)

    primePalSystemConfigFile = primePalComponent.createFileSymbol("PRIME_PAL_CONFIGURATION", None)
    primePalSystemConfigFile.setType("STRING")
    primePalSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    primePalSystemConfigFile.setSourcePath("pal/templates/system/configuration.h.ftl")
    primePalSystemConfigFile.setMarkup(True)
    

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
    global primePalPlcPVDDMonitor
    retDict= {}

    if messageID == "PLC_DEVICE":
        retDict = {"Return": "Success"}
        plcDevice = args["Device"]
        if plcDevice == "PL360":
            primePalPlcPVDDMonitor.setReadOnly(True)
            primePalPlcPVDDMonitor.setValue(False)
        else:
            primePalPlcPVDDMonitor.setReadOnly(False)
            
    elif (messageID == "SET_SYMBOL"):
        print("handleMessage: Set Symbol in PRIME PAL Configurator")
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict


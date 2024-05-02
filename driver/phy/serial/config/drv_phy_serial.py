"""*****************************************************************************
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
*****************************************************************************"""

drv_phy_serial_helpkeyword = "drv_phy_serial_configurations"

def showSymbol(symbol, event):
    symbol.setVisible(event["value"])

    if (event["value"] == 0):
        usiInstances = filter(lambda k: "srv_usi_" in k, Database.getActiveComponentIDs())
        print("usi instance: ")
        print(usiInstances)
        symbol.setMax(len(usiInstances) - 1)

def instantiateComponent(primeDrvPhySerialComponent):

    Log.writeInfoMessage("Loading PRIME PHY serial driver component")
    print("Printing PRIME PHY serial driver component")
    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")
    processor = Variables.get("__PROCESSOR")
    
    phySerialUSIInstance = primeDrvPhySerialComponent.createIntegerSymbol("DRV_PHY_SERIAL_USI_INSTANCE", None)
    phySerialUSIInstance.setLabel("USI Instance")
    phySerialUSIInstance.setDefaultValue(0)
    phySerialUSIInstance.setMax(0)
    phySerialUSIInstance.setMin(0)
    phySerialUSIInstance.setVisible(True)
    phySerialUSIInstance.setHelp(" PHY Serial USI instance ")
    phySerialUSIInstance.setDependencies(showSymbol, ["DRV_PHY_SERIAL_USI_INSTANCE"])

    ############################################################################################################
    # PHY Serial FILES

    drvPhySerialSrcFile = primeDrvPhySerialComponent.createFileSymbol("DRV_PHY_SERIAL_SOURCE", None)
    drvPhySerialSrcFile.setSourcePath("driver/phy/serial/src/drv_phy_serial.c.ftl")
    drvPhySerialSrcFile.setOutputName("drv_phy_serial.c")
    drvPhySerialSrcFile.setDestPath("driver/plc/phy_serial")
    drvPhySerialSrcFile.setProjectPath("config/" + configName + "/driver/plc/phy_serial/")
    drvPhySerialSrcFile.setType("SOURCE")
    drvPhySerialSrcFile.setMarkup(True)

    drvPhySerialHdrFile = primeDrvPhySerialComponent.createFileSymbol("DRV_PHY_SERIAL_HEADER", None)
    drvPhySerialHdrFile.setSourcePath("driver/phy/serial/src/drv_phy_serial.h.ftl")
    drvPhySerialHdrFile.setOutputName("drv_phy_serial.h")
    drvPhySerialHdrFile.setDestPath("driver/plc/phy_serial")
    drvPhySerialHdrFile.setProjectPath("config/" + configName + "/driver/plc/phy_serial/")
    drvPhySerialHdrFile.setType("HEADER")
    drvPhySerialHdrFile.setMarkup(True)

    
#### FreeMaker System Files ######################################################

    drvPhySerialSystemDefFile = primeDrvPhySerialComponent.createFileSymbol("DRV_PHY_SERIAL_DEF", None)
    drvPhySerialSystemDefFile.setType("STRING")
    drvPhySerialSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    drvPhySerialSystemDefFile.setSourcePath("driver/phy/serial/templates/system/definitions.h.ftl")
    drvPhySerialSystemDefFile.setMarkup(True)

 
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
   
global primeSrvStatus
    
INACTIVE = 0
ACTIVE = 1

def instantiateComponent(primeSrvConfigComponent):
    Log.writeInfoMessage("Loading Service Configurator for PRIME")
    
    primeSrvGroup = Database.findGroup("PRIME SERVICES")
    if (primeSrvGroup == None):
        primeStackGroup = Database.findGroup("PRIME STACK")
        if (primeStackGroup == None):
            primeSrvGroup = Database.createGroup(None, "PRIME SERVICES")
            Database.setActiveGroup("PRIME SERVICES")
            primeSrvGroup.addComponent("prime_srv_config")
        else: 
            primeSrvGroup = Database.createGroup("PRIME STACK", "PRIME SERVICES")
            
    # Status of services
    global primeSrvStatus
    primeSrvStatus = primeSrvConfigComponent.createIntegerSymbol("PRIME_SRV_STATUS", None)
    primeSrvStatus.setVisible(False)
    primeSrvStatus.setDefaultValue(INACTIVE)   
    
def destroyComponent(primeSrvConfigComponent):
    # Deactivate Services
    primeSrvDeactivate()
    Database.deactivateComponents(["prime_srv_config"])   
       
def primeSrvActivate():
    if (primeSrvStatus.getValue() == INACTIVE):
        primeSrvGroup = Database.findGroup("PRIME SERVICES")
        primeSrvGroup.addComponent("srvRandom")
        primeSrvGroup.addComponent("srvQueue")
        primeSrvGroup.addComponent("srvLogReport")
        primeSrvGroup.addComponent("srv_pcrc")
        primeSrvGroup.addComponent("srv_usi")
        primeSrvGroup.addComponent("srvSecurity")
        #primeSrvGroup.addComponent("primeFwMgmt")
        primeSrvGroup.addComponent("primeStorage")
        primeSrvGroup.addComponent("primeUserPib")
        primeSrvGroup.addComponent("primeResetHandler")        
        primeSrvGroup.addComponent("primeTimeManagement")
        Database.activateComponents(["srvRandom", "srvQueue", "srvLogReport", "srv_pcrc", "srv_usi", "srvSecurity", "primeStorage", "primeUserPib", "primeResetHandler", "primeTimeManagement"], "PRIME SERVICES")
        primeSrvStatus.setValue(ACTIVE)
        
def primeSrvDeactivate():
    if (primeSrvStatus.getValue() == ACTIVE):
        Database.deactivateComponents(["srvRandom", "srvQueue", "srvLogReport", "srv_pcrc", "srv_usi", "srvSecurity", "primeStorage", "primeUserPib", "primeResetHandler", "primeTimeManagement"])
        primeSrvStatus.setValue(INACTIVE) 


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
        print "handleMessage: Set Symbol in PRIME Services"
        retDict= {"Return": "Success"}
        Database.setSymbolValue(args["Component"], args["Id"], args["Value"])
    elif (messageID == "PRIME_SERVICES_ACTIVATE"):
        print "handleMessage: Activate PRIME Services"
        primeSrvActivate()
        retDict= {"Return": "Success"}
    elif (messageID == "PRIME_SERVICES_DEACTIVATE"):
        print "handleMessage: Deactivate PRIME Services"
        primeSrvDeactivate()
        retDict= {"Return": "Success"}
    else:
        retDict= {"Return": "UnImplemented Command"}
    return retDict


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
        #primeSrvGroup.addComponent("primeTimeMgmt")
        #primeSrvGroup.addComponent("primeUserPibs")  
        #primeSrvGroup.addComponent("primeResetHandler")
        #primeSrvGroup.addComponent("primeFwMgmt")
        primeSrvGroup.addComponent("primeStorage")
        Database.activateComponents(["srvRandom", "srvQueue", "srvLogReport", "srv_pcrc", "srv_usi", "srvSecurity", "primeStorage"], "PRIME SERVICES")
        primeSrvStatus.setValue(ACTIVE)
        
def primeSrvDeactivate():
    if (primeSrvStatus.getValue() == ACTIVE):
        Database.deactivateComponents(["srvRandom", "srvQueue", "srvLogReport", "srv_pcrc", "srv_usi", "srvSecurity", "primeStorage"])
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


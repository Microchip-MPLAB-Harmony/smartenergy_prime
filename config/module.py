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

###################### PRIME STACK  ######################
def loadModule():

    print("Load Module: Harmony Smart Energy PRIME Stack")

    ###########  PRIME PAL Configurations  ###########
    global primePalConfigComponent
    primePalConfigComponent = Module.CreateComponent("prime_pal_config", "PRIME PAL", "/SmartEnergy/PRIME Stack/PAL", "pal/config/pal_configurator.py")
    primePalConfigComponent.setDisplayType("PRIME PAL Configurator")
    
    ## PRIME PAL PLC
    primePalPlcComponent = Module.CreateComponent("primePalPlc", "PRIME PAL PLC", "/SmartEnergy/PRIME Stack/PAL", "pal/plc/config/pal_plc.py")
    primePalPlcComponent.addCapability("primePalPlc", "PRIME_PAL_PLC", True) 
    #primePalPlcComponent.addDependency("primePalPlc_PCoup_dependency", "PCOUP", True, True)
    primePalPlcComponent.setDisplayType("PRIME PAL PLC")

    ## PRIME PAL RF
    primePalRfComponent = Module.CreateComponent("primePalRf", "PRIME PAL RF", "/SmartEnergy/PRIME Stack/PAL", "pal/rf/config/pal_rf.py")
    primePalRfComponent.addCapability("primePalRf", "PRIME_PAL_RF", True) 
    #primePalRfComponent.addDependency("primePalRf_DrvRfPhy_dependency", "DRV_RF_PHY", False, True)
    primePalRfComponent.setDisplayType("PRIME PAL RF")
    
    ## PRIME PAL Serial
    primePalSerialComponent = Module.CreateComponent("primePalSerial", "PRIME PAL Serial", "/SmartEnergy/PRIME Stack/PAL", "pal/serial/config/pal_serial.py")
    primePalSerialComponent.addCapability("primePalSerial", "PRIME_PAL_SERIAL", True) 
    #primePalSerialComponent.addDependency("primePalSerial_DrvSerialPhy_dependency", "DRV_SERIAL_PHY", False, True)
    primePalSerialComponent.setDisplayType("PRIME PAL SERIAL")

    ###########  PRIME Stack Configurations  ###########
    global primeStackConfigComponent
    primeStackConfigComponent = Module.CreateComponent("prime_stack_config", "PRIME Stack", "/SmartEnergy/PRIME Stack", "prime/config/prime_configurator.py")
    primeStackConfigComponent.setDisplayType("PRIME Stack Configurator")

    ###########  PRIME Service Configurations  ###########
    global primeSrvConfigComponent
    primeSrvConfigComponent = Module.CreateComponent("prime_srv_config", "PRIME Services", "/SmartEnergy/PRIME Stack/Services", "service/config/srv_configurator.py")
    primeSrvConfigComponent.setDisplayType("PRIME Services Configurator")
    
    ## PRIME Storage Service
    primeStorageComponent = Module.CreateComponent("primeStorage", "PRIME Storage", "/SmartEnergy/PRIME Stack/Services", "service/storage/config/srv_storage.py")
    primeStorageComponent.setDisplayType("PRIME Service")
    
    

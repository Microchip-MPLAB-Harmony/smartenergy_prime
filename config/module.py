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
###################### PRIME STACK  ######################
def loadModule():

    print("Load Module: Harmony Smart Energy PRIME Stack")

    ## PRIME PAL
    primePalComponent = Module.CreateComponent("primePal", "PRIME PAL", "/SmartEnergy/PRIME Stack", "pal/config/pal.py")
    primePalComponent.addCapability("primePal", "PRIME_PAL", True) 
    primePalComponent.addDependency("primePalPlc", "DRV_PLC_PHY", True, True)
    primePalComponent.addDependency("primePalRf", "DRV_RF_PHY", True, True)
    primePalComponent.addDependency("primePalSerial", "PSER", True, True)
    primePalComponent.addDependency("primePalUSI", "USI", True, True)
    primePalComponent.setDisplayType("PRIME PAL")


    ## PRIME Driver
    primeDrvComponent = Module.CreateComponent("prime_drv_config", "PRIME Driver", "/SmartEnergy/PRIME Stack/Drivers", "driver/config/drv_configurator.py")
    primeDrvComponent.addDependency("primePhySerial", "PRIME_PHY_SERIAL", True) 
    primeDrvComponent.setDisplayType("PRIME Driver Configuration")
    
    ## PRIME PHY SERIAL
    primeDrvPhySerialComponent = Module.CreateComponent("primePhySerial", "PRIME PHY SERIAL", "/SmartEnergy/PRIME Stack/PRIME Driver", "driver/phy/serial/config/drv_phy_serial.py")
    primeDrvPhySerialComponent.addCapability("primePhySerial", "PRIME_PHY_SERIAL", True) 
    primeDrvPhySerialComponent.setDisplayType("PRIME PHY Serial")
    
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
    
    ## PRIME User PIBs Service
    primeUserPibComponent = Module.CreateComponent("primeUserPib", "PRIME User PIBs", "/SmartEnergy/PRIME Stack/Services", "service/user_pib/config/srv_user_pib.py")
    primeUserPibComponent.setDisplayType("PRIME Service")
    
    ## PRIME Reset Handler Service
    primeResetHandlerComponent = Module.CreateComponent("primeResetHandler", "PRIME Reset Handler", "/SmartEnergy/PRIME Stack/Services", "service/reset_handler/config/srv_reset_handler.py")
    primeResetHandlerComponent.setDisplayType("PRIME Service")
    
    ## PRIME Time Management
    primeTimeManagementComponent = Module.CreateComponent("primeTimeManagement", "PRIME Time Management", "/SmartEnergy/PRIME Stack/Services", "service/time_management/config/srv_time_management.py")
    primeTimeManagementComponent.addDependency("primeTimeManagement_sysTime_dependency", "SYS_TIME", True, True)
    primeTimeManagementComponent.setDisplayType("PRIME Service")   

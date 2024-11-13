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
    primePalComponent.addDependency("primePalSerial", "PRIME_PHY_SERIAL", True, True)
    primePalComponent.addDependency("primePalUSI", "USI", True, True)
    primePalComponent.setDisplayType("PRIME PAL")

    ## PRIME PHY SERIAL Driver
    primeDrvPhySerialComponent = Module.CreateComponent("primePhySerialDrv", "PRIME PHY SERIAL", "/SmartEnergy/Drivers", "driver/phy/serial/config/drv_phy_serial.py")
    primeDrvPhySerialComponent.addCapability("primePhySerial", "PRIME_PHY_SERIAL", True) 
    primeDrvPhySerialComponent.addDependency("phySerialUSI", "USI", True, True)
    primeDrvPhySerialComponent.setDisplayType("PRIME PHY Serial Driver")
    
    ###########  PRIME Stack Configurations  ###########
    global primeStackConfigComponent
    primeStackConfigComponent = Module.CreateComponent("prime_config", "PRIME Stack", "/SmartEnergy/PRIME Stack", "prime/config/prime_configurator.py")
    primeStackConfigComponent.setDisplayType("PRIME Stack")
    primeStackConfigComponent.addDependency("primePal_dep", "PRIME_PAL", True, True)
    primeStackConfigComponent.addDependency("primeUSI_dep", "USI", True, True)
    primeStackConfigComponent.addDependency("primeFu_dep", "FMW Upgrade", True, True)
    primeStackConfigComponent.addCapability("primeStackCapability", "PRIME Stack", True)

    ###########  PRIME Service Configurations  ###########
    ## PRIME Storage Service
    primeStorageComponent = Module.CreateComponent("primeStorage", "PRIME Storage", "/SmartEnergy/Services", "service/storage/config/srv_storage.py")
    primeStorageComponent.setDisplayType("PRIME Service")
    primeStorageComponent.addCapability("Storage", "PRIME Storage", True) 
    
    ## PRIME User PIBs Service
    primeUserPibComponent = Module.CreateComponent("primeUserPib", "User PIBs", "/SmartEnergy/Services", "service/user_pib/config/srv_user_pib.py")
    primeUserPibComponent.setDisplayType("PRIME Service")
    primeUserPibComponent.addCapability("User PIBs", "User PIBs", True) 
    
    ## PRIME Reset Handler Service
    primeResetHandlerComponent = Module.CreateComponent("primeResetHandler", "Reset Handler", "/SmartEnergy/Services", "service/reset_handler/config/srv_reset_handler.py")
    primeResetHandlerComponent.setDisplayType("PRIME Service")
    primeResetHandlerComponent.addCapability("Reset Handler", "Reset Handler", True) 
    
    ## PRIME Time Management
    primeTimeManagementComponent = Module.CreateComponent("primeTimeManagement", "Time Management", "/SmartEnergy/Services", "service/time_management/config/srv_time_management.py")
    primeTimeManagementComponent.setDisplayType("PRIME Service")
    primeTimeManagementComponent.addDependency("SysTime_dep", "SYS_TIME", True, True)
    primeTimeManagementComponent.addCapability("Time Management", "Time Management", True) 

    ## PRIME Firmware Upgrade
    primeFUCompoment = Module.CreateComponent("primeFirmwareUpgrade", "Firmware Upgrade", "/SmartEnergy/Services", "service/firmware_upgrade/config/srv_firmware_upgrade.py")
    primeFUCompoment.setDisplayType("PRIME Service")
    primeFUCompoment.addMultiDependency("Memory_dep", "DRV_MEDIA", "DRV_MEDIA", True)
    primeFUCompoment.addDependency("srv_usi_CRC_dep", "PCRC", "PCRC", True, True)
    primeFUCompoment.addCapability("FMW Upgrade", "FMW Upgrade", True) 

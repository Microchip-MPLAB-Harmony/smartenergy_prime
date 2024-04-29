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
   
global primeDrvStatus
    
INACTIVE = 0
ACTIVE = 1

def instantiateComponent(primeDrvConfigComponent):
    Log.writeInfoMessage("Loading driver Configurator for PRIME")
    
    primeDrvGroup = Database.findGroup("PRIME Driver")
    if (primeDrvGroup == None):
        primeStackGroup = Database.findGroup("PRIME STACK")
        if (primeStackGroup == None):
            primeDrvGroup = Database.createGroup(None, "PRIME Driver")
            Database.setActiveGroup("PRIME DRIVERS")
            primeDrvGroup.addComponent("prime_drv_config")
        else: 
            primeDrvGroup = Database.createGroup("PRIME STACK", "PRIME DRIVERS")
            
    # Status of drivers
    global primeDrvStatus
    primeDrvStatus = primeDrvConfigComponent.createIntegerSymbol("PRIME_DRV_Configuration", None)
    primeDrvStatus.setVisible(False)
    primeDrvStatus.setDefaultValue(INACTIVE)   
    
def destroyComponent(primeDrvConfigComponent):
    # Deactivate Services
    Database.deactivateComponents(["prime_drv_config"])   
       

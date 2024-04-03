/*******************************************************************************
  PRIME API Definitions Header

  Company:
    Microchip Technology Inc.

  File Name:
    prime_api_defs.h

  Summary:
    PRIME API Definitions Header File

  Description:
    This file contains definitions of the PRIME API functions to be used by the 
    PRIME application.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
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
*******************************************************************************/
//DOM-IGNORE-END


#ifndef PRIME_API_DEFS_H_INCLUDE
#define PRIME_API_DEFS_H_INCLUDE

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PRIME stack initialization

  Summary:
    Function pointer to initialize the PRIME stack.

  Description:
    This function pointer is used to initialize the PRIME stack.

  Remarks:
    None.
*/
typedef void (*PRIME_API_INITIALIZE)(void *halApi);

// *****************************************************************************
/* PRIME stack state machine maintenance

  Summary:
    Function pointer to maintain the PRIME stack state machine.

  Description:
    This function pointer is used to maintain the PRIME stack internal state 
    machine and generate callbacks.

  Remarks:
    None.
*/
typedef void (*PRIME_API_TASKS)(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* PRIME_API_DEFS_H_INCLUDE */

/*******************************************************************************
 End of File
*/

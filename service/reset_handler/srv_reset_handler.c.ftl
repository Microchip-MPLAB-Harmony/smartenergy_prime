/*******************************************************************************
  PRIME Reset Handler Service Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    srv_reset_handler.c

  Summary:
    Source code for the PRIME Reset Handle service implementation.

  Description:
    The Reset Handler service provides a simple interface to trigger system
    resets and to manage and store reset causes.This file contains the source
    code for the implementation of this service.
*******************************************************************************/

///DOM-IGNORE-BEGIN
/*
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
*/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "srv_reset_handler.h"
#include "device.h"

<#if RESET_HANDLING_EN == true>
// *****************************************************************************
// *****************************************************************************
// Section: File Scope Function Declarations
// *****************************************************************************
// *****************************************************************************

void DumpStack(uint32_t stack[]);

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void lSRV_RESET_HANDLER_StoreResetInfo(SRV_RESET_HANDLER_RESET_CAUSE resetType)
{
    uint32_t resetInfo;
    uint16_t numResets;

    /* Read and increase number of resets since start-up */
<#if (prime_config)??>
<#if (prime_config.PRIME_MODE == "SN") || (prime_config.PRIME_MODE == "BN" && prime_config.BN_SLAVE_EN == true)>
    numResets = (uint16_t)(SUPC_GPBRRead(GPBR_REGS_5) >> 16);
<#else>
    numResets = (uint16_t)(SUPC_GPBRRead(GPBR_REGS_0) >> 16);
</#if>
</#if>
    ++numResets;

    /* Store reset information */
    resetInfo = ((uint32_t)(numResets) << 16) + (uint32_t)(resetType);
<#if (prime_config)??>
<#if (prime_config.PRIME_MODE == "SN") || (prime_config.PRIME_MODE == "BN" && prime_config.BN_SLAVE_EN == true)>
    SUPC_GPBRWrite(GPBR_REGS_5, resetInfo);
<#else>
    SUPC_GPBRWrite(GPBR_REGS_0, resetInfo);
</#if>
</#if>
}


/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 5.8 deviated 6 times. Deviation record ID - H3_MISRAC_2012_R_5_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 5.8" "H3_MISRAC_2012_R_5_8_DR_1"
</#if>
void HardFault_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}

void MemoryManagement_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}

void BusFault_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}

void UsageFault_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}

<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
void DWDT0_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}
<#else>
void WDT_Handler(void)
{
    __asm volatile(
        "TST LR, #4 \n"
        "ITE EQ     \n"
        "MRSEQ R0, MSP  \n"
        "MRSNE R0, PSP  \n"
        "B DumpStack    \n");
}
</#if>

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 5.8"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
/* MISRA C-2012 deviation block end */

volatile uint32_t saved_r0;
volatile uint32_t saved_r1;
volatile uint32_t saved_r2;
volatile uint32_t saved_r3;
volatile uint32_t saved_r12;
volatile uint32_t saved_lr;
volatile uint32_t saved_pc;
volatile uint32_t saved_psr;
volatile uint32_t saved_hfsr;
volatile uint32_t saved_cfsr;
void DumpStack(uint32_t stack[])
{
    saved_r0 = stack[0];
    saved_r1 = stack[1];
    saved_r2 = stack[2];
    saved_r3 = stack[3];
    saved_r12 = stack[4];
    saved_lr = stack[5];
    saved_pc = stack[6];
    saved_psr = stack[7];
    saved_hfsr = SCB->HFSR;
    saved_cfsr = SCB->CFSR;

    /* Store registers */
<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
<#if (prime_config)??>
<#if (prime_config.PRIME_MODE == "SN") || (prime_config.PRIME_MODE == "BN" && prime_config.BN_SLAVE_EN == true)>
    SUPC_GPBRWrite(GPBR_REGS_6, saved_pc);
    SUPC_GPBRWrite(GPBR_REGS_7, saved_lr);
    SUPC_GPBRWrite(GPBR_REGS_8, saved_psr);
    SUPC_GPBRWrite(GPBR_REGS_9, saved_hfsr);
    SUPC_GPBRWrite(GPBR_REGS_10, saved_cfsr);
    SUPC_GPBRWrite(GPBR_REGS_11, saved_r0);
    SUPC_GPBRWrite(GPBR_REGS_12, saved_r1);
    SUPC_GPBRWrite(GPBR_REGS_13, saved_r2);
    SUPC_GPBRWrite(GPBR_REGS_14, saved_r3);
    SUPC_GPBRWrite(GPBR_REGS_15, saved_r12);
<#else>
    SUPC_GPBRWrite(GPBR_REGS_1, saved_pc);
    SUPC_GPBRWrite(GPBR_REGS_2, saved_lr);
    SUPC_GPBRWrite(GPBR_REGS_3, saved_psr);
    SUPC_GPBRWrite(GPBR_REGS_4, saved_hfsr);
    SUPC_GPBRWrite(GPBR_REGS_5, saved_cfsr);
    SUPC_GPBRWrite(GPBR_REGS_6, saved_r0);
    SUPC_GPBRWrite(GPBR_REGS_7, saved_r1);
    SUPC_GPBRWrite(GPBR_REGS_8, saved_r2);
    SUPC_GPBRWrite(GPBR_REGS_9, saved_r3);
    SUPC_GPBRWrite(GPBR_REGS_10, saved_r12);
</#if>
</#if>
</#if>
<#if __PROCESSOR?matches("ATSAME70*")>
<#if (prime_config)??>
<#if (prime_config.PRIME_MODE == "SN") || (prime_config.PRIME_MODE == "BN" && prime_config.BN_SLAVE_EN == true)>
    SUPC_GPBRWrite(GPBR_REGS_6, saved_pc);
    SUPC_GPBRWrite(GPBR_REGS_7, saved_lr);
<#else>
    SUPC_GPBRWrite(GPBR_REGS_1, saved_pc);
    SUPC_GPBRWrite(GPBR_REGS_2, saved_lr);
    SUPC_GPBRWrite(GPBR_REGS_3, saved_psr);
    SUPC_GPBRWrite(GPBR_REGS_4, saved_hfsr);
    SUPC_GPBRWrite(GPBR_REGS_5, saved_cfsr);
    SUPC_GPBRWrite(GPBR_REGS_6, saved_r0);
    SUPC_GPBRWrite(GPBR_REGS_7, saved_r1);
</#if>
</#if>
</#if>

    SRV_RESET_HANDLER_RESET_CAUSE resetType;

    /* Check forced hard fault */
    if ((saved_hfsr & (1UL << 30)) != 0U)
    {
        resetType = RESET_HANDLER_HARD_FAULT_RESET;
    
    } /* Check bus fault on a vector table */
    else if ((saved_hfsr & (1U << 1)) != 0U)
    {
        resetType = RESET_HANDLER_VECTOR_FAULT_RESET;
    }
    else
    {
        /* Check usage error */
        if((saved_cfsr & 0xFFFF0000U) != 0U)
        {
            resetType = RESET_HANDLER_USAGE_FAULT_RESET;
        }
        /* Check bus fault error */
        else if((saved_cfsr & 0xFF00U) != 0U)
        {
            resetType = RESET_HANDLER_BUS_FAULT_RESET;
        }
        /* Check memory management error */
        else if((saved_cfsr & 0xFFU) != 0U)
        {
            resetType = RESET_HANDLER_MEM_MANAGE_RESET;
        }
        else
        {
            resetType = RESET_HANDLER_WATCHDOG_RESET;
        }
    }

    /* Trigger reset */
    SRV_RESET_HANDLER_RestartSystem(resetType);
}
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Reset Handler Service Interface Implementation
// *****************************************************************************
// *****************************************************************************

void SRV_RESET_HANDLER_Initialize(void)
{
<#if RESET_HANDLING_EN == true>
    RSTC_RESET_CAUSE resetTypeAux;
    SRV_RESET_HANDLER_RESET_CAUSE resetType;

    /* Read reset type */
    resetTypeAux = RSTC_ResetCauseGet() >> RSTC_SR_RSTTYP_Pos;
    resetType = (SRV_RESET_HANDLER_RESET_CAUSE)(resetTypeAux);

    /* If it is a software reset, it is from one of these handlers */
    if ((resetType != RESET_HANDLER_SOFTWARE_RESET) && (resetType != RESET_HANDLER_GENERAL_RESET))
    {
        /* Store reset information for other reset types */
        lSRV_RESET_HANDLER_StoreResetInfo(resetType);
    }
</#if>
}

void SRV_RESET_HANDLER_RestartSystem(SRV_RESET_HANDLER_RESET_CAUSE resetType)
{
<#if RESET_HANDLING_EN == true>
    /* Store reset information */
    lSRV_RESET_HANDLER_StoreResetInfo(resetType);
<#else>
    (void)resetType;
</#if>

<#if (drvMet)??>
    /* Stop Metrology and its peripherals before reset */
    (void) DRV_METROLOGY_Close();
</#if>

    /* Trigger software reset */
    RSTC_Reset(RSTC_PROCESSOR_RESET);
}

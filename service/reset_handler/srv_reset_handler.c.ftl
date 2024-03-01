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
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
	numResets = (uint16_t)(SUPC_GPBRRead(GPBR_REGS_5) >> 16);
<#else>
	numResets = (uint16_t)(SUPC_GPBRRead(GPBR_REGS_0) >> 16);
</#if>
</#if>
	++numResets;

	/* Store reset information */
    resetInfo = (numResets << 16) + (uint8_t)resetType;
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
	SUPC_GPBRWrite(GPBR_REGS_5, resetInfo);
<#else>
	SUPC_GPBRWrite(GPBR_REGS_0, resetInfo);
</#if>
</#if>
}

void HardFault_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}

void MemoryManagement_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}

void BusFault_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}

void UsageFault_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}

<#if __PROCESSOR?matches("PIC32CX.*MT.*")>
void DWDT0_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}
<#else>
void WDT_Handler(void)
{
	__asm volatile(
		"TST LR, #4	\n"
		"ITE EQ		\n"
		"MRSEQ R0, MSP	\n"
		"MRSNE R0, PSP	\n"
		"B DumpStack	\n");
}
</#if>

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
<#if (prime_stack_config)??>
<#if (prime_stack_config.PRIME_MODE == "SN") || (prime_stack_config.PRIME_MODE == "BN" && prime_stack_config.BN_SLAVE_EN == true)>
	SUPC_GPBRWrite(GPBR_REGS_6, saved_pc);
	SUPC_GPBRWrite(GPBR_REGS_7, saved_lr);
	SUPC_GPBRWrite(GPBR_REGS_8, saved_psr);
	SUPC_GPBRWrite(GPBR_REGS_9, saved_hfsr);
	SUPC_GPBRWrite(GPBR_REGS_10, saved_cfsr);
	SUPC_GPBRWrite(GPBR_REGS_11, saved_r0);
	SUPC_GPBRWrite(GPBR_REGS_12, saved_r1);
	SUPC_GPBRWrite(GPBR_REGS_13, saved_r2);
	SUPC_GPBRWrite(GPBR_REGS_14, saved_r3);
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
	SUPC_GPBRWrite(GPBR_REGS_1, saved_pc);
	SUPC_GPBRWrite(GPBR_REGS_2, saved_lr);
	SUPC_GPBRWrite(GPBR_REGS_3, saved_psr);
	SUPC_GPBRWrite(GPBR_REGS_4, saved_hfsr);
	SUPC_GPBRWrite(GPBR_REGS_5, saved_cfsr);
	SUPC_GPBRWrite(GPBR_REGS_6, saved_r0);
	SUPC_GPBRWrite(GPBR_REGS_7, saved_r1);
</#if>

	SRV_RESET_HANDLER_RESET_CAUSE resetType;

	/* Check forced hard fault */
	if ((saved_hfsr & (1 << 30)) != 0) {
	  	resetType = RESET_HANDLER_HARD_FAULT_RESET;

		/* Check usage error */
		if((saved_cfsr & 0xFFFF0000) != 0) {
			resetType = RESET_HANDLER_USAGE_FAULT_RESET;
		}

		/* Check bus fault error */
		if((saved_cfsr & 0xFF00) != 0) {
			resetType = RESET_HANDLER_BUS_FAULT_RESET;
		}

		/* Check memory management error */
		if((saved_cfsr & 0xFF) != 0) {
			resetType = RESET_HANDLER_MEM_MANAGE_RESET;
		}
	/* Check bus fault on a vector table */
	} else if ((saved_hfsr & (1 << 1)) != 0) {
		resetType = RESET_HANDLER_VECTOR_FAULT_RESET;
	} else {
		resetType = RESET_HANDLER_WATCHDOG_RESET;
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
    SRV_RESET_HANDLER_RESET_CAUSE resetType;

	/* Read reset type */
	resetType = (SRV_RESET_HANDLER_RESET_CAUSE)(RSTC_ResetCauseGet() >> RSTC_SR_RSTTYP_Pos);

	/* If it is a software reset, it is from one of these handlers */
	if ((resetType != RESET_HANDLER_SOFTWARE_RESET) && (resetType != RESET_HANDLER_GENERAL_RESET)) {
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
	
	/* Trigger software reset */
    RSTC_Reset(RSTC_PROCESSOR_RESET);
}

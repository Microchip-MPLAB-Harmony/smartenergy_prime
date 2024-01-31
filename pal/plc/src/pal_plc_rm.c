/**
 * \file
 *
 * \brief PAL: Physical Abstraction Layer Robust Management Module
 *
 * Copyright (c) 2022 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/* System includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime includes */
#include "pal.h"
#include "pal_rm.h"

#ifdef PAL_MULTIPHY
#include "mpal.h"
#endif

#ifndef PAL_MULTIPHY
#define pal_plc_rm_minimum_quality pal_rm_minimum_quality
#define pal_plc_rm_get_less_robust_mod pal_rm_get_less_robust_mod
#endif


#ifdef PAL_PLC

/**
 * \weakgroup prime_pal_group
 * @{
 */

#define CINR_CONV(x)                     (((((int16_t)x) + 1000) * 4) / 100)
#define EVM_INV_CONV(x)                  ((((uint32_t)x) * 512) / 10)
#define EVM_INV_ACC_CONV(x)              ((((uint64_t)x) * 131072 * 256) / 100)

/* Conditions PL360 */
typedef struct {
	uint8_t mod_chosen;
	uint8_t nar_bnd_percent_min;
	uint8_t nar_bnd_percent_max;
	uint8_t imp_percent_min;
	uint8_t imp_percent_max;
	uint16_t evm;
	uint16_t evm_acc;
	uint8_t cinr_avg;
	uint8_t cinr_min;
	uint8_t bersoft;
	uint8_t bersoft_max;
} x_rm_conditions_360_t;

typedef struct {
	const x_rm_conditions_360_t *px_rm_condition_360;
	uint8_t uc_num_conditions;
} x_rm_conditions_t;

#define NUM_CONDITIONS_360_R_DBPSK       23
static const x_rm_conditions_360_t sx_conditions_360_r_dbpsk[NUM_CONDITIONS_360_R_DBPSK] = {
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 2000, CINR_CONV(  900), CINR_CONV(  450), 133, 136},
		{PAL_PLC_D8PSK_C,   1, 117,   0,   0, 1000, 2100, CINR_CONV(  425), CINR_CONV(  325), 162, 170},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 3250, CINR_CONV(  525), CINR_CONV(  375), 161, 165},
		{PAL_PLC_DQPSK_C,   1,  66,   0,   0, 1000, 3960, CINR_CONV( -150), CINR_CONV( -250), 166, 173},
		{PAL_PLC_DQPSK_C,  67, 255,   0,   0, 1000, 2790, CINR_CONV(   50), CINR_CONV(   50), 170, 173},
		{PAL_PLC_DBPSK_C,   0,   0,   0,   0, 1000, 4710, CINR_CONV(  300), CINR_CONV(   75), 177, 180},
		{PAL_PLC_DBPSK_C,   1,  43,   0,   0, 1000, 4310, CINR_CONV( -150), CINR_CONV( -600), 176, 177},
		{PAL_PLC_DBPSK_C,  44, 133,   0,   0, 1000, 4420, CINR_CONV( -175), CINR_CONV( -175), 178, 186},
		{PAL_PLC_DBPSK_C, 134, 146,   0,   0, 1000, 2960, CINR_CONV(   75), CINR_CONV(  -50), 173, 179},
		{PAL_PLC_DBPSK_C, 147, 255,   0,   0, 1000, 2810, CINR_CONV(  575), CINR_CONV(  375), 180, 180},
		{PAL_PLC_R_DQPSK,   0,   0,   0,   0, 1000, 5470, CINR_CONV(  225), CINR_CONV(  150), 182, 182},
		{PAL_PLC_R_DQPSK,   0,   0,   1, 255, 1000, 5360, CINR_CONV(  100), CINR_CONV(    0), 187, 191},
		{PAL_PLC_R_DQPSK,   1,  53,   0,   0, 1000, 4630, CINR_CONV( -175), CINR_CONV( -650), 176, 177},
		{PAL_PLC_R_DQPSK,  54, 139,   0,   0, 1000, 4420, CINR_CONV( -200), CINR_CONV( -200), 178, 186},
		{PAL_PLC_R_DQPSK, 140, 149,   0,   0, 1000, 3420, CINR_CONV(  125), CINR_CONV(  -50), 178, 178},
		{PAL_PLC_R_DQPSK, 150, 255,   0,   0, 1000, 3230, CINR_CONV(  575), CINR_CONV(  375), 179, 179},
		{PAL_PLC_R_DBPSK,   0,   0,   0,   0, 1000, 5950, CINR_CONV(  100), CINR_CONV(  -75), 187, 191},
		{PAL_PLC_R_DBPSK,   0,   0,   1, 255, 1000, 6890, CINR_CONV(  100), CINR_CONV(    0), 190, 190},
		{PAL_PLC_R_DBPSK,   1,  53,   0,   0, 1000, 4630, CINR_CONV( -175), CINR_CONV( -650), 176, 177},
		{PAL_PLC_R_DBPSK,  54,  75,   0,   0, 1000, 4420, CINR_CONV( -200), CINR_CONV( -200), 185, 186},
		{PAL_PLC_R_DBPSK,  76, 139,   0,   0, 1000, 3560, CINR_CONV( -200), CINR_CONV( -200), 173, 173},
		{PAL_PLC_R_DBPSK, 140, 149,   0,   0, 1000, 2940, CINR_CONV(  125), CINR_CONV(  -50), 177, 180},
		{PAL_PLC_R_DBPSK, 149, 255,   0,   0, 1000, 2760, CINR_CONV(  575), CINR_CONV(  375), 180, 180},
};

#define NUM_CONDITIONS_360_R_DQPSK       15
static const x_rm_conditions_360_t sx_conditions_360_r_dqpsk[NUM_CONDITIONS_360_R_DQPSK] = {
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 1670, CINR_CONV(  900), CINR_CONV(  675), 149, 152},
		{PAL_PLC_D8PSK_C,   1, 123,   0,   0, 1000, 1620, CINR_CONV(  550), CINR_CONV(  200), 164, 165},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 2840, CINR_CONV(  550), CINR_CONV(  475), 175, 177},
		{PAL_PLC_DQPSK_C,   1, 141,   0,   0, 1000, 2570, CINR_CONV( -150), CINR_CONV( -150), 173, 181},
		{PAL_PLC_DQPSK_C, 142, 255,   0,   0, 1000, 1800, CINR_CONV(  525), CINR_CONV(  150), 173, 173},
		{PAL_PLC_DBPSK_C,   0,   0,   0,   0, 1000, 4590, CINR_CONV(  400), CINR_CONV(  100), 184, 184},
		{PAL_PLC_DBPSK_C,   1,  75,   0,   0, 1000, 3740, CINR_CONV( -175), CINR_CONV( -575), 180, 182},
		{PAL_PLC_DBPSK_C,  76, 139,   0,   0, 1000, 2740, CINR_CONV( -200), CINR_CONV( -200), 177, 185},
		{PAL_PLC_DBPSK_C, 140, 144,   0,   0, 1000, 2960, CINR_CONV(  900), CINR_CONV(  650), 179, 181},
		{PAL_PLC_DBPSK_C, 144, 146,   0,   0, 1000, 1800, CINR_CONV(  525), CINR_CONV(  150), 173, 173},
		{PAL_PLC_DBPSK_C, 147, 255,   0,   0, 1000, 2420, CINR_CONV(  700), CINR_CONV(  400), 181, 181},
		{PAL_PLC_R_DQPSK,   0,   0,   0,   0, 1000, 5380, CINR_CONV(  200), CINR_CONV(    0), 187, 187},
		{PAL_PLC_R_DQPSK,   0,   0,   1, 255, 1000, 6900, CINR_CONV(  100), CINR_CONV(   25), 190, 190},
		{PAL_PLC_R_DQPSK,   1, 139,   0,   0, 1000, 4160, CINR_CONV( -200), CINR_CONV( -625), 178, 182},
		{PAL_PLC_R_DQPSK, 140, 255,   0,   0, 1000, 2780, CINR_CONV(  250), CINR_CONV(    0), 178, 178},
};

#define NUM_CONDITIONS_360_DBPSK_C       18
static const x_rm_conditions_360_t sx_conditions_360_dbpsk_c[NUM_CONDITIONS_360_DBPSK_C] = {
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 1770, CINR_CONV(  950), CINR_CONV(  450), 148, 148},
		{PAL_PLC_D8PSK_C,   1, 136,   0,   0, 1000, 2530, CINR_CONV(  400), CINR_CONV(  300), 163, 163},
		{PAL_PLC_D8PSK_C, 137, 255,   0,   0, 1000, 1710, CINR_CONV(  400), CINR_CONV(  275), 169, 169},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 2800, CINR_CONV(  550), CINR_CONV(  425), 159, 159},
		{PAL_PLC_DQPSK_C,   1, 152,   0,   0, 1000, 2560, CINR_CONV(  175), CINR_CONV(   25), 171, 171},
		{PAL_PLC_DQPSK_C, 153, 255,   0,   0, 1000, 2020, CINR_CONV( 1050), CINR_CONV( 1025), 171, 171},
		{PAL_PLC_DBPSK_C,   0,   0,   0,   0, 1000, 4530, CINR_CONV(  350), CINR_CONV(  225), 178, 178},
		{PAL_PLC_DBPSK_C,   1,  59,   0,   0, 1000, 4590, CINR_CONV( -200), CINR_CONV( -550), 177, 177},
		{PAL_PLC_DBPSK_C,  60, 139,   0,   0, 1000, 3890, CINR_CONV( -175), CINR_CONV( -175), 172, 172},
		{PAL_PLC_DBPSK_C, 140, 156,   0,   0, 1000, 2650, CINR_CONV(  -50), CINR_CONV(  -75), 176, 176},
		{PAL_PLC_DBPSK_C, 157, 255,   0,   0, 1000, 2710, CINR_CONV(  575), CINR_CONV(  400), 180, 180},
		{PAL_PLC_R_DQPSK,   0,   0,   0,   0, 1000, 5120, CINR_CONV(  175), CINR_CONV(   75), 186, 186},
		{PAL_PLC_R_DQPSK,   0,   0,   1,  67, 1000, 5220, CINR_CONV(  400), CINR_CONV(    0), 190, 190},
		{PAL_PLC_R_DQPSK,   0,   0,  68, 255, 1000, 5200, CINR_CONV(  250), CINR_CONV(    0), 177, 177},
		{PAL_PLC_R_DQPSK,   1,  34,   0,   0, 1000, 4680, CINR_CONV( -350), CINR_CONV( -600), 177, 177},
		{PAL_PLC_R_DQPSK,  35, 139,   0,   0, 1000, 4030, CINR_CONV( -200), CINR_CONV( -625), 174, 174},
		{PAL_PLC_R_DQPSK, 140, 157,   0,   0, 1000, 3790, CINR_CONV(  -50), CINR_CONV(  -75), 173, 173},
		{PAL_PLC_R_DQPSK, 158, 255,   0,   0, 1000, 2710, CINR_CONV(  575), CINR_CONV(  400), 180, 180},
};

#define NUM_CONDITIONS_360_DQPSK_C       12
static const x_rm_conditions_360_t sx_conditions_360_dqpsk_c[NUM_CONDITIONS_360_DQPSK_C] = {
		{PAL_PLC_DQPSK,      0,   0,   0,   0,  439,  960, CINR_CONV( 1275), CINR_CONV(  700), 124, 124},
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 1400, CINR_CONV(  900), CINR_CONV(  725), 154, 154},
		{PAL_PLC_D8PSK_C,   1, 136,   0,   0, 1000, 2530, CINR_CONV(  500), CINR_CONV(  175), 166, 166},
		{PAL_PLC_D8PSK_C, 137, 255,   0,   0, 1000, 1910, CINR_CONV(  775), CINR_CONV(  700), 173, 173},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 2820, CINR_CONV(  575), CINR_CONV(  425), 172, 172},
		{PAL_PLC_DQPSK_C,   1,  27,   0,   0, 1000, 3310, CINR_CONV(  -50), CINR_CONV( -175), 170, 170},
		{PAL_PLC_DQPSK_C,  28, 255,   0,   0, 1000, 2430, CINR_CONV(   75), CINR_CONV(   50), 176, 176},
		{PAL_PLC_DBPSK_C,   0,   0,   0,   0, 1000, 3960, CINR_CONV(  325), CINR_CONV(  200), 183, 183},
		{PAL_PLC_DBPSK_C,   1, 139,   0,   0, 1000, 4100, CINR_CONV( -300), CINR_CONV( -550), 177, 177},
		{PAL_PLC_DBPSK_C, 140, 160,   0,   0, 1000, 2960, CINR_CONV(  250), CINR_CONV(   25), 177, 177},
		{PAL_PLC_DBPSK_C, 144, 157,   0,   0, 1000, 3530, CINR_CONV(  975), CINR_CONV(  525), 180, 180},
		{PAL_PLC_DBPSK_C, 158, 255,   0,   0, 1000, 2490, CINR_CONV(  625), CINR_CONV(  400), 180, 180},
};

#define NUM_CONDITIONS_360_D8PSK_C       11
static const x_rm_conditions_360_t sx_conditions_360_d8psk_c[NUM_CONDITIONS_360_D8PSK_C] = {
		{PAL_PLC_DQPSK,      0,   0,   0,   0,  402,  760, CINR_CONV( 1000), CINR_CONV(  650), 160, 160},
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 1290, CINR_CONV(  950), CINR_CONV(  550), 168, 168},
		{PAL_PLC_D8PSK_C,   1, 101,   0,   0, 1000, 2390, CINR_CONV(  -25), CINR_CONV(  -25), 170, 170},
		{PAL_PLC_D8PSK_C,  91, 101,   0,   0, 1000, 3980, CINR_CONV(  450), CINR_CONV(  200), 175, 175},
		{PAL_PLC_D8PSK_C, 102, 255,   0,   0, 1000, 1720, CINR_CONV(  575), CINR_CONV(  300), 171, 171},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 2760, CINR_CONV(  550), CINR_CONV(  475), 178, 178},
		{PAL_PLC_DQPSK_C,   1,  91,   0,   0, 1000, 2540, CINR_CONV( -150), CINR_CONV( -975), 178, 178},
		{PAL_PLC_DQPSK_C,  91, 139,   0,   0, 1000, 4790, CINR_CONV( -200), CINR_CONV( -200), 178, 178},
		{PAL_PLC_DQPSK_C, 140, 149,   0,   0, 1000, 2270, CINR_CONV(  425), CINR_CONV(  225), 183, 183},
		{PAL_PLC_DQPSK_C, 144, 149,   0,   0, 1000, 1460, CINR_CONV( -100), CINR_CONV( -100), 172, 172},
		{PAL_PLC_DQPSK_C, 150, 255,   0,   0, 1000, 2240, CINR_CONV(  850), CINR_CONV(  600), 183, 183},
};

#define NUM_CONDITIONS_360_DBPSK         7
static const x_rm_conditions_360_t sx_conditions_360_dbpsk[NUM_CONDITIONS_360_DBPSK] = {
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0, 1000, 1320, CINR_CONV(  650), CINR_CONV(  525), 143, 143},
		{PAL_PLC_D8PSK_C,   1, 107,   0,   0, 1000, 3840, CINR_CONV(  150), CINR_CONV(    0), 165, 165},
		{PAL_PLC_D8PSK_C, 108, 255,   0,   0, 1000, 1500, CINR_CONV(  975), CINR_CONV(  525), 170, 170},
		{PAL_PLC_DQPSK_C,   0,   0,   0,   0, 1000, 2470, CINR_CONV(  550), CINR_CONV(  450), 162, 162},
		{PAL_PLC_DQPSK_C,   1, 107,   0,   0, 1000, 4090, CINR_CONV( -125), CINR_CONV( -125), 170, 170},
		{PAL_PLC_DQPSK_C, 108, 133,   0,   0, 1000, 2180, CINR_CONV(  475), CINR_CONV(  325), 170, 170},
		{PAL_PLC_DQPSK_C, 134, 255,   0,   0, 1000, 1420, CINR_CONV( 1075), CINR_CONV(  650), 170, 170},
};

#define NUM_CONDITIONS_360_DQPSK         7
static const x_rm_conditions_360_t sx_conditions_360_dqpsk[NUM_CONDITIONS_360_DQPSK] = {
		{PAL_PLC_D8PSK,      0,   0,   0,   0,  306,  400, CINR_CONV( 1900), CINR_CONV(  950), 102, 102},
		{PAL_PLC_DQPSK,      0,   0,   0,   0,  417,  790, CINR_CONV( 1250), CINR_CONV(  725), 124, 124},
		{PAL_PLC_D8PSK_C,   0,   0,   0,   0,  648, 1510, CINR_CONV(  875), CINR_CONV(  575), 150, 150},
		{PAL_PLC_D8PSK_C,   1, 108,   0,   0, 1000, 1730, CINR_CONV(  175), CINR_CONV(    0), 172, 172},
		{PAL_PLC_D8PSK_C, 109, 117,   0,   0, 1000, 1690, CINR_CONV(  300), CINR_CONV(  175), 170, 170},
		{PAL_PLC_D8PSK_C, 118, 125,   0,   0, 1000, 1340, CINR_CONV(  150), CINR_CONV(    0), 163, 163},
		{PAL_PLC_D8PSK_C, 126, 255,   0,   0, 1000, 1910, CINR_CONV(  275), CINR_CONV(  200), 175, 175},
};

#define NUM_CONDITIONS_360_D8PSK         2
static const x_rm_conditions_360_t sx_conditions_360_d8psk[NUM_CONDITIONS_360_D8PSK] = {
		{PAL_PLC_D8PSK,      0,   0,   0,   0,  238,  400, CINR_CONV( 1525), CINR_CONV(  850), 139, 139},
		{PAL_PLC_DQPSK,      0,   0,   0,   0,  429,  920, CINR_CONV( 1150), CINR_CONV(  625), 151, 151},
};

static x_rm_conditions_t sx_conditions_360[8];

/* Bandwidth of every modulation */
static const uint8_t suc_bandwidth[16] = {
		40,  /* PAL_PLC_DBPSK */
		80,  /* PAL_PLC_DQPSK */
		120, /* PAL_PLC_D8PSK */
		0,
		20,  /* PAL_PLC_DBPSK_C */
		40,  /* PAL_PLC_DQPSK_C */
		60,  /* PAL_PLC_D8PSK_C */
		0,
		0,
		0,
		0,
		0,
		5,   /* PAL_PLC_R_DBPSK */
		10,  /* PAL_PLC_R_DQPSK */
		0,
		0    /* PAL_OUTDATED_INF */
};

/*
 * \brief get modulation index for the minimum quality table
 *
 * \params modulation  Given modulation
 *
 * \return modulation index
 */
static uint8_t _get_modulation_index(uint8_t modulation)
{
	uint8_t uc_index;

	switch (modulation) {
	case PAL_PLC_D8PSK:
		uc_index = 0;
		break;

	case PAL_PLC_DQPSK:
		uc_index = 1;
		break;

	case PAL_PLC_DBPSK:
		uc_index = 2;
		break;

	case PAL_PLC_D8PSK_C:
		uc_index = 3;
		break;

	case PAL_PLC_DQPSK_C:
		uc_index = 4;
		break;

	case PAL_PLC_DBPSK_C:
		uc_index = 5;
		break;

	case PAL_PLC_R_DQPSK:
		uc_index = 6;
		break;

	case PAL_PLC_R_DBPSK:
	default:
		uc_index = 7;
		break;

	}

	return uc_index;
}

/**
 * \brief Function to provide the conditions for the given modulation
 *
 * \param uc_scheme            Scheme
 *
 * \return Pointer to the conditions for PL360
 */
static x_rm_conditions_t *_rm_get_conditions_360(uint8_t uc_scheme)
{
	uint8_t uc_index_scheme;

	uc_index_scheme = _get_modulation_index(uc_scheme);

	return(&sx_conditions_360[uc_index_scheme]);
}

/*
 * \brief function to get valid modulations
 *
 * \param px_msg                         Physical parameters from a received message
 * \param us_estimated_bitrate           Bitrate estimation in Kbs
 * \param uc_less_robust_mod             Less robust modulation
 * \param uc_max_num_channels            Maximum number of channels
 *
 * \return Less robust modulation and estimated bitrate for a valid communication
 */
void pal_plc_rm_max_modulation_rx_msg(rx_msg_t *px_msg, uint16_t *us_estimated_bitrate, uint8_t *uc_less_robust_mod, uint8_t uc_max_num_channels)
{
	x_rm_conditions_t *px_rm_conditions;
	const x_rm_conditions_360_t *px_rm_conditions_360;
	uint64_t ull_evm_acc;
	uint32_t ul_evm;
	uint8_t uc_index;
	uint8_t uc_num_conditions;
	uint8_t uc_best_mod;

	/* Get conditions for the given modulation */
	px_rm_conditions = _rm_get_conditions_360(px_msg->uc_scheme);

	uc_num_conditions = px_rm_conditions->uc_num_conditions;
	px_rm_conditions_360 = px_rm_conditions->px_rm_condition_360;

	uc_best_mod = PAL_OUTDATED_INF;
	for (uc_index = 0; uc_index < uc_num_conditions; uc_index++) {
		ul_evm = EVM_INV_CONV(px_rm_conditions_360->evm);
		ull_evm_acc = EVM_INV_ACC_CONV(px_rm_conditions_360->evm_acc);
		if ((px_msg->uc_nar_bnd_percent >= px_rm_conditions_360->nar_bnd_percent_min) &&
			(px_msg->uc_nar_bnd_percent <= px_rm_conditions_360->nar_bnd_percent_max) &&
			(px_msg->uc_imp_percent >= px_rm_conditions_360->imp_percent_min) &&
			(px_msg->uc_imp_percent <= px_rm_conditions_360->imp_percent_max) &&
			(px_msg->us_evm_payload <= ul_evm) &&
			(px_msg->us_evm_header <= ul_evm) &&
			(px_msg->ul_evm_header_acum <= (uint32_t)ull_evm_acc) &&
			(px_msg->ul_evm_payload_acum <= (uint32_t)ull_evm_acc) &&
			(px_msg->uc_cinr_avg >= px_rm_conditions_360->cinr_avg) &&
			(px_msg->uc_cinr_min >= px_rm_conditions_360->cinr_min) &&
			(px_msg->uc_ber_soft <= px_rm_conditions_360->bersoft) &&
			(px_msg->uc_ber_soft_max <= px_rm_conditions_360->bersoft_max)) {
				uc_best_mod = px_rm_conditions_360->mod_chosen;
				break;
		}

		px_rm_conditions_360++;
	}

	*uc_less_robust_mod = uc_best_mod;
	*us_estimated_bitrate = suc_bandwidth[uc_best_mod];
	if (uc_max_num_channels > 1) {
		*us_estimated_bitrate *= 2;
	}
}

/*
 * \brief function to calculate the LQI
 *
 * \param uc_cinr     Received CINR
 *
 * \return Link quality indication
 */
uint8_t pal_plc_rm_get_lqi(uint8_t uc_cinr)
{
	return ((uc_cinr + 12) / 4);
}

/**
 * \brief Check if the modulation is good enough for a low FER in the given scheme
 *
 * \param us_pch              Channel of reception of the message
 * \param uc_scheme           Modulation to compare
 * \param uc_less_robust_mod  Less robust modulation
 *
 * \return true or false
 */
bool pal_plc_rm_minimum_quality(uint16_t us_pch, uint8_t uc_scheme, uint8_t uc_less_robust_mod)
{
	(void)us_pch;

	if ((suc_bandwidth[uc_less_robust_mod] >= suc_bandwidth[uc_scheme]) && (suc_bandwidth[uc_less_robust_mod] > 0))  {
		return true;
	} else {
		return false;
	}
}

/**
 * \brief return the less robust modulation for a channel from to given
 *
 * \param us_pch            Channel of the modulation
 * \param uc_mod1           Modulation 1 to compare
 * \param uc_mod2           Modulation 2 to compare
 *
 * \return uc_mod1 or uc_mod2 scheme for the given channel
 */
uint8_t pal_plc_rm_get_less_robust_mod(uint16_t us_pch, uint8_t uc_mod1, uint8_t uc_mod2)
{
	(void) us_pch;

	if (suc_bandwidth[uc_mod1] > suc_bandwidth[uc_mod2]) {
		return uc_mod1;
	} else {
		return uc_mod2;
	}
}


/**
 * \brief Initialization function of the PAL RM module in PLC
 *
 */
void pal_plc_rm_init(void)
{
	/* Initialize modulation conditions for PL360 */
	sx_conditions_360[0].px_rm_condition_360 = sx_conditions_360_d8psk;
	sx_conditions_360[0].uc_num_conditions = NUM_CONDITIONS_360_D8PSK;

	sx_conditions_360[1].px_rm_condition_360 = sx_conditions_360_dqpsk;
	sx_conditions_360[1].uc_num_conditions = NUM_CONDITIONS_360_DQPSK;

	sx_conditions_360[2].px_rm_condition_360 = sx_conditions_360_dbpsk;
	sx_conditions_360[2].uc_num_conditions = NUM_CONDITIONS_360_DBPSK;

	sx_conditions_360[3].px_rm_condition_360 = sx_conditions_360_d8psk_c;
	sx_conditions_360[3].uc_num_conditions = NUM_CONDITIONS_360_D8PSK_C;

	sx_conditions_360[4].px_rm_condition_360 = sx_conditions_360_dqpsk_c;
	sx_conditions_360[4].uc_num_conditions = NUM_CONDITIONS_360_DQPSK_C;

	sx_conditions_360[5].px_rm_condition_360 = sx_conditions_360_dbpsk_c;
	sx_conditions_360[5].uc_num_conditions = NUM_CONDITIONS_360_DBPSK_C;

	sx_conditions_360[6].px_rm_condition_360 = sx_conditions_360_r_dqpsk;
	sx_conditions_360[6].uc_num_conditions = NUM_CONDITIONS_360_R_DQPSK;

	sx_conditions_360[7].px_rm_condition_360 = sx_conditions_360_r_dbpsk;
	sx_conditions_360[7].uc_num_conditions = NUM_CONDITIONS_360_R_DBPSK;

}

#endif /* PAL_PLC */

/* @} */

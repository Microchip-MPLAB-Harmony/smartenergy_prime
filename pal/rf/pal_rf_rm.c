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
#include "pal_types.h"
#include "driver/rf215/drv_rf215.h"

/* RF RM mode */
#define PAL_RF_RM_FORCED_OFF   1
#define PAL_RF_RM_FORCED_ON    2

/* RSSI Thresholds */
#define PAL_RF_RM_THRESHOLD_FSK_FEC_OFF     (-89)
#define PAL_RF_RM_THRESHOLD_FSK_FEC_ON      (-94)

static uint8_t palRfRmMode = PAL_RF_RM_FORCED_OFF;

/* Bandwidth of every modulation */
static const uint8_t palRfBandwidth[] = {
        0,   /* PAL_SCHEME_RF */
        50,  /* PAL_SCHEME_RF_FSK_FEC_OFF */
        25   /* PAL_SCHEME_RF_FSK_FEC_ON */
};

uint8_t PAL_RF_RM_GetLqi(int16_t rssi)
{
    if (rssi > 80) 
    {
        return 0xFE;
    } 
    else 
    {
        return (rssi + 174);
    }
}

uint8_t PAL_RF_RM_GetLessRobustModulation(PAL_SCHEME mod1, PAL_SCHEME mod2)
{
    uint8_t index1 = mod1 - PAL_SCHEME_RF;
    uint8_t index2 = mod2 - PAL_SCHEME_RF;

    if (palRfBandwidth[index1] > palRfBandwidth[index2]) 
    {
        return mod1;
    } 
    else 
    {
        return mod2;
    }
}

bool PAL_RF_RM_CheckMinimumQuality(PAL_SCHEME reference, PAL_SCHEME modulation)
{
    if (modulation == PAL_SCHEME_RF)
    {
        return false;
    }
    
    if ((reference == PAL_SCHEME_RF_FSK_FEC_OFF) && (modulation == PAL_SCHEME_RF_FSK_FEC_ON))
    {
        return false;
    } 
    else 
    {
        return true;
    }
}

PAL_SCHEME PAL_RF_RM_GetScheme(void)
{
    switch(palRfRmMode) 
    {
        case PAL_RF_RM_FORCED_OFF:
            return PAL_SCHEME_RF_FSK_FEC_OFF;

        case PAL_RF_RM_FORCED_ON:
            return PAL_SCHEME_RF_FSK_FEC_ON;

        default:
            return PAL_SCHEME_RF_FSK_FEC_OFF;
    }
}

void PAL_RF_RM_SetScheme(PAL_SCHEME scheme)
{
    if (scheme == PAL_SCHEME_RF_FSK_FEC_OFF) 
    {
        palRfRmMode = PAL_RF_RM_FORCED_OFF;
    } 
    else 
    {
        palRfRmMode = PAL_RF_RM_FORCED_ON;
    }
}

void PAL_RF_RM_GetRobustModulation(void *indObj, uint16_t *pBitRate, PAL_SCHEME *pModulation, PAL_CHANNEL_MASK channelMask)
{
    DRV_RF215_RX_INDICATION_OBJ *pIndObj;
    PAL_SCHEME bestScheme = PAL_SCHEME_RF;

    (void)channelMask;

    pIndObj = (DRV_RF215_RX_INDICATION_OBJ *)indObj;

    switch(palRfRmMode) 
    {
        case PAL_RF_RM_FORCED_OFF:
            if (pIndObj->rssiDBm >= PAL_RF_RM_THRESHOLD_FSK_FEC_OFF) 
            {
                bestScheme = PAL_SCHEME_RF_FSK_FEC_OFF;
            }

            break;

        case PAL_RF_RM_FORCED_ON:
            if (pIndObj->rssiDBm >= PAL_RF_RM_THRESHOLD_FSK_FEC_ON) 
            {
                bestScheme = PAL_SCHEME_RF_FSK_FEC_ON;
            }

            break;

        default:
            if (pIndObj->rssiDBm >= PAL_RF_RM_THRESHOLD_FSK_FEC_OFF)
            {
                bestScheme = PAL_SCHEME_RF_FSK_FEC_OFF;
            } 
            else if (pIndObj->rssiDBm >= PAL_RF_RM_THRESHOLD_FSK_FEC_ON) 
            {
                bestScheme = PAL_SCHEME_RF_FSK_FEC_ON;
            }
    }

    *pModulation = bestScheme;

    if (bestScheme != PAL_SCHEME_RF) {
        *pBitRate = palRfBandwidth[bestScheme - PAL_SCHEME_RF];
    } else {
        *pBitRate = 0;
    }
}

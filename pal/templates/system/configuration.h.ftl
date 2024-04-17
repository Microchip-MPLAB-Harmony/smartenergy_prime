/* PRIME PAL Configuration Options */
#define PRIME_PAL_INDEX                     0U
<#if PRIME_PAL_PHY_SNIFFER == true>
#define PRIME_PAL_SNIFFER_USI_INSTANCE      SRV_USI_INDEX_${PRIME_PAL_PLC_USI_INSTANCE?string}
</#if>
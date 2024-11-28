
/* Initialize PRIME */

/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 11.3 deviated twice. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    </#if>
    #pragma coverity compliance block deviate "MISRA C-2012 Rule 11.3" "H3_MISRAC_2012_R_11_3_DR_1"
</#if>
    sysObj.primeStack = PRIME_Initialize(PRIME_INDEX_0, (SYS_MODULE_INIT *)&primeInitData);
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
    #pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
    <#if core.COMPILER_CHOICE == "XC32">
    #pragma GCC diagnostic pop
    </#if>
</#if>
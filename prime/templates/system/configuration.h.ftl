/* PRIME Identification */
#define PRIME_INDEX_0                               0U
#define PRIME_INSTANCES_NUMBER                      1U

<#if (PRIME_MODE == "SN" && PRIME_PROJECT == "application project") || (PRIME_MODE == "BN")>
/* Management Plane USI port */
#define PRIME_MNG_PLANE_USI_INDEX                   ${MNGP_SPROF_USI_INSTANCE}U

</#if>

<#if PRIME_MODE == "SN" && PRIME_PROJECT == "application project">
/* PRIME SN Application Memory Region */
#define PRIME_SN_APP_ADDRESS                        ${PRIME_SN_APP_ADDRESS?string}
#define PRIME_SN_APP_SIZE                           ${PRIME_SN_APP_SIZE?string}
/* PRIME SN v1.4 FW Stack Memory Region */
#define PRIME_SN_FWSTACK14_ADDRESS                  ${PRIME_SN_FWSTACK14_ADDRESS?string}
#define PRIME_SN_FWSTACK14_SIZE                     ${PRIME_SN_FWSTACK14_SIZE?string}
/* PRIME SN PHY Layer Memory Region */
#define PRIME_SN_PHY_ADDRESS                        ${PRIME_SN_PHY_ADDRESS?string}
#define PRIME_SN_PHY_SIZE                           ${PRIME_SN_PHY_SIZE?string}
/* PRIME SN v1.3 FW Stack Memory Region */
#define PRIME_SN_FWSTACK13_ADDRESS                  ${PRIME_SN_FWSTACK13_ADDRESS?string}
#define PRIME_SN_FWSTACK13_SIZE                     ${PRIME_SN_FWSTACK13_SIZE?string}

</#if>

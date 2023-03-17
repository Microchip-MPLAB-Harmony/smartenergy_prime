#include "stack/prime/hal_api/hal_api.h"

#include "stack/prime/prime_api/prime_api.h"
#include "stack/prime/prime_api/prime_api_defs.h"
<#if (PRIME_MODE == "BN") || (PRIME_MODE == "SN" && PRIME_PROJECT == "bin project")>
#include "stack/prime/prime_api/prime_hal_wrapper.h"
</#if>

<#if (PRIME_MODE == "BN") || (PRIME_MODE == "SN" && PRIME_PROJECT == "bin project")>
#include "stack/prime/mac/mac.h"
</#if>
#include "stack/prime/mac/mac_defs.h"
#include "stack/prime/mac/mac_pib.h"

<#if (PRIME_MODE == "BN") || (PRIME_MODE == "SN" && PRIME_PROJECT == "bin project")>
#include "stack/prime/mngp/mngp.h"
</#if>
<#if PRIME_MODE == "BN" && BN_SLAVE_EN == false>
#include "stack/prime/mngp/bmng_api.h"
#include "stack/prime/mngp/bmng_defs.h"
</#if>

<#if PRIME_MODE == "BN">
#include "stack/prime/conv/sscs/null/cl_null.h"
#include "stack/prime/conv/sscs/null/cl_null_api.h"
#include "stack/prime/conv/sscs/iec_4_32/cl_432.h"
#include "stack/prime/conv/sscs/iec_4_32/cl_432_api.h"
</#if>
<#if PRIME_MODE == "SN" && PRIME_PROJECT == "bin project">
#include "stack/prime/conv/sscs/null/cl_null.h"
#include "stack/prime/conv/sscs/iec_4_32/cl_432.h"
</#if>
#include "stack/prime/conv/sscs/iec_4_32/cl_432_defs.h"

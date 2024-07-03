
// <editor-fold defaultstate="collapsed" desc="PRIME Initialization Data">
extern HAL_API primeHalAPI;

/* PRIME Initialization Data */
static PRIME_API_INIT primeApiInit = {
    /* Pointer to the HAL API */
    .halApi = (HAL_API *)&primeHalAPI,
    
    /* PAL index */
    .palIndex = PRIME_PAL_INDEX,
    
    /* Management Plane USI port */
    .mngPlaneUsiPort = PRIME_MNG_PLANE_USI_INDEX
};

// </editor-fold>
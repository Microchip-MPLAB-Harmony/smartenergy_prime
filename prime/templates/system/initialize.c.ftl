    /* Initialize PRIME */
    PRIME_API *primeApi;
    PRIME_API_GetPrimeAPI(&primeApi);
    primeApi->Initialize((PRIME_API_INIT*)&primeApiInit);

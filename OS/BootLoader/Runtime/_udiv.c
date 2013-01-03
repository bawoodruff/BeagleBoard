static unsigned int _udiv_Normalize(unsigned int dividend, unsigned int divisor)
{
    unsigned int count;

    count=0;
    if (divisor <= dividend>>(count+16)) count+=16;
    if (divisor <= dividend>>(count+8))  count+=8;
    if (divisor <= dividend>>(count+4))  count+=4;
    if (divisor <= dividend>>(count+2))  count+=2;
    if (divisor <= dividend>>(count+1))  count+=1;
    return count;
}

typedef struct _ARM_UDIVRESULT {
  unsigned int quotient; /* to be returned in R0 */
  unsigned int modulus;  /* to be returned in R1 */
} ARM_UDIVRESULT;

typedef struct _ARM_SDIVRESULT {
  int quotient; /* to be returned in R0 */
  int modulus;  /* to be returned in R1 */
} ARM_SDIVRESULT;

void __rt_udivmod(ARM_UDIVRESULT *r, unsigned int dividend, unsigned int divisor)
{
    unsigned int result,count;

    count = _udiv_Normalize(dividend,divisor);
    divisor <<= count;

    /* now do the division */
    result = 0;
    do
    {
        result = (result<<1) | (dividend >= divisor);
        if (dividend >= divisor)
            dividend -= divisor;
        divisor >>= 1;
        if (!count--) break;

        result = (result<<1) | (dividend >= divisor);
        if (dividend >= divisor)
            dividend -= divisor;
        divisor >>= 1;
     } while (count--);

    r->quotient = result;
    r->modulus = dividend;
}

void __rt_sdivmod(ARM_SDIVRESULT *r, int dividend, int divisor)
{
    int DividendNegSign,DivisorNegSign;

    DividendNegSign = dividend & 0x80000000;
    DivisorNegSign  = divisor  & 0x80000000;

    if (DividendNegSign)
        dividend = -dividend;
    if (DivisorNegSign)
        divisor = -divisor;

    __rt_udivmod((ARM_UDIVRESULT*)r,dividend,divisor);
    if (DividendNegSign ^ DivisorNegSign)
        r->quotient = -r->quotient;
    if (DividendNegSign)
        r->modulus = -r->modulus;
}


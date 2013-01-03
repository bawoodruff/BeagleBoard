;
; Copyright (c) Microsoft Corporation. All rights reserved.
; 
; Special support routines for ARM tools: division&modulus
; Compiler likes to optimize divisions by 10.
; Other toolsets will ignore.
;

	AREA	|.text|, CODE, THUMB

	EXPORT	__rt_udiv
	EXPORT	__rt_sdiv
	EXPORT	__rt_udiv10
	EXPORT	__rt_sdiv10

				
; extern __rt_u/sdivmod({UINT32 Quotient,Modulus}*Result,
;                        UINT32 Dividend, UINT32 Divisor);
;  In C, does the actual divide
; 
        IMPORT	__rt_udivmod
        IMPORT	__rt_sdivmod

ExtData
        DCD	__rt_udivmod
        DCD    	__rt_sdivmod

;
; {UINT32 Quotient, Modulus} __rt_udiv(UINT32 Divisor, UINT32 Dividend)
;
; Covers for C function, only deals with funny calling convention.
;

__rt_udiv
    ldr   r3, ExtData+0         ; __rt_udivmod

    ; Common wrapper code for both functions.

Massage
    stmdb sp!,{r0,r1,lr}        ; make room on stack and save LR
    mov   r2,r0                 ; shuffle 1st argument
    mov   r0,sp                 ; where the result goes
    adr   lr, RetHere + 1
    mov   pc,r3                 ; call into C for computation

RetHere
    ldmia sp!,{r0,r1,pc}        ; load results where desired and return 
        
;
; {INT32 Quotient, Modulus} __rt_sdiv(INT32 Divisor, INT32 Dividend)
;
; Covers for C function, only deals with funny calling convention.
;

__rt_sdiv
    ldr   r3, ExtData+4         ; __rt_sdivmod;
    b     Massage

;
; {UINT32 Quotient, Modulus} __rt_udiv10(UINT32 Dividend)
;
; Unsigned division+modulus by 10.
; In addition to the regular result in R0 it returns the modulus in R1
; NB: Dont touch R2 in case this is called by the signed guy below
; 
; - there is no HW divide instruction
; - dont use the multiply instructions because they might be too slow
;   [old ARM cores used a simplistic HW algorithm]
; - multiply by the reciprocal, meaning compute X * (2^N / 10) and shift
; - rewrite 2^N/10 to minimize adds and subs:
;  - 8/10 = b0.11001100110011.. 0.rep(1100) .. 0x0.3333333....
;  - 2^-33 * 0x33333333 = 3 * 2^-33 * 0x11111111
;  - 0x11111111 = 0x11 * 0x101 * 0x1001 = (1+2^4)(1+2^8)(1+2^16)
;  - put some of that 2^-33 into the factors e.g. (1+2^-4)
;    to control bit overflows
;  - factor the 3 back into a (1-2^-2)
; - 2^-3(1+2^-16)(1+2^-8)(1+2^-4)(1-2^-2)
;

__rt_udiv10

    sub   r1,r0,#10            ; might be less than 10 but
			       ; dont special case, constant exec time
    sub   r0,r0,r0,lsr #2      ; (1-2^-2) [3/4]
    add   r0,r0,r0,lsr #4      ; (1+2^-4)
    add   r0,r0,r0,lsr #8      ; (1+2^-8)
    add   r0,r0,r0,lsr #16     ; (1+2^-16)
    mov   r0,r0,lsr #3         ; 2^-3 left to do

    ; We are done but.. this could be off by 1 (truncation of rep(3))
    ; Its cheap to multiply back by 10 and check it over.
    ; Besides, dont we need the remainder anyways.

    add   r3,r0,r0,lsl #2      ; times 5..
    subs  r1,r1,r3,lsl #1      ; ..times 2 is (Quotient*10) 
			       ; Take it away from (Dividend-10)
    addmi r1,r1,#10            ; too much ? add 10 back into remainder
    addpl r0,r0,#1             ; otherwise adjust the result

    ; Done

    mov   pc,lr

;
; {INT32 Quotient, Modulus} __rt_udiv10(INT32 Dividend)
;
; Signed division+modulus by 10.
; In addition to the regular result in R0 it returns the modulus in R1
; 
; See above.
;

__rt_sdiv10

    mov   r12,lr              ; Preserve return address

    movs  r2,r0               ; See if Dividend is negative, remember
    rsbmi r0,r0,#0            ; negate ifso (NB: !MVN)

    bl    __rt_udiv10         ; perform div+rem

    tst   r2,r2               ; Check again if Dividend was negative
    rsbmi r0,r0,#0            ; negate result ifso
    rsbmi r1,r1,#0            ; negate remainder ifso

    ; Done

    mov  pc,r12

;
; The ARM/ARM compiler sometimes generates calls to this one.
; Supposedly it would trigger an exception if the argument is zero.
; We dont, so.
;
; It is unclear to me if it is actually supposed to perform the division
; or not. Typically, calls to this guy showup in broken user codes
; and if you fix the codes the calls go away.  In one simple repro
; case, the compiler removed a division altoghether but left the call
; to _divtest.  Unfortunately, it removed so much code that it was
; impossible to tell if the operand to be tested is in r1 (not set)
; or in r0 (but that was loaded with a non-zero constant, so ?).
;
; I am under the (strong) impression that a simple return will do here.
; This guy is an indication that dead code was removed, and on a bout
; of pedantics a test was left in case the removed division would be
; against zero (==> one side effect is the possible exception).
; 
; I would rather that people fixed their broken codes, so I am leaving it out.
;
; NB: If someone *wants* this included they can just add this C code:
; void _rt_divtest(int divisor) { if (divisor == 0) <..division by zero>;}
;
;        _EXPORT(__rt_divtest)
;_FUNCTION(__rt_divtest)
;    mov  pc,r12

       END

 

 

 

 

 

 

 

 

 

 

 

 

;
; Board specific setup info
;
; (C) Copyright 2004-2006
; Texas Instruments, <www.ti.com>
; Richard Woodruff <r-woodruff2@ti.com>
;
; See file CREDITS for list of people who contributed to this
; project.
;
; This program is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License as
; published by the Free Software Foundation; either version 2 of
; the License, or (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston,
; MA 02111-1307 USA
;

	INCLUDE config.inc
	INCLUDE	bits.inc
	INCLUDE	cpu.inc
	INCLUDE clocks343x.inc
	INCLUDE mem.inc

	EXTERN	s_init

	AREA	|.text|, CODE, THUMB

	IF (!:DEF:CFG_NAND_BOOT)

;***************************************************************************
;* cpy_clk_code: relocates clock code into SRAM where its safer to execute
;* R1 = SRAM destination address.
;***************************************************************************
	EXPORT	cpy_clk_code
cpy_clk_code
        ; Copy DPLL code into SRAM
        adr     r0, go_to_speed    	; get addr of clock setting code 
        mov     r2, #384                ; r2 size to copy (div by 32 bytes) 
        mov     r1, r1                  ; r1 <- dest address (passed in) 
        add     r2, r2, r0              ; r2 <- source end address 
next2
        ldmia   r0!, {r3-r10}           ; copy from source address [r0]    
        stmia   r1!, {r3-r10}           ; copy to   target address [r1]    
        cmp     r0, r2                  ; until source end address [r2]    
        bne     next2
	bx	lr

; **************************************************************************** 
; * NOTE: 3430 X-loader currently does not use this code.  
; *   It could be removed its is kept for compatabily with u-boot.
; *
; *  go_to_speed: -Moves to bypass, -Commits clock dividers, -puts dpll at speed
; *               -executed from SRAM.
; *  R0 = CM_CLKEN_PLL-bypass value
; *  R1 = CM_CLKSEL1_PLL-m, n, and divider values
; *  R2 = CM_CLKSEL_CORE-divider values
; *  R3 = CM_IDLEST_CKGEN - addr dpll lock wait
; *
; *  Note: If core unlocks/relocks and SDRAM is running fast already it gets
; *        confused.  A reset of the controller gets it back.  Taking away its
; *        L3 when its not in self refresh seems bad for it.  Normally, this code
; *        runs from flash before SDR is init so that should be ok.
; *****************************************************************************    

go_to_speed
        stmfd sp!, {r4-r6}

        ; move into fast relock bypass 
        ldr     r4, pll_ctl_add
        str     r0, [r4]
wait1
        ldr     r5, [r3]       ; get status 
        and     r5, r5, #0x1   ; isolate core status 
        cmp     r5, #0x1       ; still locked? 
        beq     wait1          ; if lock, loop 

	; set new dpll dividers _after_ in bypass 
	ldr     r5, pll_div_add1
        str     r1, [r5]          ; set m, n, m2 
        ldr     r5, pll_div_add2
        str     r2, [r5]          ; set l3/l4/.. dividers
        ldr     r5, pll_div_add3  ; wkup 
        ldr     r2, pll_div_val3  ; rsm val 
        str     r2, [r5]          
        ldr     r5, pll_div_add4  ; gfx 
        ldr     r2, pll_div_val4 
        str     r2, [r5]         
        ldr     r5, pll_div_add5  ; emu 
        ldr     r2, pll_div_val5 
        str     r2, [r5]     

        ; now prepare GPMC (flash) for new dpll speed 
	; flash needs to be stable when we jump back to it 
        ldr     r5, flash_cfg3_addr
        ldr     r2, flash_cfg3_val
        str     r2, [r5]
        ldr     r5, flash_cfg4_addr
        ldr     r2, flash_cfg4_val
        str     r2, [r5]
        ldr     r5, flash_cfg5_addr
        ldr     r2, flash_cfg5_val
        str     r2, [r5]
        ldr     r5, flash_cfg1_addr
        ldr     r2, [r5]
        orr     r2, r2, #0x3     ; up gpmc divider 
        str     r2, [r5]

        ; lock DPLL3 and wait a bit 
        orr     r0, r0, #0x7   ; set up for lock mode      
        str     r0, [r4]       ; lock 
        nop                    ; ARM slow at this point working at sys_clk 
        nop
        nop
        nop
wait2
        ldr     r5, [r3]       ; get status 
        and     r5, r5, #0x1   ; isolate core status 
        cmp     r5, #0x1       ; still locked? 
        bne     wait2          ; if lock, loop 
        nop
        nop
        nop
        nop
        ldmfd sp!, {r4-r6}
        bx	lr		; back to caller, locked         

_go_to_speed
	DCD go_to_speed

; these constants need to be close for PIC code 
; The Nor has to be in the Flash Base CS0 for this condition to happen 
flash_cfg1_addr
    DCD (GPMC_CONFIG_CS0 + GPMC_CONFIG1)
flash_cfg3_addr
    DCD  (GPMC_CONFIG_CS0 + GPMC_CONFIG3)
flash_cfg3_val 
    DCD  STNOR_GPMC_CONFIG3
flash_cfg4_addr
    DCD (GPMC_CONFIG_CS0 + GPMC_CONFIG4)
flash_cfg4_val
    DCD  STNOR_GPMC_CONFIG4
flash_cfg5_val
    DCD  STNOR_GPMC_CONFIG5
flash_cfg5_addr
    DCD (GPMC_CONFIG_CS0 + GPMC_CONFIG5)
pll_ctl_add
    DCD CM_CLKEN_PLL
pll_div_add1
    DCD CM_CLKSEL1_PLL 
pll_div_add2
    DCD CM_CLKSEL_CORE
pll_div_add3
    DCD CM_CLKSEL_WKUP
pll_div_val3
    DCD (WKUP_RSM << 1) 
pll_div_add4
    DCD CM_CLKSEL_GFX
pll_div_val4
    DCD (GFX_DIV << 0) 
pll_div_add5
    DCD CM_CLKSEL1_EMU
pll_div_val5
    DCD CLSEL1_EMU_VAL

	ENDIF

	EXPORT	lowlevel_init
lowlevel_init
	ldr	sp,	SRAM_STACK
        str     ip,	[sp]    ; stash old link register 
	mov	ip,	lr	; save link reg across call 
	blx     s_init          ; go setup pll,mux,memory 
        ldr     ip,	[sp]    ; restore save ip 
	mov	lr,	ip	; restore link reg 

	; back to arch calling code 
	bx	lr

	; the literal pools origin 
	LTORG

	align	4
REG_CONTROL_STATUS
	DCD CONTROL_STATUS

SRAM_STACK
	DCD LOW_LEVEL_SRAM_STACK

; DPLL(1-4) PARAM TABLES 
; Each of the tables has M, N, FREQSEL, M2 values defined for nominal
; OPP (1.2V). The fields are defined according to dpll_param struct(clock.c). 
; The values are defined for all possible sysclk and for ES1 and ES2.
 
	align	4
mpu_dpll_param
; 12MHz 
; ES1 
	DCD 0x0FE,0x07,0x05,0x01
; ES2 
	DCD 0x0FA,0x05,0x07,0x01
; 3410 
	DCD 0x085,0x05,0x07,0x01

; 13MHz 
; ES1 
	DCD 0x17D,0x0C,0x03,0x01
; ES2 
	DCD 0x1F4,0x0C,0x03,0x01
; 3410 
	DCD 0x10A,0x0C,0x03,0x01
 
; 19.2MHz 
; ES1 
	DCD 0x179,0x12,0x04,0x01
; ES2 
	DCD 0x271,0x17,0x03,0x01
; 3410 
	DCD 0x14C,0x17,0x03,0x01

; 26MHz 
; ES1 
	DCD 0x17D,0x19,0x03,0x01
; ES2 
	DCD 0x0FA,0x0C,0x07,0x01
; 3410 
	DCD 0x085,0x0C,0x07,0x01

; 38.4MHz 
; ES1 
	DCD 0x1FA,0x32,0x03,0x01
; ES2 
	DCD 0x271,0x2F,0x03,0x01
; 3410 
	DCD 0x14C,0x2F,0x03,0x01


	EXPORT	get_mpu_dpll_param
get_mpu_dpll_param
	adr 	r0, mpu_dpll_param
	bx	lr

	align	4
iva_dpll_param
; 12MHz 
; ES1 
	DCD 0x07D,0x05,0x07,0x01
; ES2 
	DCD 0x0B4,0x05,0x07,0x01
; 3410 
	DCD 0x085,0x05,0x07,0x01

; 13MHz 
; ES1 
	DCD 0x0FA,0x0C,0x03,0x01
; ES2 
	DCD 0x168,0x0C,0x03,0x01
; 3410 
	DCD 0x10A,0x0C,0x03,0x01

; 19.2MHz 
; ES1 
	DCD 0x082,0x09,0x07,0x01
; ES2 
	DCD 0x0E1,0x0B,0x06,0x01
; 3410 
	DCD 0x14C,0x17,0x03,0x01

; 26MHz 
; ES1 
	DCD 0x07D,0x0C,0x07,0x01
; ES2 
	DCD 0x0B4,0x0C,0x07,0x01
; 3410 
	DCD 0x085,0x0C,0x07,0x01

; 38.4MHz 
; ES1 
	DCD 0x13F,0x30,0x03,0x01
; ES2 
	DCD 0x0E1,0x17,0x06,0x01
; 3410 
	DCD 0x14C,0x2F,0x03,0x01

	EXPORT	get_iva_dpll_param
get_iva_dpll_param
	adr 	r0, iva_dpll_param
	bx	lr

; Core DPLL targets for L3 at 166 & L133 
	align	4
core_dpll_param
; 12MHz 
; ES1 
	DCD M_12_ES1,M_12_ES1,FSL_12_ES1,M2_12_ES1
; ES2 
	DCD M_12,N_12,FSEL_12,M2_12
; 3410 
	DCD M_12,N_12,FSEL_12,M2_12

; 13MHz 
; ES1 
	DCD M_13_ES1,N_13_ES1,FSL_13_ES1,M2_13_ES1
; ES2 
	DCD M_13,N_13,FSEL_13,M2_13
; 3410 
	DCD M_13,N_13,FSEL_13,M2_13

; 19.2MHz 
; ES1 
	DCD M_19p2_ES1,N_19p2_ES1,FSL_19p2_ES1,M2_19p2_ES1
; ES2 
	DCD M_19p2,N_19p2,FSEL_19p2,M2_19p2
; 3410 
	DCD M_19p2,N_19p2,FSEL_19p2,M2_19p2

; 26MHz 
; ES1 
	DCD M_26_ES1,N_26_ES1,FSL_26_ES1,M2_26_ES1
; ES2 
	DCD M_26,N_26,FSEL_26,M2_26
; 3410 
	DCD M_26,N_26,FSEL_26,M2_26

; 38.4MHz 
; ES1 
	DCD M_38p4_ES1,N_38p4_ES1,FSL_38p4_ES1,M2_38p4_ES1
; ES2 
	DCD M_38p4,N_38p4,FSEL_38p4,M2_38p4
; 3410 
	DCD M_38p4,N_38p4,FSEL_38p4,M2_38p4

	EXPORT get_core_dpll_param
get_core_dpll_param
	adr	r0, core_dpll_param
	bx	lr

	align	4
; PER DPLL values are same for both ES1 and ES2 
per_dpll_param
; 12MHz 
	DCD 0xD8,0x05,0x07,0x09

; 13MHz 
	DCD 0x1B0,0x0C,0x03,0x09

; 19.2MHz 
	DCD 0xE1,0x09,0x07,0x09

; 26MHz 
	DCD 0xD8,0x0C,0x07,0x09

; 38.4MHz 
	DCD 0xE1,0x13,0x07,0x09

	EXPORT	get_per_dpll_param
get_per_dpll_param
	adr 	r0, per_dpll_param
	bx	lr

	END


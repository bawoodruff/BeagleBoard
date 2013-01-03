;
;  armboot - Startup Code for OMP2420/ARM1136 CPU-core
;
;  Copyright (c) 2004-2006  Texas Instruments
;
;  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
;  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
;  Copyright (c) 2002	Gary Jennejohn <gj@denx.de>
;  Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
;  Copyright (c) 2003	Kshitij <kshitij@ti.com>
;  Copyright (c) 2004	Jian Zhang <jzhang@ti.com>
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

	INCLUDE	config.inc
	INCLUDE bits.inc
	INCLUDE	cpu.inc

	EXTERN	cpy_clk_code		; THUMB platform.s
	EXTERN	hang			; THUMB Runtime\board.c
	EXTERN	lowlevel_init		; THUMB platform.s
	EXTERN	start_armboot		; THUMB	Runtime\board.c

	AREA	|.text|, CODE

	;
	; This image is executed when the ROM code performs a branch 
	; instruction to the first executable instruction in the 
	; initial software. 
	; 
	; The execution address is the first word after the image header. 
	; After the branch, the ARM runs in public ARM supervisor mode. 
	; The R0 register points to the booting parameter structure 
	; that contains information about booting.
	;

	ARM

	GLOBAL	_start
_start
	adr	r0, reset + 1
	bx	r0
			     
_vector:
	ldr     pc, _hang       ; undefined
	ldr     pc, _hang  	; SMC
	ldr     pc, _hang  	; program abort
	ldr     pc, _hang  	; data abort
	ldr     pc, _hang  	; reserved
	ldr     pc, _hang  	; IRQ
	ldr     pc, _hang       ; FIQ

_hang
 	DCD 	do_hang

	DCD 	0x12345678
	DCD 	0x12345678
	DCD 	0x12345678
	DCD 	0x12345678
	DCD 	0x12345678
	DCD 	0x12345678
	DCD 	0x12345678 	; now 16*4=64

	GLOBAL	_end_vect
_end_vect

_armboot_start
	DCD 	_start


;************************************************************************
;
; Startup Code (reset vector)
;
; do important init only if we don't start from memory!
; setup Memory and board specific bits prior to relocation.
; relocate armboot to ram
; setup stack
;
;************************************************************************

_TEXT_BASE
	DCD	TEXT_BASE

	THUMB

	GLOBAL	reset
reset
	;
	; set the cpu mode
	;
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3     	; b00010011
					; Mode[0:4]	= 10011 -> Supervisor
					; 
					; b110 
					;      F[6]	= 1 -> FIQ disable
					;      A[7]	= 1 -> Disable imprecise data aborts
	msr	cpsr_c,r0

	;
	; Note that SRAM_VECT_CODE does not appear to be the correct address
	; Vector execution starts at 0x4020FFE4, vector table at 0x4020FFC8

	; Copy vectors to mask ROM indirect addr
	adr     r0, _start              ; r0 <- current position of code   
	add     r0, r0, #4	  	; skip reset vector			
	mov     r2, #64                 ; r2 <- size to copy  
	add     r2, r0, r2              ; r2 <- source end address         
	mov     r1, #SRAM_OFFSET0       ; build vect addr 
	mov     r3, #SRAM_OFFSET1
	add     r1, r1, r3
	mov     r3, #SRAM_OFFSET2
	add     r1, r1, r3
next
	ldmia   r0!, {r3-r10}           ; copy from source address [r0]    
	stmia   r1!, {r3-r10}           ; copy to   target address [r1]    
	cmp     r0, r2                  ; until source end address [r2]    
	bne     next                    ; loop until equal 

	blx	cpy_clk_code            ; put dpll adjust code behind vectors 

	; the mask ROM code should have PLL and others stable 

	bl  	cpu_init_crit

relocate				; relocate U-Boot to RAM	    
	adr	r0, _start		; r0 <- current position of code   
	ldr	r1, _TEXT_BASE		; test if we run from flash or RAM 
	cmp 	r0, r1                  ; no need to relocate if XIP       
	beq 	stack_setup		; skip txt cpy if XIP(SRAM, SDRAM)  

;	ldr	r2, _armboot_start

; TODO: Figure out how to fixup this reference to the bss

;	ldr	r3, _bss_start
;	sub	r2, r3, r2		
	mov	r2, #0x7600             ; r2 <- size of armboot             
	add	r2, r0, r2		; r2 <- source end address         

copy_loop
	ldmia	r0!, {r3-r10}		; copy from source address [r0]    
	stmia	r1!, {r3-r10}		; copy to   target address [r1]    
	cmp	r0, r2			; until source end addreee [r2]    
	ble	copy_loop

	; Set up the stack						    
stack_setup
	ldr	r0, _TEXT_BASE		; upper 128 KiB: relocated uboot   
	sub	r0, r0, #128		; leave 32DCWs for abort-stack   
	and	r0, r0, #~7		; 8 byte alinged for (ldr/str)d    
	mov	sp, r0

;	; Clear BSS (if any).  Is below tx (watch load addr - need space)  
;clear_bss
;	ldr	r0, _bss_start		; find start of bss segment        
;	ldr	r1, _bss_end		; stop here                        
;	mov 	r2, #0x00000000		; clear value                      
;clbss_l
;	str	r2, [r0]		; clear BSS location               
;	cmp	r0, r1			; are we at the end yet            
;	add	r0, r0, #4		; increment clear index pointer    
;	bne	clbss_l                 ; keep clearing till at end        

	blx	start_armboot		; jump to C code                   
	b	.


;************************************************************************
;
; CPU_init_critical registers
;
; setup important registers
; setup memory timing
;
;************************************************************************

	GLOBAL cpu_init_crit
cpu_init_crit

	;
	; Invalidate L1 I/D
	;
	 
        mov	r0, #0                 ; set up for MCR 
        mcr	p15, 0, r0, c8, c7, 0  ; invalidate TLBs 
        mcr	p15, 0, r0, c7, c5, 1  ; invalidate icache 

	;
	; TODO: Figure out how to issue this call, this currently results
	; in an undefined operation
	;

	IF	0
	; Invalidate L2 cache (gp device call point) 
	; - warning, this may have issues on EMU/HS devices
	; this call can corrupt r0-r5
	 
	mov 	r12, #0x1		; set up to invalidate L2  
	smc	#0			; Secure Monitor Call

	ENDIF

	; disable MMU stuff and caches
	 
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	; clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	; clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	; set bit 1 (--A-) Align

	IF	!:DEF:CONFIG_ICACHE_OFF
	orr	r0, r0, #0x00001800	; set bit 11,12 (---I Z---) BTB,I-Cache
	ENDIF
	mcr	p15, 0, r0, c1, c0, 0

	;
        ; Jump to board specific initialization... The Mask ROM will have already initialized
        ; basic memory.  Go here to bump up clock rate and handle wake up conditions.
	;
	 
	adr	r0, _start		; r0 <- current position of code   
	ldr	r1, _TEXT_BASE		; test if we run from flash or RAM 
	cmp     r0, r1                  ; pass on info about skipping some init portions 
	moveq   r0,#0x1                 ; flag to skip prcm and sdrc setup 
	movne   r0,#0x0

	mov	ip, lr          	; preserve link reg across call 

	blx	lowlevel_init   	; go setup pll,mux,memory 
	mov	lr, ip          	; restore link 
	bx	lr			; back to my caller 

;
; exception handler
;
do_hang
	ldr	sp, _TEXT_BASE		; use 32DCWs abort stack 
   	blx	hang			; hang and never return 

	END

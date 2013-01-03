;
; (C) Copyright 2006
; Texas Instruments, <www.ti.com>
; Richard Woodruff <r-woodruff2@ti.com>
;
; This program is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License as
; published by the Free Software Foundation; either version 2 of
; the License, or (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston,
; MA 02111-1307 USA
;

PLL_STOP        	EQU	1         ; PER & IVA 
PLL_LOW_POWER_BYPASS   	EQU	5  ; MPU, IVA & CORE 
PLL_FAST_RELOCK_BYPASS 	EQU	6  ; CORE 
PLL_LOCK        	EQU	7         ; MPU, IVA, CORE & PER 

; The following configurations are OPP and SysClk value independant
; and hence are defined here. All the other DPLL related values are
; tabulated in lowlevel_init.S.

; CORE DPLL 
CORE_M3X2      	EQU	2        ; 332MHz : CM_CLKSEL1_EMU 
CORE_SSI_DIV   	EQU	3        ; 221MHz : CM_CLKSEL_CORE 
CORE_FUSB_DIV  	EQU	2        ; 41.5MHz: 
CORE_L4_DIV    	EQU	2        ;  83MHz : L4 
CORE_L3_DIV    	EQU	2        ; 166MHz : L3 {DDR} 
GFX_DIV        	EQU	2        ;  83MHz : CM_CLKSEL_GFX 
WKUP_RSM       	EQU	2        ; 41.5MHz: CM_CLKSEL_WKUP 

; PER DPLL 
PER_M6X2       	EQU	3         ; 288MHz: CM_CLKSEL1_EMU 
PER_M5X2       	EQU     4         ; 216MHz: CM_CLKSEL_CAM 
PER_M4X2       	EQU     9         ; 96MHz : CM_CLKSEL_DSS-dss1 
PER_M3X2       	EQU     16        ; 54MHz : CM_CLKSEL_DSS-tv 

CLSEL1_EMU_VAL 	EQU      ((CORE_M3X2 << 16) :OR: (PER_M6X2 << 24) :OR: (0x0a50))

    IF	:DEF:PRCM_CLK_CFG2_332MHZ

; BeagleBoard config: PRCM_CLK_CFG2_332MHZ
M_12	       	EQU    	0xA6
N_12	       	EQU     0x05
FSEL_12		EQU     0x07
M2_12		EQU     0x01 	; M3 of 2 

M_12_ES1	EQU     0x19F
N_12_ES1	EQU     0x0E
FSL_12_ES1 	EQU     0x03
M2_12_ES1  	EQU     0x1 	; M3 of 2 

M_13       	EQU     0x14C
N_13       	EQU     0x0C
FSEL_13		EQU     0x03
M2_13		EQU     0x01 	; M3 of 2 

M_13_ES1	EQU   	0x1B2
N_13_ES1	EQU     0x10
FSL_13_ES1 	EQU	0x03  
M2_13_ES1	EQU	0x01 	; M3 of 2 

M_19p2     	EQU	0x19F
N_19p2     	EQU	0x17
FSEL_19p2  	EQU	0x03
M2_19p2    	EQU	0x01 	; M3 of 2 

M_19p2_ES1	EQU	0x19F
N_19p2_ES1	EQU	0x17
FSL_19p2_ES1 	EQU	0x03
M2_19p2_ES1 	EQU	0x01 	; M3 of 2 

M_26       	EQU	0xA6
N_26       	EQU	0x0C
FSEL_26    	EQU	0x07
M2_26      	EQU	0x01 	; M3 of 2 

M_26_ES1	EQU	0x1B2
N_26_ES1	EQU	0x21
FSL_26_ES1	EQU	0x03
M2_26_ES1	EQU	0x01 	; M3 of 2 

M_38p4     	EQU	0x19F
N_38p4     	EQU	0x2F
FSEL_38p4  	EQU	0x03
M2_38p4    	EQU	0x01 	; M3 of 2 

M_38p4_ES1	EQU	0x19F
N_38p4_ES1	EQU	0x2F
FSL_38p4_ES1 	EQU	0x03
M2_38p4_ES1 	EQU	0x01 	; M3 of 2 

    ENDIF

    IF	:DEF:PRCM_CLK_CFG2_266MHZ

M_12	 	EQU 	0x85
N_12            EQU 	0x05
FSEL_12         EQU 	0x07
M2_12           EQU 	0x02 	; M3 of 2 

M_12_ES1	EQU	0x85  	; 0x10A 
N_12_ES1	EQU     0x05  	; 0x05 
FSL_12_ES1 	EQU     0x07  	; 0x7 
M2_12_ES1  	EQU     0x2   	; 0x2 with an M3 of 4

M_13       	EQU     0x10A
N_13       	EQU     0x0C
FSEL_13    	EQU     0x3
M2_13      	EQU     0x1 	; M3 of 2 

M_13_ES1	EQU     0x10A 	; 0x214 
N_13_ES1	EQU     0x0C  	; 0xC 
FSL_13_ES1 	EQU     0x3   	; 0x3 
M2_13_ES1	EQU     0x1   	; 0x2 with an M3 of 4

M_19p2     	EQU     0x115
N_19p2     	EQU     0x13
FSEL_19p2  	EQU     0x03
M2_19p2    	EQU     0x01 	; M3 of 2 

M_19p2_ES1	EQU     0x115  	; 0x299 
N_19p2_ES1	EQU     0x13   	; 0x17 
FSL_19p2_ES1 	EQU     0x03 	; 0x03 
M2_19p2_ES1 	EQU     0x01  	; 0x2 with M3 of 4 

M_26		EQU     0x85
N_26		EQU     0x0C
FSEL_26		EQU     0x07
M2_26		EQU     0x01 	; M3 of 2 

M_26_ES1	EQU     0x85  	; 0x10A 
N_26_ES1	EQU     0x0C  	; 0xC 
FSL_26_ES1 	EQU     0x07  	; 0x7 
M2_26_ES1	EQU     0x01  	; 0x2 with an M3 of 4 

M_38p4     	EQU     0x11C
N_38p4     	EQU     0x28
FSEL_38p4  	EQU     0x03
M2_38p4    	EQU     0x01 	; M3 of 2 

M_38p4_ES1	EQU     0x11C  	; 0x299 
N_38p4_ES1	EQU     0x28   	; 0x2f 
FSL_38p4_ES1 	EQU     0x03 	; 0x3 
M2_38p4_ES1 	EQU     0x01  	; 0x2 with an M3 of 4

    ENDIF

    END

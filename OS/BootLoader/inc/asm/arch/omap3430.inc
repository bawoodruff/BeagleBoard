;
; (C) Copyright 2006
; Texas Instruments, <www.ti.com>
; Richard Woodruff <r-woodruff2@ti.com>
; Syed Mohammed Khasim <x0khasim@ti.com>
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

;
; 3430 specific Section
;

; Stuff on L3 Interconnect
SMX_APE_BASE	EQU	0x68000000

; L3 Firewall 
A_REQINFOPERM0	EQU	(SMX_APE_BASE + 0x05048)
A_READPERM0	EQU	(SMX_APE_BASE + 0x05050)
A_WRITEPERM0	EQU	(SMX_APE_BASE + 0x05058)

; GPMC 
OMAP34XX_GPMC_BASE	EQU	(0x6E000000)

; SMS 
OMAP34XX_SMS_BASE	EQU	0x6C000000

; SDRC 
OMAP34XX_SDRC_BASE	EQU	0x6D000000

;
; L4 Peripherals - L4 Wakeup and L4 Core now
 
OMAP34XX_CORE_L4_IO_BASE	EQU	0x48000000

OMAP34XX_WAKEUP_L4_IO_BASE	EQU	0x48300000

OMAP34XX_L4_PER			EQU	0x49000000

OMAP34XX_L4_IO_BASE		EQU	OMAP34XX_CORE_L4_IO_BASE

; CONTROL 
OMAP34XX_CTRL_BASE		EQU	(OMAP34XX_L4_IO_BASE+0x2000)

; TAP information  dont know for 3430
OMAP34XX_TAP_BASE		EQU	(0x49000000) ;giving some junk for virtio 

; UART 
OMAP34XX_UART1			EQU	(OMAP34XX_L4_IO_BASE+0x6a000)
OMAP34XX_UART2			EQU	(OMAP34XX_L4_IO_BASE+0x6c000)
OMAP34XX_UART3			EQU	(OMAP34XX_L4_PER+0x20000)

; General Purpose Timers  
OMAP34XX_GPT1			EQU	0x48318000
OMAP34XX_GPT2			EQU     0x49032000
OMAP34XX_GPT3			EQU     0x49034000 
OMAP34XX_GPT4			EQU     0x49036000
OMAP34XX_GPT5			EQU     0x49038000 
OMAP34XX_GPT6			EQU     0x4903A000 
OMAP34XX_GPT7			EQU     0x4903C000 
OMAP34XX_GPT8			EQU     0x4903E000 
OMAP34XX_GPT9			EQU     0x49040000 
OMAP34XX_GPT10			EQU     0x48086000
OMAP34XX_GPT11			EQU     0x48088000
OMAP34XX_GPT12			EQU     0x48304000

; WatchDog Timers (1 secure, 3 GP) 
WD1_BASE			EQU     (0x4830C000)
WD2_BASE			EQU     (0x48314000)
WD3_BASE			EQU     (0x49030000)

; 32KTIMER 
SYNC_32KTIMER_BASE		EQU     (0x48320000)
S32K_CR				EQU     (SYNC_32KTIMER_BASE+0x10)

; omap3 GPIO registers 
OMAP34XX_GPIO1_BASE		EQU     0x48310000
OMAP34XX_GPIO2_BASE		EQU     0x49050000
OMAP34XX_GPIO3_BASE		EQU     0x49052000
OMAP34XX_GPIO4_BASE		EQU     0x49054000
OMAP34XX_GPIO5_BASE		EQU     0x49056000
OMAP34XX_GPIO6_BASE		EQU     0x49058000

;
; SDP3430 specific Section
; 

;
;  The 343x's chip selects are programmable.  The mask ROM
;  does configure CS0 to 0x08000000 before dispatch.  So, if
;  you want your code to live below that address, you have to
;  be prepared to jump though hoops, to reset the base address.
;  Same as in SDP3430
;
 
;#ifdef CONFIG_OMAP34XX
; base address for indirect vectors (internal boot mode) 
SRAM_OFFSET0			EQU     0x40000000
SRAM_OFFSET1			EQU     0x00200000
SRAM_OFFSET2			EQU     0x0000F800
SRAM_VECT_CODE			EQU     (SRAM_OFFSET0 + SRAM_OFFSET1 + SRAM_OFFSET2)

LOW_LEVEL_SRAM_STACK		EQU     0x4020FFFC
;#endif

;#if defined(CONFIG_3430SDP) || defined(CONFIG_OMAP3EVM)
; FPGA on Debug board.

ETH_CONTROL_REG			EQU     (DEBUG_BASE+0x30b)
LAN_RESET_REGISTER		EQU     (DEBUG_BASE+0x1c)

DIP_SWITCH_INPUT_REG2		EQU     (DEBUG_BASE+0x60)
LED_REGISTER			EQU     (DEBUG_BASE+0x40)
FPGA_REV_REGISTER		EQU     (DEBUG_BASE+0x10)
EEPROM_MAIN_BRD			EQU     (DEBUG_BASE+0x10000+0x1800)
EEPROM_CONN_BRD			EQU     (DEBUG_BASE+0x10000+0x1900)
EEPROM_UI_BRD			EQU     (DEBUG_BASE+0x10000+0x1A00)
EEPROM_MCAM_BRD			EQU     (DEBUG_BASE+0x10000+0x1B00)
ENHANCED_UI_EE_NAME		EQU     "750-2075"
;#endif

;
; 343x real hardware:
;  ES1     = rev 0
;
;  ES2 onwards, the value maps to contents of IDCODE register [31:28].
;
; Note : CPU_3XX_ES20 is used in cache.S.  Please review before changing.
;
 
CPU_3XX_ES10	EQU	0
CPU_3XX_ES20	EQU	1
CPU_3XX_ES21	EQU	2
CPU_3XX_ES30	EQU	3
CPU_3XX_ES31	EQU	4
CPU_3XX_ES312	EQU	7
CPU_3XX_MAX_REV	EQU	8

CPU_3XX_ID_SHIFT	EQU	28

WIDTH_8BIT     	EQU 	0x0000
WIDTH_16BIT	EQU	0x1000	; bit pos for 16 bit in gpmc 

;
; Control idcode register contains hawkeye and revision info
;
 
CONTROL_IDCODE		EQU	0x4830A204
CONTROL_OMAP_STATUS	EQU     0x4800244C

;
; Hawkeye values
;
 
HAWKEYE_OMAP34XX	EQU	0xb7ae
HAWKEYE_AM35XX		EQU	0xb868
HAWKEYE_OMAP36XX	EQU	0xb891

HAWKEYE_SHIFT		EQU	12

;
; Define CPU families
;
CPU_OMAP34XX	EQU	0x3400	; OMAP34xx/OMAP35 devices 
CPU_AM35XX	EQU	0x3500	; AM35xx devices          
CPU_OMAP36XX	EQU	0x3600	; OMAP36xx devices        

;
; Control status register values corresponding to cpu variants
;
OMAP3503     	EQU   	0x5c00
OMAP3515	EQU	0x1c00
OMAP3525	EQU	0x4c00
OMAP3530	EQU	0x0c00

AM3505		EQU	0x5c00
AM3517		EQU	0x1c00

OMAP3730	EQU	0x0c00

    END

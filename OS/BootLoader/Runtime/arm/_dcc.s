;
; Simple DCC output for JTAG interface
;

	AREA	|.text|, CODE, THUMB

	EXPORT	_write_dcc

_write_dcc
	; Read DSCR to r1
	mrc	p14, 0, r1, c0, c1, 0

	;DTRTXfull [bit29]
	tst	r1, #0x20000000		; full? then loop
	bne	_write_dcc

	; Write r0 to DTRTX
	mcr	p14, 0, r0, c0, c5, 0
	bx	lr

	END


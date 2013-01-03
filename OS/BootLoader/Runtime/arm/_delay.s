;
; Simple runtime delay() function used during initialization
;

	AREA	|.text|, CODE, THUMB

	EXPORT	_delay

_delay
	subs	r0, r0, #1
	bne	_delay
	bx	lr

	END


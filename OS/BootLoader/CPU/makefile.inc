$(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\MLO: $(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.bin
	SignGP.exe $**
	@copy $**.ift $@

$(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.bin: $(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.exe
	PEtoBinary.exe $** $@




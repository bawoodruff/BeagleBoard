$(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\MLO: $(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.bin
	$(BEAGLE_BUILD_PATH)\internal\amd64\SignGP.exe $**
	@copy $**.ift $@

$(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.bin: $(PROJECT_OBJ_ROOT)\BootLoader\CPU\$(O)\X-Load.exe
	$(BEAGLE_BUILD_PATH)\internal\amd64\PEtoBinary.exe $** $@




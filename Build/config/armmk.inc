!IF 0

Copyright (c) Microsoft Corporation

Module Name:

    armmk.inc

Abstract:

        This module contains the ARM specific build controls.  It is included
        by makefile.def.

Revision History:

!ENDIF

#
# ARM option control
#
UMBASE=$(UMBASE:*=arm)
UMLIBS=$(UMLIBS:*=arm)
NTRES=$(NTRES:*=arm)
UMRES=$(UMRES:*=arm)
UMOBJS=$(UMOBJS:*=arm)
CRTLIBS=$(CRTLIBS:*=arm)
LINKLIBS=$(LINKLIBS:*=arm)
GDI_DRIVER_LIBS=$(GDI_DRIVER_LIBS:*=arm)
DLLBASE=$(DLLBASE:*=arm)
DLLDEF=$(DLLDEF:*=arm)
MACHINE_TARGETLIBS=$(MACHINE_TARGETLIBS:*=arm)
!ifdef USE_MSHTML_PDB_RULES
MACHINE_TARGETLIBS=$(MACHINE_TARGETLIBS:????????=*)
!endif
TARGET=$(TARGET:*=arm)
DYNLINK_LIB=$(DYNLINK_LIB:*=arm)
TARGETEXEFILES=$(TARGETEXEFILES:*=arm)
TARGETLIBFILES=$(TARGETLIBFILES:*=arm)
TARGETOBJFILES=$(TARGETOBJFILES:*=arm)
UMOBJFILES=$(UMOBJFILES:*=arm)
UMEXEFILES=$(UMEXEFILES:*=arm)
HEADERFILE=$(HEADERFILE:*=arm)
HEADEROBJNAME=$(HEADEROBJNAME:*=arm)
HEADEROBJ=$(HEADEROBJ:*=arm)
PRECOMPILED=$(PRECOMPILED:*=arm)
PRECOMPILED_CXX=$(PRECOMPILED_CXX:*=arm)
PRECOMPILED_TARGET=$(PRECOMPILED_TARGET:*=arm)
MFC_STATIC_LIB=$(MFC_STATIC_LIB:*=arm)
CRT_LIB_PATH=$(CRT_LIB_PATH:*=arm)
SDK_LIB_PATH=$(SDK_LIB_PATH:*=arm)
DDK_LIB_PATH=$(DDK_LIB_PATH:*=arm)
IFSKIT_LIB_PATH=$(IFSKIT_LIB_PATH:*=arm)
HALKIT_LIB_PATH=$(HALKIT_LIB_PATH:*=arm)
ORDER=$(ORDER:*=arm)
OBJLIBFILES=$(OBJLIBFILES:*=arm)
MISCFILES=$(MISCFILES) $(ARM_MISCFILES)
SOURCES=$(SOURCES) $(ARM_SOURCES)

!ifdef NTTARGETFILES
NTTARGETFILES=$(NTTARGETFILES:*=arm)
!endif
!ifdef NTTARGETFILE0
NTTARGETFILE0=$(NTTARGETFILE0:*=arm)
!endif
!ifdef NTTARGETFILE1
NTTARGETFILE1=$(NTTARGETFILE1:*=arm)
!endif

!ifdef PROJECT_LIB_PATH
PROJECT_LIB_PATH=$(PROJECT_LIB_PATH:*=arm)
!endif

ENTRY_SUFFIX=@4
GDI_ENTRY_SUFFIX=@12

DEFAULT_STACKRESERVE=0x40000
DEFAULT_STACKCOMMIT=0x2000

ADDITIONAL_ATL_LIBS = $(ADDITIONAL_ATL_LIBS)

!IFDEF ARM_WARNING_LEVEL
MSC_WARNING_LEVEL=$(ARM_WARNING_LEVEL)
!ENDIF

MSC_WARNING_LEVEL=$(MSC_WARNING_LEVEL) $(COMPILER_WX_SWITCH)

!ifdef ARM_PERFFLAGS
PERFFLAGS = $(ARM_PERFFLAGS)
!endif

# Set MSC_OPTIMIZATION.
# Order of precedence:
#   Platform specific override
#   Environment variable
#   System Default

!ifdef ARM_OPTIMIZATION
MSC_OPTIMIZATION=$(ARM_OPTIMIZATION)
!elseif !defined(MSC_OPTIMIZATION)
MSC_OPTIMIZATION=$(DEFAULT_MSC_OPT)
!endif

LINK_TIME_CODE_GENERATION_MSC_FLAG=
!if defined(LINK_TIME_CODE_GENERATION) && !defined(FORCENATIVEOBJECT)
# Some objects must be built native, so turn off GL for those
LINK_TIME_CODE_GENERATION_MSC_FLAG=/GL
!endif

DBGFLAGS=$(DBGFLAGS) $(MSC_OPTIMIZATION) $(LINK_TIME_CODE_GENERATION_MSC_FLAG)

!IFDEF ARM_CPPFLAGS
MSC_CPPFLAGS=$(ARM_CPPFLAGS)
!ENDIF

!ifdef NTCPPFLAGS
!error NTCPPFLAGS is deprecated.
!endif

!ifdef NO_STRING_POOLING
STRING_POOLING =
!else
!ifdef NO_READONLY_STRINGS
STRING_POOLING = /Gf
!else
STRING_POOLING = /GF
!endif
!endif

!ifdef USE_NATIVE_EH
!if "$(USE_NATIVE_EH)" == "ASYNC"
EH_FLAGS=/EHa
!else
!if "$(USE_NATIVE_EH)" == "CTHROW"
EH_FLAGS=/EHsc-
!else
EH_FLAGS=/EHsc
!endif
!endif
!else
EH_FLAGS=/EHs-c-
!endif

!if defined(USE_RTTI)
RTTI_FLAGS=/GR
! else
RTTI_FLAGS=/GR-
! endif

# On by default
!if "$(BUFFER_OVERFLOW_CHECKS)" == ""
BUFFER_OVERFLOW_CHECKS=1
!endif

!if "$(TARGETTYPE)" == "DYNLINK" && "$(DLLENTRY)" == ""
DLLENTRY=_DllMainCRTStartup
!endif

!if defined(BUFFER_OVERFLOW_CHECKS)
! if "$(BUFFER_OVERFLOW_CHECKS)" != "1" && "$(BUFFER_OVERFLOW_CHECKS)" != "0"
!  error INVALID BUFFER_OVERFLOW_CHECKS value "$(BUFFER_OVERFLOW_CHECKS)" in $(MAKEDIR)
! endif
! if "$(BUFFER_OVERFLOW_CHECKS)" == "1" 
BO_FLAGS=/GS
! endif
! if "$(BUFFER_OVERFLOW_CHECKS)" == "0" 
BO_FLAGS=/GS-
! endif

!endif  # Overflow checks

!ifndef ARM_CPU_OPTIMIZATION
ARM_CPU_OPTIMIZATION=
!endif

!if defined(ARM_STRUCT_PACKING)
MSC_STRUCT_PACKING=$(ARM_STRUCT_PACKING)
!elseif !defined(MSC_STRUCT_PACKING)
MSC_STRUCT_PACKING=/Zp8
!endif

#
# Allow the default behavior of WCHAR to be overridden.
#

!ifndef NO_WCHAR_T
COMPILER_WCHAR_T=/Zc:wchar_t-
!else
COMPILER_WCHAR_T=
!endif

#
# Turn off the default behavior of WCHAR if managed code version 1.1 is used.
#

!if "$(URT_VER)" == "1.1" && defined(MANAGED_CXX)
COMPILER_WCHAR_T=
!endif


STDFLAGS= /c $(COMPILER_WCHAR_T) /Zl $(MSC_STRUCT_PACKING) /Gy $(CBSTRING) $(MSC_WARNING_LEVEL) $(MSC_CALL_TYPE) \
          $(ERATTA_FLAGS) $(ARM_CPU_OPTIMIZATION) $(EH_FLAGS) \
          $(RTTI_FLAGS) $(STRING_POOLING) $(BO_FLAGS)

!if $(FREEBUILD)
ASM_DBG_DEFINES=/DDBG=0
!else
ARM_ADBGFLAGS=$(ARM_ADBGFLAGS) /Zi
!endif

DBGFLAGS=$(DBGFLAGS) /Zi
ARM_ADBGFLAGS=$(ARM_ADBGFLAGS) /Zi

!if !defined(USE_PDB_TO_COMPILE) || "$(USE_PDB_TO_COMPILE)" == "0"
DBGFLAGS=$(DBGFLAGS:/Zi=/Z7)
DBGFLAGS=$(DBGFLAGS:-Zi=/Z7)
!else
USER_C_FLAGS=$(USER_C_FLAGS:/Z7=/Zi)
USER_C_FLAGS=$(USER_C_FLAGS:-Z7=/Zi)
! ifndef USE_MSHTML_PDB_RULES
!  if "$(TARGETTYPE)" == "LIBRARY"
DBGFLAGS=$(DBGFLAGS) /Fd$(TARGETPATH)\$(TARGET_DIRECTORY)\$(TARGETNAME).pdb
!  else
DBGFLAGS=$(DBGFLAGS) /Fd$(OBJ_PATH)\$(_OBJ_DIR)\arm^\
!  endif
! else
!  ifndef TARGETPDB
TARGETPDB=$(TARGETNAME)
!  endif
DBGFLAGS=$(DBGFLAGS) /Fd$(OBJ_PATH)\$(_OBJ_DIR)\arm\$(TARGETPDB).pdb
! endif
!endif

!ifndef NO_BROWSER_INFO
! ifdef BROWSER_INFO
DBGFLAGS=$(DBGFLAGS) /FR$(OBJ_PATH)\$(_OBJ_DIR)\arm^\
! endif
!endif

#
# arm option control
#

!IF "$(HALTYPE)" == ""
HALDEF=
!ELSE
HALDEF=/D$(HALTYPE)=1
!ENDIF

#/arch:ARMv7VE 

STD_DEFINES=/D_ARM_ /DARM $(HALDEF) $(MSC_CALL_DEFINE) $(STD_DEFINES)

CDEFINES=$(STD_DEFINES) $(TARGET_DBG_DEFINES) \
              $(LIBC_DEFINES) $(C_DEFINES) $(NET_C_DEFINES) $(MFC_DEFINES)
CFLAGS=$(ARM_FLAGS) $(NTARMFLAGS) $(STDFLAGS) $(DBGFLAGS) $(PERFFLAGS) $(USER_C_FLAGS) /typedil- /wd4603 /wd4627

ARM_ASM_ENCODINGFLAGS=-coff_thumb2_only 

!if defined(ARM_ASM_ENCODING) 
!if "$(ARM_ASM_ENCODING)"=="ARM32"
ARM_ASM_ENCODINGFLAGS=-32
!elseif "$(ARM_ASM_ENCODING)"=="ARM16" 
ARM_ASM_ENCODINGFLAGS=-16
!elseif "$(ARM_ASM_ENCODING)"=="THUMB" 
ARM_ASM_ENCODINGFLAGS=-coff_thumb2_only 
!endif
!endif

ARM_ASMFLAGS=$(ARM_ASMFLAGS) $(ARM_ASM_ENCODINGFLAGS)
ARM_CDEFINES=$(CDEFINES)
ARM_CFLAGS=$(CFLAGS)

!if defined(SUBSTITUTE_ARM_CC_MANAGED) && defined(MANAGED_CXX)
CC_NAME=$(SUBSTITUTE_ARM_CC_MANAGED) 
!elseif defined(SUBSTITUTE_ARM_CC) && !defined(MANAGED_CXX)
CC_NAME=$(SUBSTITUTE_ARM_CC) 
!elseif !defined(CC_NAME)
CC_NAME = cl 
!endif

C_COMPILER_NAME     = $(CC_NAME)
CXX_COMPILER_NAME   = $(CC_NAME)
C_PREPROCESSOR_NAME = $(CC_NAME)
!ifndef ARM_ASSEMBLER_NAME
ARM_ASSEMBLER_NAME  = armasm
!endif

GLOBAL_C_FLAGS = /Iarm\ /I. $(INCPATH1) $(CDEFINES) $(CFLAGS)

!if !$(FREEBUILD) || defined(BUILD_NO_OPT)
GLOBAL_C_FLAGS = $(GLOBAL_C_FLAGS) /homeparams
!endif

GLOBAL_C_FLAGS=$(CRT_BUILD_FLAGS) $(GLOBAL_C_FLAGS)

NP_COMPILER_FLAGS = $(GLOBAL_C_FLAGS) $(COMPILER_WARNINGS)

CXX_COMPILER_FLAGS_NOPCH = $(NP_COMPILER_FLAGS) $(MSC_CPPFLAGS) $(NTCPPFLAGS) $(MANAGED_CXX_FLAGS)
C_COMPILER_FLAGS_NOPCH = $(NP_COMPILER_FLAGS)
C_COMPILER_FLAGS = $(C_COMPILER_FLAGS_NOPCH) $(PRECOMPILED)
CXX_COMPILER_FLAGS = $(CXX_COMPILER_FLAGS_NOPCH) $(PRECOMPILED_CXX)
C_PREPROCESSOR_FLAGS = $(GLOBAL_C_FLAGS) /EP /Tc

C_PREPROCESSOR = $(C_PREPROCESSOR_NAME) $(C_PREPROCESSOR_FLAGS)
C_ASM_PREPROCESSOR = $(C_PREPROCESSOR_NAME) $(ASM_DEFINES) $(C_PREPROCESSOR_FLAGS)
C_COMPILER     = $(C_COMPILER_NAME) $(C_COMPILER_FLAGS)
NP_C_COMPILER  = $(C_COMPILER_NAME) $(NP_COMPILER_FLAGS)
CXX_COMPILER   = $(CXX_COMPILER_NAME) $(CXX_COMPILER_FLAGS)

ARM_ASSEMBLER=$(ARM_ASSEMBLER_NAME) -I arm\ -I . $(INCPATH1:/I=-I ) $(ARM_ASMFLAGS)

ECHO_MSG=$(C_COMPILER) $<
ECHO_MSG_P=$(NP_C_COMPILER) $**
ECHO_CXX_MSG=$(CXX_COMPILER) $<

ECHO_PRECOMPILED_MSG1=$(C_COMPILER_NAME) $(C_COMPILER_FLAGS) /Yl$(TARGETNAME) /Yc$(?F) $(HEADERFILE) \
               $(HEADEROBJ) $(PRECOMPILED_FLAG) $(PRECOMPILED_SOURCEFILE) 

ECHO_PRECOMPILED_MSG2=$(C_COMPILER_NAME) $(C_COMPILER_FLAGS) /Yl$(TARGETNAME) /Yc$(?F) $(HEADERFILE) \
               $(HEADEROBJ) $(PRECOMPILED_FLAG)

{}.c{}.s:
    $(C_COMPILER_NAME) @<< /Fa $<
$(C_COMPILER_FLAGS: =
)
$(C_COMPILER_FLAGS:##= )
<<NOKEEP

{..\arm}.s{$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

{arm}.s{$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

{$(_OBJ_DIR)\arm}.s{$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

{..\arm}.s{$(OBJ_PATH)\$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

{arm}.s{$(OBJ_PATH)\$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

{$(OBJ_PATH)\$(_OBJ_DIR)\arm}.s{$(OBJ_PATH)\$(_OBJ_DIR)\arm}.obj:
    $(ARM_ASSEMBLER) -o $@ $<

!if !defined(NTKERNEL_ENTRY)
NTKERNEL_ENTRY=KiSystemStartup
!endif

!IF "$(NTDEBUGTYPE)" == "windbg" || "$(NTDEBUGTYPE)" == "both"
NTKERNEL_LINK_OPTIONS=/entry:$(NTKERNEL_ENTRY)
!ELSE
NTKERNEL_LINK_OPTIONS=/base:0x80100000 /entry:$(NTKERNEL_ENTRY)
!ENDIF

LINKER_FLAGS=$(LINKER_FLAGS)

!if !defined(USE_LOCAL_HOTPATCHBUFFER) && !defined(PURE_FORWARDER_DLL) && !defined(RESOURCE_ONLY_DLL)
LINKER_FLAGS=$(LINKER_FLAGS) $(SDK_LIB_PATH)\*\hotpatch.obj
!endif

!if defined(ROMBASE_OBJ)
LINKER_FLAGS=$(LINKER_FLAGS) /ignore:4042 $(ROMBASE_OBJ)
!endif

!if !defined(USE_LOCAL_HOTPATCHBUFFER) && !defined(PURE_FORWARDER_DLL) && !defined(RESOURCE_ONLY_DLL)
LINKER_FLAGS=$(LINKER_FLAGS) $(SDK_LIB_PATH)\*\hotpatch.obj
!endif

!ifndef USE_LOCAL_FUNCTIONPAD
LINKER_FLAGS=$(LINKER_FLAGS) /functionpadmin:6
!endif

DRIVER_ALIGNMENT=0x0080
KERNEL_ALIGNMENT=0x1000

# SEH support for libs built with an older compiler
!ifdef _NT_TARGET_VERSION
!    if $(_NT_TARGET_VERSION) < 0x600 && $(_NT_TOOLS_VERSION) >= 0x700
TARGETLIBS=$(TARGETLIBS) $(DDK_LIB_PATH)\sehupd.lib
!    endif
!endif
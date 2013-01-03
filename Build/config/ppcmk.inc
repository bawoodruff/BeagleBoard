!IF 0

Copyright (c) Microsoft Corporation

Module Name:

    ppcmk.inc

Abstract:

        This module contains the PPC/Xenon specific build controls.  It is included
        by makefile.def.

Revision History:

!ENDIF

#
# PPC/Xenon option control
#

UMBASE=$(UMBASE:*=ppc)
UMLIBS=$(UMLIBS:*=ppc)
NTRES=$(NTRES:*=ppc)
UMRES=$(UMRES:*=ppc)
UMOBJS=$(UMOBJS:*=ppc)
LINKLIBS=$(LINKLIBS:*=ppc)
GDI_DRIVER_LIBS=$(GDI_DRIVER_LIBS:*=ppc)
DLLBASE=$(DLLBASE:*=ppc)
DLLDEF=$(DLLDEF:*=ppc)
MACHINE_TARGETLIBS=$(MACHINE_TARGETLIBS:*=ppc)
!ifdef USE_MSHTML_PDB_RULES
MACHINE_TARGETLIBS=$(MACHINE_TARGETLIBS:????????=*)
!endif
TARGET=$(TARGET:*=ppc)
DYNLINK_LIB=$(DYNLINK_LIB:*=ppc)
TARGETEXEFILES=$(TARGETEXEFILES:*=ppc)
TARGETLIBFILES=$(TARGETLIBFILES:*=ppc)
TARGETOBJFILES=$(TARGETOBJFILES:*=ppc)
UMOBJFILES=$(UMOBJFILES:*=ppc)
UMEXEFILES=$(UMEXEFILES:*=ppc)
HEADERFILE=$(HEADERFILE:*=ppc)
HEADEROBJNAME=$(HEADEROBJNAME:*=ppc)
HEADEROBJ=$(HEADEROBJ:*=ppc)
PRECOMPILED=$(PRECOMPILED:*=ppc)
PRECOMPILED_CXX=$(PRECOMPILED_CXX:*=ppc)
PRECOMPILED_TARGET=$(PRECOMPILED_TARGET:*=ppc)
MFC_STATIC_LIB=$(MFC_STATIC_LIB:*=ppc)
SDK_LIB_PATH=$(SDK_LIB_PATH:*=ppc)
CRT_LIB_PATH=$(XENON_LIB_PATH)
DDK_LIB_PATH=$(DDK_LIB_PATH:*=ppc)
IFSKIT_LIB_PATH=$(IFSKIT_LIB_PATH:*=ppc)
HALKIT_LIB_PATH=$(HALKIT_LIB_PATH:*=ppc)
ORDER=$(ORDER:*=ppc)
OBJLIBFILES=$(OBJLIBFILES:*=ppc)
MISCFILES=$(MISCFILES) $(PPC_MISCFILES)
SOURCES=$(SOURCES) $(PPC_SOURCES)

ADDITIONAL_ATL_LIBS = $(ADDITIONAL_ATL_LIBS) $(ATL_LIB_PATH)\atlppc.lib

!ifdef NTTARGETFILES
NTTARGETFILES=$(NTTARGETFILES:*=ppc)
!endif
!ifdef NTTARGETFILE0
NTTARGETFILE0=$(NTTARGETFILE0:*=ppc)
!endif
!ifdef NTTARGETFILE1
NTTARGETFILE1=$(NTTARGETFILE1:*=ppc)
!endif

!ifdef PROJECT_LIB_PATH
PROJECT_LIB_PATH=$(PROJECT_LIB_PATH:*=ppc)
!endif

ENTRY_SUFFIX=@4
GDI_ENTRY_SUFFIX=@12

!IFDEF STD_CALL_ENTRY
UMENTRY=$(UMENTRY)$(ENTRY_SUFFIX)
!ENDIF

DEFAULT_STACKRESERVE=0x10000
DEFAULT_STACKCOMMIT=0x10000

ADDITIONAL_ATL_LIBS = $(ADDITIONAL_ATL_LIBS)

!IFDEF PPC_WARNING_LEVEL
MSC_WARNING_LEVEL=$(PPC_WARNING_LEVEL)
!ENDIF

MSC_WARNING_LEVEL=$(MSC_WARNING_LEVEL) $(COMPILER_WX_SWITCH)

!ifdef PPC_PERFFLAGS
PERFFLAGS = $(PPC_PERFFLAGS)
!endif

!if !defined(USE_HV) && !$(FREEBUILD) && "$(UMTYPE)" == "xbox" && !defined(PERFLIBS) && "$(PERFLIBS)" == "" && !defined(XNOPERFLIBS)
PERFLIBS=$(XENON_LIB_PATH)\xbdm.lib
!endif

!if "$(BIGENDIAN)" != ""
ENDIAN_DEFINES=-DBIGENDIAN=1 -D_BIGENDIAN
!endif

!if "$(NATIVE_WCHAR_T)" != "0"
PPC_FLAGS = $(PPC_FLAGS) /Zc:wchar_t
!else
PPC_FLAGS = $(PPC_FLAGS) /Zc:wchar_t-
!endif

# Set MSC_OPTIMIZATION.
# Order of precedence:
#   Platform specific override
#   Environment variable
#   System Default

!if defined(PPC_OPTIMIZATION)
MSC_OPTIMIZATION=$(PPC_OPTIMIZATION)
!elseif !defined(MSC_OPTIMIZATION)
MSC_OPTIMIZATION=$(DEFAULT_MSC_OPT)
!endif

!ifdef NOCODFLAGS
MSC_OPTIMIZATION=$(MSC_OPTIMIZATION:/FAcs=)
USER_C_FLAGS=$(USER_C_FLAGS:/FAcs=)
!endif

DBGFLAGS=$(DBGFLAGS) $(MSC_OPTIMIZATION)

!IFDEF PPC_CPPFLAGS
MSC_CPPFLAGS=$(PPC_CPPFLAGS)
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
! else
!  if "$(USE_NATIVE_EH)" == "CTHROW"
EH_FLAGS=/EHsc-
!  else
EH_FLAGS=/EHsc
!  endif
! endif
!else
EH_FLAGS=/EHs-c-
!endif

! ifdef USE_RTTI
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

!ifndef PPC_CPU_OPTIMIZATION
PPC_CPU_OPTIMIZATION=
!endif

!ifdef USE_OPENMP
OPENMP_FLAGS=/openmp
!else
OPENMP_FLAGS=
!endif

!ifndef NO_VMX128
EV3_FLAGS=/QVMX128
!endif

!ifndef CONFORMANCE_FLAGS
CONFORMANCE_FLAGS=/Zc:forScope-
!endif

!if defined(PPC_STRUCT_PACKING)
MSC_STRUCT_PACKING=$(PPC_STRUCT_PACKING)
!elseif !defined(MSC_STRUCT_PACKING)
MSC_STRUCT_PACKING=/Zp8
!endif

!if "$(UMTYPE)" == "xbox"

STDFLAGS= /c $(MSC_STRUCT_PACKING) /Gy $(MSC_WARNING_LEVEL) $(MSC_CALL_TYPE) $(PPC_ARCHITECTURE) $(PPC_CPU_OPTIMIZATION) \
          $(RTTI_FLAGS) $(EH_FLAGS) $(MSC_SECURITY_CHECKS) $(OPENMP_FLAGS) $(CONFORMANCE_FLAGS)

!else

STDFLAGS= /c $(MSC_STRUCT_PACKING) /Zl /Gy /wd4430 /wd4431 $(CBSTRING) $(MSC_WARNING_LEVEL:/W4=) $(MSC_CALL_TYPE) \
          $(ERATTA_FLAGS) $(PPC_ARCHITECTURE) $(PPC_CPU_OPTIMIZATION) $(EH_FLAGS)  \
          $(RTTI_FLAGS) $(STRING_POOLING) $(MSC_SECURITY_CHECKS) \
          $(OPENMP_FLAGS) $(EV3_FLAGS) $(CONFORMANCE_FLAGS)

!endif

!if !defined(PPC_NO_STACKCHECK)
STDFLAGS = $(STDFLAGS) /GS
!endif

!if $(FREEBUILD)
ASM_DBG_DEFINES=/DDBG=0
!else
PPC_ADBGFLAGS=$(PPC_ADBGFLAGS) /Zi
!endif

!IFDEF NTBBT
DBGFLAGS=$(DBGFLAGS) /Zi
PPC_ADBGFLAGS=$(PPC_ADBGFLAGS) /Zi
LINKER_FLAGS=$(LINKER_FLAGS) /debug /debugtype:cv,fixup
!ENDIF

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
DBGFLAGS=$(DBGFLAGS) /Fd$(OBJ_PATH)\$O^\
!  endif
! else
!  ifndef TARGETPDB
TARGETPDB=$(TARGETNAME)
!  endif
DBGFLAGS=$(DBGFLAGS) /Fd$(OBJ_PATH)\$O\$(TARGETPDB).pdb
! endif
!endif

!if "$(UMTYPE)" == "xbox"


!  if "$(TARGETTYPE)" == "LIBRARY" || "$(FORCE_Z7_DEBUG_INFO)" == "1"

DBGFLAGS=$(DBGFLAGS:/Zi=/Z7)
DBGFLAGS=$(DBGFLAGS:-Zi=/Z7)
USER_C_FLAGS=$(USER_C_FLAGS:/Zi=/Z7)
USER_C_FLAGS=$(USER_C_FLAGS:-Zi=/Z7)
!else
DBGFLAGS=$(DBGFLAGS:/Z7=/Zi)
DBGFLAGS=$(DBGFLAGS:-Z7=/Zi)
USER_C_FLAGS=$(USER_C_FLAGS:/Z7=/Zi)
USER_C_FLAGS=$(USER_C_FLAGS:-Z7=/Zi)
!endif

!  if "$(TARGETTYPE)" == "LIBRARY"
DBGFLAGS=$(DBGFLAGS) /Fd$(TARGETPATH)\$(TARGET_DIRECTORY)\$(TARGETNAME).pdb
!  else
DBGFLAGS=$(DBGFLAGS) /Fd$(OBJ_PATH)\$O^\
!  endif

!endif

!ifndef NO_BROWSER_INFO
! ifdef BROWSER_INFO
DBGFLAGS=$(DBGFLAGS) /FR$(OBJ_PATH)\$O^\
! endif
!endif

#
# PPC/Xenon option control
#

!IF "$(HALTYPE)" == ""
HALDEF=
!ELSE
HALDEF=/D$(HALTYPE)=1
!ENDIF

STD_DEFINES= /D_PPC_=1 /DPPC /D_MPPC_ /D_XBOX /DXBOX -D_XENON -D_XBOX_VER=200 $(ENDIAN_DEFINES) $(HALDEF) $(MSC_CALL_DEFINE) $(STD_DEFINES)

!if defined(USE_HV)
STD_DEFINES=-D_XENON64 $(STD_DEFINES)
!endif

!if defined(XENONUM) && !defined(XENONUM_KM_OVERRIDE)
STD_DEFINES=-D_XENONUM $(STD_DEFINES)
!endif


CDEFINES=$(STD_DEFINES) $(TARGET_DBG_DEFINES) $(MSC_CPPFLAGS) $(NTCPPFLAGS)\
              $(LIBC_DEFINES) $(C_DEFINES) $(NET_C_DEFINES) $(MFC_DEFINES)
CFLAGS=$(PPC_FLAGS) $(NTPPCFLAGS) $(STDFLAGS) $(DBGFLAGS) $(PERFFLAGS) $(USER_C_FLAGS)

PPC_ASMFLAGS=$(PPC_ADBGFLAGS) $(STD_DEFINES) $(ASM_DBG_DEFINES)\
              $(TARGET_DBG_DEFINES) $(MSC_CPPFLAGS) $(NTCPPFLAGS) $(ASM_DEFINES)

PPC_CDEFINES=$(CDEFINES)
PPC_CFLAGS=$(CFLAGS)

!if defined(SUBSTITUTE_PPC_CC)
CC_NAME=$(SUBSTITUTE_PPC_CC) 
!elseif !defined(CC_NAME)
CC_NAME=cl.exe 
!endif

!if defined(USE_HV)
CC_NAME=$(CC_NAME) -Qhypervisor
!else
CC_NAME=$(CC_NAME) -QDD2
!endif

C_COMPILER_NAME     = $(CC_NAME)
CXX_COMPILER_NAME   = $(CC_NAME)
C_PREPROCESSOR_NAME = $(CC_NAME)
!ifndef PPC_ASSEMBLER_NAME
PPC_ASSEMBLER_NAME  = ml.exe
!endif

GLOBAL_C_FLAGS = /IPPC\ /I. $(INCPATH1) $(CDEFINES) $(CFLAGS)

NP_COMPILER_FLAGS = $(GLOBAL_C_FLAGS) $(COMPILER_WARNINGS)

CXX_COMPILER_FLAGS_NOPCH = $(NP_COMPILER_FLAGS) $(MSC_CPPFLAGS) $(NTCPPFLAGS) 
C_COMPILER_FLAGS_NOPCH = $(NP_COMPILER_FLAGS)
C_COMPILER_FLAGS = $(C_COMPILER_FLAGS_NOPCH) $(PRECOMPILED)
CXX_COMPILER_FLAGS = $(CXX_COMPILER_FLAGS_NOPCH) $(PRECOMPILED_CXX)
C_PREPROCESSOR_FLAGS = $(GLOBAL_C_FLAGS) /EP /Tc

C_PREPROCESSOR = $(C_PREPROCESSOR_NAME) $(C_PREPROCESSOR_FLAGS)
C_ASM_PREPROCESSOR = $(C_PREPROCESSOR_NAME) $(ASM_DEFINES) $(C_PREPROCESSOR_FLAGS)
C_COMPILER     = $(C_COMPILER_NAME) $(C_COMPILER_FLAGS)
NP_C_COMPILER  = $(C_COMPILER_NAME) $(NP_COMPILER_FLAGS)
CXX_COMPILER   = $(CXX_COMPILER_NAME) $(CXX_COMPILER_FLAGS)

PPC_ASSEMBLER=$(PPC_ASSEMBLER_NAME) /c /NT /Cx /nologo /IPPC /I. $(INCPATH1) $(PPC_ASMFLAGS)

ECHO_MSG=$(C_COMPILER) $<
ECHO_MSG_P=$(NP_C_COMPILER) $**
ECHO_CXX_MSG=$(CXX_COMPILER) $<

ECHO_PRECOMPILED_MSG1=$(C_COMPILER_NAME) $(C_COMPILER_FLAGS) /Yl$(TARGETNAME) /Yc$(?F) $(HEADERFILE) \
               $(HEADEROBJ) $(PRECOMPILED_FLAG) $(PRECOMPILED_SOURCEFILE)
	       
ECHO_PRECOMPILED_MSG2=$(C_COMPILER_NAME) $(C_COMPILER_FLAGS) /Yl$(TARGETNAME) /Yc$(?F) $(HEADERFILE) \
               $(HEADEROBJ) $(PRECOMPILED_FLAG)

{}.c{}.asm:
    $(C_COMPILER_NAME) @<< /Fa $(MAKEDIR)\$<
$(C_COMPILER_FLAGS: =
)
$(C_COMPILER_FLAGS:##= )
<<NOKEEP


{..\PPC}.asm{}.lst:
    $(PPC_ASSEMBLER) /Fl$@ /Fo$O\$(@B).obj $(MAKEDIR)\$<

{PPC}.asm{}.lst:
    $(PPC_ASSEMBLER) /Fl$@ /Fo$(OBJ_PATH)\$O\$(@B).obj $(MAKEDIR)\$<

{..\PPC}.asm{$O}.obj:
    $(PPC_ASSEMBLER) /Fo$(OBJ_PATH)\$@ $(MAKEDIR)\$<

{PPC}.asm{$O}.obj:
    $(PPC_ASSEMBLER) /Fo$(OBJ_PATH)\$@ $(MAKEDIR)\$<

{$O}.asm{$O}.obj:
    $(PPC_ASSEMBLER) /Fo$(OBJ_PATH)\$@ $(MAKEDIR)\$<

{..\PPC}.asm{$(OBJ_PATH)\$O}.obj:
    $(PPC_ASSEMBLER) /Fo$@ $(MAKEDIR)\$<

{PPC}.asm{$(OBJ_PATH)\$O}.obj:
    $(PPC_ASSEMBLER) /Fo$@ $(MAKEDIR)\$<

{$(OBJ_PATH)\$O}.asm{$(OBJ_PATH)\$O}.obj:
    $(PPC_ASSEMBLER) /Fo$@ $<

DRIVER_ALIGNMENT=0x1000

KERNEL_ALIGNMENT=0x1000,0x1000

!if !defined(USE_HV)
SYSTEM_ALIGNMENT=0x80,0x1000
!endif

NO_RDATA_MERGE=1

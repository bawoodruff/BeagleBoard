#ifndef _KD_H_
#define _KD_H_


enum
{
    DBGKD_SIMULATION_NONE,
    DBGKD_SIMULATION_EXDI
};

#define KD_SECONDARY_VERSION_DEFAULT                    0
#define KD_SECONDARY_VERSION_AMD64_OBSOLETE_CONTEXT_1   0
#define KD_SECONDARY_VERSION_AMD64_OBSOLETE_CONTEXT_2   1
#define KD_SECONDARY_VERSION_AMD64_CONTEXT              2

#if defined(_AMD64_)
#define CURRENT_KD_SECONDARY_VERSION                    KD_SECONDARY_VERSION_AMD64_CONTEXT
#else
#define CURRENT_KD_SECONDARY_VERSION                    KD_SECONDARY_VERSION_DEFAULT
#endif

#define DBGKD_VERS_FLAG_MP                              0x0001
#define DBGKD_VERS_FLAG_DATA                            0x0002
#define DBGKD_VERS_FLAG_PTR64                           0x0004
#define DBGKD_VERS_FLAG_NOMM                            0x0008
#define DBGKD_VERS_FLAG_HSS                             0x0010
#define DBGKD_VERS_FLAG_PARTITIONS                      0x0020

#define KDBG_TAG                                        'GBDK'

typedef enum _DBGKD_MAJOR_TYPES
{
    DBGKD_MAJOR_NT,
    DBGKD_MAJOR_XBOX,
    DBGKD_MAJOR_BIG,
    DBGKD_MAJOR_EXDI,
    DBGKD_MAJOR_NTBD,
    DBGKD_MAJOR_EFI,
    DBGKD_MAJOR_TNT,
    DBGKD_MAJOR_SINGULARITY,
    DBGKD_MAJOR_HYPERVISOR,
    DBGKD_MAJOR_COUNT
} DBGKD_MAJOR_TYPES;

//
// The major type is in the high byte
//
#define DBGKD_MAJOR_TYPE(MajorVersion) \
    ((DBGKD_MAJOR_TYPES)((MajorVersion) >> 8))

typedef struct _DBGKD_GET_VERSION32
{
    ui16 MajorVersion;
    ui16 MinorVersion;
    ui16 ProtocolVersion;
    ui16 Flags;
    ui32 KernBase;
    ui32 PsLoadedModuleList;
    ui16 MachineType;
    ui16 ThCallbackStack;
    ui16 NextCallback;
    ui16 FramePointer;
    ui32 KiCallUserMode;
    ui32 KeUserCallbackDispatcher;
    ui32 BreakpointWithStatus;
    ui32 DebuggerDataList;
} DBGKD_GET_VERSION32, *PDBGKD_GET_VERSION32;

typedef struct _DBGKD_DEBUG_DATA_HEADER32
{
    LIST_ENTRY32 List;
    ui32 OwnerTag;
    ui32 Size;
} DBGKD_DEBUG_DATA_HEADER32, *PDBGKD_DEBUG_DATA_HEADER32;

typedef struct _KDDEBUGGER_DATA32
{
    DBGKD_DEBUG_DATA_HEADER32 Header;
    ui32 KernBase;
    ui32 BreakpointWithStatus;
    ui32 SavedContext;
    ui16 ThCallbackStack;
    ui16 NextCallback;
    ui16 FramePointer;
    ui16 PaeEnabled:1;
    ui32 KiCallUserMode;
    ui32 KeUserCallbackDispatcher;
    ui32 PsLoadedModuleList;
    ui32 PsActiveProcessHead;
    ui32 PspCidTable;
    ui32 ExpSystemResourcesList;
    ui32 ExpPagedPoolDescriptor;
    ui32 ExpNumberOfPagedPools;
    ui32 KeTimeIncrement;
    ui32 KeBugCheckCallbackListHead;
    ui32 KiBugcheckData;
    ui32 IopErrorLogListHead;
    ui32 ObpRootDirectoryObject;
    ui32 ObpTypeObjectType;
    ui32 MmSystemCacheStart;
    ui32 MmSystemCacheEnd;
    ui32 MmSystemCacheWs;
    ui32 MmPfnDatabase;
    ui32 MmSystemPtesStart;
    ui32 MmSystemPtesEnd;
    ui32 MmSubsectionBase;
    ui32 MmNumberOfPagingFiles;
    ui32 MmLowestPhysicalPage;
    ui32 MmHighestPhysicalPage;
    ui32 MmNumberOfPhysicalPages;
    ui32 MmMaximumNonPagedPoolInBytes;
    ui32 MmNonPagedSystemStart;
    ui32 MmNonPagedPoolStart;
    ui32 MmNonPagedPoolEnd;
    ui32 MmPagedPoolStart;
    ui32 MmPagedPoolEnd;
    ui32 MmPagedPoolInformation;
    ui32 MmPageSize;
    ui32 MmSizeOfPagedPoolInBytes;
    ui32 MmTotalCommitLimit;
    ui32 MmTotalCommittedPages;
    ui32 MmSharedCommit;
    ui32 MmDriverCommit;
    ui32 MmProcessCommit;
    ui32 MmPagedPoolCommit;
    ui32 MmExtendedCommit;
    ui32 MmZeroedPageListHead;
    ui32 MmFreePageListHead;
    ui32 MmStandbyPageListHead;
    ui32 MmModifiedPageListHead;
    ui32 MmModifiedNoWritePageListHead;
    ui32 MmAvailablePages;
    ui32 MmResidentAvailablePages;
    ui32 PoolTrackTable;
    ui32 NonPagedPoolDescriptor;
    ui32 MmHighestUserAddress;
    ui32 MmSystemRangeStart;
    ui32 MmUserProbeAddress;
    ui32 KdPrintCircularBuffer;
    ui32 KdPrintCircularBufferEnd;
    ui32 KdPrintWritePointer;
    ui32 KdPrintRolloverCount;
    ui32 MmLoadedUserImageList;
} KDDEBUGGER_DATA32, *PKDDEBUGGER_DATA32;

typedef struct _DBGKD_GET_VERSION64
{
    ui16 MajorVersion;
    ui16 MinorVersion;
    u8   ProtocolVersion;
    u8   KdSecondaryVersion;
    ui16 Flags;
    ui16 MachineType;
    u8   MaxPacketType;
    u8   MaxStateChange;
    u8   MaxManipulate;
    u8   Simulation;
    ui16 Unused[1];
    ui64 KernBase;
    ui64 PsLoadedModuleList;
    ui64 DebuggerDataList;
} DBGKD_GET_VERSION64, *PDBGKD_GET_VERSION64;

typedef struct _DBGKD_DEBUG_DATA_HEADER64
{
    LIST_ENTRY64 List;
    ui32 OwnerTag;
    ui32 Size;
} DBGKD_DEBUG_DATA_HEADER64, *PDBGKD_DEBUG_DATA_HEADER64;

typedef union _GCC_ULONG64
{
    uiptr Pointer;
    ui64 RealPointer;
} GCC_ULONG64, *PGCC_ULONG64;

typedef struct _KDDEBUGGER_DATA64
{
    DBGKD_DEBUG_DATA_HEADER64 Header;
    ui64 KernBase;
    uiptr BreakpointWithStatus;
    ui64 SavedContext;
    ui16 ThCallbackStack;
    ui16 NextCallback;
    ui16 FramePointer;
    ui16 PaeEnabled:1;
    uiptr KiCallUserMode;
    ui64 KeUserCallbackDispatcher;
    uiptr PsLoadedModuleList;
    uiptr PsActiveProcessHead;
    uiptr PspCidTable;
    uiptr ExpSystemResourcesList;
    uiptr ExpPagedPoolDescriptor;
    uiptr ExpNumberOfPagedPools;
    uiptr KeTimeIncrement;
    uiptr KeBugCheckCallbackListHead;
    uiptr KiBugcheckData;
    uiptr IopErrorLogListHead;
    uiptr ObpRootDirectoryObject;
    uiptr ObpTypeObjectType;
    uiptr MmSystemCacheStart;
    uiptr MmSystemCacheEnd;
    uiptr MmSystemCacheWs;
    uiptr MmPfnDatabase;
    uiptr MmSystemPtesStart;
    uiptr MmSystemPtesEnd;
    uiptr MmSubsectionBase;
    uiptr MmNumberOfPagingFiles;
    uiptr MmLowestPhysicalPage;
    uiptr MmHighestPhysicalPage;
    uiptr MmNumberOfPhysicalPages;
    uiptr MmMaximumNonPagedPoolInBytes;
    uiptr MmNonPagedSystemStart;
    uiptr MmNonPagedPoolStart;
    uiptr MmNonPagedPoolEnd;
    uiptr MmPagedPoolStart;
    uiptr MmPagedPoolEnd;
    uiptr MmPagedPoolInformation;
    ui64 MmPageSize;
    uiptr MmSizeOfPagedPoolInBytes;
    uiptr MmTotalCommitLimit;
    uiptr MmTotalCommittedPages;
    uiptr MmSharedCommit;
    uiptr MmDriverCommit;
    uiptr MmProcessCommit;
    uiptr MmPagedPoolCommit;
    uiptr MmExtendedCommit;
    uiptr MmZeroedPageListHead;
    uiptr MmFreePageListHead;
    uiptr MmStandbyPageListHead;
    uiptr MmModifiedPageListHead;
    uiptr MmModifiedNoWritePageListHead;
    uiptr MmAvailablePages;
    uiptr MmResidentAvailablePages;
    uiptr PoolTrackTable;
    uiptr NonPagedPoolDescriptor;
    uiptr MmHighestUserAddress;
    uiptr MmSystemRangeStart;
    uiptr MmUserProbeAddress;
    uiptr KdPrintCircularBuffer;
    uiptr KdPrintCircularBufferEnd;
    uiptr KdPrintWritePointer;
    uiptr KdPrintRolloverCount;
    uiptr MmLoadedUserImageList;
    uiptr NtBuildLab;
    uiptr KiNormalSystemCall;
    uiptr KiProcessorBlock;
    uiptr MmUnloadedDrivers;
    uiptr MmLastUnloadedDriver;
    uiptr MmTriageActionTaken;
    uiptr MmSpecialPoolTag;
    uiptr KernelVerifier;
    uiptr MmVerifierData;
    uiptr MmAllocatedNonPagedPool;
    uiptr MmPeakCommitment;
    uiptr MmTotalCommitLimitMaximum;
    uiptr CmNtCSDVersion;
    uiptr MmPhysicalMemoryBlock;
    uiptr MmSessionBase;
    uiptr MmSessionSize;
    uiptr MmSystemParentTablePage;
    uiptr MmVirtualTranslationBase;
    ui16 OffsetKThreadNextProcessor;
    ui16 OffsetKThreadTeb;
    ui16 OffsetKThreadKernelStack;
    ui16 OffsetKThreadInitialStack;
    ui16 OffsetKThreadApcProcess;
    ui16 OffsetKThreadState;
    ui16 OffsetKThreadBStore;
    ui16 OffsetKThreadBStoreLimit;
    ui16 SizeEProcess;
    ui16 OffsetEprocessPeb;
    ui16 OffsetEprocessParentCID;
    ui16 OffsetEprocessDirectoryTableBase;
    ui16 SizePrcb;
    ui16 OffsetPrcbDpcRoutine;
    ui16 OffsetPrcbCurrentThread;
    ui16 OffsetPrcbMhz;
    ui16 OffsetPrcbCpuType;
    ui16 OffsetPrcbVendorString;
    ui16 OffsetPrcbProcStateContext;
    ui16 OffsetPrcbNumber;
    ui16 SizeEThread;
    uiptr KdPrintCircularBufferPtr;
    uiptr KdPrintBufferSize;
    uiptr KeLoaderBlock;
    ui16 SizePcr;
    ui16 OffsetPcrSelfPcr;
    ui16 OffsetPcrCurrentPrcb;
    ui16 OffsetPcrContainedPrcb;
    ui16 OffsetPcrInitialBStore;
    ui16 OffsetPcrBStoreLimit;
    ui16 OffsetPcrInitialStack;
    ui16 OffsetPcrStackLimit;
    ui16 OffsetPrcbPcrPage;
    ui16 OffsetPrcbProcStateSpecialReg;
    ui16 GdtR0Code;
    ui16 GdtR0Data;
    ui16 GdtR0Pcr;
    ui16 GdtR3Code;
    ui16 GdtR3Data;
    ui16 GdtR3Teb;
    ui16 GdtLdt;
    ui16 GdtTss;
    ui16 Gdt64R3CmCode;
    ui16 Gdt64R3CmTeb;
    uiptr IopNumTriageDumpDataBlocks;
    uiptr IopTriageDumpDataBlocks;
#if 0 // Longhorn/Vista and later
    uiptr VfCrashDataBlock;
    uiptr MmBadPagesDetected;
    uiptr MmZeroedPageSingleBitErrorsDetected;
#endif
} KDDEBUGGER_DATA64, *PKDDEBUGGER_DATA64;

//
// Conversion Macros
//
#define COPYSE(p64, p32, f)                 \
    p64->f = (ui64)(LONG64)(LONG)p32->f

//
// Packet Size and Control Stream Size
//
#define PACKET_MAX_SIZE                     4000
#define DBGKD_MAXSTREAM                     16

//
// Magic Packet IDs
//
#define INITIAL_PACKET_ID                   0x80800000
#define SYNC_PACKET_ID                      0x00000800

//
// Magic Packet bytes
//
#define BREAKIN_PACKET                      0x62626262
#define BREAKIN_PACKET_BYTE                 0x62
#define PACKET_LEADER                       0x30303030
#define PACKET_LEADER_BYTE                  0x30
#define CONTROL_PACKET_LEADER               0x69696969
#define CONTROL_PACKET_LEADER_BYTE          0x69
#define PACKET_TRAILING_BYTE                0xAA

//
// Packet Types
//
#define PACKET_TYPE_UNUSED                  0
#define PACKET_TYPE_KD_STATE_CHANGE32       1
#define PACKET_TYPE_KD_STATE_MANIPULATE     2
#define PACKET_TYPE_KD_DEBUG_IO             3
#define PACKET_TYPE_KD_ACKNOWLEDGE          4
#define PACKET_TYPE_KD_RESEND               5
#define PACKET_TYPE_KD_RESET                6
#define PACKET_TYPE_KD_STATE_CHANGE64       7
#define PACKET_TYPE_KD_POLL_BREAKIN         8
#define PACKET_TYPE_KD_TRACE_IO             9
#define PACKET_TYPE_KD_CONTROL_REQUEST      10
#define PACKET_TYPE_KD_FILE_IO              11
#define PACKET_TYPE_MAX                     12

//
// Wait State Change Types
//
#define DbgKdMinimumStateChange             0x00003030
#define DbgKdExceptionStateChange           0x00003030
#define DbgKdLoadSymbolsStateChange         0x00003031
#define DbgKdCommandStringStateChange       0x00003032
#define DbgKdMaximumStateChange             0x00003033

//
// This is combined with the basic state change code
// if the state is from an alternate source
//
#define DbgKdAlternateStateChange           0x00010000

//
// Manipulate Types
//
#define DbgKdMinimumManipulate              0x00003130
#define DbgKdReadVirtualMemoryApi           0x00003130
#define DbgKdWriteVirtualMemoryApi          0x00003131
#define DbgKdGetContextApi                  0x00003132
#define DbgKdSetContextApi                  0x00003133
#define DbgKdWriteBreakPointApi             0x00003134
#define DbgKdRestoreBreakPointApi           0x00003135
#define DbgKdContinueApi                    0x00003136
#define DbgKdReadControlSpaceApi            0x00003137
#define DbgKdWriteControlSpaceApi           0x00003138
#define DbgKdReadIoSpaceApi                 0x00003139
#define DbgKdWriteIoSpaceApi                0x0000313A
#define DbgKdRebootApi                      0x0000313B
#define DbgKdContinueApi2                   0x0000313C
#define DbgKdReadPhysicalMemoryApi          0x0000313D
#define DbgKdWritePhysicalMemoryApi         0x0000313E
#define DbgKdQuerySpecialCallsApi           0x0000313F
#define DbgKdSetSpecialCallApi              0x00003140
#define DbgKdClearSpecialCallsApi           0x00003141
#define DbgKdSetInternalBreakPointApi       0x00003142
#define DbgKdGetInternalBreakPointApi       0x00003143
#define DbgKdReadIoSpaceExtendedApi         0x00003144
#define DbgKdWriteIoSpaceExtendedApi        0x00003145
#define DbgKdGetVersionApi                  0x00003146
#define DbgKdWriteBreakPointExApi           0x00003147
#define DbgKdRestoreBreakPointExApi         0x00003148
#define DbgKdCauseBugCheckApi               0x00003149
#define DbgKdSwitchProcessor                0x00003150
#define DbgKdPageInApi                      0x00003151
#define DbgKdReadMachineSpecificRegister    0x00003152
#define DbgKdWriteMachineSpecificRegister   0x00003153
#define OldVlm1                             0x00003154
#define OldVlm2                             0x00003155
#define DbgKdSearchMemoryApi                0x00003156
#define DbgKdGetBusDataApi                  0x00003157
#define DbgKdSetBusDataApi                  0x00003158
#define DbgKdCheckLowMemoryApi              0x00003159
#define DbgKdClearAllInternalBreakpointsApi 0x0000315A
#define DbgKdFillMemoryApi                  0x0000315B
#define DbgKdQueryMemoryApi                 0x0000315C
#define DbgKdSwitchPartition                0x0000315D
#define DbgKdMaximumManipulate              0x0000315E

//
// Debug I/O Types
//
#define DbgKdPrintStringApi                 0x00003230
#define DbgKdGetStringApi                   0x00003231

//
// Trace I/O Types
//
#define DbgKdPrintTraceApi                  0x00003330

//
// Control Request Types
//
#define DbgKdRequestHardwareBp              0x00004300
#define DbgKdReleaseHardwareBp              0x00004301

//
// File I/O Types
//
#define DbgKdCreateFileApi                 0x00003430
#define DbgKdReadFileApi                   0x00003431
#define DbgKdWriteFileApi                  0x00003432
#define DbgKdCloseFileApi                  0x00003433

//
// Control Report Flags
//
#define REPORT_INCLUDES_SEGS                0x0001
#define REPORT_STANDARD_CS                  0x0002

//
// Protocol Versions
//
#define DBGKD_64BIT_PROTOCOL_VERSION1       5
#define DBGKD_64BIT_PROTOCOL_VERSION2       6

//
// Query Memory Address Spaces
//
#define DBGKD_QUERY_MEMORY_VIRTUAL          0
#define DBGKD_QUERY_MEMORY_PROCESS          0
#define DBGKD_QUERY_MEMORY_SESSION          1
#define DBGKD_QUERY_MEMORY_KERNEL           2

//
// Query Memory Flags
//
#define DBGKD_QUERY_MEMORY_READ             0x01
#define DBGKD_QUERY_MEMORY_WRITE            0x02
#define DBGKD_QUERY_MEMORY_EXECUTE          0x04
#define DBGKD_QUERY_MEMORY_FIXED            0x08

//
// Internal Breakpoint Flags
//
#define DBGKD_INTERNAL_BP_FLAG_COUNTONLY    0x01
#define DBGKD_INTERNAL_BP_FLAG_INVALID      0x02 
#define DBGKD_INTERNAL_BP_FLAG_SUSPENDED    0x04
#define DBGKD_INTERNAL_BP_FLAG_DYING        0x08

//
// Fill Memory Flags
//
#define DBGKD_FILL_MEMORY_VIRTUAL           0x01
#define DBGKD_FILL_MEMORY_PHYSICAL          0x02

//
// Physical Memory Caching Flags
//
#define DBGKD_CACHING_DEFAULT               0
#define DBGKD_CACHING_CACHED                1
#define DBGKD_CACHING_UNCACHED              2
#define DBGKD_CACHING_WRITE_COMBINED        3

//
// Partition Switch Flags
//
#define DBGKD_PARTITION_DEFAULT             0x00
#define DBGKD_PARTITION_ALTERNATE           0x01

//
// AMD64 Control Space types
//
#define AMD64_DEBUG_CONTROL_SPACE_KPCR 0
#define AMD64_DEBUG_CONTROL_SPACE_KPRCB 1
#define AMD64_DEBUG_CONTROL_SPACE_KSPECIAL 2
#define AMD64_DEBUG_CONTROL_SPACE_KTHREAD 3


//
// KD Packet Structure
//
typedef struct _KD_PACKET
{
    ui32 PacketLeader;
    ui16 PacketType;
    ui16 ByteCount;
    ui32 PacketId;
    ui32 Checksum;
} KD_PACKET, *PKD_PACKET;

//
// KD Context
//
typedef struct _KD_CONTEXT
{
    ui32 KdpDefaultRetries;
    BOOLEAN KdpControlCPending;
} KD_CONTEXT, *PKD_CONTEXT;

//
// Control Sets for Supported Architectures
//
#include <pshpack4.h>
typedef struct _X86_DBGKD_CONTROL_SET
{
    ui32 TraceFlag;
    ui32 Dr7;
    ui32 CurrentSymbolStart;
    ui32 CurrentSymbolEnd;
} X86_DBGKD_CONTROL_SET, *PX86_DBGKD_CONTROL_SET;

typedef struct _ALPHA_DBGKD_CONTROL_SET
{
    ui32 __padding;
} ALPHA_DBGKD_CONTROL_SET, *PALPHA_DBGKD_CONTROL_SET;

typedef struct _IA64_DBGKD_CONTROL_SET
{
    ui32 Continue;
    ui64 CurrentSymbolStart;
    ui64 CurrentSymbolEnd;
} IA64_DBGKD_CONTROL_SET, *PIA64_DBGKD_CONTROL_SET;

typedef struct _AMD64_DBGKD_CONTROL_SET
{
    ui32 TraceFlag;
    ui64 Dr7;
    ui64 CurrentSymbolStart;
    ui64 CurrentSymbolEnd;
} AMD64_DBGKD_CONTROL_SET, *PAMD64_DBGKD_CONTROL_SET;

typedef struct _ARM_DBGKD_CONTROL_SET
{
    ui32 Continue;
    ui32 CurrentSymbolStart;
    ui32 CurrentSymbolEnd;
} ARM_DBGKD_CONTROL_SET, *PARM_DBGKD_CONTROL_SET;

typedef struct _DBGKD_ANY_CONTROL_SET
{
    union
    {
        X86_DBGKD_CONTROL_SET X86ControlSet;
        ALPHA_DBGKD_CONTROL_SET AlphaControlSet;
        IA64_DBGKD_CONTROL_SET IA64ControlSet;
        AMD64_DBGKD_CONTROL_SET Amd64ControlSet;
        ARM_DBGKD_CONTROL_SET ARMControlSet;
    };
} DBGKD_ANY_CONTROL_SET, *PDBGKD_ANY_CONTROL_SET;
#include <poppack.h>

#if defined(_M_IX86)
typedef X86_DBGKD_CONTROL_SET DBGKD_CONTROL_SET, *PDBGKD_CONTROL_SET;
#elif defined(_M_AMD64)
typedef AMD64_DBGKD_CONTROL_SET DBGKD_CONTROL_SET, *PDBGKD_CONTROL_SET;
#elif defined(_M_ARM)
typedef ARM_DBGKD_CONTROL_SET DBGKD_CONTROL_SET, *PDBGKD_CONTROL_SET;
#else
#error Unsupported Architecture
#endif

//
// DBGKM Structure for Exceptions
//
typedef struct _DBGKM_EXCEPTION32
{
    EXCEPTION_RECORD32 ExceptionRecord;
    ui32 FirstChance;
} DBGKM_EXCEPTION32, *PDBGKM_EXCEPTION32;

typedef struct _DBGKM_EXCEPTION64
{
    EXCEPTION_RECORD64 ExceptionRecord;
    ui32 FirstChance;
} DBGKM_EXCEPTION64, *PDBGKM_EXCEPTION64;

//
// DBGKD Structure for State Change
//
typedef struct _X86_DBGKD_CONTROL_REPORT
{
    ui32   Dr6;
    ui32   Dr7;
    ui16  InstructionCount;
    ui16  ReportFlags;
    u8    InstructionStream[DBGKD_MAXSTREAM];
    ui16  SegCs;
    ui16  SegDs;
    ui16  SegEs;
    ui16  SegFs;
    ui32   EFlags;
} X86_DBGKD_CONTROL_REPORT, *PX86_DBGKD_CONTROL_REPORT;

typedef struct _ALPHA_DBGKD_CONTROL_REPORT
{
    ui32 InstructionCount;
    u8   InstructionStream[DBGKD_MAXSTREAM];
} ALPHA_DBGKD_CONTROL_REPORT, *PALPHA_DBGKD_CONTROL_REPORT;

typedef struct _IA64_DBGKD_CONTROL_REPORT
{
    ui32 InstructionCount;
    u8   InstructionStream[DBGKD_MAXSTREAM];
} IA64_DBGKD_CONTROL_REPORT, *PIA64_DBGKD_CONTROL_REPORT;

typedef struct _AMD64_DBGKD_CONTROL_REPORT
{
    ui64 Dr6;
    ui64 Dr7;
    ui32 EFlags;
    ui16 InstructionCount;
    ui16 ReportFlags;
    u8   InstructionStream[DBGKD_MAXSTREAM];
    ui16 SegCs;
    ui16 SegDs;
    ui16 SegEs;
    ui16 SegFs;
} AMD64_DBGKD_CONTROL_REPORT, *PAMD64_DBGKD_CONTROL_REPORT;

typedef struct _ARM_DBGKD_CONTROL_REPORT
{
    ui32 Cpsr;
    ui32 InstructionCount;
    u8   InstructionStream[DBGKD_MAXSTREAM];
} ARM_DBGKD_CONTROL_REPORT, *PARM_DBGKD_CONTROL_REPORT;

typedef struct _DBGKD_ANY_CONTROL_REPORT
{
    union
    {
        X86_DBGKD_CONTROL_REPORT X86ControlReport;
        ALPHA_DBGKD_CONTROL_REPORT AlphaControlReport;
        IA64_DBGKD_CONTROL_REPORT IA64ControlReport;
        AMD64_DBGKD_CONTROL_REPORT Amd64ControlReport;
        ARM_DBGKD_CONTROL_REPORT ARMControlReport;
    };
} DBGKD_ANY_CONTROL_REPORT, *PDBGKD_ANY_CONTROL_REPORT;

#if defined(_M_IX86)
typedef X86_DBGKD_CONTROL_REPORT DBGKD_CONTROL_REPORT, *PDBGKD_CONTROL_REPORT;
#elif defined(_M_AMD64)
typedef AMD64_DBGKD_CONTROL_REPORT DBGKD_CONTROL_REPORT, *PDBGKD_CONTROL_REPORT;
#elif defined(_M_ARM)
typedef ARM_DBGKD_CONTROL_REPORT DBGKD_CONTROL_REPORT, *PDBGKD_CONTROL_REPORT;
#else
#error Unsupported Architecture
#endif

//
// DBGKD Structure for Debug I/O Type Print String
//
typedef struct _DBGKD_PRINT_STRING
{
    ui32 LengthOfString;
} DBGKD_PRINT_STRING, *PDBGKD_PRINT_STRING;

//
// DBGKD Structure for Debug I/O Type Get String
//
typedef struct _DBGKD_GET_STRING
{
    ui32 LengthOfPromptString;
    ui32 LengthOfStringRead;
} DBGKD_GET_STRING, *PDBGKD_GET_STRING;

//
// DBGKD Structure for Debug I/O
//
typedef struct _DBGKD_DEBUG_IO
{
    ui32 ApiNumber;
    ui16 ProcessorLevel;
    ui16 Processor;
    union
    {
        DBGKD_PRINT_STRING PrintString;
        DBGKD_GET_STRING GetString;
    } u;
} DBGKD_DEBUG_IO, *PDBGKD_DEBUG_IO;

//
// DBGkD Structure for Command String
//
typedef struct _DBGKD_COMMAND_STRING
{
    ui32 Flags;
    ui32 Reserved1;
    ui64 Reserved2[7];
} DBGKD_COMMAND_STRING, *PDBGKD_COMMAND_STRING;

//
// DBGKD Structure for Load Symbols
//
typedef struct _DBGKD_LOAD_SYMBOLS32
{
    ui32 PathNameLength;
    ui32 BaseOfDll;
    ui32 ProcessId;
    ui32 CheckSum;
    ui32 SizeOfImage;
    BOOLEAN UnloadSymbols;
} DBGKD_LOAD_SYMBOLS32, *PDBGKD_LOAD_SYMBOLS32;

typedef struct _DBGKD_LOAD_SYMBOLS64
{
    ui32 PathNameLength;
    ui64 BaseOfDll;
    ui64 ProcessId;
    ui32 CheckSum;
    ui32 SizeOfImage;
    BOOLEAN UnloadSymbols;
} DBGKD_LOAD_SYMBOLS64, *PDBGKD_LOAD_SYMBOLS64;

//
// DBGKD Structure for Wait State Change
//

typedef struct _DBGKD_WAIT_STATE_CHANGE32
{
    ui32 NewState;
    ui16 ProcessorLevel;
    ui16 Processor;
    ui32 NumberProcessors;
    ui32 Thread;
    ui32 ProgramCounter;
    union
	{
        DBGKM_EXCEPTION32 Exception;
        DBGKD_LOAD_SYMBOLS32 LoadSymbols;
    } u;
} DBGKD_WAIT_STATE_CHANGE32, *PDBGKD_WAIT_STATE_CHANGE32;

typedef struct _DBGKD_WAIT_STATE_CHANGE64
{
    ui32 NewState;
    ui16 ProcessorLevel;
    ui16 Processor;
    ui32 NumberProcessors;
    ui64 Thread;
    ui64 ProgramCounter;
    union
    {
        DBGKM_EXCEPTION64 Exception;
        DBGKD_LOAD_SYMBOLS64 LoadSymbols;
    } u;
} DBGKD_WAIT_STATE_CHANGE64, *PDBGKD_WAIT_STATE_CHANGE64;

typedef struct _DBGKD_ANY_WAIT_STATE_CHANGE
{
    ui32 NewState;
    ui16 ProcessorLevel;
    ui16 Processor;
    ui32 NumberProcessors;
    ui64 Thread;
    ui64 ProgramCounter;
    union
	{
        DBGKM_EXCEPTION64 Exception;
        DBGKD_LOAD_SYMBOLS64 LoadSymbols;
        DBGKD_COMMAND_STRING CommandString;
    } u;
    union
	{
        DBGKD_CONTROL_REPORT ControlReport;
        DBGKD_ANY_CONTROL_REPORT AnyControlReport;
    };
} DBGKD_ANY_WAIT_STATE_CHANGE, *PDBGKD_ANY_WAIT_STATE_CHANGE;

//
// DBGKD Manipulate Structures
//
typedef struct _DBGKD_READ_MEMORY32
{
    ui32 TargetBaseAddress;
    ui32 TransferCount;
    ui32 ActualBytesRead;
} DBGKD_READ_MEMORY32, *PDBGKD_READ_MEMORY32;

typedef struct _DBGKD_READ_MEMORY64
{
    ui64 TargetBaseAddress;
    ui32 TransferCount;
    ui32 ActualBytesRead;
} DBGKD_READ_MEMORY64, *PDBGKD_READ_MEMORY64;

typedef struct _DBGKD_WRITE_MEMORY32
{
    ui32 TargetBaseAddress;
    ui32 TransferCount;
    ui32 ActualBytesWritten;
} DBGKD_WRITE_MEMORY32, *PDBGKD_WRITE_MEMORY32;

typedef struct _DBGKD_WRITE_MEMORY64
{
    ui64 TargetBaseAddress;
    ui32 TransferCount;
    ui32 ActualBytesWritten;
} DBGKD_WRITE_MEMORY64, *PDBGKD_WRITE_MEMORY64;

typedef struct _DBGKD_GET_CONTEXT
{
    ui32 Unused;
} DBGKD_GET_CONTEXT, *PDBGKD_GET_CONTEXT;

typedef struct _DBGKD_SET_CONTEXT
{
    ui32 ContextFlags;
} DBGKD_SET_CONTEXT, *PDBGKD_SET_CONTEXT;

typedef struct _DBGKD_WRITE_BREAKPOINT32
{
    ui32 BreakPointAddress;
    ui32 BreakPointHandle;
} DBGKD_WRITE_BREAKPOINT32, *PDBGKD_WRITE_BREAKPOINT32;

typedef struct _DBGKD_WRITE_BREAKPOINT64
{
    ui64 BreakPointAddress;
    ui32 BreakPointHandle;
} DBGKD_WRITE_BREAKPOINT64, *PDBGKD_WRITE_BREAKPOINT64;

typedef struct _DBGKD_RESTORE_BREAKPOINT
{
    ui32 BreakPointHandle;
} DBGKD_RESTORE_BREAKPOINT, *PDBGKD_RESTORE_BREAKPOINT;

typedef struct _DBGKD_CONTINUE
{
    NTSTATUS ContinueStatus;
} DBGKD_CONTINUE, *PDBGKD_CONTINUE;

#include <pshpack4.h>
typedef struct _DBGKD_CONTINUE2
{
    NTSTATUS ContinueStatus;
    union
    {
        DBGKD_CONTROL_SET ControlSet;
        DBGKD_ANY_CONTROL_SET AnyControlSet;
    };
} DBGKD_CONTINUE2, *PDBGKD_CONTINUE2;
#include <poppack.h>

typedef struct _DBGKD_READ_WRITE_IO32
{
    ui32 IoAddress;
    ui32 DataSize;
    ui32 DataValue;
} DBGKD_READ_WRITE_IO32, *PDBGKD_READ_WRITE_IO32;

typedef struct _DBGKD_READ_WRITE_IO64
{
    ui64 IoAddress;
    ui32 DataSize;
    ui32 DataValue;
} DBGKD_READ_WRITE_IO64, *PDBGKD_READ_WRITE_IO64;

typedef struct _DBGKD_READ_WRITE_IO_EXTENDED32
{
    ui32 DataSize;
    ui32 InterfaceType;
    ui32 BusNumber;
    ui32 AddressSpace;
    ui32 IoAddress;
    ui32 DataValue;
} DBGKD_READ_WRITE_IO_EXTENDED32, *PDBGKD_READ_WRITE_IO_EXTENDED32;

typedef struct _DBGKD_READ_WRITE_IO_EXTENDED64
{
    ui32 DataSize;
    ui32 InterfaceType;
    ui32 BusNumber;
    ui32 AddressSpace;
    ui64 IoAddress;
    ui32 DataValue;
} DBGKD_READ_WRITE_IO_EXTENDED64, *PDBGKD_READ_WRITE_IO_EXTENDED64;

typedef struct _DBGKD_READ_WRITE_MSR
{
    ui32 Msr;
    ui32 DataValueLow;
    ui32 DataValueHigh;
} DBGKD_READ_WRITE_MSR, *PDBGKD_READ_WRITE_MSR;

typedef struct _DBGKD_QUERY_SPECIAL_CALLS
{
    ui32 NumberOfSpecialCalls;
} DBGKD_QUERY_SPECIAL_CALLS, *PDBGKD_QUERY_SPECIAL_CALLS;

typedef struct _DBGKD_SET_SPECIAL_CALL32
{
    ui32 SpecialCall;
} DBGKD_SET_SPECIAL_CALL32, *PDBGKD_SET_SPECIAL_CALL32;

typedef struct _DBGKD_SET_SPECIAL_CALL64
{
    ui64 SpecialCall;
} DBGKD_SET_SPECIAL_CALL64, *PDBGKD_SET_SPECIAL_CALL64;

typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT32
{
    ui32 BreakpointAddress;
    ui32 Flags;
} DBGKD_SET_INTERNAL_BREAKPOINT32, *PDBGKD_SET_INTERNAL_BREAKPOINT32;

typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT64
{
    ui64 BreakpointAddress;
    ui32 Flags;
} DBGKD_SET_INTERNAL_BREAKPOINT64, *PDBGKD_SET_INTERNAL_BREAKPOINT64;

typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT32
{
    ui32 BreakpointAddress;
    ui32 Flags;
    ui32 Calls;
    ui32 MaxCallsPerPeriod;
    ui32 MinInstructions;
    ui32 MaxInstructions;
    ui32 TotalInstructions;
} DBGKD_GET_INTERNAL_BREAKPOINT32, *PDBGKD_GET_INTERNAL_BREAKPOINT32;

typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT64
{
    ui64 BreakpointAddress;
    ui32 Flags;
    ui32 Calls;
    ui32 MaxCallsPerPeriod;
    ui32 MinInstructions;
    ui32 MaxInstructions;
    ui32 TotalInstructions;
} DBGKD_GET_INTERNAL_BREAKPOINT64, *PDBGKD_GET_INTERNAL_BREAKPOINT64;

typedef struct _DBGKD_BREAKPOINTEX
{
    ui32 BreakPointCount;
    NTSTATUS ContinueStatus;
} DBGKD_BREAKPOINTEX, *PDBGKD_BREAKPOINTEX;

typedef struct _DBGKD_SEARCH_MEMORY
{
    union
    {
        ui64 SearchAddress;
        ui64 FoundAddress;
    };
    ui64 SearchLength;
    ui32 PatternLength;
} DBGKD_SEARCH_MEMORY, *PDBGKD_SEARCH_MEMORY;

typedef struct _DBGKD_GET_SET_BUS_DATA
{
    ui32 BusDataType;
    ui32 BusNumber;
    ui32 SlotNumber;
    ui32 Offset;
    ui32 Length;
} DBGKD_GET_SET_BUS_DATA, *PDBGKD_GET_SET_BUS_DATA;

typedef struct _DBGKD_FILL_MEMORY
{
    ui64 Address;
    ui32 Length;
    ui16 Flags;
    ui16 PatternLength;
} DBGKD_FILL_MEMORY, *PDBGKD_FILL_MEMORY;

typedef struct _DBGKD_QUERY_MEMORY
{
    ui64 Address;
    ui64 Reserved;
    ui32 AddressSpace;
    ui32 Flags;
} DBGKD_QUERY_MEMORY, *PDBGKD_QUERY_MEMORY;

typedef struct _DBGKD_SWITCH_PARTITION
{
    ui32 Partition;
} DBGKD_SWITCH_PARTITION;

//
// DBGKD Structure for Manipulate
//
typedef struct _DBGKD_MANIPULATE_STATE32
{
    ui32 ApiNumber;
    ui16 ProcessorLevel;
    ui16 Processor;
    NTSTATUS ReturnStatus;
    union
    {
        DBGKD_READ_MEMORY32 ReadMemory;
        DBGKD_WRITE_MEMORY32 WriteMemory;
        DBGKD_READ_MEMORY64 ReadMemory64;
        DBGKD_WRITE_MEMORY64 WriteMemory64;
        DBGKD_GET_CONTEXT GetContext;
        DBGKD_SET_CONTEXT SetContext;
        DBGKD_WRITE_BREAKPOINT32 WriteBreakPoint;
        DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;
        DBGKD_CONTINUE Continue;
        DBGKD_CONTINUE2 Continue2;
        DBGKD_READ_WRITE_IO32 ReadWriteIo;
        DBGKD_READ_WRITE_IO_EXTENDED32 ReadWriteIoExtended;
        DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;
        DBGKD_SET_SPECIAL_CALL32 SetSpecialCall;
        DBGKD_SET_INTERNAL_BREAKPOINT32 SetInternalBreakpoint;
        DBGKD_GET_INTERNAL_BREAKPOINT32 GetInternalBreakpoint;
        DBGKD_GET_VERSION32 GetVersion32;
        DBGKD_BREAKPOINTEX BreakPointEx;
        DBGKD_READ_WRITE_MSR ReadWriteMsr;
        DBGKD_SEARCH_MEMORY SearchMemory;
        DBGKD_GET_SET_BUS_DATA GetSetBusData;
        DBGKD_FILL_MEMORY FillMemory;
        DBGKD_QUERY_MEMORY QueryMemory;
        DBGKD_SWITCH_PARTITION SwitchPartition;
    } u;
} DBGKD_MANIPULATE_STATE32, *PDBGKD_MANIPULATE_STATE32;

typedef struct _DBGKD_MANIPULATE_STATE64
{
    ui32 ApiNumber;
    ui16 ProcessorLevel;
    ui16 Processor;
    NTSTATUS ReturnStatus;
    union
    {
        DBGKD_READ_MEMORY64 ReadMemory;
        DBGKD_WRITE_MEMORY64 WriteMemory;
        DBGKD_GET_CONTEXT GetContext;
        DBGKD_SET_CONTEXT SetContext;
        DBGKD_WRITE_BREAKPOINT64 WriteBreakPoint;
        DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;
        DBGKD_CONTINUE Continue;
        DBGKD_CONTINUE2 Continue2;
        DBGKD_READ_WRITE_IO64 ReadWriteIo;
        DBGKD_READ_WRITE_IO_EXTENDED64 ReadWriteIoExtended;
        DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;
        DBGKD_SET_SPECIAL_CALL64 SetSpecialCall;
        DBGKD_SET_INTERNAL_BREAKPOINT64 SetInternalBreakpoint;
        DBGKD_GET_INTERNAL_BREAKPOINT64 GetInternalBreakpoint;
        DBGKD_GET_VERSION64 GetVersion64;
        DBGKD_BREAKPOINTEX BreakPointEx;
        DBGKD_READ_WRITE_MSR ReadWriteMsr;
        DBGKD_SEARCH_MEMORY SearchMemory;
        DBGKD_GET_SET_BUS_DATA GetSetBusData;
        DBGKD_FILL_MEMORY FillMemory;
        DBGKD_QUERY_MEMORY QueryMemory;
        DBGKD_SWITCH_PARTITION SwitchPartition;
    } u;
} DBGKD_MANIPULATE_STATE64, *PDBGKD_MANIPULATE_STATE64;

//
// File I/O Structure
//
typedef struct _DBGKD_CREATE_FILE
{
    ui32 DesiredAccess;
    ui32 FileAttributes;
    ui32 ShareAccess;
    ui32 CreateDisposition;
    ui32 CreateOptions;
    ui64 Handle;
    ui64 Length;
} DBGKD_CREATE_FILE, *PDBGKD_CREATE_FILE;

typedef struct _DBGKD_READ_FILE
{
    ui64 Handle;
    ui64 Offset;
    ui32 Length;
} DBGKD_READ_FILE, *PDBGKD_READ_FILE;

typedef struct _DBGKD_WRITE_FILE
{
    ui64 Handle;
    ui64 Offset;
    ui32 Length;
} DBGKD_WRITE_FILE, *PDBGKD_WRITE_FILE;

typedef struct _DBGKD_CLOSE_FILE
{
    ui64 Handle;
} DBGKD_CLOSE_FILE, *PDBGKD_CLOSE_FILE;

typedef struct _DBGKD_FILE_IO
{
    ui32 ApiNumber;
    ui32 Status;
    union
	{
        ui64 ReserveSpace[7];
        DBGKD_CREATE_FILE CreateFile;
        DBGKD_READ_FILE ReadFile;
        DBGKD_WRITE_FILE WriteFile;
        DBGKD_CLOSE_FILE CloseFile;
    } u;
} DBGKD_FILE_IO, *PDBGKD_FILE_IO;


//
// Control Request Structure
//
typedef struct _DBGKD_REQUEST_BREAKPOINT
{
    ui32 HardwareBreakPointNumber;
    ui32 Available;
} DBGKD_REQUEST_BREAKPOINT, *PDBGKD_REQUEST_BREAKPOINT;

typedef struct _DBGKD_RELEASE_BREAKPOINT
{
    ui32 HardwareBreakPointNumber;
    ui32 Released;
} DBGKD_RELEASE_BREAKPOINT, *PDBGKD_RELEASE_BREAKPOINT;

typedef struct _DBGKD_CONTROL_REQUEST
{
    ui32 ApiNumber;
    union
	{
        DBGKD_REQUEST_BREAKPOINT RequestBreakpoint;
        DBGKD_RELEASE_BREAKPOINT ReleaseBreakpoint;
    } u;
} DBGKD_CONTROL_REQUEST, *PDBGKD_CONTROL_REQUEST;

//
// Trace I/O Structure
//
typedef struct _DBGKD_PRINT_TRACE
{
    ui32 LengthOfData;
} DBGKD_PRINT_TRACE, *PDBGKD_PRINT_TRACE;

typedef struct _DBGKD_TRACE_IO
{
   ui32 ApiNumber;
   ui16 ProcessorLevel;
   ui16 Processor;
   union
   {
       ui64 ReserveSpace[7];
       DBGKD_PRINT_TRACE PrintTrace;
   } u;
} DBGKD_TRACE_IO, *PDBGKD_TRACE_IO;

static
__inline
VOID
NTAPI
ExceptionRecord32To64(IN PEXCEPTION_RECORD32 Ex32,
                      OUT PEXCEPTION_RECORD64 Ex64)
{
    ui32 i;

    Ex64->ExceptionCode = Ex32->ExceptionCode;
    Ex64->ExceptionFlags = Ex32->ExceptionFlags;
    Ex64->ExceptionRecord = Ex32->ExceptionRecord;
    COPYSE(Ex64,Ex32,ExceptionAddress);
    Ex64->NumberParameters = Ex32->NumberParameters;

    for (i = 0; i < EXCEPTION_MAXIMUM_PARAMETERS; i++)
    {
        COPYSE(Ex64,Ex32,ExceptionInformation[i]);
    }
}

#endif


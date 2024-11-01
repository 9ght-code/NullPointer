#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include "../Nullpointer/Request.h"
#include "IO_Operations.h"


NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAdress, PEPROCESS TargetProcess, PVOID TargetAdress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitFunction);
NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(__in PEPROCESS Process);

#pragma once
#include "../sharedSpace.h"

void UnloadDriver();
HANDLE loadDriver();

boolean ReadMemoryDriver(HANDLE driver, uintptr_t address, PVOID buffer, size_t size);
boolean WriteMemoryDriver(HANDLE hDevice, uintptr_t address, void* value, SIZE_T size);

boolean attach_to_process(const DWORD pid);
PHANDLE GetDriverHandle();
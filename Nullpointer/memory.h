#pragma once
#include <Windows.h>

static uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name);

DWORD get_process_id(const wchar_t* process_name);

BOOL ReadMemory(HANDLE hDevice, uintptr_t address, PVOID buffer, SIZE_T size);

BOOL WriteMemory(HANDLE hDevice, uintptr_t address, int value, SIZE_T size);

boolean attach_to_process(HANDLE DriverHandle, const DWORD pid);

void UnloadDriver(HANDLE driver);

HANDLE loadDriver();

uintptr_t initClient(HANDLE driver);
#pragma once
#include "../sharedSpace.h"

boolean _ReadMemoryWINAPI(HANDLE driver, uintptr_t address, PVOID buffer, size_t size);
boolean _WriteMemoryWINAPI(HANDLE driver, uintptr_t address, void* val, size_t size);

DWORD GetPIDField();
void SetPIDField(DWORD value);

uintptr_t get_module_base(const wchar_t* module_name);
DWORD get_process_id(const wchar_t* process_name);
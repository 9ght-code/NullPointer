#pragma once
#include "../dependencies.h"
#include "../NullPointer.h"

typedef struct _MemoryRegion {
	char* regionName;
}Page;

typedef struct {
	char memory[1024];
	char* name;
	size_t offset;
}  MemoryPool, * PMemoryPool;

HANDLE loadDriver();

boolean ReadMemory(HANDLE hDevice, uintptr_t address, PVOID buffer, SIZE_T size);
boolean WriteMemory(HANDLE hDevice, uintptr_t address, int value, SIZE_T size);
boolean attach_to_process(PHANDLE DriverHandle, const DWORD pid);

static uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name);
uintptr_t initClient(PHANDLE driver);

DWORD get_process_id(const wchar_t* process_name);

void UnloadDriver(HANDLE driver);
void* AllocatePool(PMemoryPool pool, const size_t size, const char* RegionName);
void* ReadFromPool(PMemoryPool pool, const int offset, const int itemPlacement);
void* FindByPageName(PMemoryPool pool, const char* regionName, const int offset);
int GetFreeMemory(PMemoryPool pool, int offset);
int GetItemsCount(PMemoryPool pool, const int offset);
void ResetPool(PMemoryPool pool);
void moveMouse(int dx, int dy);
void getCursorPosition(int* x, int* y);

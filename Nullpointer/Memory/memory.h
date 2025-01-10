#pragma once
#include "../dependencies.h"
#include "../NullPointer.h"

typedef struct {
	char memory[1024];
	char* name;
	size_t offset;
}  MemoryPool, * PMemoryPool;

typedef boolean(*MemoryReadFunction)(HANDLE driver, void* address, void* buffer, size_t size);
typedef boolean(*MemoryWriteFunction)(HANDLE driver, void* address, void* val, size_t size);

extern MemoryReadFunction ReadMemory;
extern MemoryWriteFunction WriteMemory;

HANDLE loadDriver();

boolean attach_to_process(PHANDLE DriverHandle, const DWORD pid);

static uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name);
uintptr_t initClient(PHANDLE driver);

DWORD get_process_id(const wchar_t* process_name);

void* AllocatePool(PMemoryPool pool, const size_t size, const char* RegionName);
void* ReadFromPool(PMemoryPool pool, const int offset, const int itemPlacement);
void* FindByPageName(PMemoryPool pool, const char* regionName, const int offset);

int GetFreeMemory(PMemoryPool pool, int offset);
int GetItemsCount(PMemoryPool pool, const int offset);

void InitializeMemoryReader();
void UnloadDriver(HANDLE driver);
void ResetPool(PMemoryPool pool);

void MoveMouse(int dx, int dy);

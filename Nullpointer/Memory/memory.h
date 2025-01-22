#pragma once
#include "DRIVER/driverFunctions.h"
#include "WINAPI/winapi.h"

typedef struct {
	char memory[1024];
	char* name;
	size_t offset;
}  MemoryPool, * PMemoryPool;

extern MemoryReadFunction ReadMemory;
extern MemoryWriteFunction WriteMemory;

void* AllocatePool(PMemoryPool pool, const size_t size, const char* RegionName);
void* ReadFromPool(PMemoryPool pool, const int offset, const int itemPlacement);
void* FindByPageName(PMemoryPool pool, const char* regionName, const int offset);

int GetFreeMemory(PMemoryPool pool, int offset);
int GetItemsCount(PMemoryPool pool, const int offset);
uintptr_t initClient(PHANDLE driver);

void InitializeMemoryReader();
void ResetPool(PMemoryPool pool);

void MoveMouse(int dx, int dy);

#include "memory.h"

MemoryReadFunction ReadMemory = NULL;
MemoryWriteFunction WriteMemory = NULL;

void InitializeMemoryReader() {

	if (GetDriverHandle() == NULL) {
		puts("[!] Using WINAPI [!]\n");

		ReadMemory = ReadMemoryWINAPI;
		WriteMemory = WriteMemoryWINAPI;
	}

	else {
		puts("[!] Using the driver [!]\n");
		ReadMemory = ReadMemoryDriver;
		WriteMemory = WriteMemoryDriver;
	}
}

uintptr_t initClient(PHANDLE driver) {

	puts("[!] Getting cs2 process... [!]\n");

	while (GetPIDField() == 0) {
		SetPIDField(get_process_id(L"cs2.exe"));
		Sleep(1000);
	}

	puts("[!] Getting client.dll [!]\n");

	uintptr_t client = 0;

	while (client == 0) {
		client = get_module_base(L"client.dll");
		Sleep(1000);
	}

	printf("Module base: %p\n", client);

	if (driver != NULL) {
		if (attach_to_process(GetPIDField()))
			puts("[+] Attachment successful [+]\n");
	}

	return client;
}

void _SetPageName(PMemoryPool region, const char* regionName) {
	region->name = regionName;
}

int GetFreeMemory(PMemoryPool pool, int offset) {

	if (pool->offset == 0)
		return 1024 - offset;

	return 1024 - pool->offset - offset;
}

void* AllocatePool(PMemoryPool pool, const size_t size, const char* regionName) {

	if (pool->offset + size > 1024)
		return NULL;

	void* ptr = &pool->memory[pool->offset];
	_SetPageName((PMemoryPool)ptr, regionName);

	pool->offset += size;
	
	return ptr;
}

int GetItemsCount(PMemoryPool pool, const int offset) {
	return ((1024 - GetFreeMemory(pool, offset)) / offset) - 1;
}

void* FindByPageName(PMemoryPool pool, const char* regionName, const int offset) {
	int totalItems = (1024 - offset) / offset;
	

	for (int page = 0; page < totalItems; page++) {
		PMemoryPool currentPage = (PMemoryPool) ReadFromPool(pool, offset, page);

		if (page == 0)
			currentPage = (PMemoryPool)ReadFromPool(pool, 0, 0);

		if (strcmp(regionName, currentPage->name) == 0) 
			return currentPage;
		
	}

	return NULL;
}

void* ReadFromPool(PMemoryPool pool, const int offset, const int itemPlacement) {
	return &pool->memory[offset * itemPlacement];
}

void ResetPool(PMemoryPool pool) {
	pool->offset = 0;
}

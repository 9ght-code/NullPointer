#include "memory.h"


static uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name) {
	uintptr_t module_base = 0;
	MODULEENTRY32 pe;
	boolean hResult;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return module_base;

	pe.dwSize = sizeof(MODULEENTRY32);
	hResult = Module32First(snap_shot, &pe);

	while (hResult) {
		if (wcsstr(module_name, pe.szModule) != NULL) {
			module_base = (uintptr_t)pe.modBaseAddr;
			break;
		}

		hResult = Module32Next(snap_shot, &pe);
	}

	CloseHandle(snap_shot);

	return module_base;
}

// Функция для перемещения мыши на дельта-вектор
void moveMouse(int dx, int dy) {
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));
}

// Функция для получения текущей позиции мыши
void getCursorPosition(int* x, int* y) {
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	*x = cursorPos.x;
	*y = cursorPos.y;
}


DWORD get_process_id(const wchar_t* process_name) {
	DWORD process_id = 0;
	PROCESSENTRY32 pe;
	boolean hResult;
	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snap_shot == INVALID_HANDLE_VALUE) {
		puts("INVALID SNAPSHOT");
		return process_id;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);
	hResult = Process32First(snap_shot, &pe);

	while (hResult) {
		if (_wcsicmp(process_name, pe.szExeFile) == 0) {
			process_id = pe.th32ProcessID;

			break;
		}

		hResult = Process32Next(snap_shot, &pe);
	}

	CloseHandle(snap_shot);

	return process_id;

}

boolean ReadMemory(HANDLE hDevice, uintptr_t address, PVOID buffer, SIZE_T size) {
	Request shared_request;
	BOOL status;

	shared_request.target = (PVOID)address;
	shared_request.buffer = buffer;
	shared_request.size = size;

	status = DeviceIoControl(
		hDevice,
		IO_READ,
		&shared_request,
		sizeof(shared_request),
		&shared_request,
		sizeof(shared_request),
		NULL,
		NULL
	);

	if (!status)
		return FALSE;


	return TRUE;
}

boolean WriteMemory(HANDLE hDevice, uintptr_t address, int value, SIZE_T size) {
	Request r;

	r.target = (PVOID)address;
	r.buffer = (PVOID)&value;
	r.size = size;

	BOOL status = DeviceIoControl(hDevice, IO_WRITE, &r, sizeof(r), &r, sizeof(r), NULL, NULL);

	if (!status)
		return FALSE;

	return TRUE;
}

boolean attach_to_process(PHANDLE DriverHandle, const DWORD pid) {
	Request req;
	req.proccess_id = (HANDLE)pid;

	return DeviceIoControl(*DriverHandle, IO_ATTACH, &req, sizeof(Request), &req, sizeof(Request), NULL, NULL);
}


void UnloadDriver(HANDLE driver) {
	DeviceIoControl(driver, IO_UNLOAD, NULL, NULL, NULL, NULL, NULL, NULL);
}


HANDLE loadDriver() {
	const HANDLE driver = CreateFile(L"\\\\.\\SecondMouse", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (driver == INVALID_HANDLE_VALUE) {
		puts("[-] Failed to load the driver [-]\n");

		return 0;
	}

	puts("[+] Driver Loaded [+]\n");

	return driver;
}


uintptr_t initClient(PHANDLE driver) {

	const DWORD pid = get_process_id(L"cs2.exe");

	if (pid == 0) {
		puts("[-] Failed to find the proccess [-]\n");

		return 0;
	}

	uintptr_t client = get_module_base(pid, L"client.dll");

	if (client == 0) {
		puts("[-] Failed to find the module base [-]\n");

		return 0;
	}

	printf("Module base: %p\n", client);

	if (attach_to_process(driver, pid))
		puts("[+] Attachment successful [+]\n");

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

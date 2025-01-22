#include "winapi.h"

static DWORD pid = 0;
MemoryReadFunction WINAPIReadFunc = NULL;
MemoryWriteFunction WINAPIWriteFunc = NULL;

uintptr_t get_module_base(const wchar_t* module_name) {
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

	pid = process_id;

	return process_id;

}

boolean ReadMemoryWINAPI(HANDLE driver, uintptr_t address, PVOID buffer, size_t size) {
	HANDLE openedProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);

	if (!openedProcess)
		return FALSE;

	boolean result = ReadProcessMemory(openedProcess, address, buffer, size, NULL);
	CloseHandle(openedProcess);

	return result;
}

boolean WriteMemoryWINAPI(HANDLE driver, uintptr_t address, void* val, size_t size) {
	HANDLE openedProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);

	if (!openedProcess) {
		printf("couldn't open da process \n");
		return FALSE;
	}

	boolean result = WriteProcessMemory(openedProcess, address, &val, size, NULL);
	CloseHandle(openedProcess);

	return result;
}


DWORD GetPIDField() {
	return pid;
}

void SetPIDField(DWORD value) {
	pid = value;
}

#include "driverFunctions.h"

static HANDLE hDevice = NULL;

boolean attach_to_process(const DWORD pid) {
	Request req;
	req.proccess_id = (HANDLE)pid;

	return DeviceIoControl(hDevice, IO_ATTACH, &req, sizeof(Request), &req, sizeof(Request), NULL, NULL);
}

boolean ReadMemoryDriver(HANDLE driver, uintptr_t address, PVOID buffer, size_t size) {
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

boolean WriteMemoryDriver(HANDLE driver, uintptr_t address, void* value, SIZE_T size) {
	Request r;

	r.target = (PVOID)address;
	r.buffer = (PVOID)&value;
	r.size = size;

	BOOL status = DeviceIoControl(hDevice, IO_WRITE, &r, sizeof(r), &r, sizeof(r), NULL, NULL);

	if (!status)
		return FALSE;

	return TRUE;
}

void UnloadDriver() {
	DeviceIoControl(hDevice, IO_UNLOAD, NULL, NULL, NULL, NULL, NULL, NULL);
}

HANDLE loadDriver() {
	const HANDLE driver = CreateFile(L"\\\\.\\SecondMouse", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (driver == INVALID_HANDLE_VALUE) {
		puts("[-] Failed to load the driver [-]\n");

		return 0;
	}

	puts("[+] Driver Loaded [+]\n");

	hDevice = driver;

	return driver;
}


PHANDLE GetDriverHandle() {
	return &hDevice;
}
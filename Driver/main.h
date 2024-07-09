
#ifndef MAIN_H
#define MAIN_H

#define IO_ATTACH CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

struct Request {
	NTSTATUS status;
	PVOID BaseAdress;
	HANDLE proccess_id;

	PVOID target;
	PVOID buffer;

	SIZE_T size;
	SIZE_T return_size;
};


extern struct Request shared_request;

#endif
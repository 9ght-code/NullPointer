#pragma once

typedef ULONGLONG QWORD;

typedef struct _Request {
	PVOID BaseAdress;
	HANDLE proccess_id;

	PVOID target;
	PVOID buffer;

	SIZE_T size;
	SIZE_T return_size;
}  Request, * PRequest;

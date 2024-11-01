#pragma once
typedef void* PVOID;
typedef void* HANDLE;
typedef unsigned __int64 SIZE_T;

typedef struct _Request {
	PVOID BaseAdress;
	HANDLE proccess_id;

	PVOID target;
	PVOID buffer;

	SIZE_T size;
	SIZE_T return_size;

}  Request, * PRequest;
#pragma once
#include "../NullPointer.h"

typedef boolean(*MemoryReadFunction)(HANDLE driver, void* address, void* buffer, size_t size);
typedef boolean(*MemoryWriteFunction)(HANDLE driver, void* address, void* val, size_t size);
#include "MemoryReadWriter.h"

MemoryReadWriter::MemoryReadWriter(void* baseHandle) {
	local_handle = CreateLocalHandle(baseHandle);
}

MemoryReadWriter::~MemoryReadWriter() {
	close();
}

void MemoryReadWriter::close() {
	if(local_handle) {
		CloseLocalHandle(local_handle);
		local_handle = nullptr;
	}
}

MemoryReadWriter* MemoryReadWriter::read(void* address, void* v, DWORD size) {
	GetProcessMemory(local_handle, address, (char*)v, size);
	return this;
}

MemoryReadWriter* MemoryReadWriter::write(void* address, void* v, DWORD size) {
	PutProcessMemory(local_handle, address, (char*)v, size);
	return this;
}
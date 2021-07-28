#pragma once
#include "export.h"

class MemoryReadWriter {
public:
	MemoryReadWriter(void* baseHandle);
	~MemoryReadWriter();
	template<typename T>
	T read(void* address) {
		T buffer;
		GetProcessMemory(local_handle, address, (char *)&buffer, sizeof(T));
		return buffer;
	}
	template<typename T>
	MemoryReadWriter* write(void* address, T* v) {
		PutProcessMemory(local_handle, address, (char*)v, sizeof(T));
		return this;
	}
	MemoryReadWriter* read(void* address, void* v, DWORD size);
	MemoryReadWriter* write(void* address, void* v, DWORD size);
	// ÊÖ¶¯Ïú»Ù¾ä±ú
	void close();
private:
	void* local_handle;
};
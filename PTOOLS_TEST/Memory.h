#pragma once
#include "export.h"
#include <string_view>
#include "MemoryReadWriter.h"
using namespace std;
class Memory {
public:
	Memory(DWORD pid);
	~Memory();

	template<typename T>
	T read(void* address) {
		T buffer = new T;
		GetProcessMemory(base_handle, address, buffer, sizeof(T));
		return buffer;
	}
	template<typename T>
	Memory* write(void* address, T * v) {
		PutProcessMemory(base_handle, address, v, sizeof(T));
		return this;
	}
	Memory* read(void* address, void* v, DWORD size);
	Memory* write(void* address, void* v, DWORD size);
	
	void* getModuleHandle(string_view moduleName) const;

	void* alloc(DWORD size, BOOL execute) const;
	Memory* free(void* address);
	MemoryReadWriter* buildReadWriter() const;
	
	// ÊÖ¶¯Ïú»Ù¾ä±ú
	void close();
private:
	void* base_handle;
};
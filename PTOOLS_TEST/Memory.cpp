#include "Memory.h"

Memory::Memory(DWORD pid) {
	base_handle = LoadProcess(pid);
}

Memory::~Memory() {
	close();
}

Memory* Memory::read(void* address, void* v, DWORD size) {
	GetProcessMemory(base_handle, address, (char *)v, size);
	return this;
}

Memory* Memory::write(void* address, void* v, DWORD size) {
	PutProcessMemory(base_handle, address, (char *)v, size);
	return this;
}

void* Memory::getModuleHandle(string_view moduleName) const {
	_PTR moduleBase = 0;
	// 获得进程内模块基址
	GetModuleBase(base_handle, moduleName.data(), &moduleBase);
	return (void *)moduleBase;
}

void* Memory::alloc(DWORD size, BOOL execute) const {
	_PTR buffer = 0;
	AllocProcessMemory(base_handle, size, execute, &buffer);
	return (void*)buffer;
}

Memory* Memory::free(void* address) {
	FreeProcessMemory(base_handle, address);
	return this;
}

void Memory::close()  {
	if (base_handle) {
		UnloadProcess(base_handle);
		base_handle = nullptr;
	}
}

MemoryReadWriter* Memory::buildReadWriter() const {
	if (!base_handle)
		return nullptr;
	return new MemoryReadWriter(base_handle);
}
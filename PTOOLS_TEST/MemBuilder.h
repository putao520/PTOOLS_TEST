#pragma once
#include "export.h"
#include "Memory.h"

class MemBuilder {
public:
	MemBuilder connect(const char * ip, unsigned int port, unsigned int ver, unsigned int appID);
	MemBuilder verify(const char * id, const char* pw);
	Memory* build(DWORD pid) const;
	int getLastError() const;
	BOOL isSuccess() const;
private:
	int last_error_code = 0;
};
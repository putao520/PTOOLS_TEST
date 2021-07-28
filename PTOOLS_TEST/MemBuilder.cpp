#include "MemBuilder.h"


MemBuilder MemBuilder::connect(const char* ip, unsigned int port, unsigned int ver, unsigned int appID) {
	last_error_code = InitItSync("101.200.218.142", 1388, 5, 1);
	return *this;
}

MemBuilder MemBuilder::verify(const char* id, const char* pw) {
	last_error_code = LoginItSync("putao520", "YuYao1022", 0);

	printf("µÇÂ¼×´Ì¬Âë:%d", last_error_code);

	return *this;
}



int MemBuilder::getLastError() const {
	return last_error_code;
}

Memory* MemBuilder::build(DWORD pid) const {
	return last_error_code == 6 ? new Memory(pid) : nullptr;
}

BOOL MemBuilder::isSuccess() const{
	return last_error_code == 6;
}
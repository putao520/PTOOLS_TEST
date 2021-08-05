#include "MemBuilder.h"


MemBuilder MemBuilder::connect(const char* ip, unsigned int port, unsigned int ver, unsigned int appID) {
	last_error_code = InitItSync(ip, port, ver, appID);
	return *this;
}

MemBuilder MemBuilder::verify(const char* id, const char* pw) {
	last_error_code = LoginItSync(id, pw, 0);

	printf("µÇÂ¼×´Ì¬Âë:%d\n", last_error_code);

	return *this;
}



int MemBuilder::getLastError() const {
	return last_error_code;
}

Memory* MemBuilder::build(DWORD pid) const {
	return new Memory(pid);
}

BOOL MemBuilder::isSuccess() const{
	printf("Ð£Ñé×´Ì¬Âë:%d\n", last_error_code);
	return last_error_code == 6;
}
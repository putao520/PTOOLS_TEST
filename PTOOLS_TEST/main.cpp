#include <cstdlib>

#include "stdio.h"
#include "export.h"
#include <iostream>
#pragma comment(lib,"./PTOOLS.lib")

using namespace std;

typedef void (WINAPI* def_change_step)(ULONG step);
void recv_status(ULONG step) {
	printf("状态:%d\n", step);
	if (step == 6) {// 成功
		void* handle = LoadProcess(::GetCurrentProcessId());
		if( !handle )
			return;
		Sleep(500);
		cout << "Create Handle:" << handle << endl;
		_PTR moduleBase = 0;
		GetModuleBase(handle, "ntdll.dll", &moduleBase);
		printf("Ntdll.dll:%#llx\n", moduleBase);
		_PTR buffer = 0;
		AllocProcessMemory(handle, 64, true, &buffer);
		printf("Buffer:%#llx\n", buffer);
		char test[] = "Test!";
		PutProcessMemory(handle, (void*)buffer, test, 6);
		printf("Write from:%s to:%#llx\n", test, buffer);
		char recv[64] = { 0 };
		GetProcessMemory(handle, (void*)buffer, recv, 6);
		printf("Read  from:%s to:%s\n", (char *)buffer, recv);
		FreeProcessMemory(handle, (void*)buffer);
		cout << "Free buffer:" << buffer << endl;

		// 创建物理内存读写句柄
		HANDLE local_handle = CreateLocalHandle(handle);
		cout << "Create Local Handle:" << local_handle << endl;
		// 新申请一个内存用来测试
		buffer = 0;
		AllocProcessMemory(handle, 64, true, &buffer);
		
		UnloadProcess(handle);
		cout << "Close Handle:" << handle << endl;
		
		PutProcessMemory(local_handle, (void*)buffer, test, 6);
		printf("[py]Write from:%s to:%#llx\n", test, buffer);
		memset(recv, 0, 64);
		GetProcessMemory(local_handle, (void*)buffer, recv, 6);
		printf("[py]Read  from:%s to:%s\n", (char*)buffer, recv);
		CloseLocalHandle(local_handle);
		cout << "Close Local Handle:" << local_handle << endl;
	}
}

int main() {
	// 注册状态监控回调
	OnProcessChange(recv_status);
	// 连接服务器
	int r = InitIt("101.200.218.142", 1388, 5, 1);
	// 登录账号
	LoginIt("putao520", "YuYao1022", 0);
	
	system("pause");
	return 0;
}
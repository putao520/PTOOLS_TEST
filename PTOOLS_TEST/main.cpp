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
		// 打开进程获得进程句柄
		void* handle = LoadProcess(::GetCurrentProcessId());
		if( !handle )
			return;
		Sleep(500);
		cout << "Create Handle:" << handle << endl;
		_PTR moduleBase = 0;
		// 获得进程内模块基址
		GetModuleBase(handle, "ntdll.dll", &moduleBase);
		printf("Ntdll.dll:%#llx\n", moduleBase);
		_PTR buffer = 0;
		// 申请进程内存
		AllocProcessMemory(handle, 64, true, &buffer);
		printf("Buffer:%#llx\n", buffer);
		char test[] = "Test!";
		// 写入进程内存
		PutProcessMemory(handle, (void*)buffer, test, 6);
		printf("Write from:%s to:%#llx\n", test, buffer);
		char recv[64] = { 0 };
		// 读取内存
		GetProcessMemory(handle, (void*)buffer, recv, 6);
		printf("Read  from:%s to:%s\n", (char *)buffer, recv);
		// 释放内存
		FreeProcessMemory(handle, (void*)buffer);
		cout << "Free buffer:" << buffer << endl;
		// 创建物理内存专用读写句柄
		HANDLE local_handle = CreateLocalHandle(handle);
		cout << "Create Local Handle:" << local_handle << endl;
		// 新申请一个内存用来测试
		buffer = 0;
		AllocProcessMemory(handle, 64, true, &buffer);
		// 关闭基础句柄
		UnloadProcess(handle);
		cout << "Close Handle:" << handle << endl;
		// 使用专用读写句柄写内存
		PutProcessMemory(local_handle, (void*)buffer, test, 6);
		printf("[py]Write from:%s to:%#llx\n", test, buffer);
		memset(recv, 0, 64);
		// 使用专用读写句柄读内存
		GetProcessMemory(local_handle, (void*)buffer, recv, 6);
		printf("[py]Read  from:%s to:%s\n", (char*)buffer, recv);
		// 关闭专用读写句柄
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

	cout << "next!" << endl;
	system("pause");
	return 0;
}
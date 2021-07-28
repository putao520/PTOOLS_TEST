#include <cstdlib>
#include "stdio.h"
#include "export.h"
// C++ need
#include "MemBuilder.h"
#include "Memory.h"
#include "MemoryReadWriter.h"

#include <iostream>
#pragma comment(lib,"./PTOOLS.lib")
typedef void (WINAPI* def_change_step)(ULONG step);

using namespace std;
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

/*
 * 载入使用 C 风格api 接口,并基于异步接口
 */
void InitByAsyncAndApi() {
	// 注册状态监控回调
	OnProcessChange(recv_status);
	// 连接服务器
	int r = InitIt("101.200.218.142", 1388, 5, 1);
	// 登录账号
	LoginIt("putao520", "YuYao1022", 0);

	cout << "next!" << endl;
	system("pause");
}

/*
 * 载入使用 C++ 风格接口，并基于同步接口 
 */
void InitBySyncAndCpp() {
	MemBuilder builder;
	Memory* memory = builder.connect("101.200.218.142", 1388, 5, 1)
		.verify("putao520", "YuYao1022").build(GetCurrentProcessId());
	if( !memory ) {
		printf("生成Memory对象失败 ->错误码:%d\n", builder.getLastError());
		return;
	}
	// 申请内存测试
	char* szText = (char *)memory->alloc(64, false);
	if( !szText ) {
		printf("申请内存地址失败！\n");
		return;
	}
	printf("申请内存地址:%#llx\n", szText);
	
	// 写测试
	char test[] = "Test!";
	memory->write(szText, test, 6);
	printf("Write from:%s to:%s\n", test, szText);

	// 读测试
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	memory->read(szText, szTEXT, 6);
	printf("Read from:%s to:%s\n", szText, szTEXT);

	// 获得模块测试
	string moduleName = "ntdll.dll";
	void* moduleBase = memory->getModuleHandle(moduleName.c_str());
	printf("模块:%s ->Load:%#llx\n", moduleName.c_str(), moduleBase);

	printf("\n=====================================\n");
	
	// 获得读写专用对象
	MemoryReadWriter* mwr = memory->buildReadWriter();
	// 释放Memory
	delete memory;
	
	// 测试专用读
	UINT32* p = (UINT32 *)szTEXT;
	*p = 0x11223344;
	UINT32 v = mwr->read<UINT32>(p);
	printf("[专用读]地址:%#llx ->值:%x\n", szTEXT, v);

	// 测试专用写
	UINT32 l = 0x10000;
	mwr->write<UINT32>(p, (UINT32 *)&l);
	printf("[专用写]地址:%#llx ->值:%d\n", &l, *p);
	// 释放专用对象
	delete mwr;
}

Memory* InitByLogin() {
	printf("测试登录块\n");
	MemBuilder builder;
	
	Memory* memory = builder
	.connect("101.200.218.142", 1388, 5, 1)
	.verify("putao520", "YuYao1022")
	.build(GetCurrentProcessId());
	
	if (!memory) {
		printf("生成Memory对象失败 ->错误码:%d\n", builder.getLastError());
		return nullptr;
	}
	
	printf("====================================\n");
	return memory;
}

MemoryReadWriter* InitByMemory(Memory* memory) {
	printf("测试内存块\n");
	
	// 申请内存测试
	char* szText = (char*)memory->alloc(64, false);
	if (!szText) {
		printf("申请内存地址失败！\n");
		return nullptr;
	}
	printf("申请内存地址:%#llx\n", szText);

	// 写测试
	char test[] = "Test!";
	memory->write(szText, test, 6);
	printf("Write from:%s to:%s\n", test, szText);

	// 读测试
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	memory->read(szText, szTEXT, 6);
	printf("Read from:%s to:%s\n", szText, szTEXT);

	// 获得模块测试
	string moduleName = "ntdll.dll";
	void* moduleBase = memory->getModuleHandle(moduleName.c_str());
	printf("模块:%s ->Load:%#llx\n", moduleName.c_str(), moduleBase);
	
	// 获得读写专用对象
	MemoryReadWriter* mwr = memory->buildReadWriter();
	
	// 释放Memory
	delete memory;
	printf("\n=====================================\n");
	return mwr;
}

void InitByLocalMemory(MemoryReadWriter* mwr) {
	printf("测试专用内存块\n");
	
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	
	// 测试专用读
	UINT32* p = (UINT32*)szTEXT;
	*p = 0x11223344;
	UINT32 v = mwr->read<UINT32>(p);
	printf("[专用读]地址:%#llx ->值:%x\n", szTEXT, v);

	// 测试专用写
	UINT32 l = 0x10000;
	mwr->write<UINT32>(p, (UINT32*)&l);
	printf("[专用写]地址:%#llx ->值:%d\n", &l, *p);
	
	// 释放专用对象
	delete mwr;
	printf("\n=====================================\n");
}

int main() {
	Memory* memory = InitByLogin();
	if (memory) {
		MemoryReadWriter* mwr = InitByMemory(memory);
		if( mwr ) {
			InitByLocalMemory(mwr);
		}
	}
	
	system("pause");
	return 0;
}
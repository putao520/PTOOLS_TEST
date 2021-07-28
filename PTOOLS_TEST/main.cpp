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
	printf("״̬:%d\n", step);
	if (step == 6) {// �ɹ�
		// �򿪽��̻�ý��̾��
		void* handle = LoadProcess(::GetCurrentProcessId());
		if( !handle )
			return;
		Sleep(500);
		cout << "Create Handle:" << handle << endl;
		_PTR moduleBase = 0;
		// ��ý�����ģ���ַ
		GetModuleBase(handle, "ntdll.dll", &moduleBase);
		printf("Ntdll.dll:%#llx\n", moduleBase);
		_PTR buffer = 0;
		// ��������ڴ�
		AllocProcessMemory(handle, 64, true, &buffer);
		printf("Buffer:%#llx\n", buffer);
		char test[] = "Test!";
		// д������ڴ�
		PutProcessMemory(handle, (void*)buffer, test, 6);
		printf("Write from:%s to:%#llx\n", test, buffer);
		char recv[64] = { 0 };
		// ��ȡ�ڴ�
		GetProcessMemory(handle, (void*)buffer, recv, 6);
		printf("Read  from:%s to:%s\n", (char *)buffer, recv);
		// �ͷ��ڴ�
		FreeProcessMemory(handle, (void*)buffer);
		cout << "Free buffer:" << buffer << endl;
		// ���������ڴ�ר�ö�д���
		HANDLE local_handle = CreateLocalHandle(handle);
		cout << "Create Local Handle:" << local_handle << endl;
		// ������һ���ڴ���������
		buffer = 0;
		AllocProcessMemory(handle, 64, true, &buffer);
		// �رջ������
		UnloadProcess(handle);
		cout << "Close Handle:" << handle << endl;
		// ʹ��ר�ö�д���д�ڴ�
		PutProcessMemory(local_handle, (void*)buffer, test, 6);
		printf("[py]Write from:%s to:%#llx\n", test, buffer);
		memset(recv, 0, 64);
		// ʹ��ר�ö�д������ڴ�
		GetProcessMemory(local_handle, (void*)buffer, recv, 6);
		printf("[py]Read  from:%s to:%s\n", (char*)buffer, recv);
		// �ر�ר�ö�д���
		CloseLocalHandle(local_handle);
		cout << "Close Local Handle:" << local_handle << endl;
	}
}

/*
 * ����ʹ�� C ���api �ӿ�,�������첽�ӿ�
 */
void InitByAsyncAndApi() {
	// ע��״̬��ػص�
	OnProcessChange(recv_status);
	// ���ӷ�����
	int r = InitIt("101.200.218.142", 1388, 5, 1);
	// ��¼�˺�
	LoginIt("putao520", "YuYao1022", 0);

	cout << "next!" << endl;
	system("pause");
}

/*
 * ����ʹ�� C++ ���ӿڣ�������ͬ���ӿ� 
 */
void InitBySyncAndCpp() {
	MemBuilder builder;
	Memory* memory = builder.connect("101.200.218.142", 1388, 5, 1)
		.verify("putao520", "YuYao1022").build(GetCurrentProcessId());
	if( !memory ) {
		printf("����Memory����ʧ�� ->������:%d\n", builder.getLastError());
		return;
	}
	// �����ڴ����
	char* szText = (char *)memory->alloc(64, false);
	if( !szText ) {
		printf("�����ڴ��ַʧ�ܣ�\n");
		return;
	}
	printf("�����ڴ��ַ:%#llx\n", szText);
	
	// д����
	char test[] = "Test!";
	memory->write(szText, test, 6);
	printf("Write from:%s to:%s\n", test, szText);

	// ������
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	memory->read(szText, szTEXT, 6);
	printf("Read from:%s to:%s\n", szText, szTEXT);

	// ���ģ�����
	string moduleName = "ntdll.dll";
	void* moduleBase = memory->getModuleHandle(moduleName.c_str());
	printf("ģ��:%s ->Load:%#llx\n", moduleName.c_str(), moduleBase);

	printf("\n=====================================\n");
	
	// ��ö�дר�ö���
	MemoryReadWriter* mwr = memory->buildReadWriter();
	// �ͷ�Memory
	delete memory;
	
	// ����ר�ö�
	UINT32* p = (UINT32 *)szTEXT;
	*p = 0x11223344;
	UINT32 v = mwr->read<UINT32>(p);
	printf("[ר�ö�]��ַ:%#llx ->ֵ:%x\n", szTEXT, v);

	// ����ר��д
	UINT32 l = 0x10000;
	mwr->write<UINT32>(p, (UINT32 *)&l);
	printf("[ר��д]��ַ:%#llx ->ֵ:%d\n", &l, *p);
	// �ͷ�ר�ö���
	delete mwr;
}

Memory* InitByLogin() {
	printf("���Ե�¼��\n");
	MemBuilder builder;
	
	Memory* memory = builder
	.connect("101.200.218.142", 1388, 5, 1)
	.verify("putao520", "YuYao1022")
	.build(GetCurrentProcessId());
	
	if (!memory) {
		printf("����Memory����ʧ�� ->������:%d\n", builder.getLastError());
		return nullptr;
	}
	
	printf("====================================\n");
	return memory;
}

MemoryReadWriter* InitByMemory(Memory* memory) {
	printf("�����ڴ��\n");
	
	// �����ڴ����
	char* szText = (char*)memory->alloc(64, false);
	if (!szText) {
		printf("�����ڴ��ַʧ�ܣ�\n");
		return nullptr;
	}
	printf("�����ڴ��ַ:%#llx\n", szText);

	// д����
	char test[] = "Test!";
	memory->write(szText, test, 6);
	printf("Write from:%s to:%s\n", test, szText);

	// ������
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	memory->read(szText, szTEXT, 6);
	printf("Read from:%s to:%s\n", szText, szTEXT);

	// ���ģ�����
	string moduleName = "ntdll.dll";
	void* moduleBase = memory->getModuleHandle(moduleName.c_str());
	printf("ģ��:%s ->Load:%#llx\n", moduleName.c_str(), moduleBase);
	
	// ��ö�дר�ö���
	MemoryReadWriter* mwr = memory->buildReadWriter();
	
	// �ͷ�Memory
	delete memory;
	printf("\n=====================================\n");
	return mwr;
}

void InitByLocalMemory(MemoryReadWriter* mwr) {
	printf("����ר���ڴ��\n");
	
	char szTEXT[MAX_PATH];
	memset(szTEXT, 0, MAX_PATH);
	
	// ����ר�ö�
	UINT32* p = (UINT32*)szTEXT;
	*p = 0x11223344;
	UINT32 v = mwr->read<UINT32>(p);
	printf("[ר�ö�]��ַ:%#llx ->ֵ:%x\n", szTEXT, v);

	// ����ר��д
	UINT32 l = 0x10000;
	mwr->write<UINT32>(p, (UINT32*)&l);
	printf("[ר��д]��ַ:%#llx ->ֵ:%d\n", &l, *p);
	
	// �ͷ�ר�ö���
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
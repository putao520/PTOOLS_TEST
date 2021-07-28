#include <cstdlib>

#include "stdio.h"
#include "export.h"
#include <iostream>
#pragma comment(lib,"./PTOOLS.lib")

using namespace std;

typedef void (WINAPI* def_change_step)(ULONG step);
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

int main() {
	// ע��״̬��ػص�
	OnProcessChange(recv_status);
	// ���ӷ�����
	int r = InitIt("101.200.218.142", 1388, 5, 1);
	// ��¼�˺�
	LoginIt("putao520", "YuYao1022", 0);

	cout << "next!" << endl;
	system("pause");
	return 0;
}
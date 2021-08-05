#include "TestApp.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

void watch_test() {
	DWORD dwPid = 0;
	HWND hwnd = nullptr;
	try {
		do {
			hwnd = FindWindow("Notepad", nullptr);
			
			if (hwnd) {
				GetWindowThreadProcessId(hwnd, &dwPid);
			}
			printf("Wait NotePad...\n");
			Sleep(1000);
		} while (!dwPid);
		printf("Find Notepad:%d\n", dwPid);
	
		void* base_handle = LoadProcess(dwPid);
		printf("Notepad Handle:%#llx\n", base_handle);
		if (base_handle)
			UnloadProcess(base_handle);
		Sleep(10 * 1000);
		UnloadIt();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
}

void iSleep() {
	Sleep(100 * 1000);
}

void wait_test(ULONG step) {
	if( step == 6 ) {
		watch_test();
	}
	printf("status:%d\n", step);
}

DWORD wait_window(const char * name) {
	HWND handle = NULL;
	do {
		handle = FindWindow(name, NULL);
		Sleep(500);
	} while (!handle);
	DWORD pid = 0;
	GetWindowThreadProcessId(handle, &pid);
	return pid;
}
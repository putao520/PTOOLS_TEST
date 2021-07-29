#include "TestApp.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

void wait_test(ULONG step) {
	if( step == 6 ) {
		DWORD dwPid = 0;
		HWND hwnd = nullptr;
		do {
			hwnd = FindWindow("Notepad", nullptr);
			if (hwnd) {
				GetWindowThreadProcessId(hwnd, &dwPid);
			}
			printf("Wait NotePad...\n");
			Sleep(1000);
		} while (!dwPid);
		printf("Find Notepad:%d\n", dwPid);
		UnloadIt();
		void* base_handle = LoadProcess(dwPid);
		printf("Notepad Handle:%#llx\n", base_handle);
		Sleep(10 * 1000);
		if(base_handle)
			UnloadProcess(base_handle);
	}
}

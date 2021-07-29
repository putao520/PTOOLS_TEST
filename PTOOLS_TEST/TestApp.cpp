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

void wait_test(ULONG step) {
	if( step == 6 ) {
		// watch_test();
	}
	printf("status:%d\n", step);
}

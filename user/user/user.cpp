#include <iostream>
#include <Windows.h>

typedef unsigned __int64 DWORD64, * PDWORD64;
struct _io
{
	int flag;
	int pid;
	DWORD64 address;
	DWORD64 buffer;
};

void call(_io i)
{
	unsigned long data = (unsigned long)&i;
	__asm {
		mov Eax, data;
		bound di, DWORD PTR[ebp];
	}
}

int main()
{
	_io io = { 1,GetCurrentProcessId(),0x123456,0x123456 };
	call(io);
	getchar();
	return 0;
}

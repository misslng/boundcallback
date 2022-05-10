#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>

PVOID callback_pvoid;
void Unload()
{
	KeDeregisterBoundCallback(callback_pvoid);
}
struct _io
{
	int flag;
	int pid;
	DWORD64 address;
	DWORD64 buffer;
};
BOUND_CALLBACK_STATUS BoundCallback(VOID)
{
	PKTHREAD thread = KeGetCurrentThread();
	PKTRAP_FRAME trap_frame = (PKTRAP_FRAME) * (DWORD64*)((char*)thread + 0x90);
	_io i = *(_io*)trap_frame->Rax;
	DbgPrintEx(0, 0, "pid: %d\n", i.pid);
	//restore rip
	trap_frame->Rip += 4;
	return BoundExceptionHandled;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT obejct, PUNICODE_STRING reg)
{
	UNREFERENCED_PARAMETER(reg);
	obejct->DriverUnload = (PDRIVER_UNLOAD)Unload;

	callback_pvoid = KeRegisterBoundCallback(BoundCallback);
	if (callback_pvoid) DbgPrintEx(0, 0, "success");

	return STATUS_SUCCESS;
}
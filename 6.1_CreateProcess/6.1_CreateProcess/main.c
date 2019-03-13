#include<windows.h>
#include<tchar.h>
#include<stdio.h>

DWORD CreateChildProcess(LPTSTR szChildProcessCmd)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess(NULL,
		szChildProcessCmd,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}

int main(void)
{
	TCHAR szCmd[] = _T("../Debug/Child.exe abc 123");
	CreateChildProcess(szCmd);
}
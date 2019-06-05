#include<Windows.h>
#include<stdio.h>
#include<tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	TCHAR szTempCmdLine[MAX_PATH];

	if (argc < 2)
	{
		_tprintf(_T("Usage: %s <sample.exe>\n"), argv[0]);
		return -1;
	}

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);

	_tcscpy(szTempCmdLine, argv[1]);

	BOOL r = CreateProcess(NULL, szTempCmdLine, NULL, NULL, FALSE,
		CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS | DEBUG_PROCESS,
		NULL, NULL, &si, &pi);
	if (!r)
	{
		return -1;
	}

	ResumeThread(pi.hThread);

	while (1)
	{
		DEBUG_EVENT de;
		DWORD dwContinueStatus = DBG_CONTINUE;

		if (!WaitForSingleObject(&de, INFINITE))
		{
			break;
		}

		switch (de.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			printf("CREATE_PROCESS_DEBUG_EVENT\n");
			break;
		}
		case CREATE_THREAD_DEBUG_EVENT:
		{
			printf("CREATE_THREAD_DEBUG_EVENT\n");
			break;
		}
		case EXIT_THREAD_DEBUG_EVENT:
		{
			printf("EXIT_THREAD_DEBUG_EVENT\n");
			break;
		}
		case EXIT_PROCESS_DEBUG_EVENT:
		{
			printf("EXIT_PROCESS_DEBUG_EVENT\n");
		}
		case EXCEPTION_DEBUG_EVENT:
		{
			DWORD r = de.u.Exception.ExceptionRecord.ExceptionCode;
			if (r != EXCEPTION_BREAKPOINT)
			{
				dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
			}
			printf("EXCEPTION_DEBUG_EVENT\n");
			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			printf("OUTPUT_DEBUG_STRING_EVENT\n");
			break;
		}
		case RIP_EVENT:
		{
			printf("RIP_EVENT\n");
			break;
		}
		case LOAD_DLL_DEBUG_EVENT:
		{
			printf("LOAD_DLL_DEBUG_EVENT\n");
			break;
		}
		case UNLOAD_DLL_DEBUG_EVENT:
		{
			printf("UNLOAD_DLL_DEBUG_EVENT\n");
			break;
		}
		}

		if (de.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			break;
		}
		ContinueDebugEvent(de.dwProcessId, de.dwThreadId, dwContinueStatus);
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;
}
#include<windows.h>
#include<stdio.h>

int main(void)
{
	if (IsDebuggerPresent())
	{
		printf("on debugger\n");
	}
	else
	{
		printf("not on debugger\n");
	}

	return 0;
}
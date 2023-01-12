#line 1 "test.c"














typedef unsigned int DWORD;
typedef void* HANDLE;
typedef void VOID, *LPVOID;
typedef DWORD* LPDWORD;
extern DWORD WriteConsole(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
extern DWORD GetLastError();
extern HANDLE GetStdHandle(DWORD nStdHandle);


#line 1 "C:\\Code\\compiler\\compiler\\test.h"

#line 25 "test.c"

int main()
{
	DWORD num;
	
	if (!WriteConsole(GetStdHandle(((DWORD)-11)), "Hello, world!\n", 14, &num, ((void*)0)))
		return GetLastError();
	if (!WriteConsole(GetStdHandle(((DWORD)-11)), "This is a test of macros!", 25, &num, ((void*)0)))
		return GetLastError();
	if (!WriteConsole(GetStdHandle(((DWORD)-11)), "Test of include files", 21, &num, ((void*)0)))
		return GetLastError();
	return 0;
}

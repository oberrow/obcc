
// This is a test of single line comments

#define MACRO "This is a test of macros!"
#define sizeMACRO 25
#define NULL ((void*)0)
#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)
typedef unsigned long DWORD;
typedef void* HANDLE;
typedef void VOID, *LPVOID;
typedef DWORD* LPDWORD;
typedef int BOOL;
extern BOOL   __stdcall WriteConsoleA(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved);
extern DWORD  __stdcall GetLastError();
extern HANDLE __stdcall GetStdHandle(DWORD nStdHandle);

/*  Test of multiline comments
	Test of multiline comments
	Test of multiline comments
	Test of multiline comments
*/
#include "test.h"

int main()
{
	DWORD num;
	// A standard library has not been implemented yet so this is our only chance at testing the compiler
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "Hello, world!\n", 14, &num, NULL)) // Print "Hello, world!"
		return GetLastError();
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), MACRO, sizeMACRO, &num, NULL)) // Print the contents of MACRO
		return GetLastError();
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), TEST_MACRO, 22, &num, NULL))
		return GetLastError();
	return 0;
}
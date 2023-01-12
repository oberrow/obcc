	DWORD num;
	// A standard library has not been implemented yet so this is our only chance at testing the compiler
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "Hello, world!\n", 14, &num, NULL))
003F0239  push        0  
003F023B  lea         eax,[num]  
003F023E  push        eax  
003F023F  push        0Eh  
003F0241  push        offset string "Hello, world!\n" (0445E50h)  
003F0246  push        0FFFFFFF5h  
003F0248  call        _GetStdHandle@4 (03ECF6Ch)  
003F024D  push        eax  
003F024E  call        _WriteConsoleA@20 (03EB3E2h)  
003F0253  test        eax,eax  
003F0255  jne         main+2Eh (03F025Eh)  
		return GetLastError();
003F0257  call        _GetLastError@0 (03EC4D1h)  
003F025C  jmp         main+7Ah (03F02AAh)  
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), MACRO, sizeMACRO, &num, NULL))
003F025E  push        0  
003F0260  lea         eax,[num]  
003F0263  push        eax  
003F0264  push        19h  
003F0266  push        offset string "This is a test of macros!" (0445E64h)  
003F026B  push        0FFFFFFF5h  
003F026D  call        _GetStdHandle@4 (03ECF6Ch)  
003F0272  push        eax  
003F0273  call        _WriteConsoleA@20 (03EB3E2h)  
003F0278  test        eax,eax  
003F027A  jne         main+53h (03F0283h)  
		return GetLastError();
003F027C  call        _GetLastError@0 (03EC4D1h)  
003F0281  jmp         main+7Ah (03F02AAh)  
	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), TEST_MACRO, 22, &num, NULL))
003F0283  push        0  
003F0285  lea         eax,[num]  
003F0288  push        eax  
003F0289  push        16h  
003F028B  push        offset string "\nTest of include files" (0445E84h)  
003F0290  push        0FFFFFFF5h  
003F0292  call        _GetStdHandle@4 (03ECF6Ch)  
003F0297  push        eax  
003F0298  call        _WriteConsoleA@20 (03EB3E2h)  
003F029D  test        eax,eax  
003F029F  jne         main+78h (03F02A8h)  
		return GetLastError();
003F02A1  call        _GetLastError@0 (03EC4D1h)  
003F02A6  jmp         main+7Ah (03F02AAh)  
	return 0;
003F02A8  xor         eax,eax  
}
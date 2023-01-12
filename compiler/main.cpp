#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>

#include "error.h"
#include "preproc.h"
#include "scanner.h"

// In the name of Allah, The Most Gracious and The Most Merciful

// Stages:
// Preprocessor: Takes input string and copies include files to the main file, handles #define, handles #if, etc.
// Compiler: Takes the preprocessed output and converts it to machine code
// Assembler: Takes machine code and outputs it to the coff file
// Linker: Takes one or more coff files and links them into an executable pe file*
// *The linker will automaticially be invoked by the compiler once the coff files are generated unless -C option is specified

// Working on: preprocessor.
// What's completed in the preprocessor:
// Loading Files
// Removing Comments
// Making Macros
// Done: Loading files, Errors.
// Not much done...

// c keywords:
// auto	    break	case	 char	const	 continue   default	 do
// double	else	enum	 extern	float	 for	    goto	 if
// int	    long	register return	short	 signed	    sizeof	 static
// struct	switch	typedef	 union	unsigned void	    volatile while
// Extra Keywords
// __attributes
// Current attributes that can be applied:
// thread_local

const char* filename;
const char* workingDirectory;

// x64 Compiler
// __fastcall decorates names like @(function name)@(size of all the parameters)
// example:
// void func1();
// void func2(int par);
// @func1@0
// @func2@4
// I don't even want to try to explain it so:
// https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170#parameter-passing

// Input: File name
// Output: Compiled COFF file named [input file].obj and if the -C option isn't specified, a program named [input file].exe
int main(int argc, char** argv)
{
    workingDirectory = "tests/";
    filename = "tests/test.c";
    ADD_TO_INCLUDE_PATH("C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um");
    ADD_TO_INCLUDE_PATH(std::filesystem::current_path().string() + "/libc");
    std::ifstream inputfile{ filename }; // we'll use test.c until a parameter parser is made
    std::string contents;
    std::string ln;
    while (std::getline(inputfile, ln)) { contents.append(ln); contents.push_back('\n'); }
    contents.pop_back();
    std::istringstream contentsStream(contents); // load the contents back to a stream
    auto preprocessorO = preproc::parse(contentsStream);
    std::cout << "Output of preprocesser on test.c: \n" << preprocessorO.output;
    error::printErrors(preprocessorO.errors); // print all the errors
#ifdef _DEBUG
    (void)_getch();
#endif
    contentsStream.~basic_istringstream();
    contents.~basic_string();
    ln.~basic_string();
    inputfile.~basic_ifstream();
    preprocessorO.errors.clear();
    preprocessorO.output.~basic_string();
    ExitProcess(0);
}
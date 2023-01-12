#pragma once
#include <string>
#include <fstream>

namespace scanner
{
	struct token
	{

	};
	class lexical_analyzer
	{
	public:
		lexical_analyzer();
		// Doesn't call run function!
		explicit lexical_analyzer(const std::string& filename);
		bool run();
		void open(const std::string& filename);
		// -2: File not open, -1 EOF Reached. Otherwise, the character that was read.
		char getfirstchar();
		// Returns 0 based line
		int getlinenumber() { return line; }
		
	private:
		// -1 File not open, 0 EOF Reached. Otherwise, the character that was read.
		char getc();
		// false File not open, true success
		bool ungetc(char put);
		std::ifstream file;
		int line;
	};
}
#include "scanner.h"
#include "symbol_table.h"

// Define the symbol table
std::unordered_map<std::string, symbols::symbol> symbols::symbol_table; // extern defined in symbol_table.h

namespace scanner
{
	char lexical_analyzer::getfirstchar()
	{
		if (!file) return -2;
		char read = 0;
		bool isbad = false;
		while (!isbad)
		{
			if (file.eof())
				return EOF;
			while (!file.eof() && std::isspace(read = getc()));
		}
		if (file.eof())
			return EOF;
		return read;
	}
	char lexical_analyzer::getc()
	{
		if (!file) return -1;
		char read = 0;
		if (file.eof())
			return 0;
		else if ((read = file.get()); read == '\n')
			line++;
		return read;
	}
	bool lexical_analyzer::ungetc(char put)
	{
		if (!file) return false;
		if (put == '\n')
			line--;
		file.putback(put);
		return true;
	}
}
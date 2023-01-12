#pragma once
#include <unordered_map>
#include <string>

namespace scanner
{
	struct token;
}

namespace symbols
{
	enum class calling_convention
	{
		CDECL,
		STDCALL,
		FASTCALL,
		THISCALL,
		VECTORCALL
	};
	// Represents a symbol
	struct symbol
	{
		// The token
		::scanner::token* token;
		// The name of the symbol
		std::string name;
		// The type of the symbol
		std::string type;
		// The line in which the scope starts in
		uint32_t line_scope;
		// The line the symbol is declared
		uint32_t line_declaration;
		// the calling convention
		calling_convention convention;
	};
	// The symbol table
	extern std::unordered_map<std::string, symbol> symbol_table;
}